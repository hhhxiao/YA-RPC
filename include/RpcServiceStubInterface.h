//
// Created by xhy on 2021/11/20.
//

#ifndef SIMPLERPC_RPCSERVICESTUBINTERFACE_H
#define SIMPLERPC_RPCSERVICESTUBINTERFACE_H

#include <string>
#include <utility>
#include "RpcRequest.h"
#include "RpcResponse.h"
#include "ServiceDefine.h"

class RpcServiceStubInterface {
public:
    RpcResponse call(const RpcRequest &rpcRequest) {
        rpcRequest.Dump();
        RpcResponse response;
        auto iter = this->stubs_.find(rpcRequest.method_name);
        if (iter == this->stubs_.end()) {
            response.status_code = METHOD_NT_FOUND;
            response.result["result"] = "";
            return response;
        }
        auto stub = iter->second;
        stub(rpcRequest.params, response);
        return response;
    };

    std::string service_name() const {
        return this->service_name_;
    }

    explicit RpcServiceStubInterface(std::string name) : service_name_(std::move(name)) {
    }

protected:
    std::unordered_map<std::string, std::function<void(const json &, RpcResponse &resp)>> stubs_;
    std::unordered_map<std::string, Method> methods_;
    std::string service_name_;
};


#endif //SIMPLERPC_RPCSERVICESTUBINTERFACE_H
