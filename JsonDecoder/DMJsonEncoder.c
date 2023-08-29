#include "DMJsonEncoder.h"

void initialize_json_obj(struct json_obj* out) 
{
	out->json_type = 0;
	out->obj_key = NULL;
	out->int_value = 0;
	out->float_value = 0;
	out->str_value = NULL;
	out->child = NULL;
	out->prev = NULL;
	out->next = NULL;
	out->parent = NULL;
}

void create_int(const char* key,int value, struct json_obj* out) 
{
	initialize_json_obj(out);
	out->obj_key = (char*)malloc(sizeof(char) * (strlen(key) + 1));
	if (out->obj_key != NULL)
	{
		strcpy(out->obj_key, key);
	}

	out->int_value = value;
	out->json_type = JSON_TYPE_INT;
}
void create_float(const char* key, const float value, struct json_obj* out) 
{
	initialize_json_obj(out);
	out->obj_key = (char*)malloc(sizeof(char) * (strlen(key) + 1));
	if (out->obj_key != NULL)
	{
		*(out->obj_key) = '\0';
		strcpy(out->obj_key, key);
	}
	out->json_type = JSON_TYPE_FLOAT;
	out->float_value = value;
}
void create_null(const char* key,struct json_obj* out) 
{
	initialize_json_obj(out);
	out->obj_key = (char*)malloc(sizeof(char) * (strlen(key) + 1));
	if (out->obj_key != NULL)
	{
		strcpy(out->obj_key, key);
	}
	out->json_type = JSON_TYPE_NULL;
	out->str_value = (char*)malloc(sizeof(char) * (strlen(JSON_NULL) + 1));
	if (out->str_value != NULL) 
	{
		strcpy(out->str_value, JSON_NULL);
	}
}
void create_bool(const char* key, uint8_t bool_value, struct json_obj* out)
{
	initialize_json_obj(out);
	out->obj_key = (char*)malloc(sizeof(char) * (strlen(key) + 1));
	if (out->obj_key != NULL)
	{
		strcpy(out->obj_key, key);
	};
	out->json_type = JSON_TYPE_BOOL;
	if (bool_value > 0)
	{
		out->str_value = JSON_BOOL_TRUE;
		out->str_value = (char*)malloc(sizeof(char) * (strlen(JSON_BOOL_TRUE)+1));
		if (out->str_value != NULL)
		{
			strcpy(out->str_value, JSON_BOOL_TRUE);
		}
	}
	else 
	{
		out->str_value = (char*)malloc(sizeof(char) * (strlen(JSON_BOOL_FALSE)+1));
		if (out->str_value != NULL)
		{
			strcpy(out->str_value, JSON_BOOL_FALSE);
		}
	}
}
void create_str(const char* key, const char* value, struct json_obj* out) 
{
	initialize_json_obj(out);
	out->obj_key = (char*)malloc(sizeof(char) * (strlen(key)+1));
	if (out->obj_key != NULL)
	{
		strcpy(out->obj_key, key);
	}
	out->json_type = JSON_TYPE_STR;
	out->str_value = (char*)malloc(sizeof(char) * (strlen(value)+1));
	if (out->str_value != NULL)
	{
		strcpy(out->str_value, value);
	}
	
}

void create_json_arr(const char* key, struct json_obj *arr, int arr_len, struct json_obj* out)
{
	initialize_json_obj(out);
	out->obj_key = (char*)malloc(sizeof(char) * (strlen(key) + 1));
	if (out->obj_key != NULL)
	{
		strcpy(out->obj_key, key);
	}
	out->json_type = JSON_TYPE_ARR;
	out->int_value = arr_len;
	out->child = &arr[0];

	for (int i = 0; i < arr_len; ++i) 
	{
		arr[i].parent = out;
		if (i < arr_len - 1) 
		{
			arr[i].next = &arr[i + 1];
		}
		if (i > 0) 
		{
			arr[i].prev = &arr[i - 1];
		}
	}

}

void create_json_obj(const char* key, struct json_obj* out)
{
	initialize_json_obj(out);
	out->obj_key = (char*)malloc(sizeof(char) * (strlen(key) + 1));
	if (out->obj_key != NULL)
	{
		strcpy(out->obj_key, key);
	}
	
	out->json_type = JSON_TYPE_OBJ;

}

void add_attribute(struct json_obj* new_att, struct json_obj* out) 
{
	new_att->parent = out;
	if (out->child == NULL) 
	{
		out->child = new_att;
	}
	else 
	{
		add_next(new_att, out->child);
	}
}

void add_next(struct json_obj* next, struct json_obj* out) 
{
	if (out->next == NULL) 
	{
		out->next = next;
		next->prev = out;
	}
	else 
	{
		add_next(next, out->next);
	}
}

int get_str_size_of_json(const struct json_obj* root) 
{
	int len = 0; 
	int next_len = 0;
	
	switch (root->json_type) 
	{
		case JSON_TYPE_INT:
			len = 11;
			break;
		case JSON_TYPE_BOOL:
			len = 6;
			break;
		case JSON_TYPE_FLOAT:
			len = 10;
			break;
		case JSON_TYPE_NULL:
			len = 5;
			break;
		case JSON_TYPE_STR:
			len = (int)strlen(root->str_value);
			break;
	case JSON_TYPE_ARR:
			if (root->child != NULL) 
			{
				len = get_str_size_of_json(root->child) + 3;
			}
			break;
		case JSON_TYPE_OBJ:
			len = get_str_size_of_json(root->child) + 3;
			break;
	}
	// Quote mark + colon + one space
	len = len + (int)strlen(root->obj_key) + 4;

	if (root->next != NULL) 
	{
		//one space for comma
		len += 1;
		printf("The next Json object is %s\n", root->next->obj_key);
		next_len = get_str_size_of_json(root->next);
	}

	len = len + next_len;

	if (root->next == NULL && root->child == NULL && root->parent == NULL) 
	{
		len += 2;
	}

	return len;
}

void print_json(const char* json_str)
{
	int length = (int)strlen(json_str);
	int level = 0;
	for (int i = 0; i < length; ++i) 
	{
		
		if (json_str[i] == '{' || json_str[i] == '[')
		{
			
			if (i != 0)
			{
				printf("\n");
				print_space(level);
				printf("%c\n", json_str[i]);
			}
			else 
			{
				printf("%c\n", json_str[i]);
			}
			level += 1;
			print_space(level);
			
		}
		else if (json_str[i] == ']' || json_str[i] == '}')
		{
			printf("\n");
			level -= 1;
			print_space(level);
			printf("%c\n", json_str[i]);
			print_space(level);
		}
		else 
		{
			if (json_str[i] == ':')
			{
				printf("%c ", json_str[i]);
			}
			else if (json_str[i] == ',')
			{
				printf("%c\n", json_str[i]);
				print_space(level);
			}
			else
			{
				printf("%c", json_str[i]);
			}
		}
	
		
		
	}
}
void print_space(int level) 
{
	if (level > 0)
	{
		for (int j = 0; j < level; ++j)
		{
			printf("%s", "    ");
		}
	}
}

void generate_json_str(struct json_obj* obj, char* json_str) 
{
	if (obj == NULL) return;
	if (obj->prev == NULL && obj->parent == NULL) 
	{
		strcat(json_str, "{");
	}
	int keyLength = 0;
	if (obj->obj_key == NULL) 
	{
		keyLength = 0;
	}
	else 
	{
		keyLength = (int)strlen(obj->obj_key);
	}

	if (obj->json_type == JSON_TYPE_INT) 
	{
		int size = 10 + 4 + keyLength + 2;
		char* temp = (char*)malloc(size * sizeof(char));
		if (temp != NULL) 
		{
			temp[0] = 0;
			sprintf(temp, "\"%s\": %d", obj->obj_key, obj->int_value);
			strcat(json_str, temp);
			free(temp);                                                    
		}
	}

	if (obj->json_type == JSON_TYPE_FLOAT) 
	{
		int size = 9 + 4 + keyLength + 2;
		char* temp = (char*)malloc(size * sizeof(char));
		if (temp != 0) 
		{
			temp[0] = 0;

			sprintf(temp, "\"%s\": %.6f", obj->obj_key, obj->float_value);
			strcat(json_str, temp);
			free(temp);
		}
	}

	if (obj->json_type == JSON_TYPE_BOOL) 
	{
		int size = 5 + 4 + keyLength + 2;
		char* temp = (char*)malloc(size * sizeof(char));
		if (temp != 0) 
		{
			*temp = 0;
			sprintf(temp, "\"%s\": %s", obj->obj_key, obj->str_value);
			strcat(json_str, temp);
			free(temp);
		}
	}

	if (obj->json_type == JSON_TYPE_STR) 
	{
		int size = (int)strlen(obj->str_value) + 4 + keyLength + 3;
		char* temp = (char*)malloc(size * sizeof(char));
		if (temp != 0) 
		{
			*temp = 0;
			sprintf(temp, "\"%s\": \"%s\"", obj->obj_key, obj->str_value);
			strcat(json_str, temp);
			free(temp);
		}
	}
	if (obj->json_type == JSON_TYPE_NULL) 
	{
		int size = 4 + 4 + keyLength + 2;
		char* temp = (char*)malloc(size * sizeof(char));
		if (temp != 0) 
		{
			temp[0] = 0;
			sprintf(temp, "\"%s\": %s", obj->obj_key, obj->str_value);
			strcat(json_str, temp);
			free(temp);
		}
	}

	if (obj->json_type == JSON_TYPE_ARR)
	{
		int size = keyLength + 4 + 2;
		char* temp = (char*)malloc(size * sizeof(char));
		if (temp != NULL)
		{
			*temp = 0;
			if (obj->obj_key[0] != 0)
			{
				sprintf(temp, "\"%s\": [", obj->obj_key);
			}
			else
			{
				sprintf(temp, "[");
			}
			strcat(json_str, temp);
			free(temp);
		}

		if (obj->child != NULL)
		{
			generate_json_str(obj->child, json_str);
		}
		
		strcat(json_str, "]");
	}

	if (obj->json_type == JSON_TYPE_OBJ)
	{
		int size = keyLength + 4 + 2;
		char* temp = (char*)malloc(size * sizeof(char));
		if (temp != NULL)
		{
			*temp = 0;
			if (obj->obj_key[0] != 0)
			{
				sprintf(temp, "\"%s\": {", obj->obj_key);
			}
			else
			{
				sprintf(temp, "{");
			}
			strcat(json_str, temp);
			free(temp);
		}

		if (obj->child != NULL)
		{
			generate_json_str(obj->child, json_str);
		}
		strcat(json_str, "}");
	}

	if (obj->next != NULL) 
	{
		
		strcat(json_str, ",");
		generate_json_str(obj->next, json_str);
	}
	else if(obj->parent == NULL)
	{
		strcat(json_str, "}");
	}

}

void clear_json(struct json_obj* root)
{
	if (root->child)
	{
		clear_json(root->child);
	}
	if (root->next)
	{
		clear_json(root->next);
	}

	if (root->obj_key != NULL)
	{
		free(root->obj_key);
	}

	if (root->str_value != NULL)
	{
		free(root->str_value);
	}
	initialize_json_obj(root);
}



