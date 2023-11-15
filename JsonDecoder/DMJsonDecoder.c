#include "DMJsonDecoder.h"

uint8_t parse_json_str(const char *json_str, DM_JSON_OBJ obj)
{
    uint8_t str_len = 0;
    uint8_t quote_start = 0;
  
	while (quote_start < 2)
    {
        if (json_str[str_len] == '\"')
        {
            ++quote_start;
        }
        ++str_len;
        //printf("%c\n", json_str[str_len]);
    }
    str_len -= 2;

    obj->str_value = (char*)malloc((str_len + 1) * sizeof(char));

    if(obj->str_value == NULL)
    { 
        return 0;
    }


    memset(obj->str_value, '\0', str_len+ 1);

    strncpy(obj->str_value, json_str + 1, str_len);
    obj->json_type = JSON_TYPE_STR;

    return str_len + 2;
}

uint8_t parse_special_json_str(const char *json_str, DM_JSON_OBJ obj) {

    uint8_t str_len = 0;
    while (json_str[str_len] == 't' || json_str[str_len] == 'r' || json_str[str_len] == 'u' 
        || json_str[str_len] == 'e' || json_str[str_len] == 'f'|| json_str[str_len] == 'a' || 
        json_str[str_len] == 'l' || json_str[str_len] == 's' || json_str[str_len] == 'e' 
        || json_str[str_len] == 'N' || json_str[str_len] == 'u' || json_str[str_len] == 'l')
    {
        ++str_len;
        printf("%c\n", json_str[str_len]);
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

uint8_t parse_json_num(const char *json_str, DM_JSON_OBJ obj)
{
    uint8_t _type = 0;
    uint8_t str_len = 0;
    while((json_str[str_len] >= 48 && json_str[str_len] <= 57) || json_str[str_len] == '.')
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

uint8_t parse_json_int(const char *json_str, DM_JSON_OBJ obj, uint8_t count)
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
	sub_str = NULL;
    endPtr = NULL;

    obj->json_type = JSON_TYPE_INT;

    return count;
}

uint8_t parse_json_float(const char *json_str, DM_JSON_OBJ obj, uint8_t count)
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
    obj->float_value = (float)strtod(sub_str, &endPtr);

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

uint8_t parse_json_arr(const char *json_str, DM_JSON_OBJ root)
{
    root->json_type = JSON_TYPE_ARR;
    int inside_arr = -1;
    int inside_bracket = -1;

    int str_len = (int)strlen(json_str);
    uint8_t obj_result = 0;

    for (int i = 0; i < str_len; ++i)
    {
    	if (*(json_str + i) == '[')
		{
			if(inside_arr == -1)
			{
                inside_arr = 1;
			}
            else
            {

                DM_JSON_OBJ child_obj = DM_JSON_MALLOC();
                int result = parse_json_arr((json_str + i), child_obj);
                if (result != 0)
                {
                    add_attribute(child_obj, root);
                }
                else
                {
                    DM_JSON_FREE(child_obj);
                }
                i = i + result - 1;
            }
		}
        else if (*(json_str + i) == '{')
        {
            DM_JSON_OBJ child_obj = DM_JSON_MALLOC();
            int result = parse_json_obj((json_str + i), child_obj);

            if(result != 0)
            {
                add_attribute(child_obj, root);
            }
            else
            {
                DM_JSON_FREE(child_obj);
            }
            i = i + result - 1;
        }
        
        else if ((*(json_str + i) >= 48 && *(json_str + i) <= 57) || *(json_str + i) == '.')
        {
            DM_JSON_OBJ child_obj = DM_JSON_MALLOC();
            int result = parse_json_num((json_str + i), child_obj);

            if(result != 0)
            {
                add_attribute(child_obj, root);
            }
            else
            {
                DM_JSON_FREE(child_obj);
            }
            i = i + result - 1;
        }
        else if ((*(json_str + i) == 't') || (*(json_str + i) == 'f') || (*(json_str + i) == 'N'))
        {
            DM_JSON_OBJ child_obj = DM_JSON_MALLOC();
            int result = parse_special_json_str(json_str + i, child_obj);

            if (result != 0)
            {
                add_attribute(child_obj, root);
            }
            else
            {
                DM_JSON_FREE(child_obj);
            }
            i = i + result - 1;

        }
        else if (*(json_str + i) == ',')
        {
            
        }
        else if (*(json_str + i) == ']')
        {
            obj_result = i + 1;
            return obj_result;
        }
    }

    return obj_result;
}

uint8_t parse_json_obj(const char *json_str, DM_JSON_OBJ root)
{
    root->json_type = JSON_TYPE_OBJ;
    int inside_bracket = -1;
    int key_start = -1;
    int value_start = -1;

    int str_len = (int)strlen(json_str);
    uint8_t obj_result = 0;

	for(int i = 0; i < str_len; ++i)
    {
        printf("%c\n", json_str[i]);
        if(*(json_str + i) == '\"')
        {
	        if(key_start == -1)
	        {
                key_start = i;
	        }
            else
            {
                if(value_start != -1)
                {
                    DM_JSON_OBJ child_obj = DM_JSON_MALLOC();
                    int result = parse_json_str(json_str + i, child_obj);
                   
                    if(result != 0)
                    {
	                    if(key_start != -1)
	                    {
		                    if(connect_obj_to_root(result, key_start, json_str, root, child_obj) == 0)
		                    {
                                DM_JSON_FREE(child_obj);
                                return 0;
		                    }
	                    }
                        else
                        {
                            DM_JSON_FREE(child_obj);
                            return 0;
                        }
                    }
					// 将字符串下标推至json单位值的最后一个字符所在位置，当下次循环开始时，刚好可以检测下个字符是否为逗号                    
                    i = i + result - 1;
                }
            }
        }

        else if(*(json_str + i) == ':')
        {
			if(value_start == -1)
			{
                value_start = 1;
			}
        }
        else if(*(json_str + i) == '{')
        {
	        if(inside_bracket == -1)
	        {
                inside_bracket = 1;
	        }
            else
            {
                DM_JSON_OBJ child_obj = DM_JSON_MALLOC();
				int result = parse_json_obj((json_str + i), child_obj);
                if(result != 0)
                {
	                if(key_start != -1)
	                {
                        if (connect_obj_to_root(result, key_start, json_str, root, child_obj) == 0)
                        {
                            DM_JSON_FREE(child_obj);
                            return 0;
                        }
	                }
                    else
                    {
                        DM_JSON_FREE(child_obj);
                        return 0;
                    }
                }
                i = i + result - 1;
            }
        }
        else if((*(json_str + i) >= 48 && *(json_str + i) <= 57) || *(json_str + i) == '.')
        {
            DM_JSON_OBJ child_obj = DM_JSON_MALLOC();
            int result = parse_json_num(json_str + i, child_obj);

            if(result != 0)
            {
	            if(key_start != -1)
	            {
                    if(connect_obj_to_root(result, key_start, json_str, root, child_obj) == 0)
                    {
                        DM_JSON_FREE(child_obj);
                        return 0;
                    }
	            }
                else
                {
                    DM_JSON_FREE(child_obj);
                    return 0;
                }
            }
            i = i + result - 1;
        }
        else if((*(json_str + i) == 't') || (*(json_str + i) == 'f') || (*(json_str + i) == 'N'))
        {
            if(value_start != -1)
            {
                DM_JSON_OBJ child_obj = DM_JSON_MALLOC();
                int result = parse_special_json_str(json_str + i, child_obj);

                if (result != 0)
                {
                    if (key_start != -1)
                    {
                        if (connect_obj_to_root(result, key_start, json_str, root, child_obj) == 0)
                        {
                            DM_JSON_FREE(child_obj);
                            return 0;
                        }
                    }
                    else
                    {
                        DM_JSON_FREE(child_obj);
                        return 0;
                    }
                }
                i = i + result - 1;
            }
        }
        else if(*(json_str + i) == ',')
        {
            key_start = -1;
            value_start = -1;
        }
        else if (*(json_str + i) == '}')
        {
            obj_result = i + 1;
            return obj_result;
        }
    }

    return obj_result;
}



uint8_t get_json_obj(const char* obj_key, DM_JSON_OBJ obj)
{
	

	return 0;
}

uint8_t get_json_int(const char* obj_key, DM_JSON_OBJ obj)
{
	return 0;
}

uint8_t get_json_float(const char* obj_key, DM_JSON_OBJ obj)
{
	return 0;
}

uint8_t get_json_bool(const char* obj_key, DM_JSON_OBJ obj)
{
	return 0;
}

uint8_t get_json_null(const char* obj_key, DM_JSON_OBJ obj)
{
	return 0;
}

uint8_t get_json_arr(const char* obj_key, DM_JSON_OBJ obj)
{
	return 0;
}

uint8_t parse_json_key(const char *json_str, DM_JSON_OBJ root)
{
    int quote_start = 0;

    uint8_t key_len = 0;

    while(quote_start < 2)
    {
        if(json_str[key_len] == '\"')
        {
            ++quote_start;
        }
        ++key_len;
    }
    key_len -= 2;

    root->obj_key = (char*)malloc((key_len + 1) * sizeof(char));

    if(root->obj_key == NULL)
    {
        printf("parse_json_key: memory allocation failed.\n");

        return 0;
    }

    memset(root->obj_key, '\0', key_len + 1);

    strncpy(root->obj_key, json_str + 1, key_len);

    return key_len;
}

uint8_t connect_obj_to_root(const int result, const int key_start, const char* json_str, DM_JSON_OBJ root,
	DM_JSON_OBJ child)
{
    if (result != 0)
    {
        if (key_start != -1)
        {
            int key_result = parse_json_key((json_str + key_start), child);
            if (key_result == 0)
            {
                DM_JSON_FREE(child);
                return 0;
            }
            add_attribute(child, root);
            return 1;
        }
    }
    return 0;
}

uint8_t create_array(DM_JSON_OBJ obj, DM_JSON_OBJ* arr)
{
    return 1;
}


