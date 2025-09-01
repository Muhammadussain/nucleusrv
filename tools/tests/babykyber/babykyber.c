#include "babykyber.h"
#include "babykyber_hal.h"
#include "babykyber_regs.h"

// BabyKyber Constants and Values

// Address constants for register mapping
#define BABYKBER_BASE_ADDR     0x40007000
#define BABYKBER_ADDR_INCREMENT 4
#define BABYKBER_ADDR_OFFSETT 8

// Random vector for encryption (8 values)
const uint32_t r_vector[8] = {25, 26, 27, 28, 29, 30, 31, 32};

// Error vector 1 for encryption (8 values)
const uint32_t e1_vector[8] = {33, 34, 35, 36, 37, 38, 39, 40};

// Error vector 2 for encryption (4 values)
const uint32_t e2_vector[4] = {41, 42, 43, 44};

// Demo message value
const uint32_t demo_message = 42;

// Demo A_t matrix (4x4)
const uint32_t demo_A_t[4][4] = {
    {1, 2, 3, 4},
    {5, 6, 7, 8},
    {9, 10, 11, 12},
    {13, 14, 15, 16}
};

// Demo s vector (8 values)
const uint32_t demo_s[8] = {17, 18, 19, 20, 21, 22, 23, 24};

// Demo e vector (8 values)
const uint32_t demo_e[8] = {25, 26, 27, 28, 29, 30, 31, 32};

// Demo public key (24 values for 2 polynomials)
const uint32_t demo_pk[24] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24};

// Demo ciphertext (12 values for decryption)
const uint32_t demo_ct[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

int main() {
    // Use the demo constants to set up the hardware registers
    
    // Set A_t matrix - using exact addresses from main.c
    babykyber_write_reg(BABYKBER_BASE_ADDR + 0 * BABYKBER_ADDR_INCREMENT, demo_A_t[0][0]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 1 * BABYKBER_ADDR_INCREMENT, demo_A_t[0][1]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 2 * BABYKBER_ADDR_INCREMENT, demo_A_t[0][2]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 2 * BABYKBER_ADDR_INCREMENT + BABYKBER_ADDR_OFFSETT, demo_A_t[0][3]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 5 * BABYKBER_ADDR_INCREMENT + BABYKBER_ADDR_OFFSETT, demo_A_t[1][0]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 8 * BABYKBER_ADDR_INCREMENT + BABYKBER_ADDR_OFFSETT, demo_A_t[1][1]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 9 * BABYKBER_ADDR_INCREMENT + BABYKBER_ADDR_OFFSETT, demo_A_t[1][2]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 10 * BABYKBER_ADDR_INCREMENT + BABYKBER_ADDR_OFFSETT, demo_A_t[1][3]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 14 * BABYKBER_ADDR_INCREMENT + 2 * BABYKBER_ADDR_OFFSETT, demo_A_t[2][0]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 15 * BABYKBER_ADDR_INCREMENT + 2 * BABYKBER_ADDR_OFFSETT, demo_A_t[2][1]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 16 * BABYKBER_ADDR_INCREMENT + 3 * BABYKBER_ADDR_OFFSETT, demo_A_t[2][2]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 17 * BABYKBER_ADDR_INCREMENT + 3 * BABYKBER_ADDR_OFFSETT, demo_A_t[2][3]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 18 * BABYKBER_ADDR_INCREMENT + 3 * BABYKBER_ADDR_OFFSETT, demo_A_t[3][0]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 19 * BABYKBER_ADDR_INCREMENT + 4 * BABYKBER_ADDR_OFFSETT, demo_A_t[3][1]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 22 * BABYKBER_ADDR_INCREMENT + 4 * BABYKBER_ADDR_OFFSETT, demo_A_t[3][2]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 24 * BABYKBER_ADDR_INCREMENT + 4 * BABYKBER_ADDR_OFFSETT, demo_A_t[3][3]);
    
    // Set s vector - using exact addresses from main.c
    babykyber_write_reg(BABYKBER_BASE_ADDR + 25 * BABYKBER_ADDR_INCREMENT + 4 * BABYKBER_ADDR_OFFSETT, demo_s[0]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 26 * BABYKBER_ADDR_INCREMENT + 4 * BABYKBER_ADDR_OFFSETT, demo_s[1]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 27 * BABYKBER_ADDR_INCREMENT + 5 * BABYKBER_ADDR_OFFSETT, demo_s[2]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 31 * BABYKBER_ADDR_INCREMENT + 5 * BABYKBER_ADDR_OFFSETT, demo_s[3]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 32 * BABYKBER_ADDR_INCREMENT + 5 * BABYKBER_ADDR_OFFSETT, demo_s[4]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 33 * BABYKBER_ADDR_INCREMENT + 5 * BABYKBER_ADDR_OFFSETT, demo_s[5]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 34 * BABYKBER_ADDR_INCREMENT + 5 * BABYKBER_ADDR_OFFSETT, demo_s[6]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 35 * BABYKBER_ADDR_INCREMENT + 6 * BABYKBER_ADDR_OFFSETT, demo_s[7]);
    
    // Set e vector - using exact addresses from main.c
    babykyber_write_reg(BABYKBER_BASE_ADDR + 39 * BABYKBER_ADDR_INCREMENT + 6 * BABYKBER_ADDR_OFFSETT, demo_e[0]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 64 * BABYKBER_ADDR_INCREMENT + 6 * BABYKBER_ADDR_OFFSETT, demo_e[1]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 65 * BABYKBER_ADDR_INCREMENT + 6 * BABYKBER_ADDR_OFFSETT, demo_e[2]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 66 * BABYKBER_ADDR_INCREMENT + 6 * BABYKBER_ADDR_OFFSETT, demo_e[3]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 67 * BABYKBER_ADDR_INCREMENT + 7 * BABYKBER_ADDR_OFFSETT, demo_e[4]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 71 * BABYKBER_ADDR_INCREMENT + 7 * BABYKBER_ADDR_OFFSETT, demo_e[5]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 80 * BABYKBER_ADDR_INCREMENT + 7 * BABYKBER_ADDR_OFFSETT, demo_e[6]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 81 * BABYKBER_ADDR_INCREMENT + 7 * BABYKBER_ADDR_OFFSETT, demo_e[7]);
    
    // Trigger key generation
    babykyber_write_reg(BABYKYBER_KEYGEN_TRIGGER, 0);
    
    // Set message
    babykyber_write_reg(BABYKYBER_MESSAGE, demo_message);
    
    // Set public key for encryption - using exact addresses from main.c
    babykyber_write_reg(BABYKBER_BASE_ADDR + 76 * BABYKBER_ADDR_INCREMENT + 7 * BABYKBER_ADDR_OFFSETT, demo_pk[0]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 77 * BABYKBER_ADDR_INCREMENT + 7 * BABYKBER_ADDR_OFFSETT, demo_pk[1]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 80 * BABYKBER_ADDR_INCREMENT + 8 * BABYKBER_ADDR_OFFSETT, demo_pk[2]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 81 * BABYKBER_ADDR_INCREMENT + 8 * BABYKBER_ADDR_OFFSETT, demo_pk[3]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 82 * BABYKBER_ADDR_INCREMENT + 8 * BABYKBER_ADDR_OFFSETT, demo_pk[4]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 83 * BABYKBER_ADDR_INCREMENT + 9 * BABYKBER_ADDR_OFFSETT, demo_pk[5]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 87 * BABYKBER_ADDR_INCREMENT + 9 * BABYKBER_ADDR_OFFSETT, demo_pk[6]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 88 * BABYKBER_ADDR_INCREMENT + 9 * BABYKBER_ADDR_OFFSETT, demo_pk[7]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 89 * BABYKBER_ADDR_INCREMENT + 9 * BABYKBER_ADDR_OFFSETT, demo_pk[8]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 90 * BABYKBER_ADDR_INCREMENT + 9 * BABYKBER_ADDR_OFFSETT, demo_pk[9]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 91 * BABYKBER_ADDR_INCREMENT + 10 * BABYKBER_ADDR_OFFSETT, demo_pk[10]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 95 * BABYKBER_ADDR_INCREMENT + 10 * BABYKBER_ADDR_OFFSETT, demo_pk[11]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 96 * BABYKBER_ADDR_INCREMENT + 10 * BABYKBER_ADDR_OFFSETT, demo_pk[12]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 97 * BABYKBER_ADDR_INCREMENT + 10 * BABYKBER_ADDR_OFFSETT, demo_pk[13]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 98 * BABYKBER_ADDR_INCREMENT + 10 * BABYKBER_ADDR_OFFSETT, demo_pk[14]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 99 * BABYKBER_ADDR_INCREMENT + 11 * BABYKBER_ADDR_OFFSETT, demo_pk[15]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 103 * BABYKBER_ADDR_INCREMENT + 11 * BABYKBER_ADDR_OFFSETT, demo_pk[16]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 128 * BABYKBER_ADDR_INCREMENT + 11 * BABYKBER_ADDR_OFFSETT, demo_pk[17]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 129 * BABYKBER_ADDR_INCREMENT + 11 * BABYKBER_ADDR_OFFSETT, demo_pk[18]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 130 * BABYKBER_ADDR_INCREMENT + 11 * BABYKBER_ADDR_OFFSETT, demo_pk[19]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 131 * BABYKBER_ADDR_INCREMENT + 12 * BABYKBER_ADDR_OFFSETT, demo_pk[20]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 135 * BABYKBER_ADDR_INCREMENT + 12 * BABYKBER_ADDR_OFFSETT, demo_pk[21]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 144 * BABYKBER_ADDR_INCREMENT + 12 * BABYKBER_ADDR_OFFSETT, demo_pk[22]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 145 * BABYKBER_ADDR_INCREMENT + 12 * BABYKBER_ADDR_OFFSETT, demo_pk[23]);
    
    // Set r vector - using exact addresses from main.c
    babykyber_write_reg(BABYKBER_BASE_ADDR + 146 * BABYKBER_ADDR_INCREMENT + 12 * BABYKBER_ADDR_OFFSETT, r_vector[0]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 148 * BABYKBER_ADDR_INCREMENT + 13 * BABYKBER_ADDR_OFFSETT, r_vector[1]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 152 * BABYKBER_ADDR_INCREMENT + 13 * BABYKBER_ADDR_OFFSETT, r_vector[2]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 160 * BABYKBER_ADDR_INCREMENT + 13 * BABYKBER_ADDR_OFFSETT, r_vector[3]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 161 * BABYKBER_ADDR_INCREMENT + 13 * BABYKBER_ADDR_OFFSETT, r_vector[4]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 162 * BABYKBER_ADDR_INCREMENT + 13 * BABYKBER_ADDR_OFFSETT, r_vector[5]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 163 * BABYKBER_ADDR_INCREMENT + 14 * BABYKBER_ADDR_OFFSETT, r_vector[6]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 167 * BABYKBER_ADDR_INCREMENT + 14 * BABYKBER_ADDR_OFFSETT, r_vector[7]);
    
    // Set e1 vector - using exact addresses from main.c
    babykyber_write_reg(BABYKBER_BASE_ADDR + 168 * BABYKBER_ADDR_INCREMENT + 14 * BABYKBER_ADDR_OFFSETT, e1_vector[0]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 169 * BABYKBER_ADDR_INCREMENT + 14 * BABYKBER_ADDR_OFFSETT, e1_vector[1]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 170 * BABYKBER_ADDR_INCREMENT + 14 * BABYKBER_ADDR_OFFSETT, e1_vector[2]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 171 * BABYKBER_ADDR_INCREMENT + 15 * BABYKBER_ADDR_OFFSETT, e1_vector[3]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 175 * BABYKBER_ADDR_INCREMENT + 15 * BABYKBER_ADDR_OFFSETT, e1_vector[4]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 176 * BABYKBER_ADDR_INCREMENT + 15 * BABYKBER_ADDR_OFFSETT, e1_vector[5]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 177 * BABYKBER_ADDR_INCREMENT + 15 * BABYKBER_ADDR_OFFSETT, e1_vector[6]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 178 * BABYKBER_ADDR_INCREMENT + 15 * BABYKBER_ADDR_OFFSETT, e1_vector[7]);
    
    // Set e2 vector - using exact addresses from main.c
    babykyber_write_reg(BABYKBER_BASE_ADDR + 179 * BABYKBER_ADDR_INCREMENT + 16 * BABYKBER_ADDR_OFFSETT, e2_vector[0]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 183 * BABYKBER_ADDR_INCREMENT + 16 * BABYKBER_ADDR_OFFSETT, e2_vector[1]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 192 * BABYKBER_ADDR_INCREMENT + 16 * BABYKBER_ADDR_OFFSETT, e2_vector[2]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 193 * BABYKBER_ADDR_INCREMENT + 16 * BABYKBER_ADDR_OFFSETT, e2_vector[3]);
    
    // Trigger encryption
    babykyber_write_reg(BABYKYBER_ENCRYPT_TRIGGER, 0);
    
    // Set ciphertext for decryption - using exact addresses from main.c
    babykyber_write_reg(BABYKBER_BASE_ADDR + 194 * BABYKBER_ADDR_INCREMENT + 16 * BABYKBER_ADDR_OFFSETT, demo_ct[0]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 196 * BABYKBER_ADDR_INCREMENT + 17 * BABYKBER_ADDR_OFFSETT, demo_ct[1]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 200 * BABYKBER_ADDR_INCREMENT + 17 * BABYKBER_ADDR_OFFSETT, demo_ct[2]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 208 * BABYKBER_ADDR_INCREMENT + 17 * BABYKBER_ADDR_OFFSETT, demo_ct[3]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 209 * BABYKBER_ADDR_INCREMENT + 17 * BABYKBER_ADDR_OFFSETT, demo_ct[4]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 210 * BABYKBER_ADDR_INCREMENT + 17 * BABYKBER_ADDR_OFFSETT, demo_ct[5]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 212 * BABYKBER_ADDR_INCREMENT + 18 * BABYKBER_ADDR_OFFSETT, demo_ct[6]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 216 * BABYKBER_ADDR_INCREMENT + 18 * BABYKBER_ADDR_OFFSETT, demo_ct[7]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 224 * BABYKBER_ADDR_INCREMENT + 18 * BABYKBER_ADDR_OFFSETT, demo_ct[8]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 225 * BABYKBER_ADDR_INCREMENT + 18 * BABYKBER_ADDR_OFFSETT, demo_ct[9]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 226 * BABYKBER_ADDR_INCREMENT + 18 * BABYKBER_ADDR_OFFSETT, demo_ct[10]);
    babykyber_write_reg(BABYKBER_BASE_ADDR + 228 * BABYKBER_ADDR_INCREMENT + 19 * BABYKBER_ADDR_OFFSETT, demo_ct[11]);
    
    // Trigger decryption
    babykyber_write_reg(BABYKYBER_DECRYPT_TRIGGER, 0);
    
    // Wait for completion (in real implementation, check status register)
    volatile uint32_t result = babykyber_read_reg(BABYKYBER_RESULT);
    (void)result; // Prevent unused warning
    
    while (1); // Halt
    
    return 0;
}