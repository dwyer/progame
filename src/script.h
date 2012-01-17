#ifndef SCRIPT_H
#define SCRIPT_H

typedef struct Script Script;

Script *Script_init(void);
int Script_run(Script *script, const char *filename);
void Script_free(Script *script);

#endif
