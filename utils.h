#pragma once

#include <cstddef>
#include <cstdint>
#include <stddef.h>
#include <stdint.h>
#include <vector>

#define container_of(ptr, type, member)                                        \
	({                                                                         \
		const typeof(((type *)0)->member) *__mptr = (ptr);                     \
		(type *)((char *)__mptr - offsetof(type, member));                     \
	})

enum {
	TAG_NIL = 0, // nil
	TAG_ERR = 1, // error code + msg
	TAG_STR = 2, // string
	TAG_INT = 3, // int64
	TAG_DBL = 4, // double
	TAG_ARR = 5, // array
};

struct Buffer {
	uint8_t *buffer_begin;
	uint8_t *buffer_end;
	uint8_t *data_begin;
	uint8_t *data_end;
};

// FNV hash
inline uint64_t str_hash(const uint8_t *data, size_t len) {
	uint32_t h = 0x811C9DC5;
	for (size_t i = 0; i < len; i++) {
		h = (h + data[i]) * 0x01000193;
	}
	return h;
}

void die(const char *msg);

void msg(const char *text);

int32_t read_full(int fd, char *buf, size_t n);

int32_t write_all(int fd, const char *buf, size_t n);

void buf_append(Buffer *buf, const uint8_t *data, size_t len);

void buf_consume(Buffer *buf, size_t n);

size_t buf_size(Buffer *buf);

Buffer *new_buffer(size_t n);
