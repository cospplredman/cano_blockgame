#ifndef MC_DT_HEADER_GUARD
#define MC_DT_HEADER_GUARD

#include<stdint.h>

#define STR(...) #__VA_ARGS__
#define MC_STR(s) ((struct mc_string){.str = (s), .len = (s) == NULL ? 0 : sizeof(s)-1})

struct mc_bitset{
	size_t len;
	int64_t *data;
};

struct mc_uuid{
	uint64_t low, high;
};

struct mc_string{
	char *str;
	size_t len;	
};

struct mc_utf16{
	uint16_t *str;
	size_t len;
	size_t code_points;
};

void write_file(void *fb, int (*fn)(void *fb, int val), char *path);

uint8_t read_uint8_t(void *fb, int (*fn)(void *fb));
int8_t read_int8_t(void *fb, int (*fn)(void *fb));
uint16_t read_uint16_t(void *fb, int (*fn)(void *fb));
int16_t read_int16_t(void *fb, int (*fn)(void *fb));
uint32_t read_uint32_t(void *fb, int (*fn)(void *fb));
int32_t read_int32_t(void *fb, int (*fn)(void *fb));
uint64_t read_uint64_t(void *fb, int (*fn)(void *fb));
int64_t read_int64_t(void *fb, int (*fn)(void *fb));
float read_float(void *fb, int (*fn)(void *fb));
double read_double(void *fb, int (*fn)(void *fb));

void write_uint8_t(void *fb, int (*fn)(void *fb, int val), uint8_t val);
void write_int8_t(void *fb, int (*fn)(void *fb, int val), int8_t val);
void write_uint16_t(void *fb, int (*fn)(void *fb, int val), uint16_t val);
void write_int16_t(void *fb, int (*fn)(void *fb, int val), int16_t val);
void write_uint32_t(void *fb, int (*fn)(void *fb, int val), uint32_t val);
void write_int32_t(void *fb, int (*fn)(void *fb, int val), int32_t val);
void write_uint64_t(void *fb, int (*fn)(void *fb, int val), uint64_t val);
void write_int64_t(void *fb, int (*fn)(void *fb, int val), int64_t val);
void write_float(void *fb, int (*fn)(void *fb, int val), float val);
void write_double(void *fb, int (*fn)(void *fb, int val), double val);

int32_t read_VarInt(void *fb, int (*fn)(void *fn));
int64_t read_VarLong(void *fb, int (*fn)(void *fn));

void write_VarInt(void *fb, int (*fn)(void *fb, int val) , int32_t value);
void write_VarLong(void *fb, int (*fn)(void *fb, int val), int64_t value);

[[nodiscard]] struct mc_string read_mc_string(void *fb, int (*fn)(void *fn));
void free_mc_string(struct mc_string str);
void write_mc_string(void *fb, int (*fn)(void *fb, int val), struct mc_string str);

[[nodiscard]] struct mc_utf16 mc_astr_to_utf16(struct mc_string str);
[[nodiscard]] struct mc_utf16 read_mc_utf16(void *fb, int (*fn)(void *fn));
void free_mc_utf16(struct mc_utf16 str);
void write_mc_utf16(void *fb, int (*fn)(void *fb, int val), struct mc_utf16 str);

struct mc_uuid read_mc_uuid(void *fb, int (*fn)(void *fn));
void write_mc_uuid(void *fb, int (*fn)(void *fb, int val), struct mc_uuid uuid);

[[nodiscard]] struct mc_bitset read_mc_bitset(void *fb, int (*fn)(void *fn));
void free_mc_bitset(struct mc_bitset bitset);
void write_mc_bitset(void *fb, int (*fn)(void *fb, int val), struct mc_bitset bitset);


void write_mc_subchunk(void *fb, int (*fn)(void *fb, int val), void *cb, int32_t (*ca)(void *cb, int32_t x, int32_t y, int32_t z), int32_t cx, int32_t cy, int32_t cz);
void write_mc_chunk(void *fb, int (*fn)(void *fb, int val), void *cb, int32_t (*ca)(void *cb, int32_t x, int32_t y, int32_t z), int32_t cx, int32_t cz);

#endif
