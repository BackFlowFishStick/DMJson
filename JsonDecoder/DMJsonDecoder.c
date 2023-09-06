#include "DMJsonDecoder.h"

uint8_t decode_json_str(const char* jsonStr, struct JsonObj* out) 
{
	int i = 0;

	int len = strlen(jsonStr);

	int isBracked = 0; 

	if(len == 0) 
	{
		return 0;
	}

	if(jsonStr[i] != '{' || jsonStr[i] != '[') 
	{
		return 0;
	}

	if(jsonStr[len - 2] != '}' || jsonStr[len - 2] != ']') 
	{
		return 0; 
	}
	
	while(jsonStr[i] != '\0') 
	{
		i++;

		if(jsonStr[i] = '{')
	}

}

void get_json_obj(const char* jsonStr, const char* objKey, struct JsonObj* obj)
{
	int i = 0;

	while(jsonStr[i] != '{') 
	{
		i++;
	}

}

void get_json_int(const char* jsonStr, const char* objKey, struct JsonObj* obj)
{

}

void get_json_float(const char* jsonStr, const char* objKey, struct JsonObj* obj)
{

}

void get_json_bool(const char* jsonStr, const char* objKey, struct JsonObj* obj)
{

}

void get_json_null(const char* jsonStr, const char* objKey, struct JsonObj* obj)
{

}

void get_json_arr(const char* jsonStr, const char* objKey, struct JsonObj* obj)
{

}
