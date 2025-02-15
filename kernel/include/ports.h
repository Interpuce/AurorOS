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

uint8_t inb(uint16_t port);
uint16_t inw(uint16_t port);
void outb(uint16_t port, uint8_t value);
void outw(uint16_t port, uint16_t data);
void outl(uint16_t port, uint32_t value);
uint32_t inl(uint16_t port);