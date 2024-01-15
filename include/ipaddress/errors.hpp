#ifndef IPADDRESS_ERRORS_HPP
#define IPADDRESS_ERRORS_HPP

#ifndef IPADDRESS_NO_EXCEPTIONS

#include "config.hpp"

namespace IPADDRESS_NAMESPACE {

class error : public std::runtime_error {
public:
    template <typename FirstArg, typename... Args>
    explicit error(const FirstArg& arg, const Args&... args) : std::runtime_error(concatenate(arg, args...)) {
    }

    explicit error(const std::string& message) : std::runtime_error(message) {
    }

    explicit error(const char* message) : std::runtime_error(message) {
    }
    
private:
    template <typename... Args>
    static std::string concatenate(const Args&... args) {
        std::ostringstream ss;
        print(ss, args...);
        return ss.str();
    }

    template <typename FirstArg, typename... Args>
    static void print(std::ostringstream& out, const FirstArg& arg, const Args&... args) {
        out << arg << ' ';
        print(out, args...);
    }

    template <typename FirstArg>
    static void print(std::ostringstream& out, const FirstArg& arg) {
        out << arg;
    }
};

class parse_error : public error {
public:
    template <typename FirstArg, typename... Args>
    explicit parse_error(const FirstArg& arg, const Args&... args) : error(arg, args...) {
    }

    explicit parse_error(const std::string& message) : error(message) {
    }

    explicit parse_error(const char* message) : error(message) {
    }
};

}

#endif

#endif
