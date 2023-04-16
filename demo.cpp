//
// Created by Jacob Scaggs on 4/16/23.
//
#include <posixx/file_descriptor.h>
#include <posixx/termios_obj.h>

#include <_types/_uint64_t.h>
#include <bitset>
#include <climits>
#include <iostream>
#include <memory>

int main(int argc, char **argv) {
  /* uint64_t intval = 0; */

  /* std::bitset<64>* asdf = reinterpret_cast<std::bitset<64>*>(&intval); */

  /* asdf->set(0,true); */
  /* std::cout << intval << std::endl; */

  posixx::file_descriptor fd{STDOUT_FILENO};
  auto tios = fd.termios();

  std::cout << "\nInput: \n";
  posixx::debug_dump_flags(tios.input());

  std::cout << "\nOutput: \n";
  posixx::debug_dump_flags(tios.output());

  std::cout << "\nControl: \n";
  posixx::debug_dump_flags(tios.control());

  std::cout << "\nLocal: \n";
  posixx::debug_dump_flags(tios.local());

  /* auto st = fd.fd_stat(); */
  /* st->debug_dump(); */

  return 0;
}
