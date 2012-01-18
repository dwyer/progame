#ifndef SCRIPT_H
#define SCRIPT_H

typedef struct Script Script;

Script *Script_init(void);
void Script_free(Script *script);

int Script_call(Script *script, int ref);
int Script_run(Script *script, const char *filename);
void Script_unref(Script *script, int ref);

#endif
