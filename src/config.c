#include <SDL/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "config.h"

struct WeaponList *Weapons = NULL;

int LoadConfig(PrgConfig * Conf)
{
	FILE *dotConf = NULL;
	int Status = 0;
	int Counter;
	char c;
	char *path = calloc(sizeof(char), 80);

	char *Fields[G_Fields] = {
		"WalkSpeed",
		"RunSpeed"
	};

	if (Conf) {
		strcat(path, getenv("HOME"));
		strcat(path, "/.config/progame/progame.conf");
		dotConf = fopen(path, "r");
		free(path);

		if (dotConf) {


		}
	}

	if (Load_Weapons() != 0)
		return -1;

	return Status;
}

int Load_Weapons()
{
	FILE *wFile = NULL;
	struct WeaponList *Current = NULL;
	char c;
	int x;
	char *path = malloc(80);
	char *Name = NULL;
	int Counter = 0;
	int counter = 0;
	char *Temp = NULL;

	char *Fields[WeaponFields] = {
		"min_AP",
		"max_AP",
		"ID",
		"Sprites"
	};

	memset(path, 0, 80);

	strcat(path, getenv("HOME"));
	strcat(path, "/.config/progame/weapons");

	wFile = fopen(path, "r");

	if (!wFile) {
		printf("Error: Could not open weapons file at %s\n", path);
		return -1;
	}


	Counter = fread(&c, 1, 1, wFile);

	do {
		switch (c) {
		case ' ':
		case '\n':
		case '\t':
			while (Counter == 1 && (c == ' ' || c == '\n' || c == '\t'))
				Counter = fread(&c, 1, 1, wFile);
			continue;
			break;

		case '[':{				/*A name for a weapon */
				if (Current)
					AddWeapon(Current);

				Current = malloc(sizeof(struct WeaponList));
				memset(Current, 0, sizeof(struct WeaponList));

				Temp = calloc(50, sizeof(char));
				memset(Temp, 0, 50);

				Counter = fread(&c, 1, 1, wFile);
				counter = 0;
				while (Counter == 1 && c != ']') {
					Temp[counter] = c;
					Counter = fread(&c, 1, 1, wFile);

					counter += 1;
				}

				Current->Name =
					(char *) realloc(Temp, (counter * sizeof(char)) + 1);

				break;
			}

		default:{
				Temp = calloc(50, sizeof(char));
				memset(Temp, 0, 50);

				Temp[0] = c;
				Counter = fread(&c, 1, 1, wFile);
				counter = 1;

				while (Counter == 1 && c != ' ' && c != '\n'
					   && c != '\t' && c != '=') {
					Temp[counter] = c;
					Counter = fread(&c, 1, 1, wFile);

					counter += 1;
				}

				if (c != '=') {
					while (Counter == 1
						   && (c == ' ' || c == '\n' || c == '\t')) {
						Counter = fread(&c, 1, 1, wFile);
					}
				}
				if (c == '=') {
					if (!Current) {
						printf
							("Field %s defined outside of any weapons. Statement has no effect.\n",
							 Temp);
						while (Counter == 1 && c != '\n')
							Counter = fread(&c, 1, 1, wFile);
						continue;
					}

					x = MatchedField(Fields, Temp);

					if (x == WeaponFields) {
						printf
							("Warning: No field '%s' within weapon; statement has no effect\n",
							 Temp);

						while (Counter == 1 && c == '\n') {
							Counter = fread(&c, 1, 1, wFile);
						}
						continue;
					}


					if (x == w_minAP || x == w_maxAP || x == w_ID) {	/* Numerical fields */
						Temp = malloc(10);
						memset(Temp, 0, 10);

						counter = 0;
						Counter = fread(&Temp[counter], 1, 1, wFile);

						while (Counter
							   && (Temp[counter] == ' '
								   || Temp[counter] == '\t'))
							Counter = fread(&Temp[counter], 1, 1, wFile);

						if (Temp[0] == '\n') {
							Counter = fread(&c, 1, 1, wFile);
							continue;
						}

						counter++;

						do {
							Counter = fread(&Temp[counter], 1, 1, wFile);
							counter++;
						} while (Counter == 1
								 && (Temp[counter] >= '0'
									 && Temp[counter] <= '9'));
						counter = ReadInt(Temp);
						free(Temp);

						switch (x) {
						case w_minAP:
							Current->min_AP = counter;
							break;


						case w_maxAP:
							Current->max_AP = counter;
							break;


						case w_ID:
							Current->ID = counter;
							break;


						default:
							break;
						}
					} else if (x == w_SprSh) {	/* String fields */
						while (Counter == 1
							   && (c == ' ' || c == '\n' || c == '\t')) {
							Counter = fread(&c, 1, 1, wFile);
						}
						if (c == '\n') {
							Counter = fread(&c, 1, 1, wFile);
							continue;
						}

						Temp = calloc(50, sizeof(char));
						memset(Temp, 0, 50);

						Counter = fread(&c, 1, 1, wFile);
						counter = 0;

						while (Counter == 1 && c != '\n') {
							Temp[counter] = c;
							Counter = fread(&c, 1, 1, wFile);

							counter += 1;
						}

						if (Current) {
							Current->Sprites =
								realloc(Temp, counter * sizeof(char) + 1);
						}
					} else {


					}
				} else {
					fseek(wFile, -1, SEEK_CUR);
				}
			}
		}

		Counter = fread(&c, 1, 1, wFile);
	} while (Counter == 1);

	if (Current)
		AddWeapon(Current);

	Current = Weapons;

	while (Current) {
		if (!Current->max_AP)
			Current->max_AP = Current->min_AP + 10;	/* Default min AP */

		if (!Current->Name)
			Current->Name = (char *) "Unnamed weapon";
		/*printf("Weapon '%s'\n",  Current->Name);
		   printf("\tmin_AP = %d\n",  Current->min_AP);
		   printf("\tmax_AP = %d\n",  Current->max_AP);
		   printf("\tID = %d\n",      Current->ID);
		   printf("\tSprites = %s\n", Current->Sprites); */
		Current = Current->Next;
	}

	fclose(wFile);
	free(path);

	return 0;
}

int AddWeapon(struct WeaponList *Sub)
{
	struct WeaponList *Current = NULL;

	if (!Sub)
		return -1;

	if (Weapons) {
		Current = Weapons;

		while (Current->Next)
			Current = Current->Next;

		Current->Next = Sub;
	} else {
		Weapons = Sub;
	}

	return 0;
}

int ReadInt(char *context)
{
	int Value = 0;
	int Offset;
	int Length = strlen(context);

	if (!context) {
		return 0;
	}

	for (Offset = 0; Offset < Length; Offset++) {
		if (context[Offset] >= '0' && context[Offset] <= '9') {

			Value = Value * 10 + (context[Offset] - '0');
		} else {
			return Value;
		}
	}

	return Value;
}

float ReadFloat(char *context)
{
	float Value = ReadInt(context);	/* Reads until '.' */
	int Offset;
	int Length = strlen(context);
	int Divisor = 10;

	if (!context) {
		return 0;
	}

	while (context[Offset] && context[Offset != '.'])
		Offset++;

	for (Offset = 0; Offset < Length; Offset++, Divisor *= 10) {
		if (context[Offset] >= '0' && context[Offset] <= '9') {

			Value += (context[Offset] - 48) / Divisor;
		} else {
			return Value;
		}
	}

	return Value;
}

int MatchedField(char **Fields, char *c)
{
	int x;

	if (!Fields || !c)
		return 9001;

	for (x = 0; Fields[x]; x++) {
		if (strcmp(c, Fields[x]))
			break;
	}

	return x;
}
