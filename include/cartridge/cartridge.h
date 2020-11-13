//
// Created by syl on 12/11/2020.
//

#ifndef NES_CPP_CARTRIDGE_H
#define NES_CPP_CARTRIDGE_H

#include <memory>
#include <filesystem>

#include <fmt/format.h>

#include "memory/memory_interface.h"

namespace nes::cartridge {
    struct cartridge_impl;

    enum class mapper_type : std::uint8_t {
        mapper_0
    };

    class cartridge : public memory::memory_iface {
    public:
        explicit cartridge(std::filesystem::path path);

        ~cartridge();

        cartridge(cartridge const &) = delete;

        cartridge &operator=(cartridge const &) = delete;

        [[nodiscard]] std::filesystem::path file() const noexcept;

        [[nodiscard]] bool mirroring() const noexcept;

        [[nodiscard]] bool battery_packed_ram() const noexcept;

        [[nodiscard]] bool trainer() const noexcept;

        [[nodiscard]] bool ignore_mirroring() const noexcept;

        [[nodiscard]] mapper_type mapper() const noexcept;

        [[nodiscard]] uint8_t fetch_u8(std::uint16_t addr) const final;

        [[nodiscard]] uint16_t fetch_u16(std::uint16_t addr) const final;

        [[nodiscard]] std::vector<uint8_t> const& data() const final;

        void store(std::uint16_t addr, std::uint8_t data) final;

        void store(std::uint16_t addr, std::uint16_t data) final;

    private:
        std::unique_ptr<cartridge_impl> _impl;
    };
}

//fmt overload
template<>
struct fmt::formatter<nes::cartridge::cartridge> {
    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(const nes::cartridge::cartridge &p, FormatContext &ctx) {
        return format_to(ctx.out(), "rom_file={} m={} b={} t={} i={} mapper={}", p.file().string(), p.mirroring(),
                         p.battery_packed_ram(), p.trainer(), p.ignore_mirroring(), p.mapper());
    }
};

#endif //NES_CPP_CARTRIDGE_H
