#include <iostream>
#include "../utils/constants.hpp"
#include "../utils/helpers.hpp"
#include <string>
#include <vector>

namespace fm_asm {
    using namespace consts;

    std::string remove_comments(const std::string code) {
        std::vector<std::string> split_code = split_lines(code);

        std::string out = "";

        for (std::string str : split_code) {
            for (char c : str) {
                if (c == ';') {
                    break;
                }
                if (c == ' ' && (out.back() != ' ')) {
                    out += c;
                }
                else if (c != ' ') {
                    out += c;
                }
            }
            out += '\n';
        }
        return out;
    }
};

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Filename required!" << std::endl;
    }
    return 0;
}
