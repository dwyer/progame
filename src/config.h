/*  *  *   * *  *  *  *   *  *
 *   * MOTHERFUCKING ASTERISKS
 * *  *   *   * * * *  *    * *
 *  *  *   *    *    *   *    */

#ifndef ConfigH
#define ConfigH

#define G_Fields 2

#define C_int    1
#define C_float  2
#define C_string 3

#define KEYS_NO 4

#define KEY_UP    0
#define KEY_DOWN  1
#define KEY_LEFT  2
#define KEY_RIGHT 3

struct Field {
	char *Name;
	char *Value;
	struct Field *Next;
};
typedef struct Field Field;

struct Section{
	int FieldNo;
	char *Name;
	Field *Fields;
	struct Section *Next;
};
typedef struct Section Section;

typedef struct {
	struct Section *Global;
	struct Section *Sections;
} Dictionary;

typedef struct {
	Input input;
} settings;

int LoadConfig();

Dictionary* I_LoadIni(char* Path);
struct Field* I_FindField(Dictionary* Dict, char* name);

int I_GetInt(Dictionary* Dict, char* name, int err);
float I_GetFloat(Dictionary* Dict, char* name, float err);
char* I_GetString(Dictionary* Dict, char* name, char* err);

struct Section* I_AddSection(Dictionary* Dict, char* name);
struct Section* I_GetSection(Dictionary* Dict, char* name);

int GenerateConfig(char* Path);

int I_SetField(Dictionary* Dict, Section* Current, char* Name, char* Val);
Field* NewField(char* Name, char* Value);

Field* I_AddField(Dictionary* Dict, Section* Sect, char* Name);

#endif
