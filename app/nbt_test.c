#include<stdio.h>
#include<unistd.h>
#include"ntwk.h"
#include"mc_dt.h"
#include"mc_nbt.h"
#include<stddef.h>
#include<assert.h>
#include<signal.h>

int file_getc(void *p){
	return fgetc(p);
}

int file_putchar(void *p, int v){
	return fputc(v, p);
}

void print_indent(size_t indent){
	for(size_t i = 0; i < indent; i++)
		printf("\t");
}

char *tag_types[] = {
	"NBT_TAG_END",
	"NBT_TAG_BYTE",
	"NBT_TAG_SHORT",
	"NBT_TAG_INT",
	"NBT_TAG_LONG",
	"NBT_TAG_FLOAT",
	"NBT_TAG_DOUBLE",
	"NBT_TAG_BYTE_ARRAY",
	"NBT_TAG_STRING",
	"NBT_TAG_LIST",
	"NBT_TAG_COMPOUND"
};

#define PRINT_TAG_NAME(tag) printf(tag.tag_name.str == NULL ? "NULL" : "\"%.*s\"" , tag.tag_name.len, tag.tag_name.str);

void nbt_print(struct nbttag tag, size_t indent){
	print_indent(indent);
	if(tag.type != NBT_TAG_END){
		printf("%s(", tag_types[tag.type]);
		PRINT_TAG_NAME(tag);
	}
	switch(tag.type){
	case NBT_TAG_END: {
		}break;
	case NBT_TAG_BYTE: {
		printf(", %d)", tag.Byte);
		}break;
	case NBT_TAG_SHORT: {
		printf(", %d)", tag.Short);
		}break;
	case NBT_TAG_INT: {
		printf(", %d)", tag.Int);
		}break;
	case NBT_TAG_LONG: {
		printf(", %ld)", tag.Long);
		}break;
	case NBT_TAG_FLOAT: {
		printf(", %a)", tag.Float);
		}break;
	case NBT_TAG_DOUBLE: {
		printf(", %a)", tag.Double);
		}break;
	case NBT_TAG_BYTE_ARRAY: {
		printf(", %d,\n", tag.len);
		for(size_t i = 0;; i++){
			print_indent(indent+1);
			printf("NBT_TAG_BYTE(NULL, %d)", tag.byte[i]);
			if(i+1 >= tag.len)
				break;
			printf(",\n");
		}
		printf("\n");
		print_indent(indent);
		printf(")\n");
		}break;
	case NBT_TAG_STRING: {
		printf(", \"%.*s\")", tag.String.len, tag.String.str);
		}break;
	case NBT_TAG_LIST: {
		printf(", %s, %d,\n", tag_types[tag.type2], tag.len);
		for(size_t i = 0;; i++){
			nbt_print(tag.tag[i], indent+1);
			if(i+1 >= tag.len)
				break;
			printf(",\n");
		}
		printf("\n");
		print_indent(indent);
		printf(")");
		}break;
	case NBT_TAG_COMPOUND: {
		printf(", %d,\n", tag.len);
		for(size_t i = 0;; i++){
			nbt_print(tag.tag[i], indent+1);
			if(i+1 >= tag.len)
				break;
			printf(",\n");
		}
		printf("\n");
		print_indent(indent);
		printf(")");
		}break;
	}
}

int main(int argc, char **argv){
	if(argc != 2){
		printf("usage: nbt_test [File]\n");
		return 0;
	}	

	char *path = argv[1];
	FILE *file = fopen(path, "r+");


	struct nbttag tag = read_nbt_tag(file, file_getc);
	nbt_print(tag, 0);
}
