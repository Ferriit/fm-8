#pragma once

#include <cstdint>

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
    
    uint8_t read(uint16_t addr, uint8_t val) {
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

    void set_vector(uint8_t page, uint8_t low_addr, uint16_t value) {
        pages[page][low_addr] = static_cast<uint8_t>(value & 0xFF);
        
        pages[page][low_addr + 1] = static_cast<uint8_t>((value >> 8) & 0xFF);
    }
};
