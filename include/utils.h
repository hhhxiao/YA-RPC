//
// Created by xhy on 2021/11/20.
//

#ifndef SIMPLERPC_UTILS_H
#define SIMPLERPC_UTILS_H

#include "json.hpp"

using json = nlohmann::json;


#define DEBUG
#ifdef WIN32
#define FN (__builtin_strrchr(__FILE__, '\\') ? __builtin_strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
#define FN (__builtin_strrchr(__FILE__, '/') ? __builtin_strrchr(__FILE__, '/') + 1 : __FILE__)
#endif
#define LOG(...) log_info(FN,__FUNCTION__,__LINE__,__VA_ARGS__)
#define  ERR(...) fprintf(stderr,__VA_ARGS__)
#define  INFO(...) fprintf(stdout,__VA_ARGS__)

void log_info(const char *file_name, const char *function_name, size_t line, const char *fmt, ...);

template<typename... Args>
std::string format(const std::string &format, Args... args) {
    int size = snprintf(nullptr, 0, format.c_str(), args...) +
               1;  // Extra space for '\0'
    if (size <= 0) return "";
    std::unique_ptr<char[]> buf(new char[size]);
    snprintf(buf.get(), size, format.c_str(), args...);
    return std::string(
            buf.get(), buf.get() + size - 1);  // We don't want the '\0' inside
}

#endif //SIMPLERPC_UTILS_H
