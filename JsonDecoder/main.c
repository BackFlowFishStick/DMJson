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

	
//	struct json_obj testObj_0;
//	struct json_obj testObj_1;
//	struct json_obj testObj_2;
//	struct json_obj testObj_3;
//	struct json_obj testObj_4;
//	struct json_obj testObj_5;
//
//	create_json_obj("", &testObj_0);
//	create_int("Test Int", 123, &testObj_1);
//	create_bool("Test Bool", 1, &testObj_2);
//	create_float("Test Float", 4.25678f, &testObj_3);
//	create_null("Test Null", &testObj_4);
//	create_str("Test String", "This is just a piece of meaningless comment.", &testObj_5);
//
//	add_attribute(&testObj_1, &testObj_0);
//	add_attribute(&testObj_2, &testObj_0);
//	add_attribute(&testObj_3, &testObj_0);
//	add_attribute(&testObj_4, &testObj_0);
//	add_attribute(&testObj_5, &testObj_0);
//
//	int const size = get_str_size_of_json(&testObj_0);
//
//	printf("The estimated size of json string is : %d\n", size);
//
//	char* test_str = (char*)malloc(size * sizeof(char));
//
//	*test_str = 0;
//
//	generate_json_str(&testObj_0, test_str);
//
//	const int actual_size = strlen(test_str);
//
//	printf("The actual size of json string is: %d\n", actual_size);
//
//	print_json(test_str);
//
//	const char* test_digits = "1237891020";
//	int start_index = 3;
//	int end_index = 7;
//
//	char* temp_piece = (char*)malloc(6 * sizeof(char));
//
//  memset(temp_piece, '\0', 6);
//
//	strncpy(temp_piece, test_digits + 3, 5);
//
//	printf("%s\n", temp_piece);
//
//  printf("Str length: %d\n", (int)strlen(temp_piece));



    char* sample_float = "\"Test Float\":  4.256780\0";

    struct json_obj test_parse_float;

    initialize_json_obj(&test_parse_float);

    parse_json_num(sample_float + 14, &test_parse_float);

    parse_json_key(sample_float + 1, &test_parse_float);

    printf("Json type: %d\n", test_parse_float.json_type);

    printf("Json value: %f\n", test_parse_float.float_value);

    printf("Json key: %s\n", test_parse_float.obj_key);
//    int str_len = get_str_size_of_json(&test_parse_root);

//    char* temp_str = (char*)malloc(str_len * sizeof(char));

//    memset(temp_str, '\0', str_len);


//    generate_json_arr_str(&test_parse_root, temp_str);
//
//    //print_json(temp_str);
//
//    printf("%s\n", temp_str);
//
//    free(temp_str);

//    clear_json(&test_parse_root);

    return 0;
}