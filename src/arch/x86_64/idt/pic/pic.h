#pragma once

#include <types.h>

void pic_remap(int offset1, int offset2);
void pic_send_eoi(uint32_t vector);
