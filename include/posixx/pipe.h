//
// Created by Jacob Scaggs on 4/16/23.
//

#ifndef POSIXX_PIPE_H
#define POSIXX_PIPE_H

#include <system_error>
#include <unistd.h>

namespace posixx {


    class pipe {
    public:
        inline pipe() noexcept = default;

        inline void open() {
            int e = ::pipe(m_fds);
            if (e < 0) {
                m_fds[0] = -1;
                m_fds[1] = -1;
                throw std::system_error(errno, std::system_category());
            }
        }

        inline std::pair<int, int> as_pair() { return {m_fds[0], m_fds[1]}; };

    private:
        int m_fds[2] = {-1, -1};
    };

}

#endif //POSIXX_PIPE_H
