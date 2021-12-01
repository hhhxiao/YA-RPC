//
// Created by xhy on 2021/10/27.
//

#ifndef SIMPLERPC_SERVICEDEFINE_H
#define SIMPLERPC_SERVICEDEFINE_H

#include <string>
#include "json.hpp"
#include <unordered_map>

using json = nlohmann::json;

enum ParamType {
    INT,
    STRING,
    FLOAT,
    BOOL,
    NONE
};

const char *ParamToStr(ParamType type);

const char *ParamToSelfString(ParamType type);

struct Method {
    std::string name;
    std::unordered_map<std::string, ParamType> params;
    ParamType return_type = NONE;
};


class ServiceDefine {
public:
    static const int VERSION_NUMBER;
    int version_;
    std::string service_name_;
    std::unordered_map<std::string, Method> methods_;

    static ParamType JsonTypeToParam(nlohmann::detail::value_t v);

    // std::unordered_map<std::string, Method> methods_();

    void Dump() const;

    bool InitFromJson(const json &j);

};


#endif //SIMPLERPC_SERVICEDEFINE_H
