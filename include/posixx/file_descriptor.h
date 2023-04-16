//
// Created by Jacob Scaggs on 4/16/23.
//

#ifndef POSIXX_FILE_DESCRIPTOR_H
#define POSIXX_FILE_DESCRIPTOR_H

#include "_predef.h"
#include "stat_obj.h"
#include "termios_obj.h"
#include <sys/termios.h>
#include <unistd.h>

namespace posixx {

class file_descriptor {
public:
  explicit inline file_descriptor(int fd = -1) : m_fd(fd){};

  inline file_descriptor(file_descriptor &&other) noexcept {
    m_fd = std::exchange(other.m_fd, -1);
    m_flags = std::exchange(other.m_flags, 0);
  }

  inline file_descriptor &operator=(file_descriptor &&other) noexcept {
    m_fd = std::exchange(other.m_fd, -1);
    m_flags = std::exchange(other.m_flags, 0);
    return *this;
  }

  file_descriptor(const file_descriptor &other) = delete;

  file_descriptor &operator=(const file_descriptor &other) = delete;

  [[nodiscard]] inline int &fd() { return m_fd; }

  inline error_or_size read(void *buf, size_t size) const {
    return ::read(m_fd, buf, size);
  }

  template <readable T> inline error_or_size read(T &data) const {
    return ::read(m_fd, data.data(), data.size());
  }

  inline error_or_size write(const void *buf, size_t size) const {
    return ::write(m_fd, buf, size);
  }

  template <writable T> inline error_or_size write(const T &data) const {
    return ::write(m_fd, data.data(), data.size());
  }

  inline error_state close() noexcept {
    if (m_fd > -1) {
      auto result = ::close(m_fd);
      m_fd = -1;
      return result;
    }
    return 0;
  }

  inline error_or<stat_obj> fd_stat() const noexcept {
    return error_or<stat_obj>::track(fstat, m_fd);
  }

  inline error_state get_stat(struct stat &op) const noexcept {
    struct stat result = {};
    return fstat(m_fd, &result);
  }

  inline struct stat get_stat() const {
    struct stat result = {};
    auto es = get_stat(result);
    if (!es) {
      throw es.as_system_error();
    }
    return result;
  }

  // TODO - Error Handling
  inline termios_obj termios() const {
    termios_obj obj;
    /* TODO */ tcgetattr(m_fd, &obj);
    return obj;
  }

  // TODO - Error Handling
  inline void set_termios(
      const termios_obj &termios,
      termios_obj::change_flag flag = termios_obj::change_flag::now) const {
    termios_obj obj;
    /* TODO */ tcsetattr(m_fd, static_cast<int>(flag), &obj);
  }

private:
  int m_fd = -1;
  uint32_t m_flags = 0;
};

struct autoclose_file_descriptor: public file_descriptor {
  using file_descriptor::file_descriptor;

  inline ~autoclose_file_descriptor() {
    close();
  }

};

} // namespace posixx

#endif // POSIXX_FILE_DESCRIPTOR_H
