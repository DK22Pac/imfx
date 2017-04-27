#pragma once
#include "DynamicTxd.h"
#include "game_sa\RenderWare.h"
#include "game_sa\CVector.h"

class Moonphases {
public:
    static DynamicTxd m_txd;
	static RwTexture *m_pMoonTexture;
    static float m_fMoonSize;
    static RwRGBAReal m_moonColor;
    static unsigned char m_nMoonBlendSrc;
    static unsigned char m_nMoonBlendDst;

    static void Setup();
    static void DrawExecutor();
    static void Draw(CVector *pos, unsigned char color);
	static void Shutdown();
	static void ProcessPerFrame();
};