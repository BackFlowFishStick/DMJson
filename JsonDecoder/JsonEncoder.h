#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#define Json_Bool_True "true" 
#define Json_Bool_False "false" 
#define Json_NULL "Null" 

#define JsonType_Null 0x00
#define JsonType_Int 0x01
#define JsonType_Float 0x02
#define JsonType_Bool 0x03
#define JsonType_Str 0x04
#define JsonType_Obj 0x05
#define JsonType_Arr 0x06

#define Json_Length 2000;
#define JsonSpace "  "

struct JsonObj
{
	uint8_t jsonType;
	char* objKey;
	int intValue;
	float floatValue;
	char* strValue;
	struct JsonObj* child;
	struct JsonObj* prev;
	struct JsonObj* next;	
	struct JsonObj* parent;
};

void initialize_json_obj(struct JsonObj* out);
void create_int(const char* key, int value, struct JsonObj* out);
void create_float(const char* key, float value, struct JsonObj* out);
void create_null(const char* key, struct JsonObj* out);
void create_bool(const char* key, uint8_t boolValue, struct JsonObj* out);
void create_str(const char* key, const char* value, struct JsonObj* out);
void create_json_obj(const char* key, struct JsonObj* out);
void add_attribute(struct JsonObj* newAtt, struct JsonObj* out);
void create_json_arr(const char* key, struct JsonObj* arrElem, int arrLen, struct JsonObj* out);
void print_json(char* jsonStr);
void print_space(int level);
void add_next(struct JsonObj* next, struct JsonObj* out);
void clear_json(struct JsonObj* root);
void generate_json_str(struct JsonObj* obj, char* jsonStr);
int get_str_size_of_json(const struct JsonObj* root);
