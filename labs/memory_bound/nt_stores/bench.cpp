#include "benchmark/benchmark.h"

#include <stdint.h>
#include <stdlib.h>

#include <chrono>
#include <cstring>
#include <iostream>
#include <map>
#include <vector>

#if defined(__x86_64__) || defined(_M_X64)
#include <emmintrin.h>
#include <immintrin.h>
#endif

inline void nt_copy_cache_line(const char* from, char* to) {
#if defined(__x86_64__) || defined(_M_X64)
#if defined(__AVX512F__)
    _mm512_stream_si512((__m512i*)to, _mm512_stream_load_si512((const __m512i*)from));
#elif defined(__AVX2__)
    _mm256_stream_si256((__m256i*)to + 0, _mm256_stream_load_si256((const __m256i*)from + 0));
    _mm256_stream_si256((__m256i*)to + 1, _mm256_stream_load_si256((const __m256i*)from + 1));
#else
    _mm_stream_si128((__m128i*)to + 0, _mm_stream_load_si128((const __m128i*)from + 0));
    _mm_stream_si128((__m128i*)to + 1, _mm_stream_load_si128((const __m128i*)from + 1));
    _mm_stream_si128((__m128i*)to + 2, _mm_stream_load_si128((const __m128i*)from + 2));
    _mm_stream_si128((__m128i*)to + 3, _mm_stream_load_si128((const __m128i*)from + 3));
#endif
#else
    asm (
        "ldnp q0, q1, [%[from]] \n\t"
        "ldnp q2, q3, [%[from], #32] \n\t"
        "stnp q0, q1, [%[to]] \n\t"
        "stnp q2, q3, [%[to], #32] \n\t"
        :
        : [from] "r" (from), [to] "r" (to)
        : "memory", "v0", "v1", "v2", "v3"
    );
#endif  // __x86_64__
}

#define SOLUTION
  
struct alignas(64) message {
    uint64_t id;
    char data[1024*8 - sizeof(uint64_t)];
};

void cpy_message(const message &from, message &to) {
    char *f = (char *)&from;
    char *t = (char *)&to;
#ifdef SOLUTION
    for (size_t i = 0; i < sizeof(message); i += 64) {
        nt_copy_cache_line(f + i, t + i);
    }
#else
    memcpy(t, f, sizeof(message));
#endif
}

std::map<uint64_t, uint64_t> lookup_map;
std::vector<message> message_buffer;
size_t message_buffer_ind = 0;

uint64_t process_message(const message &m) {
    message &cpy_to = message_buffer[message_buffer_ind];
    message_buffer_ind++;
    if (message_buffer_ind == message_buffer.size()) {
        message_buffer_ind = 0;
    }

    cpy_message(m, cpy_to);
   
    return lookup_map[m.id];
}

#ifndef N_IDS
#define N_IDS 100000
#endif

#ifndef BUF
#define BUF 100
#endif

static void bench1(benchmark::State &state) {
    srand(time(NULL));
    for (size_t i = 0; i < N_IDS; i++) {
        lookup_map[i] = i;
    }
    for (size_t i = 0; i < (1024*1024*BUF)/sizeof(message); i++) {
        message_buffer.push_back({});
    }
    message m;
    for (size_t i = 0; i < sizeof(m.data); i++) {
        m.data[i] = rand();
    }
    for (auto _ : state) {
        uint64_t total = 0;
        const uint64_t rounds = 100000;
        for (size_t i = 0; i < rounds; i++) {
            m.id = rand() % N_IDS;
            total += process_message(m);
        }
        benchmark::DoNotOptimize(total);
    }
}

BENCHMARK(bench1)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
