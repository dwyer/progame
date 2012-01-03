#include <SDL/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "config.h"

struct WeaponList *Weapons = NULL;

int LoadConfig()
{
	int Status = 0;
	char*       Path = calloc(80, 1);
	Dictionary* Dict = NULL;
	confField* Fields = calloc(G_Fields, sizeof(confField));
	int x;
	
	memset(Path, 0, 80);
	strcat(Path, getenv("HOME"));
	strcat(Path, "/.config/progame/progame.conf");
	
	Dict = LoadIni(Path);
	
	/*Fields[0].Name = (char*) "general:walkspeed";
	Fields[0].Type = C_float;
	
	for (x = 0; x < G_Fields; x++){
		LoadValue(Dict, &Fields[x]);
	}
	
	printf("Walkspeed = %f\n", *(float*)Fields[0].Value);
	*/
	return Status;
}

/*int LoadValue(dictionary* dict, confField* Field){
	if (!Field)
	return -1;
	
	if (iniparser_find_entry(dict, Field->Name)){
		switch (Field->Type){
			case C_string:
			Field->Value = iniparser_getstring(dict, Field->Name, "lookup failed");
			
			if (strcmp(Field->Value, "lookup failed") == 0){
				
			}
			break;
			
			case C_int:
			if (!Field->Value)
			Field->Value = malloc(sizeof(int));
			
			*(int*)Field->Value = GetInt(dict, Field->Name, -9001);
			break;
			
			case C_float:
			if (!Field->Value)
			Field->Value = malloc(sizeof(double));
			
			*(double*)Field->Value = GetFloat(dict, Field->Name, -9001.7);
			break;
			
			default:
			break;
		}
	}
	else {
		return -1;
	}
	
	return 0;
}
*/

Dictionary* LoadIni(char* Path){
	FILE* Ini = NULL;
	int   Counter;
	int   x;
	char  c;
	char* Temp;
	
	Ini = fopen(Path, "r");
	if (!Ini){
		printf("Config error: No such file %s\n", Path);
		return NULL;
	}
	
	Counter = fread(&c, 1, 1, Ini);
	do{
		if (c == '['){
			Temp = calloc(80, sizeof(char));
			
			for (x = 0; Counter == 1; x++){
				Counter = fread(&Temp[x], 1, 1, Ini);
				if (Temp[x] == ']'){
					Temp[x] = 0;
					break;
				}
			}
			
			if (Temp[x] == ']')
			Temp[x] = 0;
			
			printf("Temp: %s\n", Temp);
		}
		
		Counter = fread(&c, 1, 1, Ini);
	} while (Counter == 1);
}

struct Section* AddSection(Dictionary* Dict, char* name){
	struct Section* Current = NULL;
	int x;
	
	if (!Dict || !name)
	return -1;
	
	/* Checking if said section already exists */
	if ((Current = GetSection(Dict, name))){
		return Current;
	}
	
	if (!Dict->Sections){
		Dict->Sections = malloc(sizeof(struct Section));
		Current = Dict->Sections;
	}
	else {
		Current = Dict->Sections;
		while (Current->Next){
			Current = Current->Next;
		}
		
		Current->Next = malloc(sizeof(struct Section));
	}
	
	Current->Name = name;
	
	return Current;
}

struct Section* GetSection(Dictionary* Dict, char* name){
	if (!Dict || !Dict->Sections || !name) return NULL;
	
	struct Section* Current = Dict->Sections;
	
	while (Current){
		if (strcmp(Current->Name, name) == 0)
		break;
		
		Current = Current->Next;
	}
	
	return Current;
}
