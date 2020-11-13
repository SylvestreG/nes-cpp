#include <fstream>
#include <streambuf>
#include <vector>

#include <spdlog/spdlog.h>

#include "cartridge/cartridge.h"
#include "memory/block.h"

using namespace nes::cartridge;

struct nes::cartridge::cartridge_impl {
private:
    std::filesystem::path _path;
    bool _mirroring{false};
    bool _battery{false};
    bool _trainer{false};
    bool _ignore_mirr{false};
    mapper_type _mapper{mapper_type::mapper_0};

    std::unique_ptr<memory::block> _prg_rom;
    std::unique_ptr<memory::block> _prg_ram;
    std::unique_ptr<memory::block> _chr_ram;

    friend cartridge;
};

cartridge::cartridge(std::filesystem::path path) : _impl(std::make_unique<cartridge_impl>()) {
    _impl->_path = std::move(path);

    std::ifstream file(_impl->_path.string(), std::ios::binary);
    file.unsetf(std::ios::skipws);

    std::vector<uint8_t> data;
    spdlog::info("{:#06x}", std::filesystem::file_size(_impl->_path));

    data.reserve(std::filesystem::file_size(_impl->_path));
    std::copy(std::istream_iterator<uint8_t>(file), std::istream_iterator<uint8_t>(), std::back_insert_iterator(data));

    struct {
        uint8_t magic[4];
        uint8_t nb_prog_rom;
        uint8_t nb_chr_rom;
        uint8_t flag6;
    } *header{reinterpret_cast<decltype(header)>(&data[0])};

    if (header->flag6 & 0x01) _impl->_mirroring = true;
    if (header->flag6 & 0x02) _impl->_battery = true;
    if (header->flag6 & 0x04) _impl->_trainer = true;
    if (header->flag6 & 0x08) _impl->_ignore_mirr = true;

    if ((header->flag6 >> 4) != 0) {
        spdlog::error("invalid mapper"); //TBD throw...
        exit(EXIT_FAILURE);
    }

    std::vector<uint8_t> romMemory;
    auto offset = 16;
    romMemory.insert(romMemory.end(), data.begin() + offset,
                     data.begin() + offset + (header->nb_prog_rom * 16384));
    _impl->_prg_rom = std::make_unique<memory::block>(std::move(romMemory));

    std::vector<uint8_t> chrMemory;
    offset += header->nb_prog_rom * 16384;
    chrMemory.insert(chrMemory.end(), std::make_move_iterator(data.begin() + offset),
                     std::make_move_iterator(data.begin() + offset + (header->nb_prog_rom * 8192)));
    _impl->_chr_ram = std::make_unique<memory::block>(std::move(chrMemory));
}

cartridge::~cartridge() = default;

std::filesystem::path cartridge::file() const noexcept {
    return _impl->_path;
}

bool cartridge::mirroring() const noexcept {
    return _impl->_mirroring;
}

bool cartridge::battery_packed_ram() const noexcept {
    return _impl->_battery;
}

bool cartridge::trainer() const noexcept {
    return _impl->_trainer;
}

bool cartridge::ignore_mirroring() const noexcept {
    return _impl->_mirroring;
}

mapper_type cartridge::mapper() const noexcept {
    return mapper_type::mapper_0;
}

uint8_t cartridge::fetch_u8(std::uint16_t addr) const {
    if (addr < 0x2000)
        return _impl->_prg_ram->fetch_u8(addr);
    else if (addr < 0x6000)
        return _impl->_prg_rom->fetch_u8(addr - 0x2000);
    else
        return _impl->_prg_rom->fetch_u8(addr - 0x6000);
}

uint16_t cartridge::fetch_u16(std::uint16_t addr) const {
    if (addr < 0x2000)
        return _impl->_prg_ram->fetch_u16(addr);
    else if (addr < 0x6000)
        return _impl->_prg_rom->fetch_u16(addr - 0x2000);
    else
        return _impl->_prg_rom->fetch_u16(addr - 0x6000);
}

void cartridge::store(std::uint16_t addr, std::uint8_t data) {

}

void cartridge::store(std::uint16_t addr, std::uint16_t data) {

}

std::vector<uint8_t> const& cartridge::data() const {
    return _impl->_prg_rom->data();
}
