//
// Created by syl on 11/11/2020.
//

#include <vector>
#include <cstdint>

#include <spdlog/spdlog.h>

#include "memory/block.h"

using namespace nes::memory;

struct nes::memory::block_impl {
private:
    std::vector<uint8_t> _data;

    friend block;
};

block::block(std::uint16_t size) : _impl(std::make_unique<block_impl>()) {
    _impl->_data.resize(size, 0);
}

block::block(std::vector<uint8_t> vec) : _impl(std::make_unique<block_impl>()) {
    _impl->_data = std::move(vec);
}

uint8_t block::fetch_u8(std::uint16_t addr) const {
    if (addr > _impl->_data.size())
        ;//TODO

    return _impl->_data[addr];
}

uint16_t block::fetch_u16(std::uint16_t addr) const {
    if ((addr + 1) > _impl->_data.size())
    ;//TODO

    return _impl->_data[addr] + (_impl->_data[addr + 1] << 8u);
}

void block::store(std::uint16_t addr, std::uint8_t data) {
    if (addr > _impl->_data.size())
        //TODO;

    _impl->_data[addr] = data;
}

void block::store(std::uint16_t addr, std::uint16_t data) {
    if (addr > _impl->_data.size() + 1)
        //TODO;

    _impl->_data[addr] = data & 0x00ffu;
    _impl->_data[addr + 1] = (data & 0xff00u) >> 8u;
}

std::vector<uint8_t> const &block::data() const {
    return _impl->_data;
}


block::~block() = default;
