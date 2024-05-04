#include<stdlib.h>
#include"mc_nbt.h"

struct mc_string read_nbt_string(void *fb, int (*fn)(void *fn)){
	size_t len = read_int16_t(fb, fn);
	char *str = malloc(len+1);
	str[len] = 0;
	for(size_t i = 0; i < len; i++)
		str[i] = fn(fb);

	return (struct mc_string){str, len}; //TODO free
}

void write_nbt_string(void *fb, int (*fn)(void *fn, int val), struct mc_string str){
	write_int16_t(fb, fn, str.len);
	for(size_t i = 0; i < str.len; i++)
		fn(fb, str.str[i]);
}

struct nbttag read_nbt_tag(void *fb, int (*fn)(void *fn));
static struct nbttag read_nbt_unamed_tag(void *fb, int (*fn)(void *fn), uint8_t type){
	struct nbttag ret;
	char *tmp;

	ret.type = type;
	ret.tag_name = (struct mc_string){.str = NULL, .len = 0};
	switch(ret.type){
	case NBT_TAG_END:
		return ret;
	case NBT_TAG_BYTE:
		ret.Byte = read_int8_t(fb, fn);
		break;
	case NBT_TAG_SHORT:
		ret.Short = read_int16_t(fb, fn);
		break;
	case NBT_TAG_INT:
		ret.Int = read_int32_t(fb, fn);
		break;
	case NBT_TAG_LONG:
		ret.Long = read_int64_t(fb, fn);
		break;
	case NBT_TAG_FLOAT:
		ret.Float = read_float(fb, fn);
		break;
	case NBT_TAG_DOUBLE:
		ret.Double = read_double(fb, fn);
		break;
	case NBT_TAG_BYTE_ARRAY: //TODO
		ret.len = read_int16_t(fn, fb);
		ret.byte = malloc(ret.len);
		for(size_t i = 0; i < ret.len; i++)
			ret.byte[i] = read_int8_t(fn, fb);
		break;
	case NBT_TAG_STRING: {
		ret.String = read_nbt_string(fb, fn);
		} break;
	case NBT_TAG_LIST: {
		ret.type2 = read_uint8_t(fb, fn);
		ret.len = read_int32_t(fb, fn);
		ret.tag = malloc(sizeof(struct nbttag) * ret.len);
		for(size_t i = 0; i < ret.len; i++)
			ret.tag[i] = read_nbt_unamed_tag(fb, fn, ret.type2);
		} break;
	case NBT_TAG_COMPOUND: {
		//TODO arena allocation for speed
		struct nbttag_ll{
			struct nbttag cur;
			struct nbttag_ll *next;
		};

		struct nbttag_ll start = {.next = NULL};
		struct nbttag_ll *cur = &start;
		size_t len = 0;

		while(1){
			cur->next = malloc(sizeof(struct nbttag_ll));
			cur = cur->next;
			cur->cur = read_nbt_tag(fb, fn);
			len++;
			if(cur->cur.type == NBT_TAG_END)
				break;
		};

		ret.len = len-1;
		ret.tag = malloc((len-1) * sizeof(struct nbttag));

		cur = start.next;
		for(size_t i = 0; i < len-1; i++){
			ret.tag[i] = cur->cur;
			struct nbttag_ll *next = cur->next;
			free(cur);
			cur = next;
		}

		free(cur);
		} break;
	default:
		ret.len = 0;
	}

	return ret;
}


struct nbttag read_nbt_tag(void *fb, int (*fn)(void *fb)){
	struct nbttag ret;
	struct mc_string name = (struct mc_string){.str = "", .len = 0};
	char *tmp;

	uint8_t type = read_uint8_t(fb, fn);
	if(type != NBT_TAG_END)
		name = read_nbt_string(fb, fn);
	ret = read_nbt_unamed_tag(fb, fn, type);
	ret.tag_name = name;

	return ret;
}

void write_nbt_tag(void *fb, int (*fn)(void *fb, int val), struct nbttag tag){
	if(tag.tag_name.str != NULL){
		write_uint8_t(fb, fn, tag.type);
		write_nbt_string(fb, fn, tag.tag_name);
	}

	switch(tag.type){
	case NBT_TAG_END: {
		write_uint8_t(fb, fn, 0);
		}break;
	case NBT_TAG_BYTE: {
		write_int8_t(fb, fn, tag.Byte);
		}break;
	case NBT_TAG_SHORT: {
		write_int16_t(fb, fn, tag.Short);
		}break;
	case NBT_TAG_INT: {
		write_int32_t(fb, fn, tag.Int);
		}break;
	case NBT_TAG_LONG: {
		write_int64_t(fb, fn, tag.Long);
		}break;
	case NBT_TAG_FLOAT: {
		write_float(fb, fn, tag.Float);
		}break;
	case NBT_TAG_DOUBLE: {
		write_double(fb, fn, tag.Double);
		}break;
	case NBT_TAG_BYTE_ARRAY: {
		write_int16_t(fn, fb, tag.len);
		for(size_t i = 0; i < tag.len; i++)
			write_int8_t(fb, fn, tag.byte[i]);
		}break;
	case NBT_TAG_STRING: {
		write_nbt_string(fb, fn, tag.String);
		}break;
	case NBT_TAG_LIST: {
		write_uint8_t(fb, fn, tag.type2);
		write_int32_t(fb, fn, tag.len);
		for(size_t i = 0; i < tag.len; i++)
			write_nbt_tag(fb, fn, tag.tag[i]);
		}break;
	case NBT_TAG_COMPOUND: {
		for(size_t i = 0; i < tag.len; i++)
			write_nbt_tag(fb, fn, tag.tag[i]);
		write_nbt_tag(fb, fn, (struct nbttag){.type = NBT_TAG_END, .tag_name = MC_STR(NULL)});
		}break;
	default: {
		}break;
	}
}


void free_nbt_tag(struct nbttag tag){
	if(tag.tag_name.str != NULL){
		free_mc_string(tag.tag_name);
	}

	switch(tag.type){
	case NBT_TAG_END: {
		}break;
	case NBT_TAG_BYTE: {
		}break;
	case NBT_TAG_SHORT: {
		}break;
	case NBT_TAG_INT: {
		}break;
	case NBT_TAG_LONG: {
		}break;
	case NBT_TAG_FLOAT: {
		}break;
	case NBT_TAG_DOUBLE: {
		}break;
	case NBT_TAG_BYTE_ARRAY: {
		free(tag.byte);
		}break;
	case NBT_TAG_STRING: {
		free_mc_string(tag.String);
		}break;
	case NBT_TAG_LIST: {
		for(size_t i = 0; i < tag.len; i++)
			free_nbt_tag(tag.tag[i]);
		free(tag.tag);
		}break;
	case NBT_TAG_COMPOUND: {
		for(size_t i = 0; i < tag.len; i++)
			free_nbt_tag(tag.tag[i]);
		free(tag.tag);
		}break;
	default: {
		}break;
	}
}



