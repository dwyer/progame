/*  *  *   *
 *   *  *   *
 * *  *   *   *
 */     

#ifndef ConfigH
#define ConfigH

#define WeaponFields 4

#define w_minAP 0
#define w_maxAP 1
#define w_ID    2
#define w_SprSh 3

struct WeaponList{
	int ID;
	char* Name;
	int max_AP;
	int min_AP;
	SDL_Surface* Sprites;
	struct WeaponList* Next;
};

typedef struct WeaponList Weapon;

int LoadConfig();
int ReadInt(char* context);
int Load_Weapons();

extern struct WeaponList* Weapons;

#endif
