#pragma once
#include "Lensflare.h"
#include "ImVerts.h"
#include "plugin.h"
#include "game_sa\RenderWare.h"
#include "game_sa\CWorld.h"
#include "game_sa\CWeather.h"
#include "game_sa\CCamera.h"
#include <math.h>

using namespace plugin;

std::vector<Halo> Lensflare::m_halos;
DynamicTxd Lensflare::m_txd;

void Lensflare::Setup() {
    patch::RedirectJump(0x6FB2EE, DrawExecutor); // CCoronas::Render
    // remove fire coronas
    patch::Nop(0x53B7E0, 5);
    patch::Nop(0x53B893, 5);
    patch::Nop(0x53B99C, 5);
    patch::Nop(0x53BAA5, 5);
    ReadSettings();
    m_txd.Init(PLUGIN_PATH("imfx\\lensflare.txd"));
}

void Lensflare::ReadSettings() {
    char line[256];
    char path[256];
    char rotation[4];
    unsigned int r, g, b, a, src, dst;
    FILE *file = fopen(PLUGIN_PATH("imfx\\lensflare.dat"), "r");
    while (!feof(file)) {
        fgets(line, 256, file);
        if (*line != '#') {
            Halo halo;
            sscanf(line, "%f %f %f %f %f %s %d %d %d %d %d %d %s", &halo.distance, &halo.point_rotate,
                &halo.width, &halo.height, &halo.angle, rotation, &r, &g, &b, &a,
                &src, &dst, path);
            halo.color.red = r;
            halo.color.green = g;
            halo.color.blue = b;
            halo.color.alpha = a;
            halo.blendSrc = src;
            halo.blendDst = dst;
            if (*rotation == 'i')
                halo.rotation = Halo::LENSFLARE_ROTATION_IN;
            else if (*rotation == 'o')
                halo.rotation = Halo::LENSFLARE_ROTATION_OUT;
            else
                halo.rotation = Halo::LENSFLARE_ROTATION_NONE;
            strcpy_s(halo.textureName, path);
            halo.pTexture = nullptr;
            m_halos.push_back(halo);
        }
    }
    fclose(file);
}

void Lensflare::Draw(CRegisteredCorona *corona, CVector *coronaPos, CVector *coronaScreenPos) {
    float screenHalfX = SCREEN_COORD_CENTER_X;
    float screenHalfY = SCREEN_COORD_CENTER_Y;
    if (corona->m_nFlareType == 1 && CWeather::NewWeatherType != WEATHER_RAINY_SF
        && CWeather::NewWeatherType != WEATHER_RAINY_COUNTRYSIDE && CWeather::NewWeatherType != WEATHER_CLOUDY_SF
        && CWeather::NewWeatherType != WEATHER_FOGGY_SF && CWeather::NewWeatherType != WEATHER_CLOUDY_COUNTRYSIDE)
    {
        CColPoint colPoint;
        CEntity *entity = nullptr;
        CWorld::pIgnoreEntity = TheCamera.m_pTargetEntity;
        if (!CWorld::ProcessLineOfSight(*coronaPos, TheCamera.GetPosition(), colPoint, entity, false, true, true, false, false, false, false, false)) {
            if (!m_txd.Validate()) {
                m_txd.Load();
                for (Halo &halo : m_halos) {
                    halo.pTexture = m_txd.GetTexture(halo.textureName);
                }
            }
            RwEngineInstance->dOpenDevice.fpRenderStateSet(rwRENDERSTATEZTESTENABLE, FALSE);
            for (Halo &halo : m_halos) {
                float x = (coronaScreenPos->x - screenHalfX) * halo.distance + screenHalfX;
                float y = (coronaScreenPos->y - screenHalfY) * halo.distance + screenHalfY;
                float angle = 0.0f;
                if (halo.point_rotate != 0.0f) {
                    angle = halo.point_rotate / 57.295776f;
                    float xold = x;
                    float yold = y;
                    x = coronaScreenPos->x + (xold - coronaScreenPos->x) * cosf(angle) + (yold - coronaScreenPos->y) * sinf(angle);
                    y = coronaScreenPos->y - (xold - coronaScreenPos->x) * sinf(angle) + (yold - coronaScreenPos->y) * cosf(angle);
                }
                if (halo.rotation) {
                    angle = atan2(y - coronaScreenPos->y, x - coronaScreenPos->x) - 1.570796326794f;
                    if (halo.rotation == Halo::LENSFLARE_ROTATION_IN)
                        angle *= -1.0f;
                }
                angle += halo.angle / 57.295776f;
                DrawHalo(halo.pTexture, x, y, halo.width * 4, halo.height * 4, angle, halo.color.red, halo.color.green,
                    halo.color.blue, halo.color.alpha, halo.blendSrc, halo.blendDst);
            }
            RwRenderStateSet(rwRENDERSTATEZTESTENABLE, reinterpret_cast<void *>(TRUE));
            RwRenderStateSet(rwRENDERSTATETEXTURERASTER, NULL);
            RwRenderStateSet(rwRENDERSTATESRCBLEND, reinterpret_cast<void *>(rwBLENDONE));
            RwRenderStateSet(rwRENDERSTATEDESTBLEND, reinterpret_cast<void *>(rwBLENDONE));
        }
        CWorld::pIgnoreEntity = nullptr;
    }
}

void Lensflare::DrawHalo(RwTexture *tex, float x, float y, float w, float h, float angle, unsigned char r, unsigned char g, unsigned char b,
    unsigned char a, unsigned char srcBlend, unsigned char dstBlend)
{
    float x1 = x - w / 2;
    float y1 = y - h / 2;
    float x2 = x + w / 2;
    float y2 = y + h / 2;
    RwRenderStateSet(rwRENDERSTATETEXTURERASTER, tex->raster);
    RwRenderStateSet(rwRENDERSTATESRCBLEND, reinterpret_cast<void *>(srcBlend));
    RwRenderStateSet(rwRENDERSTATEDESTBLEND, reinterpret_cast<void *>(dstBlend));
    ImVerts::AddVertex(0, x1, y1, 0.0f, 3.33f, r, g, b, a, 0.0f, 0.0f);
    ImVerts::AddVertex(1, x2, y1, 0.0f, 3.33f, r, g, b, a, 1.0f, 0.0f);
    ImVerts::AddVertex(2, x2, y2, 0.0f, 3.33f, r, g, b, a, 1.0f, 1.0f);
    ImVerts::AddVertex(3, x1, y2, 0.0f, 3.33f, r, g, b, a, 0.0f, 1.0f);
    if (angle != 0.0f) {
        for (int i = 0; i < 4; i++) {
            float xold = ImVerts::m_Buffer[i].x;
            float yold = ImVerts::m_Buffer[i].y;
            ImVerts::m_Buffer[i].x = x + (xold - x) * cos(angle) + (yold - y) * sin(angle);
            ImVerts::m_Buffer[i].y = y - (xold - x) * sin(angle) + (yold - y) * cos(angle);
        }
    }
    RwIm2DRenderPrimitive(rwPRIMTYPETRIFAN, ImVerts::m_Buffer, 4);
}

void Lensflare::ProcessPerFrame() {
    m_txd.ProcessPerFrame();
}

void Lensflare::Shutdown() {
    m_txd.Unload();
}

void __declspec(naked) Lensflare::DrawExecutor() {
    __asm {
        lea  eax, [esp + 0x20]
        push eax
        lea  eax, [esp + 0x30]
        push eax
        lea  eax, [edi - 0x38]
        push eax
        call Draw
        add  esp, 0xC
        mov  eax, 0x6FB605
        jmp  eax
    }
}