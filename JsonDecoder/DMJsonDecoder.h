#pragma once
#include "DMJsonEncoder.h"

uint8_t decode_json_str(const char* jsonStr, struct JsonObj* out);
void get_json_obj(const char* jsonStr, const char* objKey, struct JsonObj* obj);
void get_json_int(const char* jsonStr, const char* objKey, struct JsonObj* obj);
void get_json_float(const char* jsonStr, const char* objKey, struct JsonObj* obj);
void get_json_bool(const char* jsonStr, const char* objKey, struct JsonObj* obj);
void get_json_null(const char* jsonStr, const char* objKey, struct JsonObj* obj);
void get_json_arr(const char* jsonStr, const char* objKey, struct JsonObj* obj);