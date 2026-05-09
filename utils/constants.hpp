#pragma once

#include <cstdint>
#include <unordered_map>
#include <string>
#include <vector>


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

        JMP,    // Jump Relative
        BA,     // Jump Absolute (Branch)
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

    enum SetupField : uint8_t {
        NMI,
        IRQ,
        START
    };

    enum OperandType {
        NONE,
        VAL,
        REG,
        LABL,
        VAR
    };

    std::vector<std::vector<OperandType>> operand_map = {
        {OperandType::REG, OperandType::REG},       // add
        {OperandType::REG, OperandType::REG},       // sub
        {OperandType::REG, OperandType::REG},       // mul
        {OperandType::REG},                         // inc
        {OperandType::REG},                         // dec
        {OperandType::REG, OperandType::REG},       // and
        {OperandType::REG},                         // not
        {OperandType::REG, OperandType::REG},       // or
        {OperandType::REG, OperandType::REG},       // eor
        {OperandType::REG},                         // shr
        {OperandType::REG},                         // shl
        {OperandType::REG, OperandType::REG},       // tst
        {OperandType::REG, OperandType::REG},       // cmp
        {OperandType::LABL},                        // jmp
        {OperandType::LABL},                        // ba
        {OperandType::LABL},                        // beq
        {OperandType::LABL},                        // bne
        {OperandType::LABL},                        // bgt
        {OperandType::LABL},                        // blt
        {OperandType::LABL},                        // bge
        {OperandType::LABL},                        // ble
        {OperandType::LABL},                        // bhi
        {OperandType::LABL},                        // blo
        {OperandType::LABL},                        // bmi
        {OperandType::LABL},                        // bpl
        {OperandType::LABL},                        // bvs
        {OperandType::LABL},                        // bvc
        {OperandType::LABL},                        // bhs
        {OperandType::LABL},                        // bls
        {OperandType::LABL},                        // call
        {OperandType::NONE},                        // ret
        {OperandType::NONE},                        // int
        {OperandType::NONE},                        // rti
        {OperandType::REG, OperandType::REG},       // mov
        {OperandType::REG, OperandType::VAL},       // ldi
        {OperandType::VAR, OperandType::REG},       // setv
        {OperandType::REG, OperandType::VAR},       // ldv
        {OperandType::REG, OperandType::REG},       // seta
        {OperandType::REG, OperandType::REG},       // lda
        {OperandType::REG},                         // push
        {OperandType::REG},                         // pop
        {OperandType::NONE},                        // nop
        {OperandType::NONE},                        // hlt
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

        {"int",  OpCode::INT},
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

    std::vector<std::string> op_list = {
        "add", "sub", "mul", "inc", "dec", "and", "not", "or", "eor", "xor",
        "shr", "shl", "tst", "cmp", "jmp", "ba", "beq", "bne", "bgt", "blt",
        "bge", "ble", "bhi", "blo", "bmi", "bpl", "vs", "bvc", "bhs", "bls",
        "call", "ret", "int", "rti", "mov", "ldi", "setv", "ldv", "seta",
        "lda", "push", "pop", "nop", "hlt"
    };

    std::unordered_map<std::string, SetupField> setup_map = {
        {"nmi", SetupField::NMI},
        {"irq", SetupField::IRQ},
        {"start", SetupField::START}
    };

    std::vector<std::string> setup_list = {
        "nmi",
        "irq",
        "rst"
    };

    enum Reg : uint8_t {
    	ra,
     	rb,
      	rc,
       	rd,
        rw,
        rx,
        ry,
        rz,
        p,
        f,
        sp,
        pc
    };

    std::unordered_map<std::string, Reg> reg_map = {
        {"ra", Reg::ra},
        {"rb", Reg::rb},
        {"rc", Reg::rc},
        {"rd", Reg::rd},
        {"rw", Reg::rw},
        {"rx", Reg::rx},
        {"ry", Reg::ry},
        {"rz", Reg::rz},
        {"p",  Reg::p},
        {"f",  Reg::f},
        {"sp", Reg::sp},
        {"pc", Reg::pc}
    };

    enum Flag : uint8_t {
        Z,
        C,
        V,
        N,
        I
    };

    enum Section {
        none,
        setup,
        data,
        text
    };
};
