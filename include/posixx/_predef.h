//
// Created by Jacob Scaggs on 4/16/23.
//

#ifndef POSIXX_PREDEF_H
#define POSIXX_PREDEF_H

#include <cstring>
#include <cerrno>
#include <system_error>
#include <concepts>


namespace posixx {

    struct error_state {
        errno_t error_no = 0;
        int state = 0;

        inline error_state() : state(0), error_no(0) {}

        inline error_state(int s) : state(s), error_no(0) {
            if (s < 0) error_no = errno;
        }

        inline error_state &operator=(const int &other) {
            state = other;
            if (other < 0) error_no = errno;
            return *this;
        }

        inline operator bool() {
            return state >= 0;
        }

        [[nodiscard]] inline std::system_error as_system_error() const {
            return {error_no, std::system_category()};
        }
    };


    struct error_or_size {
        ssize_t size;

        inline error_or_size(ssize_t i) : size(i) {}

        inline error_or_size &operator=(ssize_t i) {
            size = i;
            return *this;
        }

        [[nodiscard]] inline bool is_error() const {
            return size < 0;
        }
    };

    template<class T>
    struct error_or : std::variant<T, error_state> {
        template<class Fn, class... Args>
        inline static error_or track(Fn fn, Args... args) {
            error_or obj{};
            int result = 0;
            result = fn(args..., &std::get<T>(obj));
            if (result < 0) {
                obj.template emplace<error_state>(result);
            }
            return std::move(obj);
        }

        inline operator bool() {
            return std::holds_alternative<T>(this);
        }

        T& operator*() {
            return std::get<T>(*this);
        }

        T* operator->() {
            return &std::get<T>(*this);
        }

    };

// Concepts

    template<class T>
    concept sized = requires(const T &t) {
        { t.size() } -> std::convertible_to<size_t>;
    };

    template<class T>
    concept writable = sized<T> && requires(const T &t) {
        typename T::value_type;
        { t.data() } -> std::convertible_to<const typename T::value_type *>;
    };

    template<class T>
    concept readable = sized<T> && requires(T &t) {
        typename T::value_type;
        { t.data() } -> std::convertible_to<typename T::value_type *>;
    };

    template<class T>
    concept resizable = sized<T> && requires(T &t, size_t x) {
        typename T::value_type;
        { t.resize(x) } -> std::convertible_to<void>;
    };

}

#endif //POSIXX_PREDEF_H
