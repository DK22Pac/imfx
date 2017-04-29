#include "Gunflashes.h"
#include "plugin.h"
#include "game_sa\FxManager_c.h"
#include "game_sa\common.h"
#include "game_sa\CWeaponInfo.h"
#include "game_sa\CGeneral.h"
#include "game_sa\CCamera.h"
#include "game_sa\CTimer.h"
#include <fstream>
#include <string>

using namespace plugin;

std::vector<GunflashInfo> Gunflashes::gunflashInfos;
RwMatrix Gunflashes::mLocalParticleMatrix;
bool Gunflashes::bLocalParticleMatrixCopied = false;
bool Gunflashes::bLeftHand;

void Gunflashes::Setup() {
    patch::Nop(0x73306D, 9); // Remove default gunflashes
    patch::Nop(0x7330FF, 9); // Remove default gunflashes
    patch::SetUShort(0x5DF425, 0xE990); // Remove default gunflashes
    patch::SetUChar(0x741353, 0); // Add gunflash for cuntgun
    patch::SetUShort(0x53C1F0, 0xC483); // add esp, 8
    patch::SetUChar(0x53C1F2, 8);
    patch::Nop(0x53C1F3, 2);
    patch::RedirectJump(0x4A0DE0, MyTriggerGunflash);
    patch::RedirectCall(0x742299, DoDriveByGunflash);
    patch::SetPointer(0x86D744, MyProcessUseGunTask);
    ReadSettings();
}

void Gunflashes::UpdateAfterPreRender() {
    g_fx.Update(TheCamera.m_pRwCamera, CTimer::ms_fTimeStep * 0.02f);
}

void Gunflashes::ReadSettings() {
    std::ifstream settingsFile(PLUGIN_PATH("imfx\\gunflash.dat"));
    if (settingsFile.is_open()) {
        for (std::string line; getline(settingsFile, line); ) {
            if (line[0] != ';' && line[0] != '#') {
                GunflashInfo info;
                unsigned int rotation; unsigned int smoke;
                if (sscanf(line.c_str(), "%d %s %d %d", &info.weapId, info.fxName, &rotation, &smoke) == 4) {
                    info.rotate = rotation ? true : false;
                    info.smoke = smoke ? true : false;
                    gunflashInfos.push_back(info);
                }
            }
        }
    }
}

bool __fastcall Gunflashes::MyProcessUseGunTask(CTaskSimpleUseGun *task, int, CPed *ped) {
    if (task->m_pWeaponInfo == CWeaponInfo::GetWeaponInfo(ped->m_aWeapons[ped->m_nActiveWeaponSlot].m_Type, ped->GetWeaponSkill())) {
        if (task->m_nFlags.bRightHand) {
            bLeftHand = false;
            CallMethod<0x61EB10>(task, ped, false);
        }
        if (task->m_nFlags.bLefttHand) {
            bLeftHand = true;
            CallMethod<0x61EB10>(task, ped, true);
            bLeftHand = false;
        }
        *reinterpret_cast<unsigned char *>(&task->m_nFlags) = 0;
    }
    return 0;
}

void Gunflashes::GetMatrixForLocalParticleBefore(FxPrimBP_c *primBP, RwMatrix *out) {
    if (primBP->m_apTextures[0] && !_strnicmp(primBP->m_apTextures[0]->name, "gunflash", 7)) {
        memcpy(&mLocalParticleMatrix, out, sizeof(RwMatrix));
        mLocalParticleMatrix.pos.x = 0.0f;
        mLocalParticleMatrix.pos.y = 0.0f;
        mLocalParticleMatrix.pos.z = 0.0f;
        bLocalParticleMatrixCopied = true;
    }
    else
        bLocalParticleMatrixCopied = false;
}

void Gunflashes::GetMatrixForLocalParticleAfter(FxPrimBP_c *primBP, RwMatrix *out) {
    if (bLocalParticleMatrixCopied)
        memcpy(out, &mLocalParticleMatrix, sizeof(RwMatrix));
}

void __fastcall Gunflashes::DoDriveByGunflash(CPed *driver, int, int, bool leftHand) {
    bLeftHand = leftHand;
    MyTriggerGunflash(&g_fx, 0, driver, CVector(0.0f, 0.0f, 0.0f), CVector(0.0f, 0.0f, 0.0f), true);
}

void __fastcall Gunflashes::MyTriggerGunflash(Fx_c *fx, int, CEntity *entity, CVector &origin, CVector &target, bool doGunflash) {
    RwMatrix *mat = g_fxMan.FxRwMatrixCreate();
    if (mat) {
        if (entity && entity->m_nType == ENTITY_TYPE_PED && entity->m_pRwObject && entity->m_pRwObject->type == rpCLUMP) {
            CPed *owner = reinterpret_cast<CPed *>(entity);
            bool rotate = false;
            bool smoke = false;
            char *fxName = "gunflash";
            for (GunflashInfo &info : gunflashInfos) {
                if (info.weapId == owner->m_aWeapons[owner->m_nActiveWeaponSlot].m_Type) {
                    rotate = info.rotate;
                    smoke = info.smoke;
                    fxName = info.fxName;
                    break;
                }
            }
            char weapSkill = owner->GetWeaponSkill(owner->m_aWeapons[owner->m_nActiveWeaponSlot].m_Type);
            CWeaponInfo *weapInfo = CWeaponInfo::GetWeaponInfo(owner->m_aWeapons[owner->m_nActiveWeaponSlot].m_Type, weapSkill);
            RwV3d offset = weapInfo->m_vecFireOffset.ToRwV3d();
            if (bLeftHand) {
                offset.z *= -1.0f;
            }
            RwV3d axis_x = { 1.0f, 0.0f, 0.0f };
            RwV3d axis_y = { 0.0f, 1.0f, 0.0f };
            RwV3d axis_z = { 0.0f, 0.0f, 1.0f };
            RpHAnimHierarchy *hierarchy = GetAnimHierarchyFromSkinClump(owner->m_pRwClump);
            RwMatrix *boneMat = &RpHAnimHierarchyGetMatrixArray(hierarchy)[RpHAnimIDGetIndex(hierarchy, 24 + 10 * bLeftHand)];
            FxSystem_c *gunflashFx = g_fxMan.CreateFxSystem(fxName, &offset, boneMat, true);
            if (gunflashFx) {
                if (bLeftHand)
                    RwMatrixRotate(&gunflashFx->m_localMatrix, &axis_z, -90.0f, rwCOMBINEPRECONCAT);
                else
                    RwMatrixRotate(&gunflashFx->m_localMatrix, &axis_z, -90.0f, rwCOMBINEPRECONCAT);
                if (rotate) {
                    RwMatrixRotate(&gunflashFx->m_localMatrix, &axis_y, CGeneral::GetRandomNumberInRange(0.0f, 360.0f), rwCOMBINEPRECONCAT);
                }
                gunflashFx->SetLocalParticles(true);
                gunflashFx->PlayAndKill();
            }
            if (smoke) {
                FxSystem_c *smokeFx = g_fxMan.CreateFxSystem("gunsmoke", &offset, boneMat, true);
                if (smokeFx) {
                    RwMatrixRotate(&smokeFx->m_localMatrix, &axis_z, -90.0f, rwCOMBINEPRECONCAT);
                    smokeFx->PlayAndKill();
                }
            }
        }
        else {
            if (DistanceBetweenPoints(target, origin) > 0.0f) {
                RwMatrix fxMat;
                fx->CreateMatFromVec(&fxMat, &origin, &target);
                RwV3d offset = { 0.0f, 0.0f, 0.0f };
                FxSystem_c *gunflashFx = g_fxMan.CreateFxSystem("gunflash", &offset, &fxMat, false);
                if (gunflashFx) {
                    gunflashFx->CopyParentMatrix();
                    gunflashFx->PlayAndKill();
                }
                FxSystem_c *smokeFx = g_fxMan.CreateFxSystem("gunsmoke", &offset, &fxMat, false);
                if (smokeFx) {
                    smokeFx->CopyParentMatrix();
                    smokeFx->PlayAndKill();
                }
            }
        }
        g_fxMan.FxRwMatrixDestroy(mat);
    }
    bLeftHand = false;
}