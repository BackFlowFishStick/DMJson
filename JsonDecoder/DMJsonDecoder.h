#pragma once
#include "DMJsonEncoder.h"

// Parsing methods
uint8_t parse_json_str(const char *json_str, DM_JSON_OBJ obj);
uint8_t parse_special_json_str(const char *json_str, DM_JSON_OBJ obj);
uint8_t parse_json_num(const char *json_str, DM_JSON_OBJ obj);
uint8_t parse_json_int(const char *json_str, DM_JSON_OBJ obj, uint8_t count);
uint8_t parse_json_float(const char *json_str, DM_JSON_OBJ obj, uint8_t count);
uint8_t parse_json_arr(const char *json_str, DM_JSON_OBJ obj);
uint8_t parse_json_obj(const char *json_str, DM_JSON_OBJ root);
uint8_t parse_json_key(const char* json_str, DM_JSON_OBJ root);
// Helper methods
uint8_t connect_obj_to_root(const int result, const int key_start, const char* json_str, DM_JSON_OBJ root, DM_JSON_OBJ child);
// Searching methods 
uint8_t get_json_obj(const char* obj_key, DM_JSON_OBJ obj);
uint8_t get_json_int(const char* obj_key, DM_JSON_OBJ obj);
uint8_t get_json_float(const char* obj_key, DM_JSON_OBJ obj);
uint8_t get_json_bool(const char* obj_key, DM_JSON_OBJ obj);
uint8_t get_json_null(const char* obj_key, DM_JSON_OBJ obj);
uint8_t get_json_arr(const char* obj_key, DM_JSON_OBJ obj);