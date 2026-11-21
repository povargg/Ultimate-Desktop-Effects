#include "update.h"
#include "i18n.h"
#include "install.h"
#include "ui.h"
#include <bcrypt.h>
#include <winhttp.h>
#include <cerrno>
#include <cstdlib>
#include <cwchar>
#include <mutex>
#include <stdexcept>
#include <thread>

namespace updates {

struct Result {
    std::optional<Release> release;
    std::vector<unsigned char> binary;
    std::wstring error;
};

namespace {

constexpr wchar_t kHost[] = L"github.com";
constexpr wchar_t kReleasePath[] = L"/povargg/Ultimate-Desktop-Effects/releases/";
constexpr wchar_t kAsset[] = L"Ultimate-Desktop-Effects-x64.exe";
constexpr size_t kMaxBinary = 64 * 1024 * 1024;
using Bytes = std::vector<unsigned char>;

struct InternetHandle {
    HINTERNET value;
    ~InternetHandle() { if (value) WinHttpCloseHandle(value); }
};

struct FileHandle {
    HANDLE value;
    ~FileHandle() { if (value && value != INVALID_HANDLE_VALUE) CloseHandle(value); }
};

std::wstring Wide(std::string_view ascii) { return {ascii.begin(), ascii.end()}; }

Bytes Download(const std::wstring& path, size_t limit) {
    InternetHandle session{WinHttpOpen(APP_NAME L"/" APP_VERSION, WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY,
                                       WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0)};
    if (!session.value) throw std::runtime_error("WinHTTP: " + std::to_string(GetLastError()));
    WinHttpSetTimeouts(session.value, 8000, 8000, 15000, 15000);
    InternetHandle connection{WinHttpConnect(session.value, kHost, INTERNET_DEFAULT_HTTPS_PORT, 0)};
    if (!connection.value) throw std::runtime_error("HTTPS: " + std::to_string(GetLastError()));
    InternetHandle request{WinHttpOpenRequest(connection.value, L"GET", path.c_str(), nullptr,
                                             WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES,
                                             WINHTTP_FLAG_SECURE)};
    DWORD redirect = WINHTTP_OPTION_REDIRECT_POLICY_DISALLOW_HTTPS_TO_HTTP;
    if (!request.value ||
        !WinHttpSetOption(request.value, WINHTTP_OPTION_REDIRECT_POLICY, &redirect, sizeof(redirect)) ||
        !WinHttpSendRequest(request.value, WINHTTP_NO_ADDITIONAL_HEADERS, 0,
                            WINHTTP_NO_REQUEST_DATA, 0, 0, 0) ||
        !WinHttpReceiveResponse(request.value, nullptr)) {
        throw std::runtime_error("HTTPS: " + std::to_string(GetLastError()));
    }
    DWORD status = 0, size = sizeof(status);
    if (!WinHttpQueryHeaders(request.value, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
                             WINHTTP_HEADER_NAME_BY_INDEX, &status, &size, WINHTTP_NO_HEADER_INDEX))
        throw std::runtime_error("HTTP: " + std::to_string(GetLastError()));
    if (status != 200) throw std::runtime_error("GitHub HTTP " + std::to_string(status));

    Bytes bytes;
    std::array<unsigned char, 32 * 1024> buffer{};
    for (;;) {
        DWORD count = 0;
        if (!WinHttpReadData(request.value, buffer.data(), static_cast<DWORD>(buffer.size()), &count))
            throw std::runtime_error("HTTPS: " + std::to_string(GetLastError()));
        if (count == 0) return bytes;
        if (count > limit - bytes.size()) throw std::runtime_error("Unexpected download size.");
        bytes.insert(bytes.end(), buffer.begin(), buffer.begin() + count);
    }
}

std::string Sha256(Bytes& bytes) {
    BCRYPT_ALG_HANDLE algorithm = nullptr;
    if (BCryptOpenAlgorithmProvider(&algorithm, BCRYPT_SHA256_ALGORITHM, nullptr, 0) != 0)
        throw std::runtime_error("SHA-256 unavailable.");
    std::array<unsigned char, 32> hash{};
    const auto status = BCryptHash(algorithm, nullptr, 0, bytes.data(), static_cast<ULONG>(bytes.size()),
                                   hash.data(), static_cast<ULONG>(hash.size()));
    BCryptCloseAlgorithmProvider(algorithm, 0);
    if (status != 0) throw std::runtime_error("SHA-256 failed.");
    constexpr char hex[] = "0123456789abcdef";
    std::string result;
    result.reserve(2 * hash.size());
    for (const auto byte : hash) {
        result.push_back(hex[byte >> 4]);
        result.push_back(hex[byte & 0xF]);
    }
    return result;
}

Result CheckRelease() {
    const auto bytes = Download(std::wstring(kReleasePath) + L"latest/download/update-manifest.txt", 512);
    auto release = ParseManifest(std::string(bytes.begin(), bytes.end()));
    if (!release) throw std::runtime_error("Invalid release manifest.");
    const std::wstring current = APP_VERSION;
    const auto version = ParseVersion(std::string(current.begin(), current.end()));
    if (!version || release->version <= *version) return {};
    Result result;
    result.release = std::move(release);
    return result;
}

Result DownloadRelease(const Release& release) {
    Result result;
    result.binary = Download(std::wstring(kReleasePath) + L"download/v" + Wide(release.tag) + L"/" + kAsset,
                             kMaxBinary);
    if (result.binary.size() < 2 || result.binary[0] != 'M' || result.binary[1] != 'Z')
        throw std::runtime_error("The download is not a Windows executable.");
    if (Sha256(result.binary) != release.sha256)
        throw std::runtime_error("The SHA-256 checksum does not match the release manifest.");
    return result;
}

std::wstring PreviousPath(const std::wstring& exe, DWORD pid) {
    return exe + L".ude-update-" + std::to_wstring(pid) + L".old";
}

bool Restart(const std::wstring& exe) {
    FileHandle parent{nullptr};
    if (!DuplicateHandle(GetCurrentProcess(), GetCurrentProcess(), GetCurrentProcess(), &parent.value,
                         SYNCHRONIZE | PROCESS_QUERY_LIMITED_INFORMATION, TRUE, 0)) return false;

    SIZE_T size = 0;
    InitializeProcThreadAttributeList(nullptr, 1, 0, &size);
    if (!size) return false;
    std::vector<unsigned char> storage(size);
    auto* attributes = reinterpret_cast<PPROC_THREAD_ATTRIBUTE_LIST>(storage.data());
    if (!InitializeProcThreadAttributeList(attributes, 1, 0, &size)) return false;

    bool started = false;
    if (UpdateProcThreadAttribute(attributes, 0, PROC_THREAD_ATTRIBUTE_HANDLE_LIST, &parent.value,
                                  sizeof(parent.value), nullptr, nullptr)) {
        const auto parentValue = reinterpret_cast<uintptr_t>(parent.value);
        std::wstring command = L"\"" + exe + L"\" /update-parent " + std::to_wstring(parentValue);
        if (!IsInstalledCopy()) command += L" /portable";
        STARTUPINFOEXW startup{};
        startup.StartupInfo.cb = sizeof(startup);
        startup.lpAttributeList = attributes;
        PROCESS_INFORMATION process{};
        started = CreateProcessW(exe.c_str(), command.data(), nullptr, nullptr, TRUE,
                                 EXTENDED_STARTUPINFO_PRESENT, nullptr, nullptr,
                                 &startup.StartupInfo, &process) != FALSE;
        if (started) {
            CloseHandle(process.hThread);
            CloseHandle(process.hProcess);
        }
    }
    const DWORD error = GetLastError();
    DeleteProcThreadAttributeList(attributes);
    if (!started) SetLastError(error);
    return started;
}

std::wstring InstallRelease(const Bytes& binary) {
    const std::wstring exe = ExePath();
    const auto pid = GetCurrentProcessId();
    const auto stage = exe + L".ude-update-" + std::to_wstring(pid) + L".tmp";
    const auto backup = PreviousPath(exe, pid);
    {
        FileHandle file{CreateFileW(stage.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_NEW,
                                    FILE_ATTRIBUTE_NORMAL, nullptr)};
        if (file.value == INVALID_HANDLE_VALUE)
            return L"Windows " + std::to_wstring(GetLastError());
        DWORD written = 0;
        if (!WriteFile(file.value, binary.data(), static_cast<DWORD>(binary.size()), &written, nullptr) ||
            written != binary.size() || !FlushFileBuffers(file.value)) {
            const auto error = GetLastError();
            CloseHandle(file.value);
            file.value = nullptr;
            DeleteFileW(stage.c_str());
            return L"Windows " + std::to_wstring(error);
        }
    }
    if (!MoveFileExW(exe.c_str(), backup.c_str(), MOVEFILE_WRITE_THROUGH)) {
        const auto error = GetLastError();
        DeleteFileW(stage.c_str());
        return L"Windows " + std::to_wstring(error);
    }

    const bool swapped = MoveFileExW(stage.c_str(), exe.c_str(), MOVEFILE_WRITE_THROUGH) != FALSE;
    if (swapped && Restart(exe)) return {};

    const auto error = GetLastError();
    DeleteFileW(stage.c_str());
    if (!MoveFileExW(backup.c_str(), exe.c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH))
        return L"Windows " + std::to_wstring(error) + L"\n" + Tr(S_UPDATE_BACKUP) + L"\n" + backup;
    return L"Windows " + std::to_wstring(error);
}

void ShowError(HWND owner, const std::wstring& error) {
    const auto message = std::wstring(Tr(S_UPDATE_FAILED)) + L"\n\n" + error;
    MessageBoxW(owner, message.c_str(), APP_NAME, MB_OK | MB_ICONWARNING);
}

} // namespace

struct Job {
    std::mutex mutex;
    std::optional<Result> result;
};

static std::shared_ptr<Job> Start(HWND owner, std::optional<Release> download = std::nullopt) {
    auto job = std::make_shared<Job>();
    std::thread([job, owner, download = std::move(download)] {
        Result result;
        try {
            result = download ? DownloadRelease(*download) : CheckRelease();
        } catch (const std::exception& error) {
            result.error = Wide(error.what());
        }
        {
            std::lock_guard<std::mutex> lock(job->mutex);
            job->result.emplace(std::move(result));
        }
        PostMessageW(owner, WM_APP_UPDATE, 0, 0);
    }).detach();
    return job;
}

void Updater::Check(HWND owner, bool interactive) {
    interactive_ |= interactive;
    if (job_) {
        if (interactive) TrayBalloon(APP_NAME, Tr(S_UPDATE_BUSY));
        return;
    }
    try {
        job_ = Start(owner);
    } catch (const std::exception& error) {
        if (interactive_) ShowError(owner, Wide(error.what()));
        interactive_ = false;
    }
}

bool Updater::Finish(HWND owner) {
    if (!job_) return false;
    Result result;
    {
        std::lock_guard<std::mutex> lock(job_->mutex);
        if (!job_->result) return false;
        result = std::move(*job_->result);
    }
    job_.reset();
    const bool interactive = std::exchange(interactive_, false);
    if (!result.error.empty()) {
        if (interactive) ShowError(owner, result.error);
        return false;
    }
    if (!result.binary.empty()) {
        std::wstring error = InstallRelease(result.binary);
        if (error.empty()) return true;
        ShowError(owner, error);
        return false;
    }
    if (!result.release) {
        if (interactive) MessageBoxW(owner, Tr(S_UPDATE_CURRENT), APP_NAME, MB_OK | MB_ICONINFORMATION);
        return false;
    }

    const auto title = std::wstring(APP_NAME) + L" " + Wide(result.release->tag);
    if (!interactive) {
        TrayBalloon(title.c_str(), Tr(S_UPDATE_AVAILABLE));
    } else if (MessageBoxW(owner, Tr(S_UPDATE_ASK), title.c_str(),
                           MB_YESNO | MB_ICONINFORMATION | MB_DEFBUTTON2) == IDYES) {
        try {
            job_ = Start(owner, std::move(result.release));
            interactive_ = true;
            TrayBalloon(APP_NAME, Tr(S_UPDATE_BUSY));
        } catch (const std::exception& error) {
            ShowError(owner, Wide(error.what()));
        }
    }
    return false;
}

bool WaitForPreviousProcess(const wchar_t* handleText, DWORD& pid) {
    if (!handleText || !*handleText || *handleText < L'0' || *handleText > L'9') return false;
    wchar_t* end = nullptr;
    errno = 0;
    const auto value = std::wcstoull(handleText, &end, 10);
    if (errno || *end || value > UINTPTR_MAX) return false;
    const FileHandle handle{reinterpret_cast<HANDLE>(static_cast<uintptr_t>(value))};
    pid = GetProcessId(handle.value);
    return pid && pid != GetCurrentProcessId() && WaitForSingleObject(handle.value, 15000) == WAIT_OBJECT_0;
}

void RemovePreviousVersion(DWORD pid) {
    if (pid) DeleteFileW(PreviousPath(ExePath(), pid).c_str());
}

} // namespace updates
