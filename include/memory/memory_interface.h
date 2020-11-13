//
// Created by syl on 11/11/2020.
//

#ifndef NES_CPP_MEMORY_INTERFACE_H
#define NES_CPP_MEMORY_INTERFACE_H

namespace nes::memory {

    class memory_iface {
    public:
        memory_iface() = default;
        memory_iface(memory_iface const&) = delete;
        memory_iface& operator=(memory_iface const&) = delete;
        ~memory_iface() = default;

        void dump() const noexcept { };
        void dump_slice(std::uint16_t begin, std::uint16_t end) const noexcept {};

        virtual std::vector<uint8_t> const& data() const = 0;

        virtual uint8_t fetch_u8(std::uint16_t addr) const = 0;
        virtual uint16_t fetch_u16(std::uint16_t addr) const = 0;
        virtual void store(std::uint16_t addr, std::uint8_t data) = 0;
        virtual void store(std::uint16_t addr, std::uint16_t data) = 0;
    };
}

#endif //NES_CPP_MEMORY_INTERFACE_H
