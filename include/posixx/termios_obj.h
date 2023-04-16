#ifndef POSIXX_TERMIOS_H
#define POSIXX_TERMIOS_H
#include <array>
#include <bitset>
#include <concepts>
#include <ios>
#include <iostream>
#include <magic_enum.hpp>
#include <string_view>
#include <sys/termios.h>
#include <type_traits>

// Iterating phase means that I'm depending on magic_enum until
// I'm 100% certain of the flags I want to use.
static_assert(magic_enum::is_magic_enum_supported);

namespace posixx {
using namespace std::string_view_literals;

using tc_bitset = std::bitset<sizeof(tcflag_t) * 8>;

consteval size_t tc_shiftedby(tcflag_t flg) {
  size_t rslt = 0;
  for (auto i = 0; i < sizeof(tcflag_t) * 8; i++) {
    if ((flg >> i) == 1) {
      return i;
    }
  }
  return 0;
}

template <class T>
concept termios_flags =
    std::is_base_of<tc_bitset, T>::value &&
    requires(T t, tcflag_t f) {
      typename T::flags;
      { T::flag_count } -> std::convertible_to<size_t>;
      requires std::is_same_v<
          const std::array<typename T::flags, T::flag_count>, decltype(T::all)>;
      requires std::is_same_v<
          const std::array<typename std::string_view, T::flag_count>,
          decltype(T::flag_names)>;
    };

struct input_flags : public tc_bitset {
  /* Reference:
  #define IGNBRK 0x00000001  // ignore BREAK condition
  #define BRKINT 0x00000002  // map BREAK to SIGINTR
  #define IGNPAR 0x00000004  // ignore (discard) parity errors
  #define PARMRK 0x00000008  // mark parity and framing errors
  #define INPCK 0x00000010   // enable checking of parity errors
  #define ISTRIP 0x00000020  // strip 8th bit off chars
  #define INLCR 0x00000040   // map NL into CR
  #define IGNCR 0x00000080   // ignore CR
  #define ICRNL 0x00000100   // map CR to NL (ala CRMOD)
  #define IXON 0x00000200    // enable output flow control
  #define IXOFF 0x00000400   // enable input flow control
  #define IXANY 0x00000800   // any char will restart after stop
  #define IMAXBEL 0x00002000 // ring bell on input queue full
  #define IUTF8 0x00004000   // maintain state for UTF-8 VERASE
  */
  enum flags {
    ignbrk = tc_shiftedby(IGNBRK),
    brkint = tc_shiftedby(BRKINT),
    ignpar = tc_shiftedby(IGNPAR),
    parmrk = tc_shiftedby(PARMRK),
    inpck = tc_shiftedby(INPCK),
    istrip = tc_shiftedby(ISTRIP),
    inlcr = tc_shiftedby(INLCR),
    igncr = tc_shiftedby(IGNCR),
    icrnl = tc_shiftedby(ICRNL),
    ixon = tc_shiftedby(IXON),
    ixoff = tc_shiftedby(IXOFF),
    ixany = tc_shiftedby(IXANY),
    imaxbel = tc_shiftedby(IMAXBEL),
    iutf8 = tc_shiftedby(IUTF8),
  };
  static constexpr auto flag_count = magic_enum::enum_count<flags>();

  static constexpr std::array<flags, flag_count> all =
      magic_enum::enum_values<flags>();

  static constexpr std::array<std::string_view, flag_count> flag_names =
      magic_enum::enum_names<flags>();

  input_flags(tcflag_t flgs) : tc_bitset(flgs) {}
};

static_assert(termios_flags<input_flags>);

struct output_flags : public tc_bitset {

  /* Reference:
#define OPOST           0x00000001      // enable following output processing
#define ONLCR           0x00000002      // map NL to CR-NL (ala CRMOD)
#define OXTABS          0x00000004      // expand tabs to spaces
#define ONOEOT          0x00000008      // discard EOT's (^D) on output)
  */
  enum flags {
    opost = tc_shiftedby(OPOST),
    onlcr = tc_shiftedby(ONLCR),
    oxtabs = tc_shiftedby(OXTABS),
    onoeot = tc_shiftedby(ONOEOT),
  };

  static constexpr auto flag_count = magic_enum::enum_count<flags>();

  /* static constexpr std::array<flags, flag_count> all = */
  /*     std::array{opost, onlcr, oxtabs, onoeot}; */
  static constexpr std::array<flags, flag_count> all =
      magic_enum::enum_values<flags>();

  static constexpr std::array<std::string_view, flag_count> flag_names =
      magic_enum::enum_names<flags>();

  output_flags(tcflag_t flgs) : tc_bitset(flgs) {}
};

static_assert(termios_flags<output_flags>);

struct control_flags : public tc_bitset {

  /* Reference:
#define CIGNORE         0x00000001       ignore control flags
#endif
#define CSIZE           0x00000300       character size mask
#define     CS5             0x00000000       5 bits (pseudo)
#define     CS6             0x00000100       6 bits
#define     CS7             0x00000200       7 bits
#define     CS8             0x00000300       8 bits
#define CSTOPB          0x00000400       send 2 stop bits
#define CREAD           0x00000800       enable receiver
#define PARENB          0x00001000       parity enable
#define PARODD          0x00002000       odd parity, else even
#define HUPCL           0x00004000       hang up on last close
#define CLOCAL          0x00008000       ignore modem status lines
#define CCTS_OFLOW      0x00010000       CTS flow control of output
#define CRTS_IFLOW      0x00020000       RTS flow control of input
#define CDTR_IFLOW      0x00040000       DTR flow control of input
#define CDSR_OFLOW      0x00080000       DSR flow control of output
#define CCAR_OFLOW      0x00100000       DCD flow control of output
#define MDMBUF          0x00100000       old name for CCAR_OFLOW
  */

  enum flags {
    cignore = tc_shiftedby(CIGNORE),
    cstopb = tc_shiftedby(CSTOPB),
    cread = tc_shiftedby(CREAD),
    parenb = tc_shiftedby(PARENB),
    parodd = tc_shiftedby(PARODD),
    hupcl = tc_shiftedby(HUPCL),
    clocal = tc_shiftedby(CLOCAL),
    ccts_oflow = tc_shiftedby(CCTS_OFLOW),
    crts_iflow = tc_shiftedby(CRTS_IFLOW),
    cdtr_iflow = tc_shiftedby(CDTR_IFLOW),
    cdsr_oflow = tc_shiftedby(CDSR_OFLOW),
    ccar_oflow = tc_shiftedby(CCAR_OFLOW),
  };

  static constexpr auto flag_count = magic_enum::enum_count<flags>();

  static constexpr std::array<flags, flag_count> all =
      magic_enum::enum_values<flags>();

  static constexpr std::array<std::string_view, flag_count> flag_names =
      magic_enum::enum_names<flags>();

  static constexpr auto csize_mask = CSIZE;
  enum class char_size {
    cs5 = CS5,
    cs6 = CS6,
    cs7 = CS7,
    cs8 = CS8,
  };

  control_flags(tcflag_t flgs) : tc_bitset(flgs) {}
};

static_assert(termios_flags<control_flags>);

struct local_flags : public tc_bitset {
  /*
#define ECHOKE          0x00000001       visual erase for line kill
#define ECHOE           0x00000002       visually erase chars
#define ECHOK           0x00000004       echo NL after line kill
#define ECHO            0x00000008       enable echoing
#define ECHONL          0x00000010       echo NL even if ECHO is off
#define ECHOPRT         0x00000020       visual erase mode for hardcopy
#define ECHOCTL         0x00000040       echo control chars as ^(Char)
#define ISIG            0x00000080       enable signals INTR, QUIT, [D]SUSP
#define ICANON          0x00000100       canonicalize input lines
#define ALTWERASE       0x00000200       use alternate WERASE algorithm
#define IEXTEN          0x00000400       enable DISCARD and LNEXT
#define EXTPROC         0x00000800       external processing
#define TOSTOP          0x00400000       stop background jobs from output
#define FLUSHO          0x00800000       output being flushed (state)
#define NOKERNINFO      0x02000000       no kernel output from VSTATUS
#define PENDIN          0x20000000       XXX retype pending input (state)
#define NOFLSH          0x80000000       don't flush after interrupt
   */
  enum flags {
    echoke = tc_shiftedby(ECHOKE),
    echoe = tc_shiftedby(ECHOE),
    echok = tc_shiftedby(ECHOK),
    echo = tc_shiftedby(ECHO),
    echonl = tc_shiftedby(ECHONL),
    echoprt = tc_shiftedby(ECHOPRT),
    echoctl = tc_shiftedby(ECHOCTL),
    isig = tc_shiftedby(ISIG),
    icanon = tc_shiftedby(ICANON),
    altwerase = tc_shiftedby(ALTWERASE),
    iexten = tc_shiftedby(IEXTEN),
    extproc = tc_shiftedby(EXTPROC),
    tostop = tc_shiftedby(TOSTOP),
    flusho = tc_shiftedby(FLUSHO),
    nokerninfo = tc_shiftedby(NOKERNINFO),
    pendin = tc_shiftedby(PENDIN),
    noflsh = tc_shiftedby(NOFLSH),
  };

  static constexpr auto flag_count = magic_enum::enum_count<flags>();

  static constexpr std::array<flags, flag_count> all =
      magic_enum::enum_values<flags>();
  static constexpr std::array<std::string_view, flag_count> flag_names =
      magic_enum::enum_names<flags>();

  local_flags(tcflag_t flgs) : tc_bitset(flgs) {}
};

static_assert(termios_flags<local_flags>);

struct termios_obj : public termios {
  using termios::termios;

  inline input_flags input() { return input_flags(this->c_iflag); }
  inline void input(const input_flags &flgs) {
    this->c_iflag = flgs.to_ulong();
  }

  inline output_flags output() { return output_flags(this->c_oflag); }
  inline void output(const output_flags &flgs) {
    this->c_oflag = flgs.to_ulong();
  }

  inline control_flags control() { return control_flags(this->c_cflag); }
  inline void control(const control_flags &flgs) {
    this->c_cflag = flgs.to_ulong();
  }

  inline local_flags local() { return local_flags(this->c_lflag); }
  inline void local(const local_flags &flgs) {
    this->c_lflag = flgs.to_ulong();
  }

  /* #define TCSANOW         0               make change immediate */
  /* #define TCSADRAIN       1               drain output, then change */
  /* #define TCSAFLUSH       2               drain output, flush input */
  /* #define TCSASOFT        0x10            {BSD ONLY} flag - don't alter h.w.
   * state */
  enum class change_flag {
    now = TCSANOW,
    drain = TCSADRAIN,
    flush = TCSAFLUSH,
#ifdef TCSASOFT
    soft = TCSASOFT,
#endif
  };
};

template <termios_flags T> inline void debug_dump_flags(const T &v) {
  std::cout << std::boolalpha;
  for (auto i = 0; i < T::flag_count; i++) {
    std::cout << T::flag_names[i] << " " << v.test(T::all[i]) << "\n";
  }
  std::cout << std::noboolalpha;
}

} // namespace posixx

#endif
