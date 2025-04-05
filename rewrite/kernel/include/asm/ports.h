/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2025 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#pragma once

#include <types.h>

extern uint8 inb(uint16 port);
extern uint16 inw(uint16 port);
extern uint32 inl(uint16 port);
extern void outb(uint16 port, uint8 value);
extern void outw(uint16 port, uint16 data);
extern void outl(uint16 port, uint32 value);