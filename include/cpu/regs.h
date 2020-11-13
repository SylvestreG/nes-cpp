//
// Created by syl on 12/11/2020.
//

#ifndef NES_CPP_REGS_H
#define NES_CPP_REGS_H

#include <cstdint>

namespace nes::cpu {

    struct regs {
        uint16_t pc{0x00};
        uint8_t ac{0x00};
        uint8_t x{0x00};
        uint8_t y{0x00};
        uint8_t sr{0x00};
        uint8_t sp{0x00};
        uint8_t flags{0x00};
    };
};

#endif //NES_CPP_REGS_H
