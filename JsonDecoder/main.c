#pragma once 
#include <stdio.h>

#include "DMJsonEncoder.h"
#include "DMJsonEncoder.h"

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

	struct json_obj testObj_0;
	struct json_obj testObj_1;
	struct json_obj testObj_2;
	struct json_obj testObj_3;

	struct json_obj testObj_4;

	struct json_obj testObj_5;
	
	create_bool("TestBool", 1, &testObj_0);
	create_int("TestInt", 55, &testObj_1);
	create_null("TestNull", &testObj_2);
	create_str("TestStr", "Kind Annoying", &testObj_3);

	/*add_next(&testObj_1, &testObj_0);
	add_next(&testObj_2, &testObj_0);
	add_next(&testObj_3, &testObj_0);*/
	struct json_obj* testObjArr = (struct json_obj*)malloc(4 * sizeof(struct json_obj));

	if (testObjArr != NULL) 
	{
		testObjArr[0] = testObj_0;
		testObjArr[1] = testObj_1;
		testObjArr[2] = testObj_2;
		testObjArr[3] = testObj_3;

	}

	create_json_arr("JsonTestArray", testObjArr, 4, &testObj_4);

	create_float("TestFloat", 112.123231111f, &testObj_5);

	add_next(&testObj_5, &testObj_4);

	int len = get_str_size_of_json(&testObj_4);

	char* testStr = (char*)malloc(len * sizeof(char));
	if (testStr != NULL) 
	{
		*testStr = 0;
		printf("Total String length is %d\n", len);
		generate_json_str(&testObj_4, testStr);
		printf("The actual length of string is %d\n", (int)strlen(testStr));
		if (testStr != NULL) { printf("Current string£º%s\n", testStr); }
		print_json(testStr);

		free(testStr);
	}
	clear_json(&testObj_4);
	


	return 0;
}