/*
 * libe6bmfqax5v - Cryptographic Implementation
 * 
 * AES-256-CBC, MD5, SHA-256 implementations
 * Decompiled from original ARM64 binary
 */

#include "../include/crypto.h"
#include "../include/e6bmfqax5v.h"
#include <string.h>
#include <stdlib.h>

// ============================================================================
// AES S-box and Constants
// ============================================================================

const uint8_t e6bm_aes_sbox[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

const uint8_t e6bm_aes_inv_sbox[256] = {
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
};

const uint8_t e6bm_aes_rcon[11] = {
    0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36
};

// ============================================================================
// AES Helper Functions
// ============================================================================

uint8_t e6bm_gf_mul(uint8_t a, uint8_t b) {
    uint8_t p = 0;
    for (int i = 0; i < 8; i++) {
        if (b & 1) {
            p ^= a;
        }
        uint8_t hi_bit = a & 0x80;
        a <<= 1;
        if (hi_bit) {
            a ^= 0x1b;
        }
        b >>= 1;
    }
    return p;
}

void e6bm_aes_sub_bytes(uint8_t* state) {
    for (int i = 0; i < 16; i++) {
        state[i] = e6bm_aes_sbox[state[i]];
    }
}

void e6bm_aes_inv_sub_bytes(uint8_t* state) {
    for (int i = 0; i < 16; i++) {
        state[i] = e6bm_aes_inv_sbox[state[i]];
    }
}

void e6bm_aes_shift_rows(uint8_t* state) {
    uint8_t temp;
    
    // Row 1: shift 1
    temp = state[1];
    state[1] = state[5];
    state[5] = state[9];
    state[9] = state[13];
    state[13] = temp;
    
    // Row 2: shift 2
    temp = state[2];
    state[2] = state[10];
    state[10] = temp;
    temp = state[6];
    state[6] = state[14];
    state[14] = temp;
    
    // Row 3: shift 3
    temp = state[15];
    state[15] = state[11];
    state[11] = state[7];
    state[7] = state[3];
    state[3] = temp;
}

void e6bm_aes_inv_shift_rows(uint8_t* state) {
    uint8_t temp;
    
    // Row 1: shift 3 (inverse of 1)
    temp = state[13];
    state[13] = state[9];
    state[9] = state[5];
    state[5] = state[1];
    state[1] = temp;
    
    // Row 2: shift 2 (same as forward)
    temp = state[2];
    state[2] = state[10];
    state[10] = temp;
    temp = state[6];
    state[6] = state[14];
    state[14] = temp;
    
    // Row 3: shift 1 (inverse of 3)
    temp = state[3];
    state[3] = state[7];
    state[7] = state[11];
    state[11] = state[15];
    state[15] = temp;
}

void e6bm_aes_mix_columns(uint8_t* state) {
    uint8_t tmp[16];
    
    for (int i = 0; i < 4; i++) {
        int base = i * 4;
        tmp[base + 0] = e6bm_gf_mul(state[base + 0], 2) ^
                        e6bm_gf_mul(state[base + 1], 3) ^
                        state[base + 2] ^
                        state[base + 3];
        
        tmp[base + 1] = state[base + 0] ^
                        e6bm_gf_mul(state[base + 1], 2) ^
                        e6bm_gf_mul(state[base + 2], 3) ^
                        state[base + 3];
        
        tmp[base + 2] = state[base + 0] ^
                        state[base + 1] ^
                        e6bm_gf_mul(state[base + 2], 2) ^
                        e6bm_gf_mul(state[base + 3], 3);
        
        tmp[base + 3] = e6bm_gf_mul(state[base + 0], 3) ^
                        state[base + 1] ^
                        state[base + 2] ^
                        e6bm_gf_mul(state[base + 3], 2);
    }
    
    memcpy(state, tmp, 16);
}

void e6bm_aes_inv_mix_columns(uint8_t* state) {
    uint8_t tmp[16];
    
    for (int i = 0; i < 4; i++) {
        int base = i * 4;
        tmp[base + 0] = e6bm_gf_mul(state[base + 0], 0x0e) ^
                        e6bm_gf_mul(state[base + 1], 0x0b) ^
                        e6bm_gf_mul(state[base + 2], 0x0d) ^
                        e6bm_gf_mul(state[base + 3], 0x09);
        
        tmp[base + 1] = e6bm_gf_mul(state[base + 0], 0x09) ^
                        e6bm_gf_mul(state[base + 1], 0x0e) ^
                        e6bm_gf_mul(state[base + 2], 0x0b) ^
                        e6bm_gf_mul(state[base + 3], 0x0d);
        
        tmp[base + 2] = e6bm_gf_mul(state[base + 0], 0x0d) ^
                        e6bm_gf_mul(state[base + 1], 0x09) ^
                        e6bm_gf_mul(state[base + 2], 0x0e) ^
                        e6bm_gf_mul(state[base + 3], 0x0b);
        
        tmp[base + 3] = e6bm_gf_mul(state[base + 0], 0x0b) ^
                        e6bm_gf_mul(state[base + 1], 0x0d) ^
                        e6bm_gf_mul(state[base + 2], 0x09) ^
                        e6bm_gf_mul(state[base + 3], 0x0e);
    }
    
    memcpy(state, tmp, 16);
}

void e6bm_aes_add_round_key(uint8_t* state, const uint32_t* round_key) {
    for (int i = 0; i < 4; i++) {
        uint32_t k = round_key[i];
        state[i * 4 + 0] ^= (k >> 24) & 0xFF;
        state[i * 4 + 1] ^= (k >> 16) & 0xFF;
        state[i * 4 + 2] ^= (k >> 8) & 0xFF;
        state[i * 4 + 3] ^= k & 0xFF;
    }
}

// ============================================================================
// AES Key Expansion
// ============================================================================

void e6bm_aes_key_expansion(e6bm_aes_context_t* ctx, const uint8_t* key) {
    int nk = ctx->key_bits / 32;
    int nr = ctx->num_rounds;
    uint32_t* w = ctx->round_keys;
    
    // First nk words
    for (int i = 0; i < nk; i++) {
        w[i] = ((uint32_t)key[4 * i] << 24) |
               ((uint32_t)key[4 * i + 1] << 16) |
               ((uint32_t)key[4 * i + 2] << 8) |
               ((uint32_t)key[4 * i + 3]);
    }
    
    // Expand remaining words
    for (int i = nk; i < 4 * (nr + 1); i++) {
        uint32_t temp = w[i - 1];
        
        if (i % nk == 0) {
            // RotWord
            temp = ((temp << 8) | (temp >> 24));
            
            // SubWord
            temp = ((uint32_t)e6bm_aes_sbox[(temp >> 24) & 0xFF] << 24) |
                   ((uint32_t)e6bm_aes_sbox[(temp >> 16) & 0xFF] << 16) |
                   ((uint32_t)e6bm_aes_sbox[(temp >> 8) & 0xFF] << 8) |
                   ((uint32_t)e6bm_aes_sbox[temp & 0xFF]);
            
            // XOR with Rcon
            temp ^= ((uint32_t)e6bm_aes_rcon[i / nk] << 24);
        } else if (nk > 6 && i % nk == 4) {
            // SubWord for 256-bit keys
            temp = ((uint32_t)e6bm_aes_sbox[(temp >> 24) & 0xFF] << 24) |
                   ((uint32_t)e6bm_aes_sbox[(temp >> 16) & 0xFF] << 16) |
                   ((uint32_t)e6bm_aes_sbox[(temp >> 8) & 0xFF] << 8) |
                   ((uint32_t)e6bm_aes_sbox[temp & 0xFF]);
        }
        
        w[i] = w[i - nk] ^ temp;
    }
}

// ============================================================================
// AES Core Functions
// ============================================================================

int e6bm_aes_init(e6bm_aes_context_t* ctx, const uint8_t* key,
                  uint32_t key_bits, const uint8_t* iv) {
    if (ctx == NULL || key == NULL) {
        return E6BM_ERROR_INVALID_PARAM;
    }
    
    if (key_bits != 128 && key_bits != 192 && key_bits != 256) {
        return E6BM_ERROR_INVALID_PARAM;
    }
    
    ctx->key_bits = key_bits;
    ctx->num_rounds = (key_bits == 128) ? 10 : (key_bits == 192) ? 12 : 14;
    
    memcpy(ctx->key, key, key_bits / 8);
    if (iv) {
        memcpy(ctx->iv, iv, 16);
    } else {
        memset(ctx->iv, 0, 16);
    }
    
    e6bm_aes_key_expansion(ctx, key);
    
    return E6BM_SUCCESS;
}

void e6bm_aes_encrypt_block(const e6bm_aes_context_t* ctx,
                            const uint8_t* input, uint8_t* output) {
    uint8_t state[16];
    memcpy(state, input, 16);
    
    // Initial round
    e6bm_aes_add_round_key(state, &ctx->round_keys[0]);
    
    // Main rounds
    for (uint32_t round = 1; round < ctx->num_rounds; round++) {
        e6bm_aes_sub_bytes(state);
        e6bm_aes_shift_rows(state);
        e6bm_aes_mix_columns(state);
        e6bm_aes_add_round_key(state, &ctx->round_keys[round * 4]);
    }
    
    // Final round (no mix columns)
    e6bm_aes_sub_bytes(state);
    e6bm_aes_shift_rows(state);
    e6bm_aes_add_round_key(state, &ctx->round_keys[ctx->num_rounds * 4]);
    
    memcpy(output, state, 16);
}

void e6bm_aes_decrypt_block(const e6bm_aes_context_t* ctx,
                            const uint8_t* input, uint8_t* output) {
    uint8_t state[16];
    memcpy(state, input, 16);
    
    // Initial round
    e6bm_aes_add_round_key(state, &ctx->round_keys[ctx->num_rounds * 4]);
    
    // Main rounds
    for (int round = ctx->num_rounds - 1; round > 0; round--) {
        e6bm_aes_inv_shift_rows(state);
        e6bm_aes_inv_sub_bytes(state);
        e6bm_aes_add_round_key(state, &ctx->round_keys[round * 4]);
        e6bm_aes_inv_mix_columns(state);
    }
    
    // Final round (no mix columns)
    e6bm_aes_inv_shift_rows(state);
    e6bm_aes_inv_sub_bytes(state);
    e6bm_aes_add_round_key(state, &ctx->round_keys[0]);
    
    memcpy(output, state, 16);
}

// Continuation in next file due to length...
