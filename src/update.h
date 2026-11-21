#pragma once
#include "common.h"
#include "update_release.h"
#include <memory>

namespace updates {

struct Job;

class Updater {
public:
    void Check(HWND owner, bool interactive);
    bool Finish(HWND owner);  // true when the new process is ready for this one to exit

private:
    std::shared_ptr<Job> job_;
    bool interactive_ = false;
};

bool WaitForPreviousProcess(const wchar_t* handleText, DWORD& pid);
void RemovePreviousVersion(DWORD pid);

} // namespace updates
