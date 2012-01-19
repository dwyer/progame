#ifndef SCRIPT_H
#define SCRIPT_H

int Script_init(void);
void Script_quit(void);

int Script_call(int ref);
int Script_run(const char *filename);
void Script_unref(int ref);

#endif
