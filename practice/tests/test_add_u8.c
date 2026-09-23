#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define MAX_LENGTH 128
#define GUARD 32
#define BUFFER_SIZE (MAX_LENGTH + 2 * GUARD + 16)

void add_u8_sse2(uint8_t *dst, const uint8_t *src, size_t n);

static uint32_t rng_state = UINT32_C(0x6d2b79f5);

static uint8_t random_byte(void)
{
    /* xorshift32: deterministic test data, not cryptography. */
    rng_state ^= rng_state << 13;
    rng_state ^= rng_state >> 17;
    rng_state ^= rng_state << 5;
    return (uint8_t)rng_state;
}

static void fill_random(uint8_t *buffer, size_t length)
{
    for (size_t i = 0; i < length; ++i)
        buffer[i] = random_byte();
}

static void reference_add(uint8_t *dst, const uint8_t *src, size_t n)
{
    for (size_t i = 0; i < n; ++i)
        dst[i] = (uint8_t)(dst[i] + src[i]);
}

static int test_distinct_buffers(void)
{
    uint8_t dst[BUFFER_SIZE];
    uint8_t src[BUFFER_SIZE];
    uint8_t expected[BUFFER_SIZE];
    uint8_t src_before[BUFFER_SIZE];

    /* Include every tail length and every alignment modulo 16. */
    for (size_t n = 0; n <= MAX_LENGTH; ++n) {
        for (size_t dst_offset = 0; dst_offset < 16; ++dst_offset) {
            for (size_t src_offset = 0; src_offset < 16; ++src_offset) {
                fill_random(dst, sizeof dst);
                fill_random(src, sizeof src);
                memcpy(expected, dst, sizeof dst);
                memcpy(src_before, src, sizeof src);

                uint8_t *dst_data = dst + GUARD + dst_offset;
                const uint8_t *src_data = src + GUARD + src_offset;
                reference_add(expected + GUARD + dst_offset,
                              src_data, n);
                add_u8_sse2(dst_data, src_data, n);

                if (memcmp(dst, expected, sizeof dst) != 0) {
                    fprintf(stderr,
                            "distinct-buffer mismatch: n=%zu dst_offset=%zu "
                            "src_offset=%zu\n",
                            n, dst_offset, src_offset);
                    return 1;
                }
                if (memcmp(src, src_before, sizeof src) != 0) {
                    fprintf(stderr, "source modified: n=%zu\n", n);
                    return 1;
                }
            }
        }
    }
    return 0;
}

static int test_exact_in_place(void)
{
    uint8_t data[BUFFER_SIZE];
    uint8_t expected[BUFFER_SIZE];

    for (size_t n = 0; n <= MAX_LENGTH; ++n) {
        for (size_t offset = 0; offset < 16; ++offset) {
            fill_random(data, sizeof data);
            memcpy(expected, data, sizeof data);

            reference_add(expected + GUARD + offset,
                          expected + GUARD + offset, n);
            add_u8_sse2(data + GUARD + offset,
                        data + GUARD + offset, n);

            if (memcmp(data, expected, sizeof data) != 0) {
                fprintf(stderr,
                        "in-place mismatch: n=%zu offset=%zu\n", n, offset);
                return 1;
            }
        }
    }
    return 0;
}

int main(void)
{
    if (test_distinct_buffers() != 0 || test_exact_in_place() != 0)
        return 1;

    puts("PASS: SSE2 byte addition, tails, unaligned pointers, and in-place use");
    return 0;
}
