//
// Created by syl on 12/11/2020.
//

#ifndef NES_CPP_EXECUTE_H
#define NES_CPP_EXECUTE_H

#include <memory>

#include "cpu/cpu_mem_bus.h"
#include "cpu/decoder.h"
#include "cpu/regs.h"

namespace nes::cpu {
    class execute_impl;

    class execute {
    public:
        execute(std::shared_ptr<cpu_mem_bus> membus, std::shared_ptr<regs> regs);
        ~execute(void);
        execute(execute const&) = delete;
        execute& operator=(execute const&) = delete;

        uint8_t exec(decoded_op &op);

    private:
        std::unique_ptr<execute_impl> _impl;
    };
}

#endif //NES_CPP_EXECUTE_H
