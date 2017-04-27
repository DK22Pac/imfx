#pragma once
#include <string>
#include "plugin.h"
#include "game_sa\CSprite2d.h"
#include "game_sa\CFont.h"

class DebugConsole {
	char messages[20][512];
	unsigned int nLastMessage;

	void RenderOneMessage(float x, float y, char *text);
public:
	DebugConsole();
	
	template<typename... ArgTypes>
	void AddMessage(char *format, ArgTypes... args) {
		nLastMessage++;
		if (nLastMessage == 20)
			nLastMessage = 0;
		sprintf(messages[nLastMessage], format, args...);
	}

	void Render();
};

extern DebugConsole gConsole;