#include <spdlog/spdlog.h>

#include "cpu/cpu_mem_bus.h"
#include "memory/block.h"

using namespace nes::cpu;

struct nes::cpu::cpu_mem_bus_impl {
private:
    std::unique_ptr<memory::block> _internal_ram;
    std::shared_ptr<cartridge::cartridge> _cartridge;

    friend cpu_mem_bus;
};

cpu_mem_bus::cpu_mem_bus(std::shared_ptr<cartridge::cartridge> cartridge) noexcept: _impl(
        std::make_unique<cpu_mem_bus_impl>()) {
    _impl->_internal_ram = std::make_unique<memory::block>(0x800);
    _impl->_cartridge = std::move(cartridge);
}

cpu_mem_bus::~cpu_mem_bus() = default;

uint8_t cpu_mem_bus::fetch_u8(std::uint16_t addr) const {
    switch (addr_to_mem_type(addr)) {
        case mem_type::internal:
            spdlog::trace("internal fetch u8 at {}", addr);
            return _impl->_internal_ram->fetch_u8(addr % 0x800);
        case mem_type::cartridge:
            spdlog::trace("cartridge fetch u8 at {}", addr);
            return _impl->_cartridge->fetch_u8(addr - 0x6000);
        case mem_type::ppu:
            spdlog::error("ppu not implemented");
            break;
        case mem_type::none:
            spdlog::error("invalid address in cpu_mem_bus");
            break;
    }

    return 0;
}

uint16_t cpu_mem_bus::fetch_u16(std::uint16_t addr) const {
    switch (addr_to_mem_type(addr)) {
        case mem_type::internal:
            spdlog::trace("internal fetch u16 at {}", addr);
            return _impl->_internal_ram->fetch_u16(addr % 0x800);
        case mem_type::cartridge:
            spdlog::trace("cartridge fetch u16 at {}", addr);
            return _impl->_cartridge->fetch_u16(addr - 0x6000u);
        case mem_type::ppu:
            spdlog::error("ppu not implemented");
            break;
        case mem_type::none:
            spdlog::error("invalid address in cpu_mem_bus");
            break;
    }

    return 0;
}

void cpu_mem_bus::store(std::uint16_t addr, std::uint8_t data) {
    switch (addr_to_mem_type(addr)) {
        case mem_type::internal:
            spdlog::trace("internal store u8 {} at {}", data, addr);
            _impl->_internal_ram->store(addr, data);
            break;
        case mem_type::cartridge:
            _impl->_cartridge->store(addr, data);
            spdlog::trace("cartridge store u8 {} at {}", data, addr);
            break;
        case mem_type::ppu:
            spdlog::error("ppu not implemented");
            break;
        case mem_type::none:
            spdlog::error("invalid address in cpu_mem_bus");
            break;
    }
}

void nes::cpu::cpu_mem_bus::store(std::uint16_t addr, std::uint16_t data) {
    switch (addr_to_mem_type(addr)) {
        case mem_type::internal:
            spdlog::trace("internal store u16 {} at {}", data, addr);
            _impl->_internal_ram->store(addr, data);
            break;
        case mem_type::cartridge:
            _impl->_cartridge->store(addr, data);
            spdlog::trace("cartridge store u16 {} at {}", data, addr);
            break;
        case mem_type::ppu:
            spdlog::error("ppu not implemented");
            break;
        case mem_type::none:
            spdlog::error("invalid address in cpu_mem_bus");
            break;
    }
}

std::vector<uint8_t> const &cpu_mem_bus::data() const {
    return _impl->_internal_ram->data();
}
