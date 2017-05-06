#include "Headshot.h"
#include "game_sa\FxManager_c.h"

void Headshot::DoHeadshot(CPed *ped, int bodyPartToRemove) {
    if (bodyPartToRemove == 2 && ped->m_apBones[bodyPartToRemove]->m_pIFrame) {
        RwV3d position;
        ped->GetBonePosition(position, 0, true);
        FxSystem_c *particle = g_fxMan.CreateFxSystem("headshot", &position, 0, 0);
        if (particle)
            particle->PlayAndKill();
    }
}