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

#define G_Fields 2 		/* General fields, as in
						 * the PrgConfig Struct */
#define G_WalkSpeed 0
#define G_RunSpeed  1

typedef struct{
	float WalkSpeed;
	float RunSpeed;
} PrgConfig;

int LoadConfig(PrgConfig* Conf);
int ReadInt(char* context);
int Load_Weapons();

extern struct WeaponList* Weapons;

#endif
