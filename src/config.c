#include <SDL/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef __unix
#include <sys/stat.h>
#endif
#include "config.h"

struct WeaponList *Weapons = NULL;

int LoadConfig()
{
	int         Status = 0;
	char        Path[80];
	Dictionary* Dict = NULL;
	confField*  Fields = calloc(G_Fields, sizeof(confField));
	int x;
	
	memset(Path, 0, 80);
	strcat(Path, getenv("HOME"));
	strcat(Path, "/.config/progame/progame.conf");
	
	Dict = I_LoadIni(Path);
	
	if (!Dict){
		GenerateConfig(Path);
	}
	
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

Dictionary* I_LoadIni(char* Path){
	FILE* Ini = NULL;
	int   Counter;
	int   x;
	char  c;
	char* Temp;
	Dictionary* Result = malloc(sizeof(Dictionary));
	memset(Result, 0, sizeof(Dictionary));
	
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
		}
		
		Counter = fread(&c, 1, 1, Ini);
	} while (Counter == 1);
	
	return Result;
}

struct Section* I_AddSection(Dictionary* Dict, char* name){
	struct Section* Current = NULL;
	int x;
	
	if (!Dict || !name)
	return NULL;
	
	/* Checking if said section already exists */
	if ((Current = I_GetSection(Dict, name))){
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
	Current->Next = NULL;
	
	return Current;
}

struct Section* I_GetSection(Dictionary* Dict, char* name){
	struct Section* Current = Dict->Sections;
	if (!Dict || !Dict->Sections || !name) return NULL;
	
	while (Current){
		if (strcmp(Current->Name, name) == 0)
		break;
		
		Current = Current->Next;
	}
	
	return Current;
}

char* I_GetString(Dictionary* Dict, char* name){
	struct Field* var = I_FindField(Dict, name);
	char* value = NULL;
	
	if (var){
		return var->Value;
	}
}

Field* I_FindField(Dictionary* Dict, char* name){
	char     c;
	int      x;
	char*    Sect;
	Field*   Result = NULL;
	Section* Where = NULL;
	int Len = strlen(name);
	
	Sect = malloc(strlen(Sect));
	memset(Sect, 0, 1);
	
	for (x = 0; x < Len && name[x] != ';'; x++) /*I didnt want to realloc*/
	Sect[x] = name[x];
	
	if (strlen(Sect) == 0){
		Where = Dict->Global;
	}
}

int GenerateConfig(char* Path){
	FILE* Ini = NULL;
	
	#ifdef __unix /* Who uses Windows, anyway? */
	if (stat("~/.config/progame", malloc(sizeof(struct stat))) != 0){	
		printf("Dicks\n");
		mkdir("~/.config/progame", 777);
	}
	
	#endif
	
	Ini = fopen(Path, "w");
	if (Ini){
		fputs(
			"[general]\n",
			Ini);
		fclose(Ini);
	}
	
	return 0;
}
