#include "App.h"
#include "logger.h"

int main(void)
{
    App app;

    LOG_DEBUG("App running start");
    app.run();
    LOG_DEBUG("App running complete");

    return (0);
}
