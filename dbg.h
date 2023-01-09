#include <windows.h>

#include <sstream>
#include <utility>

template <typename T> inline
std::wostream& operator<(std::wostream& os, T&& v) {
  return os << std::forward<T>(v) << L' ';
}

namespace dbg {

inline
void break_into_debugger(bool condition = true) {
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

inline
void put(const std::wstring& wstr) {
  static endl_guard final_flush; // for DebugView without "Force Carriage Returns"
  OutputDebugStringW(wstr.c_str());
}

} // namespace impl

template <typename... Ts> inline
void wlog(Ts&&... args) {
  std::wostringstream s;
  (s < ... < std::forward<Ts>(args));
  impl::put(s.str());
}

template <typename... Ts> inline
void wlogline(Ts&&... args) {
  impl::endl_guard endl;
  wlog(std::forward<Ts>(args)...);
}

} // namespace dbg
