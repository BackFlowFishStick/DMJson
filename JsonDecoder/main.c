#pragma once 
#include <stdio.h>

#include "DMJsonEncoder.h"
#include "DMJsonDecoder.h"


int main(int argc, char** argv) 
{
	/*FILE* fp;
	char fileName[] = argv[1];
	
	fp = fopen(fileName, "r");

	if (fp == NULL) 
	{
		printf("Error opening failed.\n");
		return 1;
	}



	//fclose(fp);*/

	// Test Json generating

	/* 
	struct json_obj testObj_0;
	struct json_obj testObj_1;
	struct json_obj testObj_2;
	struct json_obj testObj_3;
	struct json_obj testObj_4;
	struct json_obj testObj_5;

	create_json_obj("", &testObj_0);
	create_int("Test Int", 123, &testObj_1);
	create_bool("Test Bool", 1, &testObj_2);
	create_float("Test Float", 4.25678f, &testObj_3);
	create_null("Test Null", &testObj_4);
	create_str("Test String", "This is just a piece of meaningless comment.", &testObj_5);

	add_attribute(&testObj_1, &testObj_0);
	add_attribute(&testObj_2, &testObj_0);
	add_attribute(&testObj_3, &testObj_0);
	add_attribute(&testObj_4, &testObj_0);
	add_attribute(&testObj_5, &testObj_0);

	int const size = get_str_size_of_json(&testObj_0);

	printf("The estimated size of json string is : %d\n", size);

	char* test_str = (char*)malloc(size * sizeof(char));

	*test_str = 0;

	generate_json_str(&testObj_0, test_str);

	const int actual_size = strlen(test_str);

	printf("The actual size of json string is: %d\n", actual_size);

	print_json(test_str);

	const char* test_digits = "1237891020";
	int start_index = 3;
	int end_index = 7;

	char* temp_piece = (char*)malloc(6 * sizeof(char));

	memset(temp_piece, '\0', 6);

	strncpy(temp_piece, test_digits + 3, 5);

	printf("%s\n", temp_piece);

	printf("Str length: %d\n", (int)strlen(temp_piece));

	clear_json(&testObj_0);
	free(test_str);
	*/

	//,\"Test Bool\" : true,\"Test Float\" : 4.256780, \"Test Null\" : Null, \"Test String\" : \"This is just a piece of meaningless comment.\"

	char* temp_json_str = "{\"Test Float\":  123.35, \"Test Int\": 123}";

	printf("%s\n", temp_json_str);

	struct json_obj test_json_root;

	initialize_json_obj(&test_json_root);

	uint8_t result = parse_json_obj(temp_json_str, &test_json_root);

	printf("Result is %d\n", result);

    if(test_json_root.child != NULL)
    {
        printf("root child key: %s\n", test_json_root.child->next->obj_key);
    }

    return 0;
}