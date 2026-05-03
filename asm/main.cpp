#include <cstdint>
#include <iostream>
#include "../utils/constants.hpp"
#include "../utils/helpers.hpp"
#include <string>
#include <vector>
#include <fstream>
#include <bits/stdc++.h>
#include <algorithm>
#include <cctype>


namespace fm_asm {
    using namespace consts;
    using namespace helpers;

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
        if (code.empty()) return "";

        std::vector<std::string> split_code = split_lines(code);
        std::string out = "";

        for (std::string str : split_code) {
            for (char c : str) {
                if (c == ';') break;

                if (c == ' ') {
                    if (!out.empty() && out.back() != ' ' && out.back() != '\n') {
                        out += c;
                    }
                }
                else {
                    out += c;
                }
            }
            if (!out.empty() && out.back() != '\n') {
                out += '\n';
            }
        }
        return out;
    }

    raw_program split_sections(const std::string code) {
    	std::string formatted_code = remove_comments(code);

        std::vector<std::string> tokens = split_whitespace(formatted_code);

        std::vector<Section> encountered_sections;

        Section section = Section::none;

        raw_program prog;

        std::string prev = "";
        for (std::string tok : tokens) {
            std::transform(tok.begin(), tok.end(), tok.begin(),
                [](unsigned char c){ return std::tolower(c); });

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
            else if (tok != "section") {
                switch (section) {
                    case (Section::setup): {
                        prog.raw_setup += tok + " ";
                        break;
                    }
                    case (Section::data): {
                        prog.raw_data += tok + " ";
                        break;
                    }
                    case (Section::text): {
                        prog.raw_text += tok + " ";
                        break;
                    }
                }
            }

            prev = tok;
        }

        // Remove double-spaces
        prog.raw_text = remove_comments(prog.raw_text);
        prog.raw_data = remove_comments(prog.raw_data);
        prog.raw_setup = remove_comments(prog.raw_setup);

        return prog;
    }

    data_section parse_data(const std::string code) {
        std::vector<std::string> tokens = split_whitespace(code);

        data_section section;

        std::string name;

        size_t idx = 0;
        // Error checking
        for (std::string tok : tokens) {
            if (idx % 2 == 0) {
                if (!is_alpha(tok)) {
                    std::cerr << "Error: Invalid variable name! Must only contain alphabetic characters! (\"" << tok << "\")" << std::endl;
                    return;
                }
            }
            else {
                if (!is_alnum(tok)) {
                    std::cerr << "Error: Invalid size! \"" << tok << "\"";
                    return;
                }
            }
            idx++;
        }
        size_t idx = 0;

        // Actual parsing
        for (std::string tok : tokens) {
            if (idx % 2 == 0) {
                name = tok;
            }
            else {
                var curr_var = {
                    .name = name,
                    .size = to_int(get_base(tok))
                };
                section.vars.push_back(curr_var);
            }
        }
        return section;
    }
};

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Filename required!" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1]);

    if (!file.is_open()) {
        std::cerr << "Failed to open file!" << std::endl;
        return 1;
    }

    std::string code;
    std::string line;
    while (getline(file, line)) {
    	code += line + '\n';
    }

    file.close();

    fm_asm::raw_program split_code = fm_asm::split_sections(code);

    std::cout << split_code.raw_setup << std::endl;
    std::cout << split_code.raw_data << std::endl;
    std::cout << split_code.raw_text << std::endl;
    return 0;
}
