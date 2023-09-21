#pragma once
#include "DMJsonEncoder.h"

uint8_t parse_json_str_to_obj(const char* json_str, struct json_obj* root_obj);
struct json_obj* get_current_attribute(const struct json_obj *root);

// Parsing methods
uint8_t parse_json_str(const char *json_str, struct json_obj *obj);
uint8_t parse_special_json_str(const char *json_str, struct json_obj *obj);
uint8_t parse_json_num(const char *json_str, struct json_obj *obj, uint8_t count);
uint8_t parse_json_int(const char *json_str, struct json_obj *obj, uint8_t count);
uint8_t parse_json_float(const char *json_str, struct json_obj *obj, uint8_t count);
uint8_t parse_json_arr(const char *json_str, struct json_obj *obj, uint8_t count);
uint8_t parse_json_obj(const char *json_str, struct json_obj *obj, uint8_t count);


// Searching methods 
uint8_t get_json_obj(const char* obj_key, struct json_obj* obj);
uint8_t get_json_int(const char* obj_key, struct json_obj* obj);
uint8_t get_json_float(const char* obj_key, struct json_obj* obj);
uint8_t get_json_bool(const char* obj_key, struct json_obj* obj);
uint8_t get_json_null(const char* obj_key, struct json_obj* obj);
uint8_t get_json_arr(const char* obj_key, struct json_obj* obj);