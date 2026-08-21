#include <SDL.h>
#include <cstdio>
#include <fstream>
#include <vector>
#include <cstdint>

#include "Bus.h"
#include "Cpu.h"

bool isLogoValid(const std::vector<uint8_t>& rom);
bool isHeaderChecksumValid(const std::vector<uint8_t>& rom);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::fprintf(stderr, "Usage: %s <rom.gb>\n", argv[0]);
        return 1;
    }

    std::ifstream romFile(argv[1], std::ios::binary);
    if (!romFile) {
        std::fprintf(stderr, "Failed to open ROM: %s\n", argv[1]);
        return 1;
    }

    std::vector<uint8_t> rom;
    char byte;
    while (romFile.get(byte)) {
        rom.push_back(static_cast<uint8_t>(byte));
    }

    if (rom.size() < 0x150) {
        std::printf("ROM size is too small to contain a Game Boy header\n");
        return 1;
    }

    std::printf("ROM size: %zu bytes\n", rom.size());
    std::printf(
        "Entry point: %02X, %02X, %02X, %02X\n",
        rom[0x100],
        rom[0x101],
        rom[0x102],
        rom[0x103]
    );
    std::printf("Title: ");
    for (std::size_t i = 0x0134; i <= 0x0143; i++){
        if (rom[i] == 0) break;
        std::printf("%c", rom[i]);
    }
    std::printf("\n");
    std::printf("Cartridge type: 0x%02X\n", rom[0x0147]);
    std::printf("ROM size code: 0x%02X\n", rom[0x0148]);
    std::printf("RAM size code: 0x%02X\n", rom[0x0149]);

    if (!isLogoValid(rom)) {
        std::fprintf(stderr, "Invalid Nintendo logo\n");
        return 1;
    }
    if (!isHeaderChecksumValid(rom)) {
        std::fprintf(stderr, "Invalid header checksum\n");
        return 1;
    }

    Bus bus(rom);
    

    // Test step
    std::vector<uint8_t> testRom(0x8000, 0);
    testRom[0x0100] = 0x3E;
    testRom[0x0101] = 0x42;

    Bus testBus(testRom);
    Cpu cpu(testBus);
    std::printf("PC before step: %04X\n", cpu.getPC());
    std::printf("A register before step: %02X\n", cpu.getA());
    int cycles = cpu.step();
    std::printf("PC after step: %04X\n", cpu.getPC());
    std::printf("A register after step: %02X\n", cpu.getA());
    std::printf("Cycles: %d\n", cycles);

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::printf("SDL_Init error: %s\n", SDL_GetError());
        return 1;
    }
    
    SDL_Window* window = SDL_CreateWindow(
        "Game Boy Emulator",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        160 * 4,
        144 * 4,
        0
    );

    if (window == nullptr) {
        std::printf("SDL_CreateWindow error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    
    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    if (renderer == nullptr) {
        std::printf("SDL_CreateRenderer error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool running = true;
    while (running) {
        SDL_SetRenderDrawColor(renderer, 100, 150, 100, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }
        SDL_Delay(10);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

bool isLogoValid(const std::vector<uint8_t>& rom) {
    const uint8_t nintendoLogo[] = {
        0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B,
        0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D,
        0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E,
        0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99,
        0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC,
        0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E
    };

    for (std::size_t i = 0; i < sizeof(nintendoLogo); i++){
        if (rom[0x0104 + i] != nintendoLogo[i]) return false;
    }
    return true;
}
bool isHeaderChecksumValid(const std::vector<uint8_t>& rom) {
    uint8_t checksum = 0;
    for (std::size_t i = 0x0134; i <= 0x014C; i++) {
        checksum = checksum - rom[i] - 1;
    }
    return checksum == rom[0x014D];
}