#pragma once
#include "game_sa\RenderWare.h"

class ImVerts {
public:
    static RwIm2DVertex m_Buffer[4];
    static void AddVertex(int id, float x, float y, float z, float rhw, unsigned char r, unsigned char g, unsigned char b, unsigned char a, float u, float v);
};