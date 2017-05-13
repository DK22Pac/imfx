#pragma once
#include "game_sa\RenderWare.h"
#include "game_sa\CPed.h"

class BloodSpots {
public:
    static RwTexDictionary *m_pTxd;
    static RwTexture *m_pBloodPool;
    static RwTexture *m_pBloodNormal;
    static RwTexture *m_pBloodSmall;
    static RwTexture *m_pBloodBleed;
    static RwTexture *m_pBloodFootStep1[2];
    static RwTexture *m_pBloodFootStep2[2];
    static RwTexture *m_pBloodFootStep3[2];
    static RwTexture *m_pBloodFootStep4[2];
    static bool m_bLeftFootStep;
    static CPed *m_pPedForFootStep;

    static void Setup();
    static void Shutdown();
    static void __fastcall MyDoPedFootLanded(CPed *ped, int, bool bLeftFoot, unsigned char arg3);
    static void DoFootStepShadow(unsigned char type, RwTexture* texture, CVector* posn, float topX, float topY, float rightX, float rightY, short intensity, unsigned char red, unsigned char greeb, unsigned char blue, float drawDistance, unsigned int time, float upDistance);
};