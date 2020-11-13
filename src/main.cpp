//
// Created by syl on 11/11/2020.
//
#include <imgui.h>
#include <imgui-SFML.h>
#include "hex_editor.h"

#include <spdlog/spdlog.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include "memory/block.h"
#include "cpu/cpu_mem_bus.h"
#include "cpu/decoder.h"
#include "cpu/regs.h"
#include "cartridge/cartridge.h"


int main(int ac, char **av) {
    auto cartridge = std::make_shared<nes::cartridge::cartridge>(std::move(std::filesystem::path(av[1])));
    auto regs = std::make_shared<nes::cpu::regs>();
    auto membus = std::make_shared<nes::cpu::cpu_mem_bus>(cartridge);
    auto decoder = nes::cpu::decoder();
    regs->pc = membus->fetch_u16(0xfffc);
    regs->sr = 0x20;
    regs->sp = 0xfd;

    sf::RenderWindow window(sf::VideoMode(1600, 800), "ImGui + SFML = <3");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    sf::Clock deltaClock;
    bool show_debug{true};
    static MemoryEditor mem_edit;
    mem_edit.GotoAddrAndHighlight(0x200, 0x300);

    auto op = decoder.decode(regs->pc, regs, membus);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                    case sf::Keyboard::Right:
                        op = decoder.decode(regs->pc, regs, membus);
                        regs->pc += op.bytes;
                        break;
                    case sf::Keyboard::H:
                        show_debug = !show_debug;
                        break;
                    default:
                        break;
                }
            }
        }


        ImGui::SFML::Update(window, deltaClock.restart());

        if (show_debug) {

            mem_edit.DrawWindow("Cartridge", const_cast<uint8_t *>(cartridge->data().data()), cartridge->data().size());

            ImGui::Begin("Registers");
            ImGui::LabelText("PC", "%s", fmt::format("{:#06x} => {:#018b}", regs->pc, regs->pc).c_str());
            ImGui::LabelText("AC", "%s", fmt::format("{:#06x} => {:#018b}", regs->ac, regs->ac).c_str());
            ImGui::LabelText("X", "%s", fmt::format("{:#06x} => {:#018b}", regs->x, regs->x).c_str());
            ImGui::LabelText("Y", "%s", fmt::format("{:#06x} => {:#018b}", regs->y, regs->y).c_str());
            ImGui::LabelText("SR", "%s", fmt::format("{:#06x} => {:#018b}", regs->sr, regs->sr).c_str());
            ImGui::LabelText("SP", "%s", fmt::format("{:#06x} => {:#018b}", regs->sp, regs->sp).c_str());
            ImGui::End();

            ImGui::Begin("Code");
            auto decode_instr = decoder.decode(5, regs->pc, regs, membus);
            auto pc = regs->pc;
            for (int i = 0; i < decode_instr.size(); i++) {
                ImGui::LabelText("PC", "%s", fmt::format("{:#06x} => | {:#04x} {:#04x} {:#04x} {:#04x}| => {}",
                                                         pc,
                                                         membus->fetch_u8(pc),
                                                         membus->fetch_u8(pc + 1),
                                                         membus->fetch_u8(pc + 2),
                                                         membus->fetch_u8(pc + 3),
                                                         decode_instr[i]).c_str());
                pc += decode_instr[i].bytes;
            }
            ImGui::End();
        }

        window.clear();
        window.draw(shape);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}