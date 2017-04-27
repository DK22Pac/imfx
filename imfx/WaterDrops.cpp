#include "WaterDrops.h"
#include "ImVerts.h"
#include "plugin.h"
#include "game_sa\RenderWare.h"
#include "game_sa\CTimer.h"
#include "game_sa\CWeather.h"
#include "game_sa\CGame.h"
#include "game_sa\CCamera.h"
#include "game_sa\common.h"
#include "game_sa\CBoat.h"
#include "game_sa\CGeneral.h"

using namespace plugin;

WaterDrop WaterDrops::m_waterDrops[MAX_WATER_DROPS];
DynamicTxd WaterDrops::m_txd;
RwTexture *WaterDrops::m_pTexture;

void WaterDrop::Render() {
    float x1 = static_cast<float>(x) - static_cast<float>(size) / 2.0f;
    float y1 = static_cast<float>(y) - static_cast<float>(size) / 2.0f;
    float x2 = static_cast<float>(x) + static_cast<float>(size) / 2.0f;
    float y2 = static_cast<float>(y) + static_cast<float>(size) / 2.0f;
    unsigned char alpha = CTimer::m_snTimeInMilliseconds > time + WATER_DROPS_TIME ? (CTimer::m_snTimeInMilliseconds - (time + WATER_DROPS_TIME)) * -(255.0f / (float)WATER_DROPS_FADE_TIME) : 255;
    ImVerts::AddVertex(0, x1, y1, 0.0f, 3.33f, 255, 255, 255, alpha, 0.0f, 0.0f);
    ImVerts::AddVertex(1, x2, y1, 0.0f, 3.33f, 255, 255, 255, alpha, 1.0f, 0.0f);
    ImVerts::AddVertex(2, x2, y2, 0.0f, 3.33f, 255, 255, 255, alpha, 1.0f, 1.0f);
    ImVerts::AddVertex(3, x1, y2, 0.0f, 3.33f, 255, 255, 255, alpha, 0.0f, 1.0f);
    RwIm2DRenderPrimitive(rwPRIMTYPETRIFAN, ImVerts::m_Buffer, 4);
}

void WaterDrops::Setup() {
    m_txd.Init(PLUGIN_PATH("imfx\\screenwater.txd"));
    m_pTexture = nullptr;
}

void WaterDrops::ProcessPerFrame() {
    m_txd.ProcessPerFrame();
}

void WaterDrops::Shutdown() {
    m_txd.Unload();
}

void WaterDrops::Clear() {
    for (int i = 0; i < MAX_WATER_DROPS; i++)
        WaterDrops::m_waterDrops[i].active = false;
}

void WaterDrops::Update() {
    if (CGeneral::GetRandomNumberInRange(0, 3) == 2 && CWeather::Rain > 0.0f && !CallAndReturn<unsigned char, 0x72DDC0>() && CWeather::UnderWaterness <= 0.0f && !CGame::currArea) {
        CVector camTarget = TheCamera.m_aCams[TheCamera.m_nActiveCam].m_vSource + TheCamera.m_aCams[TheCamera.m_nActiveCam].m_vFront;
        if (TheCamera.m_aCams[TheCamera.m_nActiveCam].m_vSource.z < camTarget.z) {
            float dx = camTarget.x - TheCamera.m_aCams[TheCamera.m_nActiveCam].m_vSource.x;
            float dy = camTarget.y - TheCamera.m_aCams[TheCamera.m_nActiveCam].m_vSource.y;
            if (dx * dx + dy * dy < CAM_MAX_DISTANCE_SQUARED)
                WaterDrops::AddWaterDrop();
        }
    }
    if (CGeneral::GetRandomNumberInRange(0, 3) == 2 && FindPlayerPed(0) && CWeather::UnderWaterness <= 0.0f) {
        CVehicle *playerVehicle = FindPlayerVehicle(0, false);
        if (playerVehicle) {
            CMatrix *m = playerVehicle->m_matrix;
            if (m && TheCamera.m_aCams[TheCamera.m_nActiveCam].m_vFront.x * m->up.x + TheCamera.m_aCams[TheCamera.m_nActiveCam].m_vFront.y * m->up.y > 0.0f) {
                for (int i = 0; i < 4; ++i) {
                    if (playerVehicle == CBoat::apFrameWakeGeneratingBoats[i])
                        WaterDrops::AddWaterDrop();
                }
            }
        }
    }
    for (int i = 0; i < MAX_WATER_DROPS; i++) {
        if (WaterDrops::m_waterDrops[i].active) {
            if (CTimer::m_snTimeInMilliseconds - WaterDrops::m_waterDrops[i].time > WATER_DROPS_TIME + WATER_DROPS_FADE_TIME)
                WaterDrops::m_waterDrops[i].active = false;
        }
    }
}

void WaterDrops::AddWaterDrop() {
    for (unsigned int i = 0; i < MAX_WATER_DROPS; i++) {
        if (!WaterDrops::m_waterDrops[i].active) {
            WaterDrops::m_waterDrops[i].active = true;
            WaterDrops::m_waterDrops[i].time = CTimer::m_snTimeInMilliseconds;
            WaterDrops::m_waterDrops[i].size = rand() % 80 + 120;
            WaterDrops::m_waterDrops[i].x = rand() % (static_cast<int>(SCREEN_WIDTH) - 200) + 100;
            WaterDrops::m_waterDrops[i].y = rand() % (static_cast<int>(SCREEN_HEIGHT) - 200) + 100;
            break;
        }
    }
}

void WaterDrops::Render() {
    bool active = false;
    for (unsigned int i = 0; i < MAX_WATER_DROPS; i++) {
        if (WaterDrops::m_waterDrops[i].active) {
            active = true;
            break;
        }
    }
    if (active) {
        if (!m_txd.Validate()) {
            m_txd.Load();
            m_pTexture = m_txd.GetTexture("screenwater");
        }
        RwRenderStateSet(rwRENDERSTATETEXTURERASTER, WaterDrops::m_pTexture->raster);
        RwRenderStateSet(rwRENDERSTATESRCBLEND, reinterpret_cast<void *>(rwBLENDSRCALPHA));
        RwRenderStateSet(rwRENDERSTATEDESTBLEND, reinterpret_cast<void *>(rwBLENDINVSRCALPHA));
        RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, reinterpret_cast<void *>(TRUE));
        for (unsigned int i = 0; i < MAX_WATER_DROPS; i++) {
            if (WaterDrops::m_waterDrops[i].active)
                WaterDrops::m_waterDrops[i].Render();
        }
        RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, FALSE);
    }
}