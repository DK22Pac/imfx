#include "Headshot.h"
#include "plugin.h"
#include "game_sa\FxManager_c.h"

using namespace plugin;

void Headshot::Setup() {
    patch::RedirectCall(0x4B3A49, DoHeadshot);
}

void __fastcall Headshot::DoHeadshot(CPed *ped, int, unsigned int boneId, int) {
    if (ped->m_apBones[boneId]->m_pIFrame) {
        ped->m_nPedFlags.bRemoveHead = true;
        ped->m_nBodypartToRemove = boneId;
        RwV3d position;
        ped->GetBonePosition(position, 0, true);
        FxSystem_c *particle = g_fxMan.CreateFxSystem("headshot", &position, 0, 0);
        particle->PlayAndKill();
    }
}