//
// Created by xhy on 2021/11/25.
//

#include "RpcResponse.h"

RpcResponse RpcResponse::BuildErrorResponse(RPC_STATUS s) {
    RpcResponse response;
    response.status_code = s;
    response.result["result"] = "";
    return response;
}

