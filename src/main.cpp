#include <iostream>
#include "app/application.h"
#include "exceptions/baseException.h"

int main() {
    try {
        Application app;
        app.run();
    } catch (const BaseException &e) {
        std::cerr << e.what();
        return 1;
    }
    return 0;
}
