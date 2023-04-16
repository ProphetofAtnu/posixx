#ifndef POSIXX_STAT_H
#define POSIXX_STAT_H
#include <iostream>
#include <sys/stat.h>

namespace posixx {

struct stat_obj : public stat {
  using stat::stat;

  inline void debug_dump() {
    // clang-format off
    std::cout 
      << "device: " << st_dev
      << "\nmode: " << st_mode
      << "\nnlink: " << st_nlink
      << "\nino: " << st_ino
      << "\nuid: " << st_uid
      << "\ngid: " << st_gid
      << "\nrdev: " << st_rdev
      << "\nsize: " << st_size
      << "\nblocks: " << st_blocks
      << "\nflags: " << st_flags
      << "\ngen: " << st_gen
      << "\nmtime: " << st_mtime
      << "\natime: " << st_atime
      << "\nctime: " << st_ctime << "\n";
    // clang-format on
    //
    std::cout << "\n";
    debug_dump_mode();
  }

  inline void debug_dump_mode() {
    std::cout << "Flags: ";
    /* if ((st_mode & S_IFMT) == S_IFMT) { std::cout << "\nS_IFMT " << "type of
     * file mask"; } */

#define _DEBUG_OUT_FLAG(flg, msg)                                              \
  if (st_mode & flg) {                                                \
    std::cout << "\n" #flg << " - " msg;                                       \
  }

    _DEBUG_OUT_FLAG(S_IFMT, "type of file mask")
    _DEBUG_OUT_FLAG(S_IFIFO, "named pipe (fifo)")
    _DEBUG_OUT_FLAG(S_IFCHR, "character special")
    _DEBUG_OUT_FLAG(S_IFDIR, "directory")
    _DEBUG_OUT_FLAG(S_IFBLK, "block special")
    _DEBUG_OUT_FLAG(S_IFREG, "regular")
    _DEBUG_OUT_FLAG(S_IFLNK, "symbolic link")
    _DEBUG_OUT_FLAG(S_IFSOCK, "socket")
    _DEBUG_OUT_FLAG(S_IRWXU, "RWX mask for owner")
    _DEBUG_OUT_FLAG(S_IRUSR, "R for owner")
    _DEBUG_OUT_FLAG(S_IWUSR, "W for owner")
    _DEBUG_OUT_FLAG(S_IXUSR, "X for owner")
    _DEBUG_OUT_FLAG(S_IRWXG, "RWX mask for group")
    _DEBUG_OUT_FLAG(S_IRGRP, "R for group")
    _DEBUG_OUT_FLAG(S_IWGRP, "W for group")
    _DEBUG_OUT_FLAG(S_IXGRP, "X for group")
    _DEBUG_OUT_FLAG(S_IRWXO, "RWX mask for other")
    _DEBUG_OUT_FLAG(S_IROTH, "R for other")
    _DEBUG_OUT_FLAG(S_IWOTH, "W for other")
    _DEBUG_OUT_FLAG(S_IXOTH, "X for other")
    _DEBUG_OUT_FLAG(S_ISUID, "set user id on execution")
    _DEBUG_OUT_FLAG(S_ISGID, "set group id on execution")
    _DEBUG_OUT_FLAG(S_ISVTX, "directory restrcted delete")
    _DEBUG_OUT_FLAG(S_ISTXT, "sticky bit: not supported")
    _DEBUG_OUT_FLAG(S_IREAD, "backward compatability")
    _DEBUG_OUT_FLAG(S_IWRITE, "backward compatability")
    _DEBUG_OUT_FLAG(S_IEXEC, "backward compatability")

#undef _DEBUG_OUT_FLAG
  }
};

} // namespace posixx

#endif
