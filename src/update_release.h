#pragma once
#include <algorithm>
#include <array>
#include <charconv>
#include <optional>
#include <string>
#include <string_view>

namespace updates {

using Version = std::array<unsigned, 3>;

inline std::optional<Version> ParseVersion(std::string_view text) {
    Version version{};
    for (size_t i = 0; i < version.size(); ++i) {
        const size_t dot = text.find('.');
        if ((i + 1 == version.size()) != (dot == text.npos)) return std::nullopt;
        const auto component = text.substr(0, dot);
        if (component.empty() || component.size() > 6 ||
            (component.size() > 1 && component.front() == '0')) return std::nullopt;
        const auto* end = component.data() + component.size();
        const auto parsed = std::from_chars(component.data(), end, version[i]);
        if (parsed.ec != std::errc{} || parsed.ptr != end) return std::nullopt;
        if (dot != text.npos) text.remove_prefix(dot + 1);
    }
    return version;
}

struct Release {
    std::string tag;
    Version version;
    std::string sha256;
};

inline std::optional<Release> ParseManifest(std::string_view text) {
    const auto newline = text.find('\n');
    if (newline == text.npos) return std::nullopt;
    auto number = text.substr(0, newline);
    if (!number.empty() && number.back() == '\r') number.remove_suffix(1);
    const auto version = ParseVersion(number);
    if (!version) return std::nullopt;

    auto hash = text.substr(newline + 1);
    if (!hash.empty() && hash.back() == '\n') hash.remove_suffix(1);
    if (!hash.empty() && hash.back() == '\r') hash.remove_suffix(1);
    if (hash.size() != 64 || !std::all_of(hash.begin(), hash.end(), [](char ch) {
            return (ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F');
        })) return std::nullopt;
    Release release{std::string(number), *version, std::string(hash)};
    for (char& ch : release.sha256)
        if (ch >= 'A' && ch <= 'F') ch += 'a' - 'A';
    return release;
}

} // namespace updates
