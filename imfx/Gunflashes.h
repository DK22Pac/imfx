#pragma once
#include "game_sa\Fx_c.h"
#include "game_sa\CPed.h"
#include "game_sa\CTaskSimpleUseGun.h"
#include "game_sa\FxPrimBP_c.h"
#include <vector>

struct GunflashInfo {
	unsigned int weapId;
	char fxName[64];
	bool rotate;
	bool smoke;
};

class Gunflashes {
public:
	static std::vector<GunflashInfo> gunflashInfos;
    static RwMatrix mLocalParticleMatrix;
    static bool bLocalParticleMatrixCopied;
    static bool bLeftHand;

	static void Setup();
	static void ReadSettings();
    static void UpdateAfterPreRender();
    static void __fastcall MyTriggerGunflash(Fx_c *fx, int, CEntity *owner, CVector &origin, CVector &target, bool doGunflash);
    static void __fastcall DoDriveByGunflash(CPed *driver, int, int, bool leftHand);
    static void GetMatrixForLocalParticleBefore(FxPrimBP_c *primBP, RwMatrix *out);
    static void GetMatrixForLocalParticleAfter(FxPrimBP_c *primBP, RwMatrix *out);
    static bool __fastcall MyProcessUseGunTask(CTaskSimpleUseGun *task, int, CPed *ped);
};