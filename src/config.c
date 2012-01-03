#include <SDL/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "iniparser.h"
#include "config.h"

struct WeaponList *Weapons = NULL;

int LoadConfig(PrgConfig* Conf)
{
	int Status = 0;
	char*       Path = calloc(80, 1);
	dictionary* Dict = NULL;
	confField* Fields = calloc(G_Fields, sizeof(confField));
	int x;
	
	memset(Path, 0, 80);
	strcat(Path, getenv("HOME"));
	strcat(Path, "/.config/progame/progame.conf");
	
	Dict = iniparser_load(Path);
	
	for (x = 0; x < G_Fields; x++){
		LoadValue(Dict, Fields[x]);
	}
	
	return Status;
}

int LoadValue(dictionary* dict, confField* Field){
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
			
			*(int*)Field->Value = iniparser_getint(dict, Field->Name, -9001);
			break;
			
			case C_float:
			if (!Field->Value)
			Field->Value = malloc(sizeof(double));
			
			*(double*)Field->Value = iniparser_getdouble(dict, Field->Name, -9001.7);
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
