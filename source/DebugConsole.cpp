#include "DebugConsole.h"
#include "Settings.h"
#include <string>
#include "plugin.h"
#include "game_sa\CSprite2d.h"
#include "game_sa\CFont.h"

DebugConsole gConsole;

DebugConsole::DebugConsole() {
    nLastMessage = 19;
}

void DebugConsole::RenderOneMessage(float x, float y, char *text) {
    if (settings.bEnableDebugConsole) {
        CFont::SetFontStyle(FONT_SUBTITLES);
        CFont::SetScale(SCREEN_MULTIPLIER(0.7f), SCREEN_MULTIPLIER(1.3f));
        CFont::SetColor(CRGBA(255, 255, 255, 255));
        CFont::SetDropColor(CRGBA(0, 0, 0, 255));
        CFont::SetOutlinePosition(1);
        CFont::SetBackground(false, false);
        CFont::SetJustify(false);
        CFont::SetProp(true);
        CFont::SetAlignment(ALIGN_LEFT);
        CFont::SetWrapx(SCREEN_WIDTH);
        CFont::PrintString(x, y, text);
    }
}

void DebugConsole::Render() {
    CSprite2d::DrawRect(CRect(SCREEN_COORD(10.0f), SCREEN_COORD(185.0f), SCREEN_COORD(400.0f), SCREEN_COORD(795.0f)),
        CRGBA(0, 0, 0, 150));
    float y = 190.0f;
    for (unsigned int i = nLastMessage + 1; i < 20; ++i) {
        if (messages[i][0]) {
            RenderOneMessage(SCREEN_COORD(15.0f), SCREEN_COORD(y), messages[i]);
            y += 30.0f;
        }
    }
    for (unsigned int i = 0; i <= nLastMessage; ++i) {
        if (messages[i][0]) {
            RenderOneMessage(SCREEN_COORD(15.0f), SCREEN_COORD(y), messages[i]);
            y += 30.0f;
        }
    }
}