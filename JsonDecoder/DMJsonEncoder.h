#pragma once
#ifdef Windows
	#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#define JSON_BOOL_TRUE "true" 
#define JSON_BOOL_FALSE "false" 
#define JSON_NULL "Null" 

#define JSON_TYPE_NULL 0x00
#define JSON_TYPE_INT 0x01
#define JSON_TYPE_FLOAT 0x02
#define JSON_TYPE_BOOL 0x03
#define JSON_TYPE_STR 0x04
#define JSON_TYPE_OBJ 0x05
#define JSON_TYPE_ARR 0x06

#define JSON_LENGTH 2000;
#define JSON_SPACE "  "

#define DM_JSON_MALLOC json_malloc
#define DM_JSON_FREE json_release

#define DM_JSON_OBJ struct json_obj*

struct json_obj
{
	uint8_t json_type;
	char* obj_key;
	int int_value;
	float float_value;
	char* str_value;
	DM_JSON_OBJ child;
	DM_JSON_OBJ prev;
	DM_JSON_OBJ next;
	DM_JSON_OBJ parent;
};

DM_JSON_OBJ json_malloc();
void json_release(DM_JSON_OBJ obj);

void initialize_json_obj(DM_JSON_OBJ out);
void create_int(const char* key, int value, DM_JSON_OBJ out);
void create_float(const char* key, float value, DM_JSON_OBJ out);
void create_null(const char* key, DM_JSON_OBJ out);
void create_bool(const char* key, uint8_t bool_value, DM_JSON_OBJ out);
void create_str(const char* key, const char* value, DM_JSON_OBJ out);
void create_json_obj(const char* key, DM_JSON_OBJ out);
void add_attribute(DM_JSON_OBJ new_att, DM_JSON_OBJ out);
void create_json_arr(const char* key, DM_JSON_OBJ* arr_elems, int arr_len, DM_JSON_OBJ out);
void print_json(const char* json_str);
void print_space(int level);
void add_next(DM_JSON_OBJ next, DM_JSON_OBJ out);
void clear_json(DM_JSON_OBJ root);
void generate_json_obj_str(DM_JSON_OBJ obj, char* json_str);
void generate_json_arr_str(DM_JSON_OBJ obj, char* json_str);
void generate_json_str(DM_JSON_OBJ obj, char* json_str);
int get_str_size_of_json(const DM_JSON_OBJ root);
