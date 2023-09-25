#pragma once

#define _CRT_SECURE_NO_WARNINGS

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

struct json_obj
{
	uint8_t json_type;
	char* obj_key;
	int int_value;
	float float_value;
	char* str_value;
	struct json_obj* child;
	struct json_obj* prev;
	struct json_obj* next;	
	struct json_obj* parent;
};

struct json_obj* DMJson_malloc();
void DMJson_release(struct json_obj* obj);

void initialize_json_obj(struct json_obj* out);
void create_int(const char* key, int value, struct json_obj* out);
void create_float(const char* key, float value, struct json_obj* out);
void create_null(const char* key, struct json_obj* out);
void create_bool(const char* key, uint8_t bool_value, struct json_obj* out);
void create_str(const char* key, const char* value, struct json_obj* out);
void create_json_obj(const char* key, struct json_obj* out);
void add_attribute(struct json_obj* new_att, struct json_obj* out);
void create_json_arr(const char* key, struct json_obj* arr_elem, int arr_len, struct json_obj* out);
void print_json(const char* json_str);
void print_space(int level);
void add_next(struct json_obj* next, struct json_obj* out);
void clear_json(struct json_obj* root);
void generate_json_obj_str(struct json_obj* obj, char* json_str);
void generate_json_arr_str(struct json_obj* obj, char* json_str);
void generate_json_str(struct json_obj* obj, char* json_str);
int get_str_size_of_json(const struct json_obj* root);
