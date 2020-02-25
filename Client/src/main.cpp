#include "Application.h"

int main()
{
    std::unique_ptr<Application> app = std::make_unique<Application>("Battleships", 724, 543);
    app->run();

    return 0;
}
