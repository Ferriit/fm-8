#pragma once

#include <cstdint>
#include <unordered_map>
#include <string>


namespace consts {
    enum OpCode : uint8_t {
        ADD,
        SUB,
        MUL,
        INC,
        DEC,
        AND,
        NOT,
        OR,
        EOR,
        SHR,
        SHL,

        TST,
        CMP,

        JMP,    // Jump Absolute
        BA,     // Jump Relative (Branch)
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
        RET,

        INT,
        RTI,

        MOV,
        LDI,
        SETV,   // Set Variable (on page 0)
        LDV,    // Load Variable (on page 0)
        SETA,   // Set Ram Address (on page P)
        LDA,    // Load Ram Address (on page P)

        PUSH,
        POP,

        NOP,
        HLT,
    };

    std::unordered_map<std::string, OpCode> op_map = {
        // Arithmetic & Logic
        {"add",  OpCode::ADD},
        {"sub",  OpCode::SUB},
        {"mul",  OpCode::MUL},
        {"inc",  OpCode::INC},
        {"dec",  OpCode::DEC},
        {"and",  OpCode::AND},
        {"not",  OpCode::NOT},
        {"or",   OpCode::OR},
        {"eor",  OpCode::EOR},
        {"xor",  OpCode::EOR}, // Alias for EOR
        {"shr",  OpCode::SHR},
        {"shl",  OpCode::SHL},

        {"tst",  OpCode::TST},
        {"cmp",  OpCode::CMP},

        {"jmp",  OpCode::JMP},
        {"ba",   OpCode::BA},
        {"beq",  OpCode::BEQ},
        {"bne",  OpCode::BNE},
        {"bgt",  OpCode::BGT},
        {"blt",  OpCode::BLT},
        {"bge",  OpCode::BGE},
        {"ble",  OpCode::BLE},
        {"bhi",  OpCode::BHI},
        {"blo",  OpCode::BLO},
        {"bmi",  OpCode::BMI},
        {"bpl",  OpCode::BPL},
        {"bvs",  OpCode::BVS},
        {"bvc",  OpCode::BVC},
        {"bhs",  OpCode::BHS},
        {"bls",  OpCode::BLS},
        {"call", OpCode::CALL},
        {"ret",  OpCode::RET},

        {"int", OpCode::INT},
        {"rti",  OpCode::RTI},

        {"mov",  OpCode::MOV},
        {"ldi",  OpCode::LDI},
        {"setv", OpCode::SETV},
        {"ldv",  OpCode::LDV},
        {"seta", OpCode::SETA},
        {"lda",  OpCode::LDA},

        {"push", OpCode::PUSH},
        {"pop",  OpCode::POP},
        {"nop",  OpCode::NOP},
        {"hlt",  OpCode::HLT}
    };

    enum Reg : uint8_t {
    	ra,
     	rb,
      	rc,
       	rd,
        re,
        p,
        f,
        sp
    };
};
