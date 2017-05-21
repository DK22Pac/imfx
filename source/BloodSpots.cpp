#include "BloodSpots.h"
#include "plugin.h"
#include "game_sa\CFileLoader.h"
#include "game_sa\CShadows.h"
#include "game_sa\CKeyGen.h"

using namespace plugin;

RwTexDictionary *BloodSpots::m_pTxd = nullptr;
RwTexture *BloodSpots::m_pBloodPool = nullptr;
RwTexture *BloodSpots::m_pBloodNormal = nullptr;
RwTexture *BloodSpots::m_pBloodSmall = nullptr;
RwTexture *BloodSpots::m_pBloodBleed = nullptr;
RwTexture *BloodSpots::m_pBloodFootStep1[2] = { nullptr, nullptr };
RwTexture *BloodSpots::m_pBloodFootStep2[2] = { nullptr, nullptr };
RwTexture *BloodSpots::m_pBloodFootStep3[2] = { nullptr, nullptr };
RwTexture *BloodSpots::m_pBloodFootStep4[2] = { nullptr, nullptr };
bool BloodSpots::m_bLeftFootStep = false;
CPed *BloodSpots::m_pPedForFootStep = nullptr;

void BloodSpots::Setup() {
    m_pTxd = CFileLoader::LoadTexDictionary(PLUGIN_PATH("imfx\\bloodspots.txd"));
    if (m_pTxd) {
        m_pBloodPool = RwTexDictionaryFindNamedTexture(m_pTxd, "bloodpool");
        m_pBloodNormal = RwTexDictionaryFindNamedTexture(m_pTxd, "bloodnormal");
        m_pBloodSmall = RwTexDictionaryFindNamedTexture(m_pTxd, "bloodsmall");
        m_pBloodBleed = RwTexDictionaryFindNamedTexture(m_pTxd, "bloodbleed");
        m_pBloodFootStep1[0] = RwTexDictionaryFindNamedTexture(m_pTxd, "bloodfootstep1_l");
        m_pBloodFootStep2[0] = RwTexDictionaryFindNamedTexture(m_pTxd, "bloodfootstep2_l");
        m_pBloodFootStep3[0] = RwTexDictionaryFindNamedTexture(m_pTxd, "bloodfootstep3_l");
        m_pBloodFootStep4[0] = RwTexDictionaryFindNamedTexture(m_pTxd, "bloodfootstep4_l");
        m_pBloodFootStep1[1] = RwTexDictionaryFindNamedTexture(m_pTxd, "bloodfootstep1_r");
        m_pBloodFootStep2[1] = RwTexDictionaryFindNamedTexture(m_pTxd, "bloodfootstep2_r");
        m_pBloodFootStep3[1] = RwTexDictionaryFindNamedTexture(m_pTxd, "bloodfootstep3_r");
        m_pBloodFootStep4[1] = RwTexDictionaryFindNamedTexture(m_pTxd, "bloodfootstep4_r");
    }
    // change blood color
    patch::RedirectCall(0x630EA2, AddBloodShadow1);
    patch::RedirectCall(0x630E63, AddBloodShadow2);
    patch::RedirectCall(0x49EB86, MyConstructBloodFxInfo);
    patch::SetUChar(0x49ED79, 255);
    patch::SetUChar(0x49ED7B, 255);
    patch::SetUInt(0x49ED7D, 255);
    if (m_pBloodPool) {
        patch::SetPointer(0x630E32, m_pBloodPool);
        patch::SetPointer(0x630E74, m_pBloodPool);
        patch::SetPointer(0x679ED0, m_pBloodPool);
        patch::SetPointer(0x679FAA, m_pBloodPool);
        patch::SetPointer(0x707B0B, m_pBloodPool);
    }

    // small
    patch::SetUChar(0x49EDD2, 255);
    patch::SetUChar(0x49EDD4, 255);
    patch::SetUInt(0x49EDD6, 255);
    if (m_pBloodSmall)
        patch::SetPointer(0x49ED6F, m_pBloodSmall);

    // normal
    patch::SetUChar(0x5E5476, 255);
    patch::SetUChar(0x5E5478, 255);
    patch::SetUInt(0x5E5480, 255);
    if (m_pBloodNormal)
        patch::SetPointer(0x49EDBD, m_pBloodNormal);

    // footstep
    patch::SetUChar(0x5E9447, 255);
    patch::SetUChar(0x5E9449, 255);
    patch::SetUInt(0x5E944B, 255);
    if (m_pBloodFootStep1[0] && m_pBloodFootStep2[0] && m_pBloodFootStep3[0] && m_pBloodFootStep4[0]
        && m_pBloodFootStep1[1] && m_pBloodFootStep2[1] && m_pBloodFootStep3[1] && m_pBloodFootStep4[1])
    {
        patch::RedirectCall(0x5E5E64, MyDoPedFootLanded);
        patch::RedirectCall(0x5E5EB5, MyDoPedFootLanded);
        patch::RedirectCall(0x5E54C1, DoFootStepShadow);
    }

    // bleed
    if (m_pBloodBleed)
        patch::SetPointer(0x5E9457, m_pBloodBleed);
}

void BloodSpots::Shutdown() {
    if (m_pTxd)
        RwTexDictionaryDestroy(m_pTxd);
}

void __fastcall BloodSpots::MyDoPedFootLanded(CPed *ped, int, bool bLeftFoot, unsigned char arg3) {
    m_bLeftFootStep = bLeftFoot;
    m_pPedForFootStep = ped;
    ped->DoFootLanded(bLeftFoot, arg3);
}

void BloodSpots::DoFootStepShadow(unsigned char type, RwTexture* texture, CVector* posn, float topX, float topY, float rightX, float rightY, short intensity, unsigned char red, unsigned char greeb, unsigned char blue, float drawDistance, unsigned int time, float upDistance) {
    RwTexture *myTexture = texture;
    if (m_pPedForFootStep) {
        unsigned int texNum = m_bLeftFootStep ? 0 : 1;
        if (m_pPedForFootStep->m_wModelIndex == MODEL_NULL && m_pPedForFootStep->m_pPlayerData) {
            unsigned int key = m_pPedForFootStep->m_pPlayerData->m_pClothes->m_adwTextureKeys[3];
            if (!key || key == CKeyGen::GetUppercaseKey("FOOT"))
                myTexture = m_pBloodFootStep4[texNum];
            else if(key == CKeyGen::GetUppercaseKey("SANDAL") || key == CKeyGen::GetUppercaseKey("SANDALSOCK"))
                myTexture = m_pBloodFootStep1[texNum];
            else
                myTexture = m_pBloodFootStep2[texNum];
        }
        else {
            switch (m_pPedForFootStep->m_wModelIndex) {
            case MODEL_BFOST:
            case MODEL_HFOST:
            case MODEL_HFYST:
            case MODEL_WMYMECH:
            case MODEL_OFOST:
            case MODEL_OFYST:
            case MODEL_WMOPJ:
            case MODEL_SWMOTR1:
            case MODEL_WMOTR1:
            case MODEL_BMOTR1:
            case MODEL_BALLAS1:
            case MODEL_LSV3:
            case MODEL_VLA3:
            case MODEL_SBMYTR3:
            case MODEL_SWMOTR2:
            case MODEL_SWMOTR3:
            case MODEL_WFYBE:
            case MODEL_BFYBE:
            case MODEL_HFYBE:
            case MODEL_WFYCRK:
            case MODEL_HMYCM:
            case MODEL_DWFYLC1:
            case MODEL_WMYBE:
            case MODEL_CWFYHB:
            case MODEL_CWMOHB1:
                myTexture = m_pBloodFootStep1[texNum];
                break;
            case MODEL_BFYRI:
            case MODEL_HFYRI:
            case MODEL_OFYRI:
            case MODEL_BFYPRO:
            case MODEL_HFYPRO:
            case MODEL_WFYSTEW:
            case MODEL_WFYPRO:
            case MODEL_WFYRI:
            case MODEL_DNFYLC:
            case MODEL_SOFYBU:
            case MODEL_WFYBU:
            case MODEL_SOFYRI:
            case MODEL_WFYSEX:
            case MODEL_VHFYPRO:
            case MODEL_VWFYWAI:
            case MODEL_SBFORI:
            case MODEL_SWFYRI:
            case MODEL_SBFYRI:
            case MODEL_SOFORI:
            case MODEL_SWFYST:
            case MODEL_SHFYPRO:
            case MODEL_SBFYPRO:
            case MODEL_SFYPRO:
            case MODEL_VBFYST2:
            case MODEL_VBFYPRO:
            case MODEL_VHFYST3:
            case MODEL_SBFYSTR:
            case MODEL_SWFYSTR:
            case MODEL_VWFYWA2:
                myTexture = m_pBloodFootStep3[texNum];
                break;
            case MODEL_BMYBE:
            case MODEL_HMYBE:
            case MODEL_WMYLG:
            case MODEL_CWMYHB1:
            case MODEL_WFYLG:
            case MODEL_WMYVA2:
                myTexture = m_pBloodFootStep4[texNum];
                break;
            case MODEL_WFYRO:
            case MODEL_WMYRO:
                myTexture = 0;
            default:
                myTexture = m_pBloodFootStep2[texNum];
            }
        }
    }
    if (myTexture)
        CShadows::AddPermanentShadow(type, myTexture, posn, topX, topY, rightX, rightY, intensity, 49, 17, 16, drawDistance, time, upDistance);
}

void BloodSpots::AddBloodShadow1(unsigned char type, RwTexture* texture, CVector* posn, float topX, float topY, float rightX, float rightY, short intensity, unsigned char red, unsigned char greeb, unsigned char blue, float drawDistance, unsigned int time, float upDistance) {
    CShadows::AddPermanentShadow(type, texture, posn, topX, topY, rightX, rightY, intensity, 255, 255, 255, drawDistance, time, upDistance);
}

void BloodSpots::AddBloodShadow2(unsigned int id, unsigned char type, RwTexture* texture, CVector* posn, float topX, float topY, float rightX, float rightY, short intensity, unsigned char red, unsigned char greeb, unsigned char blue, float zDistance, float scale, float drawDistance, bool temporaryShadow, float upDistance) {
    CShadows::StoreStaticShadow(id, type, texture, posn, topX, topY, rightX, rightY, intensity, 255, 255, 255, zDistance, scale, drawDistance, temporaryShadow, upDistance);
}

FxPrtMult_c * __fastcall BloodSpots::MyConstructBloodFxInfo(FxPrtMult_c *_this, int, float red, float green, float blue, float alpha, float size, float arg5, float lastFactor) {
    CallMethod<0x4AB290>(_this, 1.0f, 1.0f, 1.0f, 1.0f, size, arg5, lastFactor);
    return _this;
}