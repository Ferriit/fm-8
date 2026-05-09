#include <vector>
#include <cstdint>
#include <iostream>
#include <fstream>
#include "memory.hpp"
#include "../utils/logging.hpp"
#include "../utils/constants.hpp"


using namespace consts;

int load_memory(memory &mem, const std::vector<uint8_t> &code) {
    if (code.size() < 6) {
        log_err("File too small for vectors!");
        return 1;
    }

    mem.set_vector(0xFF, 0xFA, (code[0] << 8) | code[1]); // NMI
    log("loaded NMI Vector into RAM");
    mem.set_vector(0xFF, 0xFC, (code[2] << 8) | code[3]); // RST
    log("loaded RST Vector into RAM");
    mem.set_vector(0xFF, 0xFE, (code[4] << 8) | code[5]); // IRQ
    log("loaded IRQ Vector into RAM");

    size_t code_idx = 6;
    uint16_t mem_addr = 0x0100;

    while (code_idx < code.size() && mem_addr < 0xFFFA) {
        mem.write(mem_addr, code[code_idx]);
        code_idx++;
        mem_addr++;
    }

    std::cout << std::endl;
    log("Loaded " + std::to_string(code_idx - 6) + " bytes into memory, starting at page #1.");
    return 0;
}

struct processor {
private:
    memory mem;
    uint16_t addr = 0x100;

public:
    uint16_t get_instr_length(OpCode opcode) {
        std::vector<OperandType> operands = operand_map[opcode];

        if (operands[0] != OperandType::NONE) {
            return (uint16_t)(operands.size() + 1);
        }
        return (uint16_t)1;
    }

    void execute_instr(uint8_t opcode, uint8_t op1, uint8_t op2) {
        uint16_t size = get_instr_length((OpCode)opcode);

        switch (opcode) {
            case (OpCode::ADD) : {

            }
        }
    }
};

int main(int argc, char** argv) {
    if (argc < 2) {
        log_err("Filename required!");
        return 1;
    }

    std::ifstream file(argv[1], std::ios::binary | std::ios::ate);

    if (!file) {
        log_err("Failed to open file!");
        return 1;
    }
    
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> buffer(size);

    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        log_err("Unable to read from file!");
        return 1;
    }

    memory *mem = new memory;

    load_memory(*mem, buffer);
}
