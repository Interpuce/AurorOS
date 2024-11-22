/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2024 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

// DECLARATION FILE
// The source for these function can be found at:
//   lib/math

#pragma once

#include <types.h>

// Prime numbers
extern int is_prime(int n);
extern int next_prime_number(int n);
extern int previous_prime_number(int n);
extern bool has_previous_prime_number(int n);

// GCD
extern int gcd(int a, int b);

// LCM
extern int lcm(int a, int b);

// Powers
extern int highest_power_of_two(int n);
extern int power(int base, int exp);

// Covertion to string
extern void int_to_hex(int n, string buffer);
extern void int_to_text(long long n, string buffer);