#include "TestRPCServer.h"

int main() {
    TestRPCServer rpcServer(4, 9000);
    rpcServer.Run();
    return 0;
}

