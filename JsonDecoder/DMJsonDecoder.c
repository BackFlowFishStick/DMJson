#include "DMJsonDecoder.h"

uint8_t decode_json_str_to_obj(const char* json_str, struct json_obj* out) 
{
	int inside_braces = 0;
	int obj_start_index = 0;
	int obj_end_index = 0;

	int inside_quotation = 0;
	int quotation_start_index = 0;
	int quotation_end_index = 0;

	initialize_json_obj(out);

	for (int i = 1; i < strlen(json_str); i++) {
		if (json_str[i] == '{') {

			inside_braces = 1;

			obj_start_index = i + 1;

			struct json_obj child_node;
			initialize_json_obj(&child_node);
			const int child_node_result = decode_json_str_to_obj((json_str + obj_start_index), &child_node);

			if(child_node_result == 1)
			{
				add_attribute(&child_node, out);
			}
		}
		else if (json_str[i] == '}' && inside_braces == 1) 
		{
			obj_end_index = i; 
			break; 
		}
	}

	if(obj_end_index == 0)
	{
		printf("The json object format is not correct.\n");
		return 0;
	}

	return 1;
}

uint8_t get_json_obj(const char* obj_key, struct json_obj* obj)
{
	

	return 0;
}

uint8_t get_json_int(const char* obj_key, struct json_obj* obj)
{
	return 0;
}

uint8_t get_json_float(const char* obj_key, struct json_obj* obj)
{
	return 0;
}

uint8_t get_json_bool(const char* obj_key, struct json_obj* obj)
{
	return 0;
}

uint8_t get_json_null(const char* obj_key, struct json_obj* obj)
{
	return 0;
}

uint8_t get_json_arr(const char* obj_key, struct json_obj* obj)
{
	return 0;
}

uint8_t get_json_obj_num(const char* json_str)
{

	const int len = (int)strlen(json_str);

	int isBracket = 0;
	int got_left_curly_brace = 0;
	int got_right_curly_brace = 0;
	int is_parantheses = 0;
	int is_key = 0;


	for(int i = 0; json_str[i] != '\0'; ++i)
	{
		if ('{' == json_str[i])
		{
			got_left_curly_brace += 1;
		}
		else if('}' == json_str[i])
		{
			got_right_curly_brace += 1;
		}
	}

	if(got_left_curly_brace != got_right_curly_brace)
	{
		printf("This is not Json format\n");
		return 0;
	}



	return 0;
}
