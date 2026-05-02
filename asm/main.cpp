#include <cstdint>
#include <iostream>
#include "../utils/constants.hpp"
#include "../utils/helpers.hpp"
#include <string>
#include <vector>
#include <fstream>

namespace fm_asm {
    using namespace consts;

    struct instr {
    	OpCode opcode;
    	uint8_t val1, val2;
     	Reg reg1, reg2;
    };

    struct var {
    	std::string name;
     	uint8_t size;
    };


    struct setup_section {
    	uint16_t nmi_addr;
     	uint16_t irq_addr;
      	uint16_t start_addr;
    };

    struct data_section {
    	std::vector<var> vars;
    };

    struct text_section {
    	std::vector<instr> instructions;
    };


    struct program {
    	setup_section setup;
     	data_section data;
      	text_section text;
    };

    struct raw_program {
    	std::string setup;
     	std::string data;
      	std::string text;
    };


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
        return 1;
    }

    std::ifstream file(argv[1]);

    std::string code;
    std::string line;
    while (getline(file, line)) {
    	code += line + '\n';
    }

    file.close();

    std::cout << fm_asm::remove_comments(code) << std::endl;
    return 0;
}
