
#ifndef DEMO_ARCSERVICESTUB_H
#define DEMO_ARCSERVICESTUB_H

#include "ArcService.h"
#include "ServiceDefine.h"
#include "RpcServiceStubInterface.h"


class ArcServiceStub : public RpcServiceStubInterface {
public:

    explicit ArcServiceStub() : RpcServiceStubInterface("Arc") {
        this->RegisterMethods();
    }
    
  void SubStub(const json &params, RpcResponse &response) {
        try {
                 int b = params["b"].get<int>();
     int a = params["a"].get<int>();
 auto c = arcService.Sub(b, a  );
            response.result["result"] = c;
        } catch (std::exception &e) {
            response.result["result"] = "";
        }
}



  void MulStub(const json &params, RpcResponse &response) {
        try {
                 int b = params["b"].get<int>();
     int a = params["a"].get<int>();
 auto c = arcService.Mul(b, a  );
            response.result["result"] = c;
        } catch (std::exception &e) {
            response.result["result"] = "";
        }
}



  void AddStub(const json &params, RpcResponse &response) {
        try {
                 int b = params["b"].get<int>();
     int a = params["a"].get<int>();
 auto c = arcService.Add(b, a  );
            response.result["result"] = c;
        } catch (std::exception &e) {
            response.result["result"] = "";
        }
}



    void RegisterMethods() {
    
        std::string Sub_method_name = "Sub";
        Method Sub_method;
        Sub_method.name = "Sub";
        Sub_method.params = {
                    {"b", ParamType::INT},
{"a", ParamType::INT} 

        };
        Sub_method.return_type = ParamType::INT;
        this->methods_[Sub_method_name] = Sub_method;
        this->stubs_["Sub"] = [&](const json &req, RpcResponse &response) {
            return this->SubStub(req, response);
        };


        std::string Mul_method_name = "Mul";
        Method Mul_method;
        Mul_method.name = "Mul";
        Mul_method.params = {
                    {"b", ParamType::INT},
{"a", ParamType::INT} 

        };
        Mul_method.return_type = ParamType::INT;
        this->methods_[Mul_method_name] = Mul_method;
        this->stubs_["Mul"] = [&](const json &req, RpcResponse &response) {
            return this->MulStub(req, response);
        };


        std::string Add_method_name = "Add";
        Method Add_method;
        Add_method.name = "Add";
        Add_method.params = {
                    {"b", ParamType::INT},
{"a", ParamType::INT} 

        };
        Add_method.return_type = ParamType::INT;
        this->methods_[Add_method_name] = Add_method;
        this->stubs_["Add"] = [&](const json &req, RpcResponse &response) {
            return this->AddStub(req, response);
        };


    }
private:
    ArcService arcService;
};
#endif
