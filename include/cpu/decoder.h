//
// Created by syl on 12/11/2020.
//

#ifndef NES_CPP_DECODER_H
#define NES_CPP_DECODER_H

#include <string_view>
#include <spdlog/spdlog.h>

#include "cpu/regs.h"
#include "cpu/cpu_mem_bus.h"

namespace nes::cpu {

    enum class opcode {
        BRK, BPL, JSR, BMI, RTI, BVC, RTS, BVS, BCC, LDY, BCS, CPY, BNE, CPX, BEQ, BIT,
        STY, ORA, AND, EOR, ADC, STA, LDA, CMP, SBC, ASL, ROL, LSR, ROR, STX, LDX, DEC,
        INC, PHP, CLC, PLP, SEC, PHA, CLI, PLA, SEI, DEY, CLV, TAY, TYA, JMP, INY, CLD,
        INX, SED, TXA, TAX, TXS, NOP, DEX, TSX
    };

    enum class address_mode {
        Acc, Abs, AbsX, AbsY, Imm, Ind, XInd,
        IndY, Rel, Zpg, ZpgX, ZpgY, Impl
    };

    class decoded_op_impl;

    struct decoded_op {
        opcode op;
        address_mode mode;
        uint8_t cycles;
        uint8_t bytes;
        bool boundary_hint;
        bool page_hint;
        uint16_t addr;
        uint8_t val;
    };

    class decoder {
    public:
        decoder() = default;

        ~decoder() = default;

        decoder(decoder const &) = delete;

        decoder &operator=(decoder const &) = delete;

        decoded_op decode(uint16_t addr, std::shared_ptr<regs> regs, std::shared_ptr<cpu_mem_bus> membus);

        std::vector<decoded_op>
        decode(uint8_t nb_instr, uint16_t addr, std::shared_ptr<regs> regs, std::shared_ptr<cpu_mem_bus> membus);
    };


    constexpr std::string_view opcode2string(opcode op) {
        switch (op) {
            case opcode::BRK:
                return "BRK";
            case opcode::BPL:
                return "BPL";
            case opcode::JSR:
                return "JSR";
            case opcode::BMI:
                return "BMI";
            case opcode::RTI:
                return "RTI";
            case opcode::BVC:
                return "BVC";
            case opcode::RTS:
                return "RTS";
            case opcode::BVS:
                return "BVS";
            case opcode::BCC:
                return "BCC";
            case opcode::LDY:
                return "LDY";
            case opcode::BCS:
                return "BCS";
            case opcode::CPY:
                return "CPY";
            case opcode::BNE:
                return "BNE";
            case opcode::CPX:
                return "CPX";
            case opcode::BEQ:
                return "BEQ";
            case opcode::BIT:
                return "BIT";
            case opcode::STY:
                return "STY";
            case opcode::ORA:
                return "ORA";
            case opcode::AND:
                return "AND";
            case opcode::EOR:
                return "EOR";
            case opcode::ADC:
                return "ADC";
            case opcode::STA:
                return "STA";
            case opcode::LDA:
                return "LDA";
            case opcode::CMP:
                return "CMP";
            case opcode::SBC:
                return "SBC";
            case opcode::ASL:
                return "ASL";
            case opcode::ROL:
                return "ROL";
            case opcode::LSR:
                return "LSR";
            case opcode::ROR:
                return "ROR";
            case opcode::STX:
                return "STX";
            case opcode::LDX:
                return "LDX";
            case opcode::DEC:
                return "DEC";
            case opcode::INC:
                return "INC";
            case opcode::PHP:
                return "PHP";
            case opcode::CLC:
                return "CLC";
            case opcode::PLP:
                return "PLP";
            case opcode::SEC:
                return "SEC";
            case opcode::PHA:
                return "PHA";
            case opcode::CLI:
                return "CLI";
            case opcode::PLA:
                return "PLA";
            case opcode::SEI:
                return "SEI";
            case opcode::DEY:
                return "DEY";
            case opcode::CLV:
                return "CLV";
            case opcode::TAY:
                return "TAY";
            case opcode::TYA:
                return "TYA";
            case opcode::JMP:
                return "JMP";
            case opcode::INY:
                return "INY";
            case opcode::CLD:
                return "CLD";
            case opcode::INX:
                return "INX";
            case opcode::SED:
                return "SED";
            case opcode::TXA:
                return "TXA";
            case opcode::TAX:
                return "TAX";
            case opcode::TXS:
                return "TXS";
            case opcode::NOP:
                return "NOP";
            case opcode::DEX:
                return "DEX";
            case opcode::TSX:
                return "TSX";
        }
    }
}

//fmt overload
template<>
struct fmt::formatter<nes::cpu::decoded_op> {
    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(const nes::cpu::decoded_op &p, FormatContext &ctx) {
        switch (p.mode) {
            case nes::cpu::address_mode::Impl:
                return format_to(ctx.out(), "{}", nes::cpu::opcode2string(p.op));
            case nes::cpu::address_mode::Imm :
                return format_to(ctx.out(), "{} {:#04x}", nes::cpu::opcode2string(p.op), p.val);
            case nes::cpu::address_mode::Abs :
                return format_to(ctx.out(), "{}[{:#06x}] = {:#04x}", nes::cpu::opcode2string(p.op), p.addr, p.val);
            case nes::cpu::address_mode::AbsX:
                return format_to(ctx.out(), "{}[{:#06x}] = {:#04x}", nes::cpu::opcode2string(p.op), p.addr, p.val);
            case nes::cpu::address_mode::AbsY:
                return format_to(ctx.out(), "{}[{:#06x},y] = {:#04x}", nes::cpu::opcode2string(p.op), p.addr, p.val);
            case nes::cpu::address_mode::Zpg :
                return format_to(ctx.out(), "{}[{:#06x}] = {:#04x}", nes::cpu::opcode2string(p.op), p.addr, p.val);
            case nes::cpu::address_mode::ZpgX:
                return format_to(ctx.out(), "{}[{:#06x},x] = {:#04x}", nes::cpu::opcode2string(p.op), p.addr, p.val);
            case nes::cpu::address_mode::ZpgY:
                return format_to(ctx.out(), "{}[{:#06x},y] = {:#04x}", nes::cpu::opcode2string(p.op), p.addr, p.val);
            case nes::cpu::address_mode::Ind :
                return format_to(ctx.out(), "{}", nes::cpu::opcode2string(p.op));
            case nes::cpu::address_mode::XInd:
                return format_to(ctx.out(), "{}", nes::cpu::opcode2string(p.op));
            case nes::cpu::address_mode::IndY:
                return format_to(ctx.out(), "{}", nes::cpu::opcode2string(p.op));
            case nes::cpu::address_mode::Acc :
                return format_to(ctx.out(), "{}", nes::cpu::opcode2string(p.op));
            case nes::cpu::address_mode::Rel:
                return format_to(ctx.out(), "{} PC{:+} = {:#04x}", nes::cpu::opcode2string(p.op),
                                 static_cast<int8_t>(p.addr), p.val);
        }
    }
};

#endif //NES_CPP_DECODER_H
