
#ifndef DEMO_STRINGSERVICESTUB_H
#define DEMO_STRINGSERVICESTUB_H

#include "StringService.h"
#include "ServiceDefine.h"
#include "RpcServiceStubInterface.h"


class StringServiceStub : public RpcServiceStubInterface {
public:

    explicit StringServiceStub() : RpcServiceStubInterface("String") {
        this->RegisterMethods();
    }

    void LenStub(const json &params, RpcResponse &response) {
        try {
            std::string str = params["str"].get<std::string>();
            auto c = stringService.Len(str);
            response.result["result"] = c;
        } catch (std::exception &e) {
            response.result["result"] = "";
        }
    }

    void RegisterMethods() {

        std::string Len_method_name = "Len";
        Method Len_method;
        Len_method.name = "Len";
        Len_method.params = {
                {"str", ParamType::STRING}
        };
        Len_method.return_type = ParamType::INT;
        this->methods_[Len_method_name] = Len_method;
        this->stubs_["Len"] = [&](const json &req, RpcResponse &response) {
            return this->LenStub(req, response);
        };


    }

private:
    StringService stringService;
};

#endif
