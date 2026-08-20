# Game Boy Memory Map

This document summarizes the original Game Boy (DMG) 16-bit CPU address space.

## Address Space Overview

| Address Range | Region | Notes |
|---|---|---|
| `0x0000–0x3FFF` | Cartridge ROM Bank 0 | Fixed ROM bank |
| `0x4000–0x7FFF` | Cartridge ROM Bank 1–N | Switchable ROM bank when using an MBC |
| `0x8000–0x9FFF` | VRAM | Video RAM used by the PPU |
| `0xA000–0xBFFF` | External RAM | Cartridge RAM, when present |
| `0xC000–0xDFFF` | WRAM | Internal Work RAM |
| `0xE000–0xFDFF` | Echo RAM | Mirror of `0xC000–0xDDFF` |
| `0xFE00–0xFE9F` | OAM | Sprite/Object Attribute Memory |
| `0xFEA0–0xFEFF` | Unusable | Reads/writes should not be treated as normal RAM |
| `0xFF00–0xFF7F` | I/O Registers | Joypad, timers, PPU registers, audio, etc. |
| `0xFF80–0xFFFE` | HRAM | High RAM |
| `0xFFFF` | IE Register | Interrupt Enable register |

## Address Translation

The CPU sees a single 16-bit address space:

```text
0x0000 ... 0xFFFF
```

The `Bus` is responsible for deciding which component owns a given address.

Conceptually:

```text
CPU
 |
 | read(address) / write(address, value)
 v
Bus
 |
 +--> Cartridge ROM / RAM
 +--> VRAM
 +--> WRAM
 +--> OAM
 +--> I/O registers
 +--> HRAM
```

For memory regions stored in local arrays, translate a CPU address into an array index with:

```text
local_index = address - region_start
```

Example for VRAM:

```text
VRAM starts at 0x8000

0x8000 -> vram[0x0000]
0x8001 -> vram[0x0001]
0x8123 -> vram[0x0123]
```

Example for WRAM:

```text
WRAM starts at 0xC000

0xC000 -> wram[0x0000]
0xC001 -> wram[0x0001]
0xC123 -> wram[0x0123]
```

## Region Sizes

The size of an inclusive address range is:

```text
size = end_address - start_address + 1
```

Useful examples:

```text
VRAM:
0x9FFF - 0x8000 + 1 = 0x2000 = 8192 bytes

WRAM:
0xDFFF - 0xC000 + 1 = 0x2000 = 8192 bytes

OAM:
0xFE9F - 0xFE00 + 1 = 0x00A0 = 160 bytes

HRAM:
0xFFFE - 0xFF80 + 1 = 0x007F = 127 bytes
```

## Echo RAM

`0xE000–0xFDFF` mirrors part of WRAM:

```text
0xE000 -> 0xC000
0xE001 -> 0xC001
...
0xFDFF -> 0xDDFF
```

No separate storage is required for Echo RAM.

The mirrored WRAM address can be obtained conceptually by translating the Echo RAM address back into the WRAM range.

## Initial Emulator Scope

For the first Bus implementation, support only:

- Cartridge ROM
- VRAM
- WRAM
- HRAM

Then add:

- Echo RAM
- OAM
- Interrupt Enable (`0xFFFF`)
- I/O registers
- External cartridge RAM

Memory Bank Controllers (MBCs) can be implemented later.

## Bus Interface

A minimal Bus interface can expose:

```cpp
uint8_t read(uint16_t address) const;
void write(uint16_t address, uint8_t value);
```

The CPU should not need to know whether an address maps to a vector, array, hardware register, or another emulated component.

For example:

```text
PC = 0x0100
opcode = bus.read(PC)
```

The Bus determines that `0x0100` belongs to cartridge ROM and returns the corresponding byte.

Likewise:

```text
bus.write(0xC123, 0x42)
```

maps to WRAM, while a future write to an address such as `0xFF40` will represent access to a memory-mapped hardware register rather than ordinary RAM.
