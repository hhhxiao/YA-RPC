//
// Created by xhy on 2021/11/20.
//

#include "RpcServer.h"
#include "utils.h"

RpcResponse RpcServer::ConsumeRequest(const RpcRequest &req) {
    auto iter = this->services_.find(req.service_name);
    if (iter == this->services_.end()) {
        return RpcResponse::BuildErrorResponse(SERVICE_NT_FOUND);
    }
    auto service = iter->second;
    return service->call(req);
}

bool RpcServer::AddService(RpcServiceStubInterface *rpcService) {
    if (!rpcService) {
        return false;
    }
    auto iter = this->services_.find(rpcService->service_name());
    if (iter != this->services_.end()) {
        INFO("Service %s has exists!", rpcService->service_name().c_str());
        return false;
    }
    this->services_[rpcService->service_name()] = rpcService;
    return true;
}

void RpcServer::Run() {
    std::transform(this->threads_.begin(), this->threads_.end(), this->threads_.begin(), [this](std::thread */*t*/) {
        return new std::thread([this]() {
            uWS::App().ws<WSData>("/*", {
                    /* Settings */
                    .compression = uWS::SHARED_COMPRESSOR,
                    .maxPayloadLength = 16 * 1024,
                    .idleTimeout = 16,
                    .maxBackpressure = 1 * 1024 * 1024,
                    /* Handlers */
                    .upgrade = nullptr,
                    .open = [](auto */*ws*/) {
                        printf("An Client has connect to this RPC Servier\n");
                    },
                    .message = [this](auto *ws, std::string_view message, uWS::OpCode opCode) {
                        std::string ms = {message.begin(), message.end()};
                        RpcRequest request;
                        if (RpcRequest::FromJsonString(ms, request)) {
                            auto resp = this->ConsumeRequest(request);
                            ws->send(resp.ToString(), opCode);
                        } else {
                            ws->send(RpcResponse::BuildErrorResponse(INVALID_FORMAT).ToString(), opCode);
                        }
                    },
                    .drain = [](auto */*ws*/) {
                        /* Check getBufferedAmount here */
                    },
                    .ping = [](auto */*ws*/, std::string_view) {

                    },
                    .pong = [](auto */*ws*/, std::string_view) {

                    },
                    .close = [](auto */*ws*/, int /*code*/, std::string_view /*message*/) {

                    }
            }).listen(this->port_, [&](auto *listen_socket) {
                if (listen_socket) {
                    std::cout << "Thread " << std::this_thread::get_id() << " listening on port " << this->port_
                              << std::endl;
                } else {
                    std::cout << "Thread " << std::this_thread::get_id() << " failed to listen on port 9001"
                              << std::endl;
                }
            }).run();
        });
    });
    for (auto &t: this->threads_) {
        t->join();
    }
}

RpcServer::RpcServer(size_t work_thead_num, uint16_t port) {
    this->port_ = port;
    this->threads_ = std::vector<std::thread *>(work_thead_num);
}


