//
// Created by xhy on 2021/11/30.
//

#ifndef SIMPLERPC_TESTRPCSERVER_H
#define SIMPLERPC_TESTRPCSERVER_H

#include "RpcServer.h"
#include "SampleServiceStub.h"
#include "StringServiceStub.h"
#include "ArcServiceStub.h"

class TestRPCServer : public RpcServer {
public:
    TestRPCServer(size_t work_thead_num, uint16_t port) : RpcServer(work_thead_num, port) {
        this->InitService();
    }

private:
    void InitService() {
        this->AddService(new SampleServiceStub());
        this->AddService(new ArcServiceStub());
        this->AddService(new StringServiceStub());
    }
};


#endif //SIMPLERPC_TESTRPCSERVER_H
