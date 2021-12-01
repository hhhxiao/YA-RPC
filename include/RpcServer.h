//
// Created by xhy on 2021/11/20.
//

#ifndef SIMPLERPC_RPCSERVER_H
#define SIMPLERPC_RPCSERVER_H

#include <unordered_map>
#include <string>
#include "RpcServiceStubInterface.h"
#include "RpcRequest.h"
#include "uWebSockets/App.h"

struct WSData {
};

class RpcServer {
public:

    void Run();

    RpcServer(size_t work_thead_num, uint16_t port);

protected:

    bool AddService(RpcServiceStubInterface *rpcService);

    RpcResponse ConsumeRequest(const RpcRequest &req);

private:
    std::vector<std::thread *> threads_;
    uint16_t port_ = 9001;
    std::unordered_map<std::string, RpcServiceStubInterface *> services_;
};


#endif //SIMPLERPC_RPCSERVER_H
