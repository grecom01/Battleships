#include "Server.h"

int main()
{
    std::unique_ptr<Server> server = std::make_unique<Server>(33333);
    server->run();

    return 0;
}
