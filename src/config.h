/*  *  *   * *  *  *  *   *
 *   *  *   * MOTHERFUCKING ASTERISKS
 * *  *   *   * * * *  *
 */

#ifndef ConfigH
#define ConfigH

#define G_Fields 2

#define C_int    1
#define C_float  2
#define C_string 3

struct Field {
	char *Name;
	char *Value;
	struct Field *Next;
};
typedef struct Field Field; /* I really don't eant to have to type ``struct
								x'' every goddamned time */
struct Section{
	int FieldNo;
	char *Name;
	struct Field *Fields;
	struct Section *Next;
};
typedef struct Section Section;

typedef struct {
	struct Section *Global;
	struct Section *Sections;
} Dictionary;

typedef struct {
	int Type;
	void *Value;
	char *Name;
} confField;

int LoadConfig();

Dictionary* I_LoadIni(char* Path);
struct Field* I_FindField(Dictionary* Dict, char* name);

int I_GetInt(Dictionary* Dict, char* name, int err);
float I_Getfloat(Dictionary* Dict, char* name, float err);

struct Section* I_AddSection(Dictionary* Dict, char* name);
struct Section* I_GetSection(Dictionary* Dict, char* name);

int GenerateConfig(char* Path);

#endif
