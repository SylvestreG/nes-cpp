#include <array>
#include <spdlog/spdlog.h>

#include "cpu/decoder.h"

using namespace nes::cpu;

static decoded_op b0_stage_mux(uint8_t b, uint8_t c) {
    switch (b) {
        case 1:
            switch (c) {
                case 1:
                    return decoded_op{opcode::BIT, address_mode::Zpg, 3, 2, false, false};
                case 3:
                    return decoded_op{opcode::BIT, address_mode::Abs, 4, 3, false, false};
                default:
                    spdlog::error("unkown BIT opcode b={} c={}", b, c);
                    exit(EXIT_FAILURE);
            }
        case 4:
            switch (c) {
                case 1:
                    return decoded_op{opcode::STY, address_mode::Zpg, 3, 2, false, false};
                case 3:
                    return decoded_op{opcode::STY, address_mode::Abs, 4, 3, false, false};
                case 5:
                    return decoded_op{opcode::STY, address_mode::ZpgX, 4, 2, false, false};
                default:
                    spdlog::error("unkown STY opcode b={} c={}", b, c);
                    exit(EXIT_FAILURE);
            }
        case 5:
            switch (c) {
                case 0:
                    return decoded_op{opcode::LDY, address_mode::Imm, 2, 2, false, false};
                case 1:
                    return decoded_op{opcode::LDY, address_mode::Zpg, 3, 2, false, false};
                case 3:
                    return decoded_op{opcode::LDY, address_mode::Abs, 4, 3, false, false};
                case 5:
                    return decoded_op{opcode::LDY, address_mode::ZpgX, 4, 3, false, false};
                case 7:
                    return decoded_op{opcode::LDY, address_mode::AbsX, 4, 3, true, false};
                default:
                    spdlog::error("unkown LDY opcode b={} c={}", b, c);
                    exit(EXIT_FAILURE);
            }
        case 6:
            switch (c) {
                case 0:
                    return decoded_op{opcode::CPY, address_mode::Imm, 2, 2, false, false};
                case 1:
                    return decoded_op{opcode::CPY, address_mode::Zpg, 3, 2, false, false};
                case 2:
                    return decoded_op{opcode::CPY, address_mode::Abs, 4, 3, false, false};
                default:
                    spdlog::error("unkown CPY opcode b={} c={}", b, c);
                    exit(EXIT_FAILURE);
            }
        case 7:
            switch (c) {
                case 0:
                    return decoded_op{opcode::CPX, address_mode::Imm, 2, 2, false, false};
                case 1:
                    return decoded_op{opcode::CPX, address_mode::Zpg, 3, 2, false, false};
                case 2:
                    return decoded_op{opcode::CPX, address_mode::Abs, 4, 3, false, false};
                default:
                    spdlog::error("unkown CPX opcode b={} c={}", b, c);
                    exit(EXIT_FAILURE);
            }
        default:
            spdlog::error("unkown b2 opcode b={} c={}", b, c);
            exit(EXIT_FAILURE);
    }
}

static decoded_op b1_stage_mux(uint8_t op, uint8_t b, uint8_t c) {
    if (op == 24) {
        spdlog::error("unkown STA opcode={}", op);
        exit(EXIT_FAILURE);
    }

    std::vector<address_mode> addr_mode{address_mode::XInd, address_mode::Zpg, address_mode::Imm, address_mode::Abs,
                                        address_mode::IndY, address_mode::ZpgX, address_mode::AbsY, address_mode::AbsX};

    std::array<opcode, 8> mnemonic{opcode::ORA, opcode::AND, opcode::EOR, opcode::ADC, opcode::STA, opcode::LDA,
                                   opcode::CMP,
                                   opcode::SBC};
    std::array<uint8_t, 8> byte_pattern{2, 2, 2, 3, 2, 2, 3, 3};
    std::array<uint8_t, 8> cycle_pattern{6, 3, 2, 4, 5, 4, 4, 4};
    std::array<bool, 8> boundary_pattern{false, false, false, false, true, false, true, true};

    if (mnemonic[b] == opcode::STA) {
        byte_pattern = std::array<uint8_t, 8>{2, 2, 0, 3, 2, 2, 3, 3};
        cycle_pattern = std::array<uint8_t, 8>{6, 3, 0, 4, 6, 4, 5, 5};
    }

    return decoded_op{mnemonic[b], addr_mode[c], cycle_pattern[c], byte_pattern[c], boundary_pattern[c], false};
}

static decoded_op b2_stage_mux(uint8_t b, uint8_t c) {
    decoded_op op;
    std::array<address_mode, 8> addr_mode{address_mode::Imm, address_mode::Zpg, address_mode::Acc, address_mode::Abs,
                                          address_mode::Imm, address_mode::ZpgX, address_mode::Imm, address_mode::AbsX};
    std::array<address_mode, 8> addr_mode_y{address_mode::Imm, address_mode::Zpg, address_mode::Acc, address_mode::Abs,
                                            address_mode::Imm, address_mode::ZpgY, address_mode::Imm,
                                            address_mode::AbsY};
    std::array<uint8_t, 8> byte_pattern1{0, 2, 1, 3, 0, 2, 0, 3};
    std::array<uint8_t, 8> byte_pattern2{0, 2, 0, 3, 0, 2, 0, 3};
    std::array<uint8_t, 8> cycle_pattern1{0, 5, 2, 6, 0, 6, 0, 7};
    std::array<uint8_t, 8> cycle_pattern2{0, 5, 0, 6, 0, 6, 0, 7};

    switch (b) {
        case 0:
            switch (c) {
                case 0:
                case 4:
                case 6:
                    spdlog::error("unkown ASL opcode b={} c={}", b, c);
                    exit(EXIT_FAILURE);
                default:
                    return decoded_op{opcode::ASL, addr_mode[c], cycle_pattern1[c], byte_pattern1[c], false, false};
            }
            break;
        case 1:
            switch (c) {
                case 0:
                case 4:
                case 6:
                    spdlog::error("unkown ROL opcode b={} c={}", b, c);
                    exit(EXIT_FAILURE);
                default:
                    return decoded_op{opcode::ROL, addr_mode[c], cycle_pattern1[c], byte_pattern1[c], false, false};
            }
            break;
        case 2:
            switch (c) {
                case 0:
                case 4:
                case 6:
                    spdlog::error("unkown LSR opcode b={} c={}", b, c);
                    exit(EXIT_FAILURE);
                default:
                    return decoded_op{opcode::LSR, addr_mode[c], cycle_pattern1[c], byte_pattern1[c], false, false};
            }
            break;
        case 3:
            switch (c) {
                case 0:
                case 4:
                case 6:
                    spdlog::error("unkown ROR opcode b={} c={}", b, c);
                    exit(EXIT_FAILURE);
                default:
                    return decoded_op{opcode::ROR, addr_mode[c], cycle_pattern1[c], byte_pattern1[c], false, false};
            }
            break;
        case 4:
            switch (c) {
                case 0:
                case 2:
                case 4:
                case 6:
                case 7:
                    spdlog::error("unkown STX opcode b={} c={}", b, c);
                    exit(EXIT_FAILURE);
                default:
                    byte_pattern1 = std::array<uint8_t, 8>{0, 2, 0, 3, 0, 2, 0, 0};
                    cycle_pattern1 = std::array<uint8_t, 8>{0, 3, 0, 4, 0, 4, 0, 0};
                    return decoded_op{opcode::STX, addr_mode_y[c], cycle_pattern1[c], byte_pattern1[c], false, false};
            }
            break;
        case 5:
            switch (c) {
                case 2:
                case 4:
                case 6:
                    spdlog::error("unkown LDX opcode b={} c={}", b, c);
                    exit(EXIT_FAILURE);
                default:
                    byte_pattern1 = std::array<uint8_t, 8>{2, 2, 0, 3, 0, 2, 0, 3};
                    cycle_pattern1 = std::array<uint8_t, 8>{2, 3, 0, 4, 0, 4, 0, 4};
                    std::array<bool, 8> boundary{false, false, false, false, false, false, false, true};
                    return decoded_op{opcode::LDX, addr_mode_y[c], cycle_pattern1[c], cycle_pattern1[c], boundary[c],
                                      false};
            }
            break;
        case 6:
            switch (c) {
                case 0:
                case 2:
                case 4:
                case 6:
                    spdlog::error("unkown DEC opcode b={} c={}", b, c);
                    exit(EXIT_FAILURE);
                default:
                    return decoded_op{opcode::DEC, addr_mode[c], cycle_pattern2[c], byte_pattern2[c], false, false};
            }
            break;
        case 7:
            switch (c) {
                case 0:
                case 2:
                case 4:
                case 6:
                    spdlog::error("unkown INC opcode b={} c={}", b, c);
                    exit(EXIT_FAILURE);
                default:
                    return decoded_op{opcode::INC, addr_mode[c], cycle_pattern2[c], byte_pattern2[c], false, false};
            }
            break;
        default:
            spdlog::error("unkown b2 opcode b={} c={}", b, c);
            exit(EXIT_FAILURE);
    }
}

static decoded_op a_stage_mux(uint8_t opcode, uint8_t a, uint8_t b, uint8_t c) {
    switch (opcode) {
        case 0x00:
            return decoded_op{opcode::BRK, address_mode::Impl, 7, 1, false, false};
        case 0x08:
            return decoded_op{opcode::PHP, address_mode::Impl, 3, 1, false, false};
        case 0x10:
            return decoded_op{opcode::BPL, address_mode::Rel, 2, 2, false, true};
        case 0x18:
            return decoded_op{opcode::CLC, address_mode::Impl, 2, 1, false, false};
        case 0x20:
            return decoded_op{opcode::JSR, address_mode::Abs, 6, 3, false, false};
        case 0x28:
            return decoded_op{opcode::PLP, address_mode::Impl, 4, 1, false, false};
        case 0x30:
            return decoded_op{opcode::BMI, address_mode::Rel, 2, 2, false, true};
        case 0x38:
            return decoded_op{opcode::SEC, address_mode::Impl, 2, 1, false, false};
        case 0x40:
            return decoded_op{opcode::RTI, address_mode::Impl, 6, 1, false, false};
        case 0x48:
            return decoded_op{opcode::PHA, address_mode::Impl, 3, 1, false, false};
        case 0x4C:
            return decoded_op{opcode::JMP, address_mode::Abs, 3, 2, false, false};
        case 0x50:
            return decoded_op{opcode::BVC, address_mode::Rel, 2, 2, false, true};
        case 0x58:
            return decoded_op{opcode::CLI, address_mode::Impl, 2, 1, false, false};
        case 0x60:
            return decoded_op{opcode::RTS, address_mode::Impl, 6, 1, false, false};
        case 0x68:
            return decoded_op{opcode::PLA, address_mode::Impl, 4, 1, false, false};
        case 0x6C:
            return decoded_op{opcode::JMP, address_mode::Ind, 5, 3, false, false};
        case 0x70:
            return decoded_op{opcode::BVS, address_mode::Rel, 2, 1, false, true};
        case 0x78:
            return decoded_op{opcode::SEI, address_mode::Impl, 2, 1, false, false};
        case 0x88:
            return decoded_op{opcode::DEY, address_mode::Impl, 2, 1, false, false};
        case 0x90:
            return decoded_op{opcode::BCC, address_mode::Rel, 2, 2, false, true};
        case 0x98:
            return decoded_op{opcode::TYA, address_mode::Impl, 2, 1, false, false};
        case 0xA8:
            return decoded_op{opcode::TAY, address_mode::Impl, 2, 1, false, false};
        case 0xB0:
            return decoded_op{opcode::BCS, address_mode::Rel, 2, 2, false, true};
        case 0xB8:
            return decoded_op{opcode::CLV, address_mode::Impl, 2, 1, false, false};
        case 0xC8:
            return decoded_op{opcode::INY, address_mode::Impl, 2, 1, false, false};
        case 0xD0:
            return decoded_op{opcode::BNE, address_mode::Rel, 2, 2, false, true};
        case 0xD8:
            return decoded_op{opcode::CLD, address_mode::Impl, 2, 1, false, false};
        case 0xE8:
            return decoded_op{opcode::INX, address_mode::Impl, 2, 1, false, false};
        case 0xF0:
            return decoded_op{opcode::BEQ, address_mode::Impl, 2, 2, false, true};
        case 0xF8:
            return decoded_op{opcode::SED, address_mode::Impl, 2, 1, false, false};
        case 0x8A:
            return decoded_op{opcode::TXA, address_mode::Impl, 2, 1, false, false};
        case 0x9A:
            return decoded_op{opcode::TXS, address_mode::Impl, 2, 1, false, false};
        case 0xAA:
            return decoded_op{opcode::TAX, address_mode::Impl, 2, 1, false, false};
        case 0xBA:
            return decoded_op{opcode::TSX, address_mode::Impl, 2, 1, false, false};
        case 0xCA:
            return decoded_op{opcode::DEX, address_mode::Impl, 2, 1, false, false};
        case 0xEA:
            return decoded_op{opcode::NOP, address_mode::Impl, 2, 1, false, false};

        default:
            switch (a) {
                case 0:
                    return std::move(b0_stage_mux(b, c));
                case 1:
                    return std::move(b1_stage_mux(opcode, b, c));
                default:
                    return b2_stage_mux(b, c);
            }
    }
}

decoded_op decoder::decode(uint16_t addr, std::shared_ptr<regs> regs, std::shared_ptr<cpu_mem_bus> membus) {
    auto raw_op = membus->fetch_u8(addr);

    auto a = raw_op & 0x03;
    auto b = (raw_op & 0xe0) >> 5;
    auto c = (raw_op & 0x1c) >> 2;

    decoded_op ret = a_stage_mux(raw_op, a, b, c);

    auto immediate = [&ret, &membus, &addr]() { ret.addr = membus->fetch_u8(addr + 1); };
    auto abs_x = [&ret, &membus, &addr](uint8_t x) {
        ret.addr = membus->fetch_u16(addr + 1) + x;
        ret.val = membus->fetch_u8(ret.addr);
    };
    auto zpg_x = [&ret, &membus, &addr](uint8_t x) {
        ret.addr = membus->fetch_u8(addr + 1) + x;
        ret.val = membus->fetch_u8(ret.addr);
    };
    auto ind_x = [&ret, &membus, &addr](uint8_t x) {
        ret.addr = membus->fetch_u8(addr + 1) + x;
        ret.val = membus->fetch_u8(membus->fetch_u16(ret.addr));
    };
    auto rel = [&ret, &membus, &addr](uint16_t pc) {
        ret.addr = membus->fetch_u8(addr + 1);
        ret.val = membus->fetch_u8(pc + static_cast<int8_t>(ret.addr));
    };

    switch (ret.mode) {
        case address_mode::Impl:
            break;
        case address_mode::Imm:
            immediate();
            break;
        case address_mode::Abs:
            abs_x(0);
            break;
        case address_mode::AbsX:
            abs_x(regs->x);
            break;
        case address_mode::AbsY:
            abs_x(regs->y);
            break;
        case address_mode::Zpg:
            zpg_x(0);
            break;
        case address_mode::ZpgX:
            zpg_x(regs->x);
            break;
        case address_mode::ZpgY:
            zpg_x(regs->y);
            break;
        case address_mode::Ind:
            ind_x(0);
            break;
        case address_mode::XInd:
            ind_x(regs->x);
            break;
        case address_mode::IndY:
            ind_x(regs->y);
            break;
        case address_mode::Acc:
            ret.val = regs->ac;
            break;
        case address_mode::Rel:
            rel(regs->pc);
            break;
    }

    return ret;
}

std::vector<decoded_op>
decoder::decode(uint8_t nb_instr, uint16_t addr, std::shared_ptr<regs> regs, std::shared_ptr<cpu_mem_bus> membus) {
    std::vector<decoded_op> ret{nb_instr};

    auto opcode_offset = 0;
    while (nb_instr > 0) {
        auto op = decode(addr + opcode_offset, regs, membus);
        ret.emplace_back(op);
        opcode_offset += op.bytes;
        nb_instr--;
    }
    return ret;
}
