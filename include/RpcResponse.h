//
// Created by xhy on 2021/11/25.
//

#ifndef SIMPLERPC_RPCRESPONSE_H
#define SIMPLERPC_RPCRESPONSE_H

#include "RpcRequest.h"

struct RpcResponse {
    RPC_STATUS status_code;
    json result;

    std::string ToString() {
        json j;
        j["code"] = status_code;
        if (result.contains("result")) {
            j["result"] = result["result"];
        }
        return j.dump(4);
    }

    static RpcResponse BuildErrorResponse(RPC_STATUS s);
};


#endif //SIMPLERPC_RPCRESPONSE_H
