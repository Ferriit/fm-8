#include <vector>
#include <cstdint>
#include <iostream>
#include "memory.hpp"
#include "../utils/logging.hpp"


int load_memory(memory &mem, const std::vector<uint8_t> &code) {
    // Load nmi, irq and rst vectors

    if (code.size() < 6) {
        std::cerr << "Binary missing valid vectors!" << std::endl;
        return 1;
    }
    if (code.size() > 0xFF00) {
        std::cerr << "Binary too large!" << std::endl;
        return 1;
    }

    size_t idx = 0;

    // Loading Vectors
    mem.set_vector(0xFF, 0xFA, code[idx] << 8 + code[idx + 1]);  // NMI
    idx += 2;
    mem.set_vector(0xFF, 0xFC, code[idx] << 8 + code[idx + 1]);  // RST
    idx += 2;
    mem.set_vector(0xFF, 0xFE, code[idx] << 8 + code[idx + 1]);  // IRQ
    idx += 2;

    // Loading Code
    // Start on first address of first page
    for (uint16_t addr = 0x100; addr < code.size(); addr++) {
        mem.write(addr, code[idx]);
        idx++;
    }

    return 0;
}

int main() {
    return 0;
}