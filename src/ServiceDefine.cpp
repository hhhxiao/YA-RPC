//
// Created by xhy on 2021/11/3.
//
#include "ServiceDefine.h"

const int ServiceDefine::VERSION_NUMBER = 0;

const char *ParamToStr(ParamType type) {
    switch (type) {
        case INT:
            return "int";
        case STRING:
            return "std::string";
        case FLOAT:
            return "float";
        case BOOL:
            return "bool";
        case NONE:
            return "unknown";
    }
    return "unknown";
}

const char *ParamToSelfString(ParamType type) {
    switch (type) {
        case INT:
            return "ParamType::INT";
        case STRING:
            return "ParamType::STRING";
        case FLOAT:
            return "ParamType::FLOAT";
        case BOOL:
            return "ParamType::BOOL";
        case NONE:
            return "ParamType::NONE";
    }
    return "ParamType::NONE";
}

void ServiceDefine::Dump() const {
    printf("Service %s\n", this->service_name_.c_str());
    int i = 0;
    for (auto &item: this->methods_) {
        printf("[%d]Method name:   %s\n", i, item.first.c_str());
        auto &method = item.second;
        for (auto &param: method.params) {
            printf("%s: %s\n", param.first.c_str(), ParamToStr(param.second));
        }
        printf("Return type: %s\n------------------\n", ParamToStr(method.return_type));
        ++i;
    }
}

bool ServiceDefine::InitFromJson(const json &j) {
    try {
        this->version_ = j["version"].get<int>();
        this->service_name_ = j["name"].get<std::string>();
        auto methods_json = j["methods"];
        if (!methods_json.is_array()) {
            printf("empty service");
            return false;
        }
        for (auto &mj: methods_json) {
            Method method;
            method.name = mj["name"].get<std::string>();
            auto params = mj["params"];
            for (auto &[key, value]: params.items()) {
                ParamType t = JsonTypeToParam(value.type());
                if (t == NONE) {
                    printf("error when parse json");
                    return false;
                }
                method.params[key] = t;
                method.return_type = JsonTypeToParam(mj["return"]);
            }
            this->methods_[method.name] = method;
        }
    } catch (std::exception &e) {
        fprintf(stderr, "%s", e.what());
        return false;
    }
    return true;
}

ParamType ServiceDefine::JsonTypeToParam(nlohmann::detail::value_t v) {
    static const std::unordered_map<nlohmann::detail::value_t, ParamType> map{
            {nlohmann::detail::value_t::string,          STRING},
            {nlohmann::detail::value_t::number_unsigned, INT},
            {nlohmann::detail::value_t::number_integer,  INT},
            {nlohmann::detail::value_t::number_float,    FLOAT},
            {nlohmann::detail::value_t::boolean,         BOOL}
    };

    auto iter = map.find(v);
    if (iter != map.end())return iter->second;
    return ParamType::NONE;
}