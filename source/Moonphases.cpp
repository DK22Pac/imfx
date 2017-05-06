#include "Moonphases.h"
#include "imfx.h"
#include "ImVerts.h"
#include "plugin.h"
#include "game_sa\CCoronas.h"
#include "game_sa\CDraw.h"
#include "game_sa\CClock.h"

using namespace plugin;

DynamicTxd Moonphases::m_txd;
RwTexture *Moonphases::m_pMoonTexture;
float Moonphases::m_fMoonSize;
RwRGBAReal Moonphases::m_moonColor;
unsigned char Moonphases::m_nMoonBlendSrc;
unsigned char Moonphases::m_nMoonBlendDst;

void Moonphases::Setup() {
    patch::RedirectJump(0x713ACB, DrawExecutor); // CClouds::Render
    m_txd.Init(/*PLUGIN_PATH*/("imfx\\moon.txd"));
    m_pMoonTexture = nullptr;
    config_file config(PLUGIN_PATH("imfx\\moonphases.dat"));
    m_fMoonSize = config["MOON_SIZE"].asFloat(2.0f);
    CRect rect = config["MOON_COLOR"].asRect(CRect(1.0f, 1.0f, 0.85f, 1.0f));
    m_moonColor.red = rect.m_fLeft;
    m_moonColor.green = rect.m_fBottom;
    m_moonColor.blue = rect.m_fRight;
    m_moonColor.alpha = rect.m_fTop;
    m_nMoonBlendSrc = config["MOON_SRCBLEND"].asInt(7);
    m_nMoonBlendDst = config["MOON_DSTBLEND"].asInt(2);
}

void Moonphases::Shutdown() {
    m_txd.Unload();
}

void Moonphases::ProcessPerFrame() {
    m_txd.ProcessPerFrame();
}

void Moonphases::Draw(CVector *pos, unsigned char color) {
    RwRenderStateSet(rwRENDERSTATESRCBLEND, reinterpret_cast<void *>(rwBLENDDESTALPHA));
    RwRenderStateSet(rwRENDERSTATEDESTBLEND, reinterpret_cast<void *>(rwBLENDONE));
    RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, FALSE);
    if (!m_txd.Validate()) {
        m_txd.Load();
        m_pMoonTexture = m_txd.GetTexture("moon");
    }
    RwRenderStateSet(rwRENDERSTATETEXTURERASTER, m_pMoonTexture->raster);
    float h = m_fMoonSize * 10.0f * (static_cast<float>(CCoronas::MoonSize) + static_cast<float>(CCoronas::MoonSize) + 4.0f) / 2.0f;
    float x1 = pos->x - h;
    float y1 = pos->y - h;
    float x2 = pos->x + h;
    float y2 = pos->y + h;
    float rhw = 1.0f / CDraw::ms_fFarClipZ;
    unsigned char r = color * m_moonColor.red;
    unsigned char g = color * m_moonColor.green;
    unsigned char b = color * m_moonColor.blue;
    unsigned char a = color * m_moonColor.alpha;
    unsigned char id = static_cast<unsigned char>(static_cast<float>(CClock::ms_nGameClockDays - 1) /
        static_cast<float>(CClock::daysInMonth[CClock::ms_nGameClockMonth - 1] - 1) * 23.0f);
    float z = (RwEngineInstance->dOpenDevice.zBufferFar - RwEngineInstance->dOpenDevice.zBufferNear) * (CDraw::ms_fFarClipZ - CDraw::ms_fNearClipZ) *
        CDraw::ms_fFarClipZ / ((CDraw::ms_fFarClipZ - CDraw::ms_fNearClipZ) * CDraw::ms_fFarClipZ);
    float v = id / 4;
    float u = (id - (v * 4)) * 0.25f;
    v *= 0.1666666666666667f;
    ImVerts::AddVertex(0, x1, y1, z, rhw, r, g, b, a, u, v);
    ImVerts::AddVertex(1, x2, y1, z, rhw, r, g, b, a, u + 0.25f, v);
    ImVerts::AddVertex(2, x2, y2, z, rhw, r, g, b, a, u + 0.25f, v + 0.1666666666666667f);
    ImVerts::AddVertex(3, x1, y2, z, rhw, r, g, b, a, u, v + 0.1666666666666667f);
    RwIm2DRenderPrimitive(rwPRIMTYPETRIFAN, ImVerts::m_Buffer, 4);
    RwRenderStateSet(rwRENDERSTATESRCBLEND, reinterpret_cast<void *>(rwBLENDONE));
    RwRenderStateSet(rwRENDERSTATEDESTBLEND, reinterpret_cast<void *>(rwBLENDONE));
}

void __declspec(naked) Moonphases::DrawExecutor() {
    __asm {
        push esi
        lea  eax, [esp + 0x28]
        push eax
        call Draw
        add  esp, 8
        mov  eax, 0x713D24
        jmp  eax
    }
}