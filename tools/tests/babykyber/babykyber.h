#ifndef BABYKYBER_H
#define BABYKYBER_H
                 
#include <stdint.h>

// BabyKyber API

// Data structures
typedef struct {
    uint32_t A_t[4][4];  // 4x4 matrix
} babykyber_matrix_t;

typedef struct {
    uint32_t s[8];  // Secret vector
    uint32_t e[8];  // Error vector
} babykyber_secret_key_t;

typedef struct {
    uint32_t pk[2][4][4];  // Public key (2 matrices of 4x4)
} babykyber_public_key_t;

typedef struct {
    uint32_t ct[2][4];  // Ciphertext (2 vectors of 4)
    uint32_t u[4];      // Additional ciphertext component
} babykyber_ciphertext_t;

// Function declarations

// Initialize the BabyKyber accelerator
void babykyber_init(void);

// Generate key pair
void babykyber_keygen(babykyber_public_key_t *pk, babykyber_secret_key_t *sk);

// Encrypt a message
void babykyber_encrypt(const babykyber_public_key_t *pk, uint32_t message, babykyber_ciphertext_t *ct);

// Decrypt a ciphertext
uint32_t babykyber_decrypt(const babykyber_secret_key_t *sk, const babykyber_ciphertext_t *ct);

// Utility functions
void babykyber_set_matrix(const uint32_t matrix[4][4], uint32_t addr);
void babykyber_set_vector(const uint32_t vector[], uint32_t count, uint32_t addr);
void babykyber_get_vector(uint32_t vector[], uint32_t count, uint32_t addr);

#endif // BABYKYBER_H
