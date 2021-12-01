//
// Created by xhy on 2021/11/20.
//

#ifndef SIMPLERPC_RPCREQUEST_H
#define SIMPLERPC_RPCREQUEST_H

#include <unordered_map>
#include <string>
#include "json.hpp"
#include "utils.h"

enum RPC_STATUS {
    SUCCESS = 0,
    INVALID_FORMAT = 1,
    SERVICE_NT_FOUND = 2,
    METHOD_NT_FOUND = 3,
    AUTH_FALIURE = 4,
    CAL_TIME_OUT = 5
};

struct RpcRequest {

    int version;
    std::string client;
    std::string service_name;
    std::string method_name;
    json params;

    void Dump() const;;

    static bool FromJsonString(const std::string &str, RpcRequest &req);
};


#endif //SIMPLERPC_RPCREQUEST_H
