/*  *  *   * *  *  *  *   *
 *   *  *   * ASTERISKS  *
 * *  *   *   * * * *  *
 */

#ifndef ConfigH
#define ConfigH

#define G_Fields 2

#define C_int    1
#define C_float  2
#define C_string 3

typedef struct {				/* Main configuration values */

} PrgConfig;

typedef struct {
	int Type;
	void *Value;
	char *Name;
} confField;

#endif
