#include "dbg.h"

int main() {
  dbg::wlog(123, L' ', 3.14, L' ');
  dbg::wlogline(L"str");
  dbg::wlog(L"END");
}
