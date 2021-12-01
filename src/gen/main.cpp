//
// Created by xhy on 2021/11/30.
//
#include <string>
#include "ServiceDefine.h"
#include <vector>
#include <fstream>
#include <filesystem>
#include "RpcServiceGenerator.h"

namespace fs = std::filesystem;

void Usage() {
    fprintf(stderr, "Usage: ./Rpcgen [definition dir] [output dir]");
}

std::vector<ServiceDefine> getDefines(const std::string &root_dir) {
    fs::path p(root_dir);
    if (!fs::exists(p)) {
        fprintf(stderr, "Dict [%s] doesn't exists\n", root_dir.c_str());
        return {};
    }

    std::vector<ServiceDefine> defs;
    for (auto &file: fs::directory_iterator(p)) {
        if (fs::is_regular_file(file) && file.path().extension() == ".json") {
            try {
                ServiceDefine define;
                std::ifstream in(file.path().string());
                json j;
                in >> j;
                if (define.InitFromJson(j)) {
                    printf("Read service [%s]\n", define.service_name_.c_str());
                    defs.push_back(define);
                } else {
                    fprintf(stderr, "Can not Read file [%s]\n", file.path().c_str());
                }
            } catch (std::exception &e) {
                fprintf(stderr, "%s", e.what());
            }
        }
    }
    return defs;
}


void ProcessGeneration(const std::string &def_path, const std::string &out_put_path) {
    auto service = getDefines(def_path);
    std::vector<std::ofstream> of_streams;
    for (auto &ser: service) {
        GenRpcService("Demo", ser);
    }

}

int main(int argc, const char *argv[]) {
//    if (argc != 3) {
//        Usage();
//        return -1;
//    }
    ProcessGeneration("../definitions/", "");
    return 0;
}