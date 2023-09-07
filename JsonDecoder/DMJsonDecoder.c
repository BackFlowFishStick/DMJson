#include "DMJsonDecoder.h"

uint8_t decode_json_str_to_obj(const char* jsonStr, struct JsonObj* out) 
{
	int i = 0;

	const int len = (int)strlen(jsonStr);

	int isBracket = 1;
	int isParantheses = 1;
	int isKey = 1;

	if(len == 0) 
	{
		return 0;
	}

	if(jsonStr[0] != '{' && jsonStr[0] != '[') 
	{
		return 0;
	}

	if(jsonStr[len - 2] != '}' && jsonStr[len - 2] != ']') 
	{
		return 0; 
	}
	
	while(jsonStr[i] != '\0') 
	{
		i++;

		if(jsonStr[i] == '{')
		{
			isBracket = 0;

		}

		else if(jsonStr[i] == '[' )
		{
			isParantheses = 0;
		}

		if(jsonStr[i] == '}' && isBracket == 0)
		{
			isBracket = 1;
		}
		else if(jsonStr[i] == ']' && isParantheses == 0)
		{
			isParantheses = 1;
		}
		if (jsonStr[i] == '\"')
		{
			isKey = 0;
			
			for(int j = i + 1; jsonStr[j] != '\"'; ++j)
			{
				
			}
		}
		
	}

	if(isBracket == 0 || isParantheses == 0)
	{
		return 0;
	}

}

uint8_t get_json_obj(const char* objKey, struct JsonObj* obj)
{
	

	return 0;
}

uint8_t get_json_int(const char* objKey, struct JsonObj* obj)
{
	return 0;
}

uint8_t get_json_float(const char* objKey, struct JsonObj* obj)
{
	return 0;
}

uint8_t get_json_bool(const char* objKey, struct JsonObj* obj)
{
	return 0;
}

uint8_t get_json_null(const char* objKey, struct JsonObj* obj)
{
	return 0;
}

uint8_t get_json_arr(const char* objKey, struct JsonObj* obj)
{
	return 0;
}
