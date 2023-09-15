#include "DMJsonDecoder.h"

uint8_t decode_json_str_to_obj(const char *json_str, struct json_obj *out, int *current_index)
{
    if(json_str == NULL)
    {
        printf("Json text is required!\n");
        return 0;
    }
    if(out == NULL)
    {
        printf("Json Object is required!\n");
        return 0;
    }

    if(current_index == NULL || *current_index < 0 || *current_index > strlen(json_str))
    {
        printf("The current index is invalid.\n");
        return 0;
    }

    int inside_braces = 0;
	int obj_start_index = -1;
	int obj_end_index = -1;

	int inside_quotation = 0;
	int key_start_index = -1;
	int key_end_index = -1;
    int str_value_start_index = -1;
    int str_value_end_index = -1;

	int inside_square_bracket = 0;
	int arr_start_index = -1;
	int arr_end_index = -1;

    int is_obj_finish = 0;
    int is_key_finish = 0;
	int is_after_colon = 0;

 
	initialize_json_obj(out);

	for (int i = *current_index; i < strlen(json_str); i++)
    {
        if(json_str[i] == '\"')
        {
            if(inside_quotation == 0)
            {
                inside_quotation = 1;

                if(is_after_colon != 0)
                {
                    str_value_start_index = i + 1;
                }
                else
                {
                    key_start_index = i + 1;
                }
            }
            else
            {
                inside_quotation = 0;

                if(is_after_colon != 0)
                {
                    str_value_end_index = i - 1;
                }
                else
                {
                    key_end_index = i - 1;
                }

            }
        }
        if(json_str[i] == ':')
        {
            if(is_after_colon != 0)
            {
                goto invalid_format;
            }
            else
            {
                is_after_colon = 1;
            }
        }

        if(json_str[i] == '{')
        {
            if(inside_braces == 0)
            {
                obj_start_index = i + 1;
            }
            inside_braces += 1;
            if(obj_start_index > strlen(json_str))
            {
                goto invalid_format;
            }
        }

        if(json_str[i] == '}')
        {
            inside_braces -= 1;
            if(inside_braces == 0)
            {
                obj_end_index = i - 1;
            }
            if(obj_end_index < 0)
            {
                goto invalid_format;
            }
        }
        if(json_str[i] == '[')
        {
            if(inside_square_bracket == 0)
            {
                arr_start_index = i + 1;
            }
            inside_square_bracket += 1;

            if(arr_start_index > strlen(json_str))
            {
                goto invalid_format;
            }
        }

        if(json_str[i] == ']')
        {
            inside_square_bracket -= 1;
            if(inside_square_bracket == 0)
            {
                arr_end_index = i - 1;
            }
            if(arr_end_index < 0)
            {
                goto invalid_format;
            }
        }

        if(json_str[i] == ',')
        {
            struct json_obj child_node;
            initialize_json_obj(&child_node);


        }

	}

    return 1;

    invalid_format:
    {
        printf("The format is invalid.\n");
        return 0;
    };


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

struct json_obj *get_current_attribute(const struct json_obj *root) {

    return NULL;
}
