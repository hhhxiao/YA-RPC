//
// Created by xhy on 2021/11/25.
//
#include "RpcRequest.h"

bool RpcRequest::FromJsonString(const std::string &str, RpcRequest &req) {
    try {
        json j = json::parse(str);
        req.method_name = j["method"];
        req.service_name = j["service"];
        req.params = j["params"];
    } catch (std::exception &e) {
        return false;
    }
    return true;
}

void RpcRequest::Dump() const {
    printf("Client id: %s\n", this->client.c_str());
    printf("Service: %s\n", this->service_name.c_str());
    printf("Method name: %s\n", this->method_name.c_str());
    printf("Params: %s", this->params.dump(4).c_str());
}

