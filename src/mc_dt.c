#include<math.h>
#include<stdlib.h>
#include<stdio.h>
#include"mc_dt.h"

void write_file(void *fb, int (*fn)(void *fb, int val), char *path){
	FILE *file = fopen(path, "r+");
	int cur_char;
	while((cur_char = fgetc(file)) != EOF)
		fn(fb, cur_char);

	fclose(file);
}

#define READ_INT(T, S)                               \
T read_##T(void *fb, int (*fn)(void *fn)){           \
	T ret = 0;                                   \
	for(size_t i = 0; i < (S/8); i++)  \
		ret += (T)(uint8_t)fn(fb) << (((S/8) - i - 1) * 8);             \
	return ret;                                  \
}

READ_INT(int8_t, 8)
READ_INT(uint8_t, 8)
READ_INT(int16_t, 16)
READ_INT(uint16_t, 16)
READ_INT(int32_t, 32)
READ_INT(uint32_t, 32)
READ_INT(int64_t, 64)
READ_INT(uint64_t, 64)

//mostly works
float read_float(void *fb, int (*fn)(void *fn)){
        uint32_t ret = read_uint32_t(fb, fn);

        int exponent = (ret >> 23) & 0xff;
	exponent -= 127;

        float mantissa = ((float)(0x800000 + (ret & 0x7fffff))) * 0x1p-23;
	float sign = ret & (1u << 31u) ? -1.0 : 1.0;

        return ldexpf(mantissa, exponent) * sign;
}

/*
double read_double(void *fb, int (*fn)(void *fn)){
	uint64_t ret = read_uint64_t(fb, fn);
	return *((double*)&ret);
}
*/

//mostly works
double read_double(void *fb, int (*fn)(void *fn)){
        uint64_t ret = read_uint64_t(fb, fn);

        int exponent = (ret >> 52) & 0x7ff;
        exponent -= 1023;

        double mantissa = ((double)((1ull << 52) + (ret & ((1ull << 52)-1)))) * 0x1p-52;
        double sign = ret & (1ull << 63u) ? -1.0 : 1.0;

        return ldexp(mantissa, exponent) * sign;
}


#define WRITE_INT(T, S)                             \
void write_##T(void *fb, int (*fn)(void *fn, int val), T val){                  \
	for(size_t i = 0; i < (S/8); i++) \
		fn(fb, (val >> (((S/8) - i - 1) * 8)) & 0xff);  \
}

WRITE_INT(int8_t, 8)
WRITE_INT(uint8_t, 8)
WRITE_INT(int16_t, 16)
WRITE_INT(uint16_t, 16)
WRITE_INT(int32_t, 32)
WRITE_INT(uint32_t, 32)
WRITE_INT(int64_t, 64)
WRITE_INT(uint64_t, 64)

//mostly works
static uint32_t write_float_(float val){
        if(isnanf(val))
            return 0x7fc00000;

        if(isinf(val))
            return val < 0 ? -0xff800000 : 0x7f800000; 

        if(val == 0)
            return 0;

        int exponent;
        int32_t mantissa = frexpf(val, &exponent) * 0x1p24;

        if(exponent < -126)
            return 0;

        if(exponent > 127)
            return val < 0 ? -0xff800000 : 0x7f800000;

        uint32_t sign = 0;

        exponent -= 1;
        if(mantissa < 0){
            sign = 1 << 31;
            mantissa *= -1;
        }
        
        return sign | (((exponent + 127) & 0xff) << 23) | (mantissa & 0x7fffff);
}

void write_float(void *fb, int (*fn)(void *fn, int val), float val){
	write_uint32_t(fb, fn, write_float_(val));
}
/*
void write_double(void *fb, int (*fn)(void *fn, int val), double val){
	write_uint64_t(fb, fn, *((uint64_t*)&val));
}*/

//mostly works
static uint64_t write_double_(double val){
        if(isnan(val))
            return 0x7ff8000000000000ull;

        if(isinf(val))
            return val < 0 ? 0xfff0000000000000ull : 0x7ff0000000000000ull;

        if(val == 0)
            return 0;

        int exponent;
        int64_t mantissa = frexp(val, &exponent) * 0x1p53;

        if(exponent < -1023)
            return 0;

        if(exponent > 1024)
            return val < 0 ? 0xfff0000000000000ull : 0x7ff0000000000000ull;

        uint64_t sign = 0;

        exponent -= 1;
        if(mantissa < 0){
            sign = 1ull << 63;
            mantissa *= -1;
        }

       return sign | (((exponent + 1023ull) & 0x7ffull) << 52) | (mantissa & ((1ull << 52)-1));
}

void write_double(void *fb, int (*fn)(void *fb, int val), double val){
	write_uint64_t(fb, fn, write_double_(val));
}

#define SEGMENT_BITS 0x7F
#define CONTINUE_BIT 0x80

int32_t read_VarInt(void *fb, int (*fn)(void *fn)) {
	int32_t value = 0;
	size_t position = 0;
	uint8_t currentByte;

	while (1) {
		currentByte = fn(fb);
		value |= (currentByte & SEGMENT_BITS) << position;
		if ((currentByte & CONTINUE_BIT) == 0)
			break;

		position += 7;
		if (position >= 32)
			break;
	}

	return value;
}

void write_VarInt(void *fb, int (*fn)(void *fb, int val) , int32_t val) {
	uint32_t value = ((uint32_t)val) & 0xffffffff;
	while (1) {
		if ((value & ~SEGMENT_BITS) == 0) {
			fn(fb, value);
			return;
		}

		fn(fb, (value & SEGMENT_BITS) | CONTINUE_BIT);
		value >>= 7;
	}
}

int64_t read_VarLong(void *fb, int (*fn)(void *fn)){
	int64_t value = 0;
	size_t position = 0;
	uint8_t currentByte;

	while (1) {
		currentByte = fn(fb);
		value |= (long) (currentByte & SEGMENT_BITS) << position;

		if ((currentByte & CONTINUE_BIT) == 0) break;

		position += 7;

		if (position >= 64)
			break;
	}

	return value;
}

void write_VarLong(void *fb, int (*fn)(void *fb, int val), int64_t val) {
	uint64_t value = ((uint64_t)val) & 0xffffffffffffffffull;
	while (1) {
		if ((value & ~((int64_t) SEGMENT_BITS)) == 0) {
			fn(fb, value);
			return;
		}

		fn(fb, (value & SEGMENT_BITS) | CONTINUE_BIT);
		value >>= 7;
	}
}

struct mc_string read_mc_string(void *fb, int (*fn)(void *fn)){
	size_t len = read_VarInt(fb, fn);
	char *str = malloc(len+1);
	str[len] = 0;
	for(size_t i = 0; i < len; i++)
		str[i] = fn(fb);

	return (struct mc_string){str, len}; //TODO free
}

void write_mc_string(void *fb, int (*fn)(void *fb, int val), struct mc_string str){
	write_VarInt(fb, fn, str.len);
	for(size_t i = 0; i < str.len; i++)
		fn(fb, str.str[i]);
}

void free_mc_string(struct mc_string str){
	free(str.str);
}


struct mc_utf16 read_mc_utf16(void *fb, int (*fn)(void *fn)){
	size_t len = read_int16_t(fb, fn);
	uint16_t *str = malloc(sizeof(uint16_t) * (len+1));
	str[len] = 0;
	for(size_t i = 0; i < len; i++)
		str[i] = read_uint16_t(fb, fn); //TODO deal with utf16

	return (struct mc_utf16){.str = str, .len = len, .code_points = len}; //TODO free
}

void write_mc_utf16(void *fb, int (*fn)(void *fb, int val), struct mc_utf16 str){
	write_VarInt(fb, fn, str.len);
	for(size_t i = 0; i < str.len; i++)
		write_uint16_t(fb, fn, str.str[i]); //TODO deal with utf16
}

void free_mc_utf16(struct mc_utf16 str){
	free(str.str);
}






struct mc_uuid read_mc_uuid(void *fb, int (*fn)(void *fn)){
	return (struct mc_uuid){
		read_uint64_t(fb, fn),
		read_uint64_t(fb, fn)
	};
}

void write_mc_uuid(void *fb, int (*fn)(void *fb, int val), struct mc_uuid uuid){
	write_uint64_t(fb, fn, uuid.low);
	write_uint64_t(fb, fn, uuid.high);
}

struct mc_bitset read_mc_bitset(void *fb, int (*fn)(void *fn)){
	struct mc_bitset ret;

	ret.len = read_VarInt(fb, fn);
	ret.data = malloc(ret.len * sizeof(int64_t));
	for(size_t i = 0; i < ret.len; i++)
		ret.data[i] = read_int64_t(fb, fn);

	return ret; 
}

void write_mc_bitset(void *fb, int (*fn)(void *fb, int val), struct mc_bitset bitset){
	write_VarInt(fb, fn, bitset.len);
	for(size_t i = 0; i < bitset.len; i++)
		write_int64_t(fb, fn, bitset.data[i]);
}

void free_mc_bitset(struct mc_bitset bitset){
	free(bitset.data);
}

void write_mc_subchunk(void *fb, int (*fn)(void *fb, int val), void *cb, int32_t (*ca)(void *cb, int32_t x, int32_t y, int32_t z), int32_t cx, int32_t cy, int32_t cz){
	size_t block_count = 0;

	int64_t x = ((int64_t)cx) * 16;
	int64_t y = ((int64_t)cy) * 16;
	int64_t z = ((int64_t)cz) * 16;
	for(size_t i = 0; i < 4096; i++)
		if(ca(cb, x + ((i >> 0) & 0xf), y + ((i >> 8) & 0xf), z + ((i >> 4) & 0xf)) != 0)
			block_count++;

	write_int16_t(fb, fn, block_count);

	//container
	size_t bits = 15; //NOTE could change
	write_uint8_t(fb, fn, bits);
	//pallete TODO using Direct pallet

	//
	
	size_t data_len = (64 / bits);
	data_len = ((4096 / data_len) + ((4096 % data_len) > 0));
	write_VarInt(fb, fn, data_len);

	uint64_t buf = 0;
	size_t offset = 0;
	int32_t mask = (1 << (bits + 1)) - 1;
	for(size_t i = 0; i < 4096; i++){
		int32_t block = ca(cb, x + ((i >> 0) & 0xf), y + ((i >> 8) & 0xf), z + ((i >> 4) & 0xf));
		buf |= ((uint64_t)(block & mask)) << offset;
		offset += bits;

		if(64 < bits + offset){
			write_uint64_t(fb, fn, buf);	
			offset = 0;
			buf = 0;
		}
	}


	//TODO using single valued pallet
	write_uint8_t(fb, fn, 0);
	write_VarInt(fb, fn, 0);
	write_VarInt(fb, fn, 0);

};

static int inc(void *fb, int val){
	((size_t *)fb)[0]++;
}

void write_mc_chunk(void *fb, int (*fn)(void *fb, int val), void *cb, int32_t (*ca)(void *cb, int32_t x, int32_t y, int32_t z), int32_t cx, int32_t cz){
	size_t len = 0;
	for(size_t i = 0; i < 16; i++)
		write_mc_subchunk(&len, inc, cb, ca, cx, i, cz);

	write_VarInt(fb, fn, len);
	for(size_t i = 0; i < 16; i++)
		write_mc_subchunk(fb, fn, cb, ca, cx, i, cz);

}















