#pragma once
#include "DMJsonEncoder.h"

uint8_t decode_json_str_to_obj(const char *json_str, struct json_obj *out, uint8_t *level_count);
struct json_obj* get_current_attribute(const struct json_obj *root);
uint8_t get_json_obj(const char* obj_key, struct json_obj* obj);
uint8_t get_json_int(const char* obj_key, struct json_obj* obj);
uint8_t get_json_float(const char* obj_key, struct json_obj* obj);
uint8_t get_json_bool(const char* obj_key, struct json_obj* obj);
uint8_t get_json_null(const char* obj_key, struct json_obj* obj);
uint8_t get_json_arr(const char* obj_key, struct json_obj* obj);
uint8_t get_json_obj_num(const char* json_str);