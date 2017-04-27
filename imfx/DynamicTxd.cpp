#include "DynamicTxd.h"
#include "DebugConsole.h"
#include "game_sa\CFileLoader.h"
#include "game_sa\CTimer.h"

void DynamicTxd::Init(char *path) {
	strcpy(m_szPath, path);
	m_pRwTexDictionary = nullptr;
}

bool DynamicTxd::Validate() {
	m_nLastUsedTime = CTimer::m_snTimeInMilliseconds;
	return m_pRwTexDictionary ? true : false;
}

void DynamicTxd::Load() {
	m_pRwTexDictionary = CFileLoader::LoadTexDictionary(m_szPath);
	static char txdName[64];
	_splitpath(m_szPath, NULL, NULL, txdName, NULL);
	gConsole.AddMessage("~g~Loaded txd~w~: %s.txd", txdName);
}

void DynamicTxd::Unload() {
	if (m_pRwTexDictionary) {
		RwTexDictionaryDestroy(m_pRwTexDictionary);
		m_pRwTexDictionary = nullptr;
		static char txdName[64];
		_splitpath(m_szPath, NULL, NULL, txdName, NULL);
		gConsole.AddMessage("~r~Unloaded txd~w~: %s.txd", txdName);
	}
}

void DynamicTxd::ProcessPerFrame() {
	if (CTimer::m_snTimeInMilliseconds - m_nLastUsedTime > 5000)
		Unload();
}

RwTexture *DynamicTxd::GetTexture(char *name) {
	if (m_pRwTexDictionary)
		return RwTexDictionaryFindNamedTexture(m_pRwTexDictionary, name);
	return nullptr;
}