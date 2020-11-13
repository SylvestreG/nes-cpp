//
// Created by syl on 12/11/2020.
//

#include "cpu/execute.h"

using namespace nes::cpu;

class nes::cpu::execute_impl {
private:
    std::shared_ptr<cpu_mem_bus> _membus;
    std::shared_ptr<regs> _regs;

    friend execute;
};

uint8_t nes::cpu::execute::exec(nes::cpu::decoded_op &op) {
    uint8_t cycles = op.cycles;

    switch (op.op) {
        case opcode::SEI:
            _impl->_regs->flags |= 0x4u;
            break;
        case opcode::CLD:
            _impl->_regs->flags &= ~0x8u;
            break;
        case opcode::LDX:
            _impl->_regs->x = op.val;
            break;
        case opcode::STX:
            _impl->_regs->sp = _impl->_regs->x;
            break;
        case opcode::LDA:
            _impl->_regs->ac = op.val;
            break;
        default:
            break;
    }

    return cycles;
}

execute::execute(std::shared_ptr<cpu_mem_bus> membus, std::shared_ptr<regs> regs) : _impl(
        std::make_unique<execute_impl>()) {
    _impl->_membus = std::move(membus);
    _impl->_regs = std::move(regs);
}
