#ifndef TMX_H
#define TMX_H

typedef struct {
    char source[32];
    int width;
    int height;
    int depth;
    int tilesetw;
    int tileseth;
    int32_t ***data;
    SDL_Surface **layers;
    SDL_Surface *tileset;
} TMP_Tilemap;

TMP_Tilemap *TMP_LoadTilemap(const char *filename);
void TMP_DrawTilemap(TMP_Tilemap *tilemap, SDL_Surface *surface);
void TMP_FreeTilemap(TMP_Tilemap *tilemap);

#endif
