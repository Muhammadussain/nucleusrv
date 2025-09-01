#ifndef BABYKYBER_HAL_H
#define BABYKYBER_HAL_H

#include <stdint.h>

// Hardware Abstraction Layer functions for BabyKyber

// Write a 32-bit value to a register
void babykyber_write_reg(uint32_t addr, uint32_t value);

// Read a 32-bit value from a register
uint32_t babykyber_read_reg(uint32_t addr);

// Write an array of 32-bit values to consecutive registers
void babykyber_write_array(uint32_t base_addr, const uint32_t *data, uint32_t count);

// Read an array of 32-bit values from consecutive registers
void babykyber_read_array(uint32_t base_addr, uint32_t *data, uint32_t count);

#endif // BABYKYBER_HAL_H
