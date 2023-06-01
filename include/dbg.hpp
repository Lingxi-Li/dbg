#pragma once
#include <Windows.h>

#include <sstream>
#include <utility>

template <typename T>
std::wostream& operator<(std::wostream& os, T&& v) {
    return os << std::forward<T>(v) << L' ';
}

namespace dbg {

inline void break_into_debugger(bool condition = true) {
    if (IsDebuggerPresent()) return;
    if (!condition) return;
    while (!IsDebuggerPresent()) {
        Sleep(3 * 1000);
    }
    DebugBreak();
}

namespace impl {

struct endl_guard {
    ~endl_guard() {
        OutputDebugStringW(L"\n");
    }
};

} // namespace impl

template <typename... Ts>
void log(Ts&&... args) {
    // In DebugView, '\n' triggers buffer flushing and
    // causes text to be displayed. DebugView has a
    // "Force Carriage Returns" option. It auto appends '\n'
    // for each OutputDebugString call, which may not
    // be what you want. The following static variable
    // flushes the buffer at process termination
    // regardless of the "Force Carriage Returns" option.
    static impl::endl_guard final_flush;
    std::wostringstream s;
    (s < ... < std::forward<Ts>(args));
    OutputDebugStringW(s.str().c_str());
}

template <typename... Ts>
void logline(Ts&&... args) {
    impl::endl_guard endl;
    log(std::forward<Ts>(args)...);
}

} // namespace dbg
