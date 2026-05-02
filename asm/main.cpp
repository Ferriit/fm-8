#include <cstdint>
#include <iostream>
#include "../utils/constants.hpp"
#include "../utils/helpers.hpp"
#include <string>
#include <vector>
#include <fstream>
#include <bits/stdc++.h>

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
    	std::string raw_setup;
     	std::string raw_data;
      	std::string raw_text;
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

    raw_program tokenize(const std::string code) {
    	std::string formatted_code = remove_comments(code);

        std::vector<std::string> tokens = split_whitespace(formatted_code);

        std::vector<Section> encountered_sections;

        Section section;

        raw_program prog;

        std::string prev = "";
        for (std::string tok : tokens) {
            // Update Section
            if (((tok == ".text") || (tok == ".data") || (tok == ".setup")) && (prev == "section")) {
                if (tok == ".text") {
                    section = Section::text;
                }
                else if (tok == ".data") {
                    section = Section::data;
                }
                else if (tok == ".setup") {
                    section = Section::setup;
                }

                encountered_sections.push_back(section);

                // Check if section already has been encountered.
                if (std::count(encountered_sections.begin(), encountered_sections.end(), section) > 1) {
                    std::cout << "Panic on token \"" << tok << "\"!\n\nExpected new section. Got \"" << tok << "\"." << std::endl;
                }
            }

            else if (section == Section::setup) {

            }
            prev = tok;
        }
        return prog;
    }
};

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Filename required!" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1]);

    if (!file.is_open()) {
        std::cout << "Unable to open file!" << std::endl;
        return 1;
    }

    std::string code;
    std::string line;
    while (getline(file, line)) {
    	code += line + '\n';
    }

    file.close();

    std::cout << fm_asm::tokenize(code).raw_setup << std::endl;
    return 0;
}
