#include "babykyber_hal.h"

// Hardware Abstraction Layer implementation

void babykyber_write_reg(uint32_t addr, uint32_t value) {
    *(volatile uint32_t *)addr = value;
}

uint32_t babykyber_read_reg(uint32_t addr) {
    return *(volatile uint32_t *)addr;
}

void babykyber_write_array(uint32_t base_addr, const uint32_t *data, uint32_t count) {
    for (uint32_t i = 0; i < count; i++) {
        babykyber_write_reg(base_addr + i * 4, data[i]);
    }
}

void babykyber_read_array(uint32_t base_addr, uint32_t *data, uint32_t count) {
    for (uint32_t i = 0; i < count; i++) {
        data[i] = babykyber_read_reg(base_addr + i * 4);
    }
}
