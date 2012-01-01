#include <SDL/SDL.h>
#include "motion.h"

int GetFPS(int* CurrentFPS, int* AverageFPS, int StartTime)
{
	*CurrentFPS = SDL_GetTicks() - StartTime;
	
	*AverageFPS += *CurrentFPS;
	if (*AverageFPS != *CurrentFPS)
	*AverageFPS /= 2;

	return 0;
}

/* 
 * Floating point arithmetic may be CPU intensive
 * but this yields great benifits, and scales better than 
 * any fixed speed alternative.
 */
float Interpolate(float Speed, int FPS)
{
	return Speed * FPS;
}
