//
// Created by xhy on 2021/10/27.
//

#ifndef SIMPLERPC_SAMPLESERVICESTUB_H
#define SIMPLERPC_SAMPLESERVICESTUB_H

#include "SampleService.h"
#include "ServiceDefine.h"
#include "RpcServiceStubInterface.h"

class SampleServiceStub : public RpcServiceStubInterface {
public:

    explicit SampleServiceStub() : RpcServiceStubInterface("Sample") {
        this->RegisterMethods();
    }

    void AddStub(const json &params, RpcResponse &response) {
        //get
        try {
            int a = params["a"].get<int>();
            int b = params["b"].get<int>();
            auto c = sampleService.Add(a, b);
            response.result["result"] = c;
            INFO("c = %d\n", c);
        } catch (std::exception &e) {
            response.result["result"] = "";
        }
    }

    void MulStub(const json &params, RpcResponse &response) {
        try {
            int a = params["a"].get<int>();
            int b = params["b"].get<int>();
            int c = sampleService.Mul(2, 1);
            response.result["result"] = c;
        } catch (std::exception &e) {
            response.result["result"] = "";
        }
    }

    void RegisterMethods() {

        std::string Add_method_name = "Add";
        Method Add_method;
        Add_method.name = "Add";
        Add_method.params = {
                {"a", ParamType::INT},
                {"b", ParamType::INT}
        };
        Add_method.return_type = ParamType::INT;
        this->methods_[Add_method_name] = Add_method;
        this->stubs_["Add"] = [&](const json &req, RpcResponse &response) {
            return this->AddStub(req, response);
        };


        std::string Mul_method_name = "Mul";
        Method Mul_method;
        Mul_method.name = "Mul";
        Add_method.params = {
                {"a", ParamType::INT},
                {"b", ParamType::INT}
        };
        Add_method.return_type = ParamType::INT;
        this->methods_[Mul_method_name] = Mul_method;
        this->stubs_["Mul"] = [&](const json &req, RpcResponse &response) {
            return this->MulStub(req, response);
        };
    }

private:
    SampleService sampleService;
};

#endif //SIMPLERPC_SAMPLESERVICESTUB_H
