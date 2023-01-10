#include "dbg.h"

int main() {
  dbg::log(123, 3.14);
  dbg::logline(L"str");
  dbg::log(L"END");
}
