//
// Created by xhy on 2021/11/30.
//

#include "RpcServiceGenerator.h"

#include <fstream>
#include <string>
#include "utils.h"

std::string UpperString(const std::string &s) {
    auto res = s;
    const char diff = 'a' - 'A';
    for (char &i: res) {
        if (i >= 'a' && i <= 'z') {
            i = (char) ((int) i - diff);
        }
    }
    return res;
}

std::string LowerString(const std::string &s) {
    auto res = s;
    const char diff = 'a' - 'A';
    for (char &i: res) {
        if (i >= 'A' && i <= 'a') {
            i = (char) ((int) i + diff);
        }
    }
    return res;
}

std::string MethodToFunctionImplemention(const std::string &class_name, const Method &method) {
    std::string d = std::string(ParamToStr(method.return_type)) + " " + class_name + "::" + +" " + method.name + "(";
    for (auto &param: method.params) {
        d += std::string(ParamToStr(param.second)) + " " + param.first + ",";
    }
    d[d.size() - 1] = ')';
    d += "{\n}\n\n";
    return d;
}

std::string MethodToFunctionDeclation(const Method &method) {
    std::string d = std::string(ParamToStr(method.return_type)) + " " + method.name + "(";
    for (auto &param: method.params) {
        d += std::string(ParamToStr(param.second)) + " " + param.first + ",";
    }
    d[d.size() - 1] = ')';
    d += ";";
    return d;
}

std::string MethodToStubFunction(const std::string &service_name, const Method &method) {
    std::string stub_fun_template = R"(
  void %sStub(const json &params, RpcResponse &response) {
        try {
            %s
            response.result["result"] = c;
        } catch (std::exception &e) {
            response.result["result"] = "";
        }
}
)";
    std::string params;
    std::string args;
    for (auto &p: method.params) {
        auto type = ParamToStr(p.second);
        auto name = p.first;
        args += name + ", ";
        params += format(
                R"(     %s %s = params["%s"].get<%s>();
)", type, name.c_str(), name.c_str(), type
        );
    }
    args[args.size() - 2] = ' ';
    params += format(R"( auto c = %sService.%s(%s);)",
                     LowerString(service_name).c_str(),
                     method.name.c_str(), args.c_str()
    );

    return format(stub_fun_template,
                  method.name.c_str(),
                  params.c_str()
    );
}

std::string MethodToRegisterMethods(const Method &method) {
    std::string template_string = R"(
        std::string %s_method_name = "%s";
        Method %s_method;
        %s_method.name = "%s";
        %s_method.params = {
                    %s
        };
        %s_method.return_type = %s;
        this->methods_[%s_method_name] = %s_method;
        this->stubs_["%s"] = [&](const json &req, RpcResponse &response) {
            return this->%sStub(req, response);
        };
)";
    auto method_name = method.name;
    std::string params;
    for (auto &param: method.params) {
        params += format(R"({"%s", %s},
)", param.first.c_str(), ParamToSelfString(param.second));
    }
    if (params.size() > 2)
        params[params.size() - 2] = ' ';
    return format(template_string,
                  method_name.c_str(),
                  method_name.c_str(),
                  method_name.c_str(),
                  method_name.c_str(),
                  method_name.c_str(),
                  method_name.c_str(),
                  params.c_str(),
                  method_name.c_str(),
                  ParamToSelfString(method.return_type),
                  method_name.c_str(),
                  method_name.c_str(),
                  method_name.c_str(),
                  method_name.c_str()
    );
}


std::string ServiceStubFile(const std::string &app_name, const ServiceDefine &define) {
    std::string template_head_str = R"(
#ifndef %s_%sSERVICESTUB_H
#define %s_%sSERVICESTUB_H

#include "%sService.h"
#include "ServiceDefine.h"
#include "RpcServiceStubInterface.h"

)";
    auto upper_app_name = UpperString(app_name);
    auto upper_service_name = UpperString(define.service_name_);
    auto head_str = format(template_head_str,
                           upper_app_name.c_str(),
                           upper_service_name.c_str(),
                           upper_app_name.c_str(),
                           upper_service_name.c_str(),
                           define.service_name_.c_str());

    std::string class_body_template_str = R"(
class %sServiceStub : public RpcServiceStubInterface {
public:

    explicit %sServiceStub() : RpcServiceStubInterface("%s") {
        this->RegisterMethods();
    }
    %s
    void RegisterMethods() {
    %s
    }
private:
    %sService %sService;
};
#endif
)";
    std::string stubFuns;
    std::string registerFuncs;
    for (auto &m: define.methods_) {
        stubFuns += MethodToStubFunction(define.service_name_, m.second) + "\n\n";
        registerFuncs += MethodToRegisterMethods(m.second) + "\n";
    }

    return head_str + format(class_body_template_str,
                             define.service_name_.c_str(), define.service_name_.c_str(), define.service_name_.c_str(),
                             stubFuns.c_str(),
                             registerFuncs.c_str(),
                             define.service_name_.c_str(), LowerString(define.service_name_).c_str()
    );
}

std::string ServiceCPPFile(const ServiceDefine &serviceDefine) {
    std::string s = "#include <string>\n#include \"" + serviceDefine.service_name_ + "Service.h\"\n\n";
    for (auto &methods: serviceDefine.methods_) {
        s += MethodToFunctionImplemention(serviceDefine.service_name_ + "Service", methods.second);
    }
    return s;
}

std::string ServiceHeaderFile(const std::string &app_name, const ServiceDefine &define) {
    std::string template_string = R"(
#ifndef %s_%sSERVICE_H
#define %s_%sSERVICE_H

class %sService {
public:
%s
};
#endif //SIMPLERPC_SAMPLESERVICE_
)";

    std::string method_list;
    for (auto &method: define.methods_) {
        method_list += "    " + MethodToFunctionDeclation(method.second) + "\n\n";
    }
    auto upper_app_name = UpperString(app_name);
    auto upper_service_name = UpperString(define.service_name_);
    return format(
            template_string,
            upper_app_name.c_str(),
            upper_service_name.c_str(),
            upper_app_name.c_str(),
            upper_service_name.c_str(),
            define.service_name_.c_str(),
            method_list.c_str()
    );
}


bool GenRpcService(const std::string &app_name, const ServiceDefine &serviceDefine) {
    auto headerfile = ServiceHeaderFile(app_name, serviceDefine);
    auto cppFile = ServiceCPPFile(serviceDefine);
    auto stubFile = ServiceStubFile(app_name, serviceDefine);
    const std::string file_name = "./services/" + serviceDefine.service_name_ + "Service";
    std::ofstream ocpp(file_name + ".cpp");
    ocpp << cppFile;
    ocpp.close();
    std::ofstream oh(file_name + ".h");
    oh << headerfile;
    oh.close();
    std::ofstream ostub(file_name + "Stub.h");
    ostub << stubFile;
    ostub.close();
    return false;
}
