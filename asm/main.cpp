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
#include "../utils/tokenscanner.hpp"


namespace fm_asm {
    using namespace consts;
    using namespace helpers;

    struct map_vector {
        std::vector<std::string> vec;
        std::unordered_map<std::string, uint16_t> map;
    };

    struct instr {
    	OpCode opcode;
    	uint8_t field1, field2;
    };

    struct setup_section {
    	uint16_t nmi_addr;
     	uint16_t irq_addr;
      	uint16_t start_addr;
    };

    struct data_section {
    	map_vector vars;
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

    int get_instruction_size(OpCode op) {
        auto& ops = operand_map[op];
        int size = 1; // Opcode itself
        for (auto type : ops) {
            if (type == OperandType::LABL || type == OperandType::VAR) size += 2;
            else if (type != OperandType::NONE) size += 1;
        }
        return size;
    }

    data_section parse_data(const std::string code) {
        std::vector<std::string> tokens = split_whitespace(code);
        data_section section;
        uint16_t current_offset = 0;

        for (size_t i = 0; i + 1 < tokens.size(); i += 2) {
            std::string name = tokens[i];
            uint16_t size = to_int(get_base(tokens[i+1]));

            auto it = std::find(section.vars.vec.begin(), section.vars.vec.end(), name);

            if (it != section.vars.vec.end()) {
                section.vars.vec.push_back(name);
                section.vars.map[name] = current_offset;
            }
            else {
                std::cerr << "Variable \"" << name << "\" declared more than once!" << std::endl;
                break;
            }

            current_offset += size;

            if (current_offset > 256) {
                std::cerr << "Data section overflow!" << std::endl;
                break;
            }
        }
        return section;
    }

    instr parse_instruction(TokenScanner &scanner, map_vector labels, map_vector vars) {
        std::string mnemonic = scanner.next(); // Consume the mnemonic immediately
        instr instruction;
        
        OpCode opcode = op_map[mnemonic];
        instruction.opcode = opcode;
        std::vector<OperandType> operands = operand_map[opcode];
    
        if (!operands.empty() && operands[0] != OperandType::NONE) {
            std::string tok = scanner.next(); 
            
            switch (operands[0]) {
                case OperandType::VAL:  instruction.field1 = to_int(get_base(tok)); break;
                case OperandType::REG:  instruction.field1 = reg_map[tok]; break;
                case OperandType::VAR:  instruction.field1 = vars.map[tok]; break;
                case OperandType::LABL: {
                    uint16_t addr = labels.map.count(tok) ? labels.map[tok] : to_int(get_base(tok));
                    instruction.field1 = (addr >> 8) & 0xFF;
                    instruction.field2 = addr & 0xFF;
                    return instruction;
                }
            }
        }
    
        // Handle Second Operand
        if (operands.size() == 2) {
            scanner.expect(",");
            std::string tok = scanner.next();
        
            switch (operands[1]) {
                case OperandType::VAL: instruction.field2 = to_int(get_base(tok)); break;
                case OperandType::REG: instruction.field2 = reg_map[tok]; break;
                case OperandType::VAR: instruction.field2 = vars.map[tok]; break;
            }
        }
    
        return instruction;
    }

    map_vector get_labels(const std::vector<std::string> tokens) {
        map_vector labels;

        const std::vector<OpCode> jumps = {
            JMP,
            BA,
            BEQ,
            BNE,
            BGT,
            BLT,
            BGE,
            BLE,
            BHI,
            BLO,
            BMI,
            BPL,
            BVS,
            BVC,
            BHS,
            BLS,
            CALL,
        };

        uint16_t address = 0x001;   // 0xFF is the last address of the first page. Execution starts the second page.

        bool prev_jump = false;

        for (std::string tok : tokens) {
            if (tok.empty()) continue; // Safety check

            auto it = std::find(jumps.begin(), jumps.end(), op_map[tok]);
            bool jump_instr = (it != jumps.end());

            // Check for label
            if (tok.back() == ':') {
                std::string name = tok.substr(0, tok.length() - 1);
                labels.vec.push_back(name);
                labels.map[name] = address;
                continue; 
            }

            // Jumps are the only instructions to have an operand that takes up two bytes
            if (!prev_jump) {
                address++;
            } else {
                address += 2;
            }

            prev_jump = jump_instr;
        }
        return labels;
    }

    text_section parse_text(const std::string code, data_section data, map_vector labels) {
        std::vector<std::string> tokens = split_tokens(code);

        text_section section;
        TokenScanner scanner(tokens);

        while (!scanner.is_at_end()) {
            std::string tok = scanner.peek();

            if (!tok.empty() && tok.back() == ':') {
                scanner.next();
                continue;
            }

            if (tok.empty()) {
                scanner.next();
                continue;
            }

            try {
                instr instruction = parse_instruction(scanner, labels, data.vars);
                section.instructions.push_back(instruction);
            } catch (const std::exception& e) {
                std::cerr << "Parsing Error: " << e.what() << " at token: " << tok << std::endl;
                break; 
            }
        }

        return section;
    }

    setup_section parse_setup(const std::string code, map_vector labels) {
        std::vector<std::string> tokens = split_whitespace(code);

        std::string prev = "";

        std::string nmi = "";
        std::string irq = "";
        std::string rst = "";

        // Find labels
        for (std::string tok : tokens) {
            auto it = std::find(setup_list.begin(), setup_list.end(), prev);

            if (it != setup_list.end()) {
                if (prev == "nmi") {
                    nmi = tok;
                }
                else if (prev == "irq") {
                    irq = tok;
                }
                else if (prev == "rst") {
                    rst = tok;
                }
            }

            prev = tok;
        }

        // Check for label existence
        auto nmi_it = std::find(labels.vec.begin(), labels.vec.end(), nmi);
        auto irq_it = std::find(labels.vec.begin(), labels.vec.end(), irq);
        auto start_it = std::find(labels.vec.begin(), labels.vec.end(), rst);

        if (nmi_it == labels.vec.end()) {
            std::cerr << "Invalid NMI Vector! Label \"" << nmi << "\" doesn't exist!" << std::endl;
            return (setup_section){};
        }
        if (irq_it == labels.vec.end()) {
            std::cerr << "Invalid IRQ Vector! Label \"" << irq << "\" doesn't exist!" << std::endl;
            return (setup_section){};
        }
        if (start_it == labels.vec.end()) {
            std::cerr << "Invalid RST Vector! Label \"" << rst << "\" doesn't exist!" << std::endl;
            return (setup_section){};
        }

        // Construct section thingy
        setup_section Section;
        Section.nmi_addr = labels.map[nmi];
        Section.irq_addr = labels.map[irq];
        Section.start_addr = labels.map[rst];

        return Section;
    }

    program parse_program(const std::string code) {
        raw_program raw = split_sections(code);

        std::vector<std::string> text_tokens = split_whitespace(raw.raw_text);
        map_vector labels = get_labels(text_tokens);

        setup_section setup = parse_setup(raw.raw_setup, labels); 
        data_section data = parse_data(raw.raw_data);
        text_section text = parse_text(raw.raw_text, data, labels);

        return {setup, data, text};
    }


    void push_16_t(std::vector<uint8_t> &bytes, uint16_t elem) {
        uint8_t upper = (elem >> 8) & 0xFF;
        uint8_t lower = elem & 0xFF;

        bytes.push_back(upper);
        bytes.push_back(lower);
    }


    std::vector<uint8_t> generate_assembly(const std::string code) {
        program prog = parse_program(code);

        std::vector<uint8_t> bytes;

        push_16_t(bytes, prog.setup.nmi_addr);
        push_16_t(bytes, prog.setup.irq_addr);
        push_16_t(bytes, prog.setup.start_addr);

        for (int i = 0; i < prog.text.instructions.size(); i++) {
            std::vector<OperandType> operands = operand_map[prog.text.instructions[i].opcode];

            bytes.push_back(prog.text.instructions[i].opcode);
            if (operands[0] != OperandType::NONE) {
                bytes.push_back(prog.text.instructions[i].field1);
            }
            if (operands[0] == OperandType::LABL) {
                bytes.push_back(prog.text.instructions[i].field2);
            }
            if (operands.size() == 2) {
                bytes.push_back(prog.text.instructions[i].field2);
            }
        }
        return bytes;
    }
};


std::string get_output_name(int argc, char** argv) {
    std::string prev = "";

    for (int i = 0; i < argc; i++) {
        if (prev == "-o") {
            return std::string(argv[i]);
        }

        prev = argv[i];
    }
    return "app.fm8";
}

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

    std::vector<uint8_t> prog = fm_asm::generate_assembly(code);

    std::ofstream outfile(get_output_name(argc, argv), std::ios::out | std::ios::binary);

    if (!outfile.is_open()) {
        std::cerr << "Error: Could not open file for writing!" << std::endl;
        return 1;
    }

    outfile.write(reinterpret_cast<const char*>(prog.data()), prog.size());
    outfile.close();

    return 0;
}
