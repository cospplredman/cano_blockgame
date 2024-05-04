#ifndef MC_PKT_HEADER_GUARD
#define MC_PKT_HEADER_GUARD

#include<arpa/inet.h>
#include"ntwk.h"
#include"mc_dt.h"
#include"mc_nbt.h"
#include<stdint.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>

#define ADD_ARGS(...) (void *fb, int (*fn)(void *fb, int val) __VA_OPT__(,) __VA_ARGS__)
#define WRITE_ALL(name, args, ...) void write_##name ADD_ARGS args;
#include"mc_pkt.inc"

#undef ADD_ARGS
#undef WRITE_ALL

#endif
