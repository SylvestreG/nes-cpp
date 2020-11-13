#include <memory>
#include "memory_interface.h"

namespace nes::memory {
    struct block_impl;

    class block : public memory_iface {
    public:
        explicit block(std::uint16_t size);
        explicit block(std::vector<uint8_t> data);
        ~block();
        block(block const&) = delete;
        block& operator=(block const&) = delete;

        uint8_t fetch_u8(std::uint16_t addr) const final;
        uint16_t fetch_u16(std::uint16_t addr) const final;
        void store(std::uint16_t addr, std::uint8_t data) final;
        void store(std::uint16_t addr, std::uint16_t data) final;
        [[nodiscard]] std::vector<uint8_t> const& data() const final;

    private:
        std::unique_ptr<block_impl> _impl;
    };
}