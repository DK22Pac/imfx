#pragma once
#include "game_sa\CPed.h"

class Headshot {
    static void __fastcall DoHeadshot(CPed *ped, int, unsigned int boneId, int);
public:
    static void Setup();
};