#include "DMJsonDecoder.h"

uint8_t parse_json_str_to_obj(const char* json_str, struct json_obj* root_obj)
{
    if(root_obj == NULL || json_str == NULL)
    {
        printf("This method require both parameters.\n");

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

    int digit_start_index = -1;
    int digit_end_index = -1;

    int other_start_index = -1;
    int other_end_index = -1;

    initialize_json_obj(root_obj);


	for (int i = 0; i < (int)strlen(json_str); i++)
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

                    struct json_obj str_obj;
                    initialize_json_obj(&str_obj);
                    if(parse_json_str((json_str + str_value_start_index), &str_obj, (str_value_end_index - str_value_start_index+ 1)) != 0)
                    {
                        add_attribute(&str_obj, root_obj);

                        if(key_end_index != -1 && key_start_index != -1)
                        {
                            str_obj.obj_key = (char*)malloc((key_end_index + 2 - key_start_index) * sizeof(char));

                            strncpy(str_obj.obj_key, (json_str + key_start_index), (key_end_index - key_start_index + 1));
                        }
                    }
                    else
                    {
                        goto invalid_format;
                    }

                }
                else
                {
                    key_end_index = i - 1;
                }

            }
        }
        else if(json_str[i] == ':')
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

        else if(json_str[i] == '{')
        {
            if(inside_braces == 0)
            {
                obj_start_index = i + 1;
            }
            inside_braces += 1;
            if(obj_start_index > (int)strlen(json_str))
            {
                goto invalid_format;
            }
        }

        else if(json_str[i] == '}')
        {
            inside_braces -= 1;
            if(inside_braces == 0)
            {
//                obj_end_index = i - 1;
//
//                struct json_obj child_obj;
//                initialize_json_obj(&child_obj);
//
//                if(parse_json_obj((json_str + obj_start_index), &child_obj, 0) != 0)
//                {
//                    add_attribute(&child_obj, root_obj);
//
//                    if (key_end_index != -1 && key_start_index != -1)
//                    {
//                        child_obj.obj_key = (char*)malloc((key_end_index + 2 - key_start_index) * sizeof(char));
//
//                        strncpy(child_obj.obj_key, (json_str + key_start_index), (key_end_index - key_start_index + 1));
//                    }
//                }
//                else
//                {
//                    goto invalid_format;
//                }

            }
            if(obj_end_index < 0)
            {
                goto invalid_format;
            }
        }
        else if(json_str[i] == '[')
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

        else if(json_str[i] == ']')
        {
            inside_square_bracket -= 1;
            if(inside_square_bracket == 0)
            {
//                arr_end_index = i - 1;
//
//                struct json_obj child_obj;
//                initialize_json_obj(&child_obj);
//
//                if (parse_json_arr((json_str + obj_start_index), &child_obj, 0) != 0)
//                {
//                    add_attribute(&child_obj, root_obj);
//
//                    if (key_end_index != -1 && key_start_index != -1)
//                    {
//                        child_obj.obj_key = (char*)malloc((key_end_index + 2 - key_start_index) * sizeof(char));
//
//                        strncpy(child_obj.obj_key, (json_str + key_start_index), (key_end_index - key_start_index + 1));
//                    }
//                }
//                else
//                {
//                    goto invalid_format;
//                }
            }
            if(arr_end_index < 0)
            {
                goto invalid_format;
            }
        }

        else if(json_str[i] == ',')
        {
            if(inside_quotation == 0 && digit_start_index != -1)
            {
                struct json_obj digit_obj;
                initialize_json_obj(&digit_obj);

                if(parse_json_num(json_str + digit_start_index, &digit_obj, 0) != 0)
                {
                    add_attribute(&digit_obj, root_obj);
                }
            }

        }
        else
        {
	        if(json_str[i] >= 48 && json_str[i] <= 57)
	        {
		        if(digit_start_index == -1)
		        {
                    digit_start_index = i;
		        }
                else
                {
                    digit_end_index = i;
                }

	        }
        }

	}

    return 1;

    invalid_format:
    {
        clear_json(root_obj);
        printf("The format is invalid.\n");
        return 0;
    }


}

uint8_t parse_json_str(const char *json_str, struct json_obj *obj, uint8_t count)
{
    obj->str_value = (char*)malloc((count + 1) * sizeof(char));

    if(obj->str_value == NULL)
    {
        return 0;
    }
    memset(obj->str_value, '\0', count + 1);

    strncpy(obj->str_value, json_str, count);
    obj->json_type = JSON_TYPE_STR;

    return 1;
}

uint8_t parse_special_json_str(const char *json_str, struct json_obj *obj, uint8_t count) {

    obj->str_value = (char*)malloc((count + 1) * sizeof(char));

    if(obj->str_value == NULL)
    {
        return 0;
    }
    memset(obj->str_value, '\0', count + 1);

    strncpy(obj->str_value, json_str, count);

    if(strcmp(obj->str_value, JSON_TYPE_NULL) == 0)
    {
        obj->json_type = JSON_TYPE_NULL;
    }
    else
    {
        obj->json_type = JSON_TYPE_BOOL;
    }

    return 1;
}

uint8_t parse_json_num(const char *json_str, struct json_obj *obj, uint8_t count)
{
    uint8_t _type = 0;
    for(int i = 0; i < count; ++i)
    {
        _type = 1;
        if(json_str[i] == 0x2e)
        {
            _type = 0;
            break;
        }
    }

    if(_type == 1)
    {
        return parse_json_int(json_str, obj, count);
    }
    else
    {
        return parse_json_float(json_str, obj, count);
    }
}

uint8_t parse_json_int(const char *json_str, struct json_obj *obj, uint8_t count)
{
    char* sub_str = (char*)malloc((count + 1)*sizeof(char));

    if(sub_str == NULL)
    {
        printf("parse_json_int(): Memory allocation failed\n");
        return 0;
    }

    memset(sub_str, '\0', count + 1);

    strncpy(sub_str, json_str, count);


    char* endPtr = NULL;
    obj->int_value = strtol(sub_str, &endPtr, 10);

    if (*endPtr != '\0') {
        printf("Conversion failed, not an integer.\n");
        return 0;
    } else {
        printf("Conversion succeed: %d\n", obj->int_value);
    }

    free(sub_str);
    //free(endPtr);
    sub_str = NULL;
    endPtr = NULL;

    obj->json_type = JSON_TYPE_INT;

    return 1;
}

uint8_t parse_json_float(const char *json_str, struct json_obj *obj, uint8_t count)
{
    char* sub_str = (char*)malloc((count + 1)*sizeof(char));

    if(sub_str == NULL)
    {
        printf("parse_json_float(): Memory allocation failed\n");
        return 0;
    }

    memset(sub_str, '\0', count + 1);

    strncpy(sub_str, json_str, count);

    char* endPtr = NULL;
    obj->float_value = (float)strtod(json_str, &endPtr);

    if (*endPtr != '\0') {
        printf("Conversion failed, not an float.\n");
        return 0;
    } else {
        printf("Conversion succeed: %f\n", obj->float_value);
    }

    free(sub_str);
    sub_str = NULL;
    endPtr = NULL;

    obj->json_type = JSON_TYPE_FLOAT;

    return 1;
}

uint8_t parse_json_arr(const char *json_str, struct json_obj *obj, uint8_t count)
{


    return 0;
}

uint8_t parse_json_obj(const char *json_str, struct json_obj *obj, uint8_t count)
{

    for(int i = 0; i < count + 1; ++i)
    {
        if(json_str[i] == '{')
        {
            struct json_obj child_obj;
            initialize_json_obj(&child_obj);

        }
    }

    return 0;
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


