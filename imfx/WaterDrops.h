#pragma once
#include "DynamicTxd.h"
#include "game_sa\RenderWare.h"
#include "game_sa\CVector.h"

const unsigned int MAX_WATER_DROPS = 8;
const unsigned int WATER_DROPS_TIME = 3000;
const unsigned int WATER_DROPS_FADE_TIME = 1000;
const float CAM_MAX_DISTANCE_SQUARED = 0.8f;

class WaterDrop {
public:
	bool active;
	unsigned char size;
	unsigned short x, y;
	int time;
	void Render();
};

class WaterDrops {
public:
	static void AddWaterDrop();
	static void Clear();
	static void Update();
	static void Render();
	static void Setup();
	static void Shutdown();
	static void ProcessPerFrame();
	static WaterDrop m_waterDrops[MAX_WATER_DROPS];
	static DynamicTxd m_txd;
	static RwTexture *m_pTexture;
};