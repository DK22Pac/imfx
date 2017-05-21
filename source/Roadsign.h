#pragma once
#include "DynamicTxd.h"
#include "game_sa\RenderWare.h"

class Roadsign {
public:
    static DynamicTxd m_txd;
    static RwTexture *m_pTexture;

    static void Setup();
    static void Shutdown();
    static void ProcessPerFrame();
    static RwRaster *MyCreateRoadsignRaster(unsigned int width, unsigned int height, unsigned int depth, unsigned int flags);
    static bool MyGenerateRoadsignRaster(char *roadname, unsigned int numletters, RwRaster *, int, RwRaster *raster);
};