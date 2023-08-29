#include "JsonEncoder.h"

void initialize_json_obj(struct JsonObj* out) 
{
	out->jsonType = 0;
	out->objKey = NULL;
	out->intValue = 0;
	out->floatValue = 0;
	out->strValue = NULL;
	out->child = NULL;
	out->prev = NULL;
	out->next = NULL;
	out->parent = NULL;
}

void create_int(const char* key,int value, struct JsonObj* out) 
{
	initialize_json_obj(out);
	out->objKey = (char*)malloc(sizeof(char) * (strlen(key) + 1));
	if (out->objKey != NULL)
	{
		strcpy(out->objKey, key);
	}

	out->intValue = value;
	out->jsonType = JsonType_Int;
}
void create_float(const char* key, const float value, struct JsonObj* out) 
{
	initialize_json_obj(out);
	out->objKey = (char*)malloc(sizeof(char) * (strlen(key) + 1));
	if (out->objKey != NULL)
	{
		*(out->objKey) = '\0';
		strcpy(out->objKey, key);
	}
	out->jsonType = JsonType_Float;
	out->floatValue = value;
}
void create_null(const char* key,struct JsonObj* out) 
{
	initialize_json_obj(out);
	out->objKey = (char*)malloc(sizeof(char) * (strlen(key) + 1));
	if (out->objKey != NULL)
	{
		strcpy(out->objKey, key);
	}
	out->jsonType = JsonType_Null;
	out->strValue = (char*)malloc(sizeof(char) * (strlen(Json_NULL) + 1));
	if (out->strValue != NULL) 
	{
		strcpy(out->strValue, Json_NULL);
	}
}
void create_bool(const char* key, uint8_t boolValue, struct JsonObj* out)
{
	initialize_json_obj(out);
	out->objKey = (char*)malloc(sizeof(char) * (strlen(key) + 1));
	if (out->objKey != NULL)
	{
		strcpy(out->objKey, key);
	};
	out->jsonType = JsonType_Bool;
	if (boolValue > 0)
	{
		out->strValue = Json_Bool_True;
		out->strValue = (char*)malloc(sizeof(char) * (strlen(Json_Bool_True)+1));
		if (out->strValue != NULL)
		{
			strcpy(out->strValue, Json_Bool_True);
		}
	}
	else 
	{
		out->strValue = (char*)malloc(sizeof(char) * (strlen(Json_Bool_False)+1));
		if (out->strValue != NULL)
		{
			strcpy(out->strValue, Json_Bool_False);
		}
	}
}
void create_str(const char* key, char* value, struct JsonObj* out) 
{
	initialize_json_obj(out);
	out->objKey = (char*)malloc(sizeof(char) * (strlen(key)+1));
	if (out->objKey != NULL)
	{
		strcpy(out->objKey, key);
	}
	out->jsonType = JsonType_Str;
	out->strValue = (char*)malloc(sizeof(char) * (strlen(value)+1));
	if (out->strValue != NULL)
	{
		strcpy(out->strValue, value);
	}
	
}

void create_json_arr(const char* key, struct JsonObj *arr, int arrLen, struct JsonObj* out)
{
	initialize_json_obj(out);
	out->objKey = (char*)malloc(sizeof(char) * (strlen(key) + 1));
	if (out->objKey != NULL)
	{
		strcpy(out->objKey, key);
	}
	out->jsonType = JsonType_Arr;
	out->intValue = arrLen;
	out->child = &arr[0];

	for (int i = 0; i < arrLen; ++i) 
	{
		arr[i].parent = out;
		if (i < arrLen - 1) 
		{
			arr[i].next = &arr[i + 1];
		}
		if (i > 0) 
		{
			arr[i].prev = &arr[i - 1];
		}
	}

}

void create_json_obj(const char* key, struct JsonObj* out)
{
	initialize_json_obj(out);
	out->objKey = (char*)malloc(sizeof(char) * (strlen(key) + 1));
	if (out->objKey != NULL)
	{
		strcpy(out->objKey, key);
	}
	
	out->jsonType = JsonType_Obj;

}

void add_attribute(struct JsonObj* newAtt, struct JsonObj* out) 
{
	newAtt->parent = out;
	if (out->child == NULL) 
	{
		out->child = newAtt;
	}
	else 
	{
		add_next(newAtt, out->child);
	}
}

void add_next(struct JsonObj* next, struct JsonObj* out) 
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

int get_str_size_of_json(const struct JsonObj* root) 
{
	int len = 0; 
	int next_len = 0;
	
	switch (root->jsonType) 
	{
		case JsonType_Int:
			len = 10;
			break;
		case JsonType_Bool:
			len = 5;
			break;
		case JsonType_Float:
			len = 10;
			break;
		case JsonType_Null:
			len = 4;
			break;
		case JsonType_Str:
			len = (int)strlen(root->strValue);
			break;
		case JsonType_Arr:
			//printf("Called Once \n");
			if (root->child != NULL) 
			{
				len = get_str_size_of_json(root->child) + 3;
			}
			break;
		case JsonType_Obj:
			len = get_str_size_of_json(root->child) + 3;
			break;
	}
	// Quote mark + colon + one space
	len = len + (int)strlen(root->objKey) + 4;

	if (root->next != NULL) 
	{
		//one space for comma
		len += 1;
		printf("The next Json object is %s\n", root->next->objKey);
		next_len = get_str_size_of_json(root->next);
	}

	len = len + next_len;

	if (root->next == NULL && root->child == NULL && root->parent == NULL) 
	{
		len += 2;
	}

	return len;
}

void print_json(char* jsonStr)
{
	int length = (int)strlen(jsonStr);
	int level = 0;
	for (int i = 0; i < length; ++i) 
	{
		
		if (jsonStr[i] == '{' || jsonStr[i] == '[')
		{
			
			if (i != 0)
			{
				printf("\n");
				print_space(level);
				printf("%c\n", jsonStr[i]);
			}
			else 
			{
				printf("%c\n", jsonStr[i]);
			}
			level += 1;
			print_space(level);
			
		}
		else if (jsonStr[i] == ']' || jsonStr[i] == '}')
		{
			printf("\n");
			level -= 1;
			print_space(level);
			printf("%c\n", jsonStr[i]);
			print_space(level);
		}
		else 
		{
			if (jsonStr[i] == ':')
			{
				printf("%c ", jsonStr[i]);
			}
			else if (jsonStr[i] == ',')
			{
				printf("%c\n", jsonStr[i]);
				print_space(level);
			}
			else
			{
				printf("%c", jsonStr[i]);
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

void generate_json_str(struct JsonObj* obj, char* jsonStr) 
{
	if (obj == NULL) return;
	if (obj->prev == NULL && obj->parent == NULL) 
	{
		strcat(jsonStr, "{");
	}
	int keyLength = 0;
	if (obj->objKey == NULL) 
	{
		keyLength = 0;
	}
	else 
	{
		keyLength = (int)strlen(obj->objKey);
	}

	if (obj->jsonType == JsonType_Int) 
	{
		int size = 10 + 4 + keyLength + 2;
		char* temp = (char*)malloc(size * sizeof(char));
		if (temp != NULL) 
		{
			temp[0] = 0;
			sprintf(temp, "\"%s\": %d", obj->objKey, obj->intValue);
			strcat(jsonStr, temp);
			free(temp);                                                    
		}
	}

	if (obj->jsonType == JsonType_Float) 
	{
		int size = 10 + 4 + keyLength + 2;
		char* temp = (char*)malloc(size * sizeof(char));
		if (temp != 0) 
		{
			temp[0] = 0;

			sprintf(temp, "\"%s\": %.6f", obj->objKey, obj->floatValue);
			strcat(jsonStr, temp);
			free(temp);
		}
	}

	if (obj->jsonType == JsonType_Bool) 
	{
		int size = 5 + 4 + keyLength + 2;
		char* temp = (char*)malloc(size * sizeof(char));
		if (temp != 0) 
		{
			*temp = 0;
			sprintf(temp, "\"%s\": %s", obj->objKey, obj->strValue);
			strcat(jsonStr, temp);
			free(temp);
		}
	}

	if (obj->jsonType == JsonType_Str) 
	{
		int size = (int)strlen(obj->strValue) + 4 + keyLength + 3;
		char* temp = (char*)malloc(size * sizeof(char));
		if (temp != 0) 
		{
			*temp = 0;
			sprintf(temp, "\"%s\": \"%s\"", obj->objKey, obj->strValue);
			strcat(jsonStr, temp);
			free(temp);
		}
	}
	if (obj->jsonType == JsonType_Null) 
	{
		int size = 4 + 4 + keyLength + 2;
		char* temp = (char*)malloc(size * sizeof(char));
		if (temp != 0) 
		{
			temp[0] = 0;
			sprintf(temp, "\"%s\": %s", obj->objKey, obj->strValue);
			strcat(jsonStr, temp);
			free(temp);
		}
	}

	if (obj->jsonType == JsonType_Arr)
	{
		int size = keyLength + 4 + 2;
		char* temp = (char*)malloc(size * sizeof(char));
		if (temp != NULL)
		{
			*temp = 0;
			if (obj->objKey[0] != 0)
			{
				sprintf(temp, "\"%s\": [", obj->objKey);
			}
			else
			{
				sprintf(temp, "[");
			}
			strcat(jsonStr, temp);
			free(temp);
		}

		if (obj->child != NULL)
		{
			generate_json_str(obj->child, jsonStr);
		}
		
		strcat(jsonStr, "]");
	}

	if (obj->jsonType == JsonType_Obj)
	{
		int size = keyLength + 4 + 2;
		char* temp = (char*)malloc(size * sizeof(char));
		if (temp != NULL)
		{
			*temp = 0;
			if (obj->objKey[0] != 0)
			{
				sprintf(temp, "\"%s\": {", obj->objKey);
			}
			else
			{
				sprintf(temp, "{");
			}
			strcat(jsonStr, temp);
			free(temp);
		}

		if (obj->child != NULL)
		{
			generate_json_str(obj->child, jsonStr);
		}
		strcat(jsonStr, "}");
	}

	if (obj->next != NULL) 
	{
		
		strcat(jsonStr, ",");
		generate_json_str(obj->next, jsonStr);
	}
	else if(obj->parent == NULL)
	{
		strcat(jsonStr, "}");
	}

}

void clear_json(struct JsonObj* root)
{
	if (root->child)
	{
		clear_json(root->child);
	}
	if (root->next)
	{
		clear_json(root->next);
	}

	if (root->objKey != NULL)
	{
		free(root->objKey);
	}

	if (root->strValue != NULL)
	{
		free(root->strValue);
	}
	initialize_json_obj(root);
}



