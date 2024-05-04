#ifndef MC_NBT_HEADER_GUARD
#define MC_NBT_HEADER_GUARD

#include"mc_dt.h"

enum{
	NBT_TAG_END = 0,	
	NBT_TAG_BYTE,
	NBT_TAG_SHORT,
	NBT_TAG_INT,
	NBT_TAG_LONG,
	NBT_TAG_FLOAT,
	NBT_TAG_DOUBLE,
	NBT_TAG_BYTE_ARRAY,
	NBT_TAG_STRING,
	NBT_TAG_LIST,
	NBT_TAG_COMPOUND
};

struct nbttag {
	uint8_t type, type2;
	struct mc_string tag_name;
	union {
		struct mc_string String;
		int8_t Byte;
		int16_t Short;
		int32_t Int;
		int64_t Long;
		float Float;
		double Double;
		struct{
			size_t len;
			union{
				struct nbttag *tag;
				int8_t *byte;
			};
		};
	};
};


#define NBT_TAG_SHORT(name, n) ((struct nbttag){.tag_name = MC_STR(name), .type = NBT_TAG_SHORT, .Short = n })
#define NBT_TAG_BYTE(name, n) ((struct nbttag){.tag_name = MC_STR(name), .type = NBT_TAG_BYTE, .Byte = n })
#define NBT_TAG_INT(name, n) ((struct nbttag){.tag_name = MC_STR(name), .type = NBT_TAG_INT, .Int = n })
#define NBT_TAG_LONG(name, n) ((struct nbttag){.tag_name = MC_STR(name), .type = NBT_TAG_LONG, .Long = n })
#define NBT_TAG_FLOAT(name, n) ((struct nbttag){.tag_name = MC_STR(name), .type = NBT_TAG_FLOAT, .Float = n })
#define NBT_TAG_DOUBLE(name, n) ((struct nbttag){.tag_name = MC_STR(name), .type = NBT_TAG_DOUBLE, .Double = n })
#define NBT_TAG_STRING(name, n) ((struct nbttag){.tag_name = MC_STR(name), .type = NBT_TAG_STRING, .String = MC_STR(n) })
#define NBT_TAG_END() ((struct nbttag){.type = NBT_TAG_END})

#define NBT_TAG_BYTE_ARRAY(name, l, ...) ((struct nbttag){.tag_name = MC_STR(name), .type = NBT_TAG_BYTE_ARRAY, .len = l, .byte = (char[]){__VA_ARGS__}})
#define NBT_TAG_LIST(name, t, l, ...) ((struct nbttag){.tag_name = MC_STR(name), .type = NBT_TAG_LIST, .type2 = t, .len = l, .tag = (struct nbttag[]){__VA_ARGS__}})
#define NBT_TAG_COMPOUND(name, l, ...) ((struct nbttag){.tag_name = MC_STR(name), .type = NBT_TAG_COMPOUND, .len = l, .tag = (struct nbttag[]){__VA_ARGS__}})


struct mc_string read_nbt_string(void *fb, int (*fn)(void *fn));
void write_nbt_string(void *fb, int (*fn)(void *fn, int val), struct mc_string str);
void write_nbt_tag(void *fb, int (*fn)(void *fn, int val), struct nbttag str);
struct nbttag read_nbt_tag(void *fb, int (*fn)(void *fn));
static struct nbttag read_nbt_unamed_tag(void *fb, int (*fn)(void *fn), uint8_t type);

#endif
