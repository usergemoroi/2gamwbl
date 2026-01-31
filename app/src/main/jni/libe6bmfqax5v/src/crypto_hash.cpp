/*
 * libe6bmfqax5v - Hash Functions (MD5, SHA-256)
 */

#include "../include/crypto.h"
#include <string.h>

// ============================================================================
// MD5 Implementation
// ============================================================================

static const uint32_t md5_t[64] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

void e6bm_md5_init(e6bm_md5_context_t* ctx) {
    ctx->state[0] = 0x67452301;
    ctx->state[1] = 0xefcdab89;
    ctx->state[2] = 0x98badcfe;
    ctx->state[3] = 0x10325476;
    ctx->count = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

void e6bm_md5_transform(uint32_t state[4], const uint8_t block[64]) {
    uint32_t a = state[0], b = state[1], c = state[2], d = state[3];
    uint32_t x[16];
    
    for (int i = 0; i < 16; i++) {
        x[i] = ((uint32_t)block[i*4]) | ((uint32_t)block[i*4+1] << 8) |
               ((uint32_t)block[i*4+2] << 16) | ((uint32_t)block[i*4+3] << 24);
    }
    
    // Round 1
    #define FF(a, b, c, d, x, s, ac) { \
        (a) += E6BM_MD5_F((b), (c), (d)) + (x) + (ac); \
        (a) = E6BM_MD5_ROTATE_LEFT((a), (s)); \
        (a) += (b); \
    }
    
    FF(a, b, c, d, x[ 0],  7, md5_t[ 0]);
    FF(d, a, b, c, x[ 1], 12, md5_t[ 1]);
    FF(c, d, a, b, x[ 2], 17, md5_t[ 2]);
    FF(b, c, d, a, x[ 3], 22, md5_t[ 3]);
    FF(a, b, c, d, x[ 4],  7, md5_t[ 4]);
    FF(d, a, b, c, x[ 5], 12, md5_t[ 5]);
    FF(c, d, a, b, x[ 6], 17, md5_t[ 6]);
    FF(b, c, d, a, x[ 7], 22, md5_t[ 7]);
    FF(a, b, c, d, x[ 8],  7, md5_t[ 8]);
    FF(d, a, b, c, x[ 9], 12, md5_t[ 9]);
    FF(c, d, a, b, x[10], 17, md5_t[10]);
    FF(b, c, d, a, x[11], 22, md5_t[11]);
    FF(a, b, c, d, x[12],  7, md5_t[12]);
    FF(d, a, b, c, x[13], 12, md5_t[13]);
    FF(c, d, a, b, x[14], 17, md5_t[14]);
    FF(b, c, d, a, x[15], 22, md5_t[15]);
    
    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
}

void e6bm_md5_update(e6bm_md5_context_t* ctx, const uint8_t* data, size_t len) {
    size_t index = (ctx->count / 8) % 64;
    ctx->count += len * 8;
    
    size_t part_len = 64 - index;
    size_t i = 0;
    
    if (len >= part_len) {
        memcpy(&ctx->buffer[index], data, part_len);
        e6bm_md5_transform(ctx->state, ctx->buffer);
        
        for (i = part_len; i + 63 < len; i += 64) {
            e6bm_md5_transform(ctx->state, &data[i]);
        }
        index = 0;
    }
    
    memcpy(&ctx->buffer[index], &data[i], len - i);
}

void e6bm_md5_final(e6bm_md5_context_t* ctx, uint8_t* hash) {
    uint8_t bits[8];
    for (int i = 0; i < 8; i++) {
        bits[i] = (uint8_t)((ctx->count >> (i * 8)) & 0xFF);
    }
    
    size_t index = (ctx->count / 8) % 64;
    size_t pad_len = (index < 56) ? (56 - index) : (120 - index);
    
    uint8_t padding[64];
    padding[0] = 0x80;
    memset(padding + 1, 0, sizeof(padding) - 1);
    
    e6bm_md5_update(ctx, padding, pad_len);
    e6bm_md5_update(ctx, bits, 8);
    
    for (int i = 0; i < 4; i++) {
        hash[i*4]     = (ctx->state[i]) & 0xFF;
        hash[i*4 + 1] = (ctx->state[i] >> 8) & 0xFF;
        hash[i*4 + 2] = (ctx->state[i] >> 16) & 0xFF;
        hash[i*4 + 3] = (ctx->state[i] >> 24) & 0xFF;
    }
}

int e6bm_md5_compute(const uint8_t* input, size_t input_len, uint8_t* output) {
    e6bm_md5_context_t ctx;
    e6bm_md5_init(&ctx);
    e6bm_md5_update(&ctx, input, input_len);
    e6bm_md5_final(&ctx, output);
    return E6BM_SUCCESS;
}

int e6bm_md5_hash(const uint8_t* input, size_t input_len, uint8_t* output) {
    return e6bm_md5_compute(input, input_len, output);
}

// ============================================================================
// SHA-256 Implementation
// ============================================================================

const uint32_t e6bm_sha256_k[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

void e6bm_sha256_init(e6bm_sha256_context_t* ctx) {
    ctx->state[0] = 0x6a09e667;
    ctx->state[1] = 0xbb67ae85;
    ctx->state[2] = 0x3c6ef372;
    ctx->state[3] = 0xa54ff53a;
    ctx->state[4] = 0x510e527f;
    ctx->state[5] = 0x9b05688c;
    ctx->state[6] = 0x1f83d9ab;
    ctx->state[7] = 0x5be0cd19;
    ctx->count = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

void e6bm_sha256_transform(uint32_t state[8], const uint8_t block[64]) {
    uint32_t w[64];
    uint32_t a, b, c, d, e, f, g, h;
    
    // Prepare message schedule
    for (int i = 0; i < 16; i++) {
        w[i] = ((uint32_t)block[i*4] << 24) | ((uint32_t)block[i*4+1] << 16) |
               ((uint32_t)block[i*4+2] << 8) | ((uint32_t)block[i*4+3]);
    }
    
    for (int i = 16; i < 64; i++) {
        w[i] = E6BM_SHA256_SIG1(w[i-2]) + w[i-7] + E6BM_SHA256_SIG0(w[i-15]) + w[i-16];
    }
    
    // Initialize working variables
    a = state[0];
    b = state[1];
    c = state[2];
    d = state[3];
    e = state[4];
    f = state[5];
    g = state[6];
    h = state[7];
    
    // Main loop
    for (int i = 0; i < 64; i++) {
        uint32_t t1 = h + E6BM_SHA256_EP1(e) + E6BM_SHA256_CH(e, f, g) + e6bm_sha256_k[i] + w[i];
        uint32_t t2 = E6BM_SHA256_EP0(a) + E6BM_SHA256_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }
    
    // Add working variables back to state
    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;
    state[5] += f;
    state[6] += g;
    state[7] += h;
}

void e6bm_sha256_update(e6bm_sha256_context_t* ctx, const uint8_t* data, size_t len) {
    size_t index = (ctx->count / 8) % 64;
    ctx->count += len * 8;
    
    size_t part_len = 64 - index;
    size_t i = 0;
    
    if (len >= part_len) {
        memcpy(&ctx->buffer[index], data, part_len);
        e6bm_sha256_transform(ctx->state, ctx->buffer);
        
        for (i = part_len; i + 63 < len; i += 64) {
            e6bm_sha256_transform(ctx->state, &data[i]);
        }
        index = 0;
    }
    
    memcpy(&ctx->buffer[index], &data[i], len - i);
}

void e6bm_sha256_final(e6bm_sha256_context_t* ctx, uint8_t* hash) {
    uint8_t bits[8];
    uint64_t count = ctx->count;
    for (int i = 7; i >= 0; i--) {
        bits[i] = (uint8_t)(count & 0xFF);
        count >>= 8;
    }
    
    size_t index = (ctx->count / 8) % 64;
    size_t pad_len = (index < 56) ? (56 - index) : (120 - index);
    
    uint8_t padding[64];
    padding[0] = 0x80;
    memset(padding + 1, 0, sizeof(padding) - 1);
    
    e6bm_sha256_update(ctx, padding, pad_len);
    e6bm_sha256_update(ctx, bits, 8);
    
    for (int i = 0; i < 8; i++) {
        hash[i*4]     = (ctx->state[i] >> 24) & 0xFF;
        hash[i*4 + 1] = (ctx->state[i] >> 16) & 0xFF;
        hash[i*4 + 2] = (ctx->state[i] >> 8) & 0xFF;
        hash[i*4 + 3] = (ctx->state[i]) & 0xFF;
    }
}

int e6bm_sha256_compute(const uint8_t* input, size_t input_len, uint8_t* output) {
    e6bm_sha256_context_t ctx;
    e6bm_sha256_init(&ctx);
    e6bm_sha256_update(&ctx, input, input_len);
    e6bm_sha256_final(&ctx, output);
    return E6BM_SUCCESS;
}

int e6bm_sha256_hash(const uint8_t* input, size_t input_len, uint8_t* output) {
    return e6bm_sha256_compute(input, input_len, output);
}
