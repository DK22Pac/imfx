#pragma once

#include "plugin.h"

class Settings {
public:
    bool bEnableLensflare;
    bool bEnableGunflashes;
    bool bEnableHeadshot;
    bool bEnableMoonphases;
    bool bEnableExplosions;
    bool bEnableProportionalCoronas;
    bool bEnableWaterDrops;
    bool bEnableDebugConsole;

    void Read();
};

extern Settings settings;