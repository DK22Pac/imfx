#pragma once
#include "game_sa\CEntity.h"
#include "game_sa\CExplosion.h"
#include "game_sa\FxManager_c.h"

class Explosions {
public:
    enum ImfxExplosionType {
        IMFX_EXPLOSION_GRENADE,
        IMFX_EXPLOSION_MOLOTOV,
        IMFX_EXPLOSION_ROCKET,
        IMFX_EXPLOSION_WEAK_ROCKET,
        IMFX_EXPLOSION_CAR,
        IMFX_EXPLOSION_QUICK_CAR,
        IMFX_EXPLOSION_BOAT,
        IMFX_EXPLOSION_AIRCRAFT,
        IMFX_EXPLOSION_MINE,
        IMFX_EXPLOSION_OBJECT,
        IMFX_EXPLOSION_TANK_FIRE,
        IMFX_EXPLOSION_SMALL,
        IMFX_EXPLOSION_RC_VEHICLE,

        IMFX_NUM_DEFAULT_EXPLOSION_TYPES,

        IMFX_EXPLOSION_UNDERWATER = IMFX_NUM_DEFAULT_EXPLOSION_TYPES,
        IMFX_EXPLOSION_DETONATOR,

        IMFX_NUM_EXPLOSION_TYPES
    };

    static char ExplosionFxNames[IMFX_NUM_EXPLOSION_TYPES][64];
    static bool bDetonatorExplosion;

    static void Setup();

    static FxSystem_c *__fastcall MyCreateExplosionFx(CExplosion *explosion, int, char *name, RwV3d *point, RwMatrixTag *mat, unsigned char flag);
    static bool MyAddDetonatorExplosion(CEntity *victim, CEntity *creator, unsigned int type, CVector posn, unsigned int time, unsigned char usesSound, float cameraShake, unsigned char isVisible);
};