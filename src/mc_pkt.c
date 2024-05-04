#include"mc_pkt.h"

static int $, $_;
static char pkt_buf[1<<22];

static int buf_putchar(fb, val)
	char **fb;
	int val;
{
	*((*fb)++) = val; //fails when overruns buffer
}

static size_t VarIntLen(int32_t val){
	char buf[8], *end = buf;
	write_VarInt(&end, buf_putchar, val);
	return end - buf;
}

#define OP(a, ...) write_##a ((void*)&end, buf_putchar, __VA_ARGS__)
#define $_(a, ...) OP (a, __VA_ARGS__); $
#define $(a, ...) OP (a, __VA_ARGS__); $_

#define ADD_ARGS(...) (void *fb, int (*fn)(void *fb, int val) __VA_OPT__(,) __VA_ARGS__)
#define WRITE_ALL(name, args, rst) void write_##name ADD_ARGS args {\
	char *end, *dummy, *start; \
	end = pkt_buf + 4; \
	$ rst; \
	dummy = start = pkt_buf + 4 - VarIntLen(end - (pkt_buf + 4)); \
	write_VarInt((void*)&dummy, buf_putchar, end - (pkt_buf + 4)); \
	while(start < end) \
		fn(fb, *(start++)); \
}

#include"mc_pkt.inc"
