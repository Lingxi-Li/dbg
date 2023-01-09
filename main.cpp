#include "dbg.h"

int main() {
  dbg::wlog(123, 3.14);
  dbg::wlogline(L"str");
  dbg::wlog(L"END");
}
