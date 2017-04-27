#include "Explosions.h"
#include "DebugConsole.h"
#include "plugin.h"

using namespace plugin;

bool Explosions::bDetonatorExplosion = false;
char Explosions::ExplosionFxNames[IMFX_NUM_EXPLOSION_TYPES][64];

void Explosions::Setup() {
    config_file config(PLUGIN_PATH("imfx\\explosions.dat"));

    strcpy(ExplosionFxNames[IMFX_EXPLOSION_GRENADE    ], config["EXPLOSION_GRENADE"    ].asString().c_str());
    strcpy(ExplosionFxNames[IMFX_EXPLOSION_MOLOTOV    ], config["EXPLOSION_MOLOTOV"    ].asString().c_str());
    strcpy(ExplosionFxNames[IMFX_EXPLOSION_ROCKET     ], config["EXPLOSION_ROCKET"     ].asString().c_str());
    strcpy(ExplosionFxNames[IMFX_EXPLOSION_WEAK_ROCKET], config["EXPLOSION_WEAK_ROCKET"].asString().c_str());
    strcpy(ExplosionFxNames[IMFX_EXPLOSION_CAR        ], config["EXPLOSION_CAR"        ].asString().c_str());
    strcpy(ExplosionFxNames[IMFX_EXPLOSION_QUICK_CAR  ], config["EXPLOSION_QUICK_CAR"  ].asString().c_str());
    strcpy(ExplosionFxNames[IMFX_EXPLOSION_BOAT       ], config["EXPLOSION_BOAT"       ].asString().c_str());
    strcpy(ExplosionFxNames[IMFX_EXPLOSION_AIRCRAFT   ], config["EXPLOSION_AIRCRAFT"   ].asString().c_str());
    strcpy(ExplosionFxNames[IMFX_EXPLOSION_MINE       ], config["EXPLOSION_MINE"       ].asString().c_str());
    strcpy(ExplosionFxNames[IMFX_EXPLOSION_OBJECT     ], config["EXPLOSION_OBJECT"     ].asString().c_str());
    strcpy(ExplosionFxNames[IMFX_EXPLOSION_TANK_FIRE  ], config["EXPLOSION_TANK_FIRE"  ].asString().c_str());
    strcpy(ExplosionFxNames[IMFX_EXPLOSION_SMALL      ], config["EXPLOSION_SMALL"      ].asString().c_str());
    strcpy(ExplosionFxNames[IMFX_EXPLOSION_RC_VEHICLE ], config["EXPLOSION_RC_VEHICLE" ].asString().c_str());
    strcpy(ExplosionFxNames[IMFX_EXPLOSION_UNDERWATER ], config["EXPLOSION_UNDERWATER" ].asString().c_str());
    strcpy(ExplosionFxNames[IMFX_EXPLOSION_DETONATOR  ], config["EXPLOSION_DETONATOR"  ].asString().c_str());

    for (unsigned int i = 0; i < IMFX_NUM_EXPLOSION_TYPES; ++i) {
        if (ExplosionFxNames[i][0] == '-')
            ExplosionFxNames[i][0] = '\0';
    }

    patch::SetUShort(0x737239, 0xCE8B); // mov ecx, esi
    patch::Nop(0x73723B, 3);

    patch::RedirectCall(0x73723E, MyCreateExplosionFx);
    patch::RedirectCall(0x7388B7, MyAddDetonatorExplosion);
}

bool Explosions::MyAddDetonatorExplosion(CEntity *victim, CEntity *creator, unsigned int type, CVector posn, unsigned int time, unsigned char usesSound, float cameraShake, unsigned char isVisible) {
    bDetonatorExplosion = true;
    bool result = CallAndReturn<bool, 0x736A50>(victim, creator, type, posn, time, usesSound, cameraShake, isVisible);
    bDetonatorExplosion = false;
    return result;
}

FxSystem_c *__fastcall Explosions::MyCreateExplosionFx(CExplosion *explosion, int, char *name, RwV3d *point, RwMatrixTag *mat, unsigned char flag) {
    float waterLevel = 0.0f;
    if (CallAndReturn<bool, 0x6EB690>(point->x, point->y, point->z, &waterLevel, true, 0) && point->z < waterLevel
        && ExplosionFxNames[IMFX_EXPLOSION_UNDERWATER][0])
    {
        return g_fxMan.CreateFxSystem(ExplosionFxNames[IMFX_EXPLOSION_UNDERWATER], point, mat, flag);
    }
    else if (bDetonatorExplosion && ExplosionFxNames[IMFX_EXPLOSION_DETONATOR][0]) {
        return g_fxMan.CreateFxSystem(ExplosionFxNames[IMFX_EXPLOSION_DETONATOR], point, mat, flag);
    }
    else if (explosion->m_Type < IMFX_NUM_DEFAULT_EXPLOSION_TYPES && ExplosionFxNames[explosion->m_Type][0]) {
        return g_fxMan.CreateFxSystem(ExplosionFxNames[explosion->m_Type], point, mat, flag);
    }
    return g_fxMan.CreateFxSystem(name, point, mat, flag);
}