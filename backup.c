uint8_t decode_json_str_to_obj(const char *json_str, struct json_obj *out, int *start_index, int *end_index)
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
    if(start_index == NULL || *start_index > strlen(json_str))
    {
        printf("The start index is invalid.\n");
        return 0;
    }
    if(end_index == NULL || *end_index < 0)
    {
        printf("The end index is invalid.\n");
        return 0;
    }

    int inside_braces = 0;
	int obj_start_index = -1;
	int obj_end_index = -1;

	int inside_quotation = 0;
	int quotation_start_index = -1;
	int quotation_end_index = -1;

	int inside_square_bracket = 0;
	int arr_start_index = -1;
	int arr_end_index = -1;

    int is_obj_finish = 0;
    int is_key_finish = 0;
	int is_after_colon = 0;

 
	initialize_json_obj(out);

	for (int i = *start_index; i < strlen(json_str); i++) {
		if (json_str[i] == '{')
        {
            inside_braces = 1;

			obj_start_index = i + 1;

		}

		else if(json_str[i] == '[')
		{
			inside_square_bracket = 1;

			arr_start_index = i + 1;
		}

        else if(json_str[i] == '\"')
        {
            if(inside_quotation == 0)
            {
                quotation_start_index = i + 1;
                inside_quotation = 1;
            }
            else
            {
                if(is_after_colon == 0)
                {
                    quotation_end_index = i - 1;
                    int key_len = quotation_end_index + 2 - quotation_start_index;
                    out->obj_key = (char*)malloc(sizeof(char) * key_len);
                    strncpy(out->obj_key, json_str + i, (quotation_end_index - quotation_start_index + 1));
                }
                else
                {
                    quotation_end_index = i - 1;
                    int str_len = quotation_end_index + 2 - quotation_start_index;
                    out->str_value = (char*)malloc(sizeof(char) * str_len);
                    strncpy(out->str_value, json_str + i, (quotation_end_index - quotation_start_index + 1));

                    out->json_type = strcmp(out->str_value, JSON_TYPE_NULL) == 0 ? JSON_TYPE_NULL : JSON_TYPE_STR;
                }

            }
        }

		else if(json_str[i] == ':')
		{
			if(is_after_colon == 0)
			{
                is_after_colon = 1;
			}
            else
            {
                printf("The format is invalid.\n");
                return 0;
            }
		}

		else if(json_str[i] == ']' && inside_square_bracket == 1)
		{
			arr_end_index = i - 1;

            inside_square_bracket = 0;
		}

        else if (json_str[i] == '}')
        {
            if(inside_braces == 1)
            {
                struct json_obj child_node;

                initialize_json_obj(&child_node);

                const int child_node_result = decode_json_str_to_obj((json_str + obj_start_index), &child_node, 0, 0);

                if(child_node_result == 1)
                {
                    obj_end_index = i - 1;

                    add_attribute(&child_node, out);

                    inside_braces = 0;
                }
                // The end of a json object, so we return 1 to let user know that the function just generated a json object.
                return 1;
            }
            else
            {
                // Detect wrong format of json text, so we return 0 to let user know they used wrong format text.
                printf("The text format is incorrect.\n");

                return 0;
            }


        }
	}

	if(obj_end_index == 0 || inside_braces != 0 || inside_square_bracket != 0 || inside_quotation != 0)
	{
		printf("The json object format is not correct.\n");
		return 0;
	}

	return 1;
}