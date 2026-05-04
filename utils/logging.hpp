#pragma once

#include <iostream>
#include <string>

void log_err(std::string msg) {
    std::cerr << "\x1b[1;31m[ERROR]: \x1b[22mm" << msg << "\x1b[0m" << std::endl;
}

void log_warn(std::string msg) {
    std::cout << "\x1b[1;33m[WARN]: \x1b[22mm" << msg << "\x1b[0m" << std::endl;
}

void log(std::string msg) {
    std::cout << "\x1b[1;38;5;117m[INFO]: \x1b[22m" << msg << "\x1b[0m" << std::endl;
}

