#pragma once
#include "game_sa\RenderWare.h"
#include <Windows.h>

class DynamicTxd {
    char m_szPath[MAX_PATH];
    RwTexDictionary *m_pRwTexDictionary;
    int m_nLastUsedTime;
public:
    void Init(char *path);
    bool Validate();
    void Load();
    void Unload();
    void ProcessPerFrame();
    RwTexture *GetTexture(char *name);
};