//
// Created by syl on 12/11/2020.
//

#ifndef NES_CPP_CPU_MEM_BUS_H
#define NES_CPP_CPU_MEM_BUS_H

#include <memory>
#include "cartridge/cartridge.h"
#include "memory/memory_interface.h"

namespace nes::cpu {
    struct cpu_mem_bus_impl;

    enum class mem_type {
        internal,
        cartridge,
        ppu,
        none
    };

    constexpr mem_type addr_to_mem_type(uint16_t addr) {
        if (addr < 0x2000)
            return mem_type::internal;
        else if (addr < 0x4000)
            return mem_type::ppu;
        else if (addr > 0x4000)
            return mem_type::cartridge;
        else
            return mem_type::none;
    }

    class cpu_mem_bus : public memory::memory_iface {
    public:
        explicit cpu_mem_bus(std::shared_ptr<cartridge::cartridge> cartridge) noexcept;
        ~cpu_mem_bus();
        cpu_mem_bus(cpu_mem_bus const&) = delete;
        cpu_mem_bus& operator=(cpu_mem_bus const&) = delete;

        uint8_t fetch_u8(std::uint16_t addr) const final;
        uint16_t fetch_u16(std::uint16_t addr) const final;
        void store(std::uint16_t addr, std::uint8_t data) final;
        void store(std::uint16_t addr, std::uint16_t data) final;
        [[nodiscard]] std::vector<uint8_t> const& data() const final;

    private:
        std::unique_ptr<cpu_mem_bus_impl> _impl;
    };
}
#endif //NES_CPP_CPU_MEM_BUS_H
