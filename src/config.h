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

struct Field{
	char* Name;
	char* Value;
	struct Field* Next;
};

struct Section{
	int FieldNo;
	char* Name;
	struct Field* Fields;
	struct Section* Next;
};

typedef struct{
	struct Section* Global;
	struct Section* Sections;
} Dictionary;

typedef struct{
	int Type;
	void* Value;
	char* Name;
} confField;

int LoadConfig();

Dictionary* LoadIni(char* Path);
int FindField(Dictionary* Dict, char* name);

int GetInt(Dictionary* Dict, char* name, int err);
float Getfloat(Dictionary* Dict, char* name, float err);

struct Section* AddSection(Dictionary* Dict, char* name);
struct Section* GetSection(Dictionary* Dict, char* name);

#endif
