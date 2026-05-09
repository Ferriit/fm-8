#pragma once

#include <cstdint>
#include "../utils/constants.hpp"

struct memory {
private:
    uint8_t pages[256][256];

public:
    uint8_t get_content(uint8_t page, uint8_t addr) {
        return pages[page][addr];
    }

    void set_content(uint8_t page, uint8_t addr, uint8_t val) {
        pages[page][addr] = val;
    }
    
    uint8_t read(uint16_t addr) {
        return get_content(addr >> 8, addr & 0x00FF);
    }

    void write(uint16_t addr, uint8_t val) {
        set_content(addr >> 8, addr & 0x00FF, val);
    }

    uint16_t get_vector(uint8_t page, uint8_t low_addr) {
        uint8_t low = pages[page][low_addr];
        uint8_t high = pages[page][low_addr + 1];
        return (static_cast<uint16_t>(high) << 8) | low;
    }

    uint16_t get_nmi() {
        return get_vector(0xFF, 0xFA);
    }

    uint16_t get_rst() {
        return get_vector(0xFF, 0xFC);
    }

    uint16_t get_irq() {
        return get_vector(0xFF, 0xFE);
    }

    void set_vector(uint8_t page, uint8_t low_addr, uint16_t value) {
        pages[page][low_addr] = static_cast<uint8_t>(value & 0xFF);
        
        pages[page][low_addr + 1] = static_cast<uint8_t>((value >> 8) & 0xFF);
    }
};


class regs {
private:
    uint8_t* registers;
    
public:
    regs() {
        registers = new uint8_t[consts::reg_map.size()];
    }

    uint8_t get_reg(consts::Reg reg) {
        return registers[reg];
    }

    uint16_t get_pc() {
        return registers[consts::Reg::pc];
    }

    void set_reg(consts::Reg reg, uint8_t val) {
        registers[reg] = val;
    }

    void set_pc(uint16_t val) {
        registers[consts::Reg::pc] = val;
    }

    ~regs() {
        delete[] registers;
    }
};

