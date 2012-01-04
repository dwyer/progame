#include <SDL/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef __unix
#include <sys/stat.h>
#endif
#include "input.h"
#include "config.h"

struct WeaponList *Weapons = NULL;

int LoadConfig(settings* Pref){
	int         Status = 0;
	char        Path[80];
	Dictionary* Dict = NULL;
	int         x;
	char*       Errc = "it'soverninethousand";
	char*       Value = NULL;
	char*       Keys[KEYS_NO] = {
		"keys:up",
		"keys:down",
		"keys:left",
		"keys:right"
	};
	int (*callback)(int Action, void* x);

	memset(Path, 0, 80);
	strcat(Path, getenv("HOME"));
	strcat(Path, "/.config/progame/progame.conf");
	
	Dict = I_LoadIni(Path);
	
	if (!Dict){
		GenerateConfig(Path);
		Dict = I_LoadIni(Path);
	}
	
	for (x = 0; x < KEYS_NO; x++){
		Value = I_GetString(Dict, Keys[x], Errc);
		
		if (Value != Errc && strcmp(Errc, Value) != 0){
			
			if (strcmp(Value, "move_up") == 0){
				callback = move_up;
			}
			else if (strcmp(Value, "move_down") == 0){
				callback = move_down;
			}
			else if (strcmp(Value, "move_left") == 0){
				callback = move_left;
			}
			else if (strcmp(Value, "move_right") == 0){
				callback = move_right;
			}
			else {
				printf("Config:%s:No known key action ``%s'' for %s; statement has no effect\n",
					Path, Value, Keys[x]);
			}
			
			switch (x){
				case KEY_UP:
				Pref->input.up.callback = callback;
				break;
				
				case KEY_DOWN:
				Pref->input.down.callback = callback;
				break;
				
				case KEY_LEFT:
				Pref->input.left.callback = callback;
				break;
				
				case KEY_RIGHT:
				Pref->input.right.callback = callback;
				break;
				
				default:
				break;
			}
		}
	}
	
	return Status;
}

Dictionary* I_LoadIni(char* Path){
	FILE* Ini = NULL;
	int   Counter;
	int   x;
	char  c;
	char* Temp;
	char* Name;
	Dictionary* Result = malloc(sizeof(Dictionary));
	Section*    Current = Result->Global;
	memset(Result, 0, sizeof(Dictionary));

	Ini = fopen(Path, "r");
	if (!Ini) {
		printf("Config error: No such file %s\n", Path);
		return NULL;
	}
	
	Result->Global = malloc(sizeof(Section));
	Current = Result->Global;
	Counter = fread(&c, 1, 1, Ini);
	
	do {
		if (c == ' ' || c == '\n' || c == '\t'){ /* Whitespace */
			while (Counter && (c == ' ' || c == '\n' || c == '\t'))
			Counter = fread(&c, 1, 1, Ini);
			
			continue;
		}
		else if (c == ';'){ /* Comments */
			while (Counter && c != '\n')
			Counter = fread(&c, 1, 1, Ini);
			Counter = fread(&c, 1, 1, Ini);
			continue;
		}
		else if (c == '[') {
			Temp = calloc(80, sizeof(char));
			memset(Temp, 0, 80);
			
			for (x = 0; Counter == 1; x++) {
				Counter = fread(&Temp[x], 1, 1, Ini);
				if (Temp[x] == ']') {
					Temp[x] = 0;
					break;
				}
			}

			if (Temp[x] == ']')
			Temp[x] = 0;
			
			Current = I_AddSection(Result, Temp);
		}
		else {
			Temp = calloc(80, sizeof(char));
			memset(Temp, 0, 80);
			
			Temp[0] = c;
			for (x = 1; Counter == 1; x++) {
				Counter = fread(&Temp[x], 1, 1, Ini);
				if (Temp[x] == ' '  ||
					Temp[x] == '\n' ||
					Temp[x] == '\t' ||
					Temp[x] == '=') {
					Temp[x] = 0;
					break;
				}
			}
			
			fseek(Ini, -1, SEEK_CUR);
			Counter = fread(&c, 1, 1, Ini);
			
			while (Counter && (c == ' '  ||c == '\n' || c == '\t'))
			Counter = fread(&c, 1, 1, Ini);
			
			if (c == '='){
				Name = Temp;
				
				Temp = malloc(80);
				
				Counter = fread(Temp, 1, 1, Ini);
				while (Counter && (Temp[0] == ' ' || Temp[0] == '\t')){
					Counter = fread(Temp, 1, 1, Ini);
				}
				
				for (x = 1; Counter && Temp[x - 1] != '\n'; x++){
					Counter = fread(&Temp[x], 1, 1, Ini);
				}
				Temp[x - 1] = 0;
				
				I_SetField(Result, Current, Name, Temp);
			}
			else {
				printf("Config: Syntax Error near '%s' symbol\n", Temp);
			}
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

	if (!Dict->Sections) {
		Dict->Sections = malloc(sizeof(struct Section));
		Current = Dict->Sections;
	} else {
		Current = Dict->Sections;
		while (Current->Next) {
			Current = Current->Next;
		}

		Current->Next = malloc(sizeof(struct Section));
		Current = Current->Next;
	}
	
	Current->Name = name;
	Current->Next = NULL;
	Current->Fields = NULL;
	
	return Current;
}

struct Section* I_GetSection(Dictionary* Dict, char* name){
	struct Section* Current = Dict->Sections;
	if (!Dict || !Dict->Sections || !name) {
		return NULL;
	}
	
	while (Current){
		if (strcmp(Current->Name, name) == 0)
		break;

		Current = Current->Next;
	}

	return Current;
}

char* I_GetString(Dictionary* Dict, char* name, char* err){
	struct Field* var = I_FindField(Dict, name);
	
	if (var){
		return var->Value;
	}
	else {
		return err;
	}
}

int I_GetInt(Dictionary* Dict, char* name, int err){
	int Result = 0;
	int Offset = 0;
	struct Field* var = I_FindField(Dict, name);
	char* String;
	
	if (var){
		String = var->Value;
		
		while (String[Offset] && !(String[Offset] >= '0' && String[Offset] <= '9'))
		Offset++;
		
		while (String[Offset] >= '0' && String[Offset] <= '9'){
			Result = Result * 10 + (String[Offset] - 48);
			Offset++;
		}
		return Result;
	}
	else {
		return err;
	}
}

float I_GetFloat(Dictionary* Dict, char* name, float err){
	float Result = 0;
	int Divisor;
	int Offset = 0;
	struct Field* var = I_FindField(Dict, name);
	char* String;
	
	if (var){
		String = var->Value;
		
		while (String[Offset] && !(String[Offset] >= '0' && String[Offset] <= '9'))
		Offset++;
		
		while (String[Offset] >= '0' && String[Offset] <= '9'){
			Result = Result * 10 + (String[Offset] - 48);
			Offset++;
		}
		
		if (String[Offset] == '.'){
			Divisor = 10;
			Offset++;
			while (String[Offset] >= '0' && String[Offset] <= '9'){
				Result += (float)(String[Offset] - 48) / Divisor;
				
				Offset++;
				Divisor *= 10;
			}
		}
		return Result;
	}
	else {
		return err;
	}
}

Field* I_FindField(Dictionary* Dict, char* name){
	char*    Name = NULL;
	int      x;
	char*    Sect;
	Field*   Result = NULL;
	Section* Where = NULL;
	int      Len;
	
	if (!Dict || !name)
	return NULL;
	
	Len = strlen(name);
	Sect = malloc(Len);
	memset(Sect, 0, Len);
	
	for (x = 0; x < Len && name[x] != ':'; x++)
	Sect[x] = name[x];
	
	if (x == 0){
		Where = Dict->Global;
	}
	else if (Dict->Sections){
		Where = I_GetSection(Dict, Sect);
	}
	if (!Where){
		printf("Couldn't find section '%s'\n", Sect);
		return NULL;
	}
	
	Result = Where->Fields;
	
	Name = malloc(Len - x);
	memset(Name, 0, Len);
	
	Len = x + 1;
	for (x++; name[x]; x++)
	Name[x - Len] = name[x];
	
	while (Result && strcmp(Name, Result->Name) != 0)
	Result = Result->Next;
	
	return Result;
}

int GenerateConfig(char* Path){
	FILE* Ini = NULL;
	
	#ifdef __unix /* Who uses Windows, anyway? */
	if (stat("~/.config/progame", malloc(sizeof(struct stat))) != 0){	
		mkdir("~/.config/progame", 777);
	}
	#endif
	
	Ini = fopen(Path, "w");
	if (Ini){
		fputs("[general]\n", Ini);
		fputs("up = move_up\n", Ini);
		fputs("down = move_down\n", Ini);
		fputs("right = move_right", Ini);
		fputs("left = move_left", Ini);
		fclose(Ini);
	}
	
	return 0;
}

int I_SetField(Dictionary* Dict, Section* Sect, char* Name, char* Val){
	char*  FullName;
	Field* Subject = NULL;
	int    Len;
	
	if (!Dict || !Sect || !Name || !Val)
	return -1;
	
	Len = strlen(Sect->Name);
	FullName = malloc(Len + strlen(Name) + 2);
	memset(FullName, 0, 1);
	
	strcat(FullName, Sect->Name);
	FullName[Len] = ':';
	
	strcat(FullName, Name);
	Subject = I_FindField(Dict, FullName);
	
	if (!Subject)
	Subject = I_AddField(Dict, Sect, Name);
	
	Subject->Value = Val;
	
	return 0;
}

Field* I_AddField(Dictionary* Dict, Section* Sect, char* Name){
	Field* Current = NULL;
	
	if (!Dict || !Sect || !Name)
	return NULL;
	
	if (Sect->Fields){
		Current = Sect->Fields;
		
		while (Current->Next)
		Current = Current->Next;
		
		Current->Next = NewField(Name, NULL);
		return Current->Next;
	}
	else {
		Sect->Fields = NewField(Name, NULL);
		return Sect->Fields;
	}
}

Field* NewField(char* Name, char* Value){
	Field* Result = malloc(sizeof(Field));
	Result->Next = NULL;
	Result->Name = Name;
	Result->Value = Value;
	
	return Result;
}
