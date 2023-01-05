#include "controller.hxx"
#include <iostream>
#include <stdexcept>

int
main()
{
    ////    You can also provide an explicit list of words:

    try {
        Controller().run();
    }

    catch (std::runtime_error const& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
}