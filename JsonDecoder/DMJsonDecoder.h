#pragma once
#include "DMJsonEncoder.h"

uint8_t decode_json_str_to_obj(const char* jsonStr, struct JsonObj* out);
uint8_t get_json_obj(const char* objKey, struct JsonObj* obj);
uint8_t get_json_int(const char* objKey, struct JsonObj* obj);
uint8_t get_json_float(const char* objKey, struct JsonObj* obj);
uint8_t get_json_bool(const char* objKey, struct JsonObj* obj);
uint8_t get_json_null(const char* objKey, struct JsonObj* obj);
uint8_t get_json_arr(const char* objKey, struct JsonObj* obj);