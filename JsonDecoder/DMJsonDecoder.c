#include "DMJsonDecoder.h"

uint8_t parse_json_str(const char *json_str, struct json_obj *obj)
{
    uint8_t str_len = 0;
    while(json_str[str_len] != '\"')
    {
        ++str_len;
    }

    obj->str_value = (char*)malloc((str_len + 1) * sizeof(char));

    if(obj->str_value == NULL)
    {
        return 0;
    }


    memset(obj->str_value, '\0', str_len+ 1);

    strncpy(obj->str_value, json_str, str_len);
    obj->json_type = JSON_TYPE_STR;

    return str_len;
}

uint8_t parse_special_json_str(const char *json_str, struct json_obj *obj) {

    uint8_t str_len = 0;
    while (json_str[str_len] != ',' && json_str[str_len] != '\0')
    {
        ++str_len;
    }

    obj->str_value = (char*)malloc((str_len + 1) * sizeof(char));

    if (obj->str_value == NULL)
    {
        return 0;
    }


    memset(obj->str_value, '\0', str_len + 1);

    strncpy(obj->str_value, json_str, str_len);

    if(strcmp(obj->str_value, JSON_NULL) == 0)
    {
        obj->json_type = JSON_TYPE_NULL;
    }
    else
    {
        obj->json_type = JSON_TYPE_BOOL;
    }

    return str_len;
}

uint8_t parse_json_num(const char *json_str, struct json_obj *obj)
{
    uint8_t _type = 0;
    uint8_t str_len = 0;
    while(json_str[str_len] != ',' && json_str[str_len] != '\0')
    {
        ++str_len;
        if(json_str[str_len] == 0x2e)
        {
            _type = 1;
        }
    }

    if(_type == 0)
    {
        return parse_json_int(json_str, obj, str_len);
    }
    else
    {
        return parse_json_float(json_str, obj, str_len);
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

    return count;
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

    return count;
}

uint8_t parse_json_arr(const char *json_str, struct json_obj *obj)
{


    return 0;
}

uint8_t parse_json_obj(const char *json_str, struct json_obj *obj)
{
    int inside_braces = -1;
    int key_start = -1;
    int obj_end = -1;
    int value_start = -1;
    int inside_bracket = -1;

    int str_len = (int)strlen(json_str);

    for(int i = 0; i < str_len; ++i)
    {
        if(json_str[i] == '\"' && key_start == -1 && value_start == -1)
        {
            key_start = i;
        }
        else if (json_str[i] == '\"' && key_start != -1 && value_start != -1)
        {
            struct json_obj child_obj;
            initialize_json_obj(&child_obj);
            uint8_t result = parse_json_str(json_str + i + 1, &child_obj);

            if(result != 0)
            {
                parse_json_key(json_str + key_start, &child_obj);
                add_attribute(&child_obj, obj);
                i = result <= str_len ? result : 0;
            }
            else
            {
                clear_json(&child_obj);
                clear_json(obj);
                return 0;
            } 
        }
        else if(json_str[i] == '{' && inside_braces != -1)
        {
            struct json_obj child_obj;
            initialize_json_obj(&child_obj);
            uint8_t result = parse_json_obj(json_str + i + 1, &child_obj);

        	if(result != 0)
            {
                parse_json_key(json_str + key_start, &child_obj);
                add_attribute(&child_obj, obj);
                i = result <= str_len ? result : 0;
            }
            else
            {
                clear_json(&child_obj);
                clear_json(obj);
                return 0;
            }
        }
        else if(json_str[i] == ':' && value_start == -1)
        {
            value_start = i + 1;
        }
        else if(json_str[i] == '[' && inside_bracket == -1)
        {
            struct json_obj child_obj;
            initialize_json_obj(&child_obj);
            uint8_t result = parse_json_arr(json_str + i + 1, &child_obj);

            if (result != 0)
            {
                parse_json_key(json_str + key_start, &child_obj);
                add_attribute(&child_obj, obj);
                i = result <= str_len ? result : 0;
            }
            else
            {
                clear_json(&child_obj);
                clear_json(obj);
                return 0;
            }
        }
        else if(json_str[i] >= 48 && json_str[i] <= 57 && value_start != -1)
        {
            struct json_obj child_obj;
            initialize_json_obj(&child_obj);
            uint8_t result = parse_json_num(json_str + i + 1, &child_obj);

            if (result != 0)
            {
                parse_json_key(json_str + key_start, &child_obj);
                add_attribute(&child_obj, obj);
                i = result <= str_len ? result : 0;
            }
            else
            {
                clear_json(&child_obj);
                clear_json(obj);
                return 0;
            }
        }
        else if(json_str[i] == '}' && inside_braces != -1)
        {
            obj_end = i + 1;
            inside_braces = 0; 
        }
        else if(json_str[i] == ',')
        {
            struct json_obj child_obj;
            initialize_json_obj(&child_obj);
            uint8_t result = parse_special_json_str(json_str + value_start, &child_obj);

            if (result != 0)
            {
                parse_json_key(json_str + key_start, &child_obj);
                add_attribute(&child_obj, obj);
                i = result <= str_len ? result : 0;
            }
            else
            {
                clear_json(&child_obj);
                clear_json(obj);
                return 0;
            }

            inside_braces = -1;
            key_start = -1;
            obj_end = -1;
            value_start = -1;
            inside_bracket = -1;
        }
    }

    return obj_end;
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

uint8_t parse_json_key(const char *json_str, struct json_obj *obj) {

    uint8_t key_len = 0;
    while(json_str[key_len] != 0x22)
    {
        ++key_len;
    }

    obj->obj_key = (char*)malloc((key_len + 1) * sizeof(char));

    if(obj->obj_key == NULL)
    {
        printf("parse_json_key: memory allocation failed.\n");

        return 0;
    }

    memset(obj->obj_key, '\0', key_len + 1);

    strncpy(obj->obj_key, json_str, key_len);

    printf("%s\n", obj->obj_key);

    return key_len;
}


