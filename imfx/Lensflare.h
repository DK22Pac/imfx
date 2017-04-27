#pragma once
#include "DynamicTxd.h"
#include "game_sa\CRegisteredCorona.h"
#include "game_sa\CRGBA.h"
#include "game_sa\RenderWare.h"
#include "game_sa\CVector.h"
#include <stdio.h>
#include <vector>

struct Halo {
	enum eRotation {
		LENSFLARE_ROTATION_NONE,
		LENSFLARE_ROTATION_IN,
		LENSFLARE_ROTATION_OUT
	};

	float distance;
	float point_rotate;
	float width;
	float height;
	float angle;
	eRotation rotation;
	unsigned char blendSrc;
	unsigned char blendDst;
	CRGBA color;
	char textureName[32];
	RwTexture *pTexture;
};

class Lensflare {
public:
	static void ReadSettings();
	static void DrawExecutor();
	static void ProcessPerFrame();
	static void Draw(CRegisteredCorona *corona, CVector *coronaPos, CVector *coronaScreenPos);
	static void DrawHalo(RwTexture *tex, float x, float y, float w, float h, float angle, unsigned char r, unsigned char g, unsigned char b,
		unsigned char a, unsigned char srcBlend, unsigned char dstBlend);
	static void Setup();
	static void Shutdown();
	static std::vector<Halo> m_halos;
	static DynamicTxd m_txd;
};