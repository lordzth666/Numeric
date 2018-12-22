//
// Created by lordzth on 12/1/18.
//

#include "../include/Workspace.hpp"

#define DEBUG

int main() {
    std::cout << "[GCC 7.3.0] on linux2" << std::endl;
    std::cout << "Prebuilt on linux2 (Dec 5 2018, 00:30:00)" << std::endl;
    Numeric::Workspace::Workspace<double> workspace;
    std::string line;
    long cnt = 0;
    while (cnt++ < 100) {
        std::cout << "# ";
        std::getline(std::cin, line);
        if (!std::cin.good()) {
            std::cout << std::endl;
            break;
        }
        if (line.empty())
            continue;
        std::cout << line << std::endl;
        workspace.ParseCommand(line);
    }


    return 0;
}
