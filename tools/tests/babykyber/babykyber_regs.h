#ifndef BABYKYBER_REGS_H
#define BABYKYBER_REGS_H

#include <stdint.h>

// ================= Base Address =================
#define BABYKYBER_BASE_ADDR   0x40007000UL

// ================= A_t Matrix (4x4) =================
#define BABYKYBER_A_T_BASE    (BABYKYBER_BASE_ADDR + 0x0000)

// ================= s Vector (2x4 = 8 values) =================
#define BABYKYBER_S_BASE      (BABYKYBER_BASE_ADDR + 0x0064)

// ================= e Vector (2x4 = 8 values) =================
#define BABYKYBER_E_BASE      (BABYKYBER_BASE_ADDR + 0x0096)

// ================= Message =================
#define BABYKYBER_MESSAGE     (BABYKYBER_BASE_ADDR + 0x0128)

// ================= Public Key (2x4x3 = 24 values) =================
#define BABYKYBER_PK_BASE     (BABYKYBER_BASE_ADDR + 0x0132)

// ================= Result (optional) =================
#define BABYKYBER_RESULT      (BABYKYBER_BASE_ADDR + 0x0144)

// ================= r Vector (2x4 = 8 values) =================
#define BABYKYBER_R_BASE      (BABYKYBER_BASE_ADDR + 0x0228)

// ================= e1 Vector (2x4 = 8 values) =================
#define BABYKYBER_E1_BASE     (BABYKYBER_BASE_ADDR + 0x0260)

// ================= e2 Vector (4 values) =================
#define BABYKYBER_E2_BASE     (BABYKYBER_BASE_ADDR + 0x0292)

// ================= Ciphertext (12 values) =================
#define BABYKYBER_CT_BASE     (BABYKYBER_BASE_ADDR + 0x0308)

// ================= Control Registers =================
#define BABYKYBER_KEYGEN_TRIGGER   (BABYKYBER_BASE_ADDR + 0x0356)
#define BABYKYBER_ENCRYPT_TRIGGER  (BABYKYBER_BASE_ADDR + 0x0360)
#define BABYKYBER_DECRYPT_TRIGGER  (BABYKYBER_BASE_ADDR + 0x0364)

// ================= HAL Access Macros =================
#define REG32(addr)           (*(volatile uint32_t *)(addr))

#endif // BABYKYBER_REGS_H
