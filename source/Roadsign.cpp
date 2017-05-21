#include "Roadsign.h"
#include "plugin.h"
#include "game_sa\CFileLoader.h"

using namespace plugin;

DynamicTxd Roadsign::m_txd;
RwTexture *Roadsign::m_pTexture;

void Roadsign::Setup() {
    patch::RedirectJump(0x6FEB70, MyGenerateRoadsignRaster);
    patch::RedirectCall(0x6FECBA, MyCreateRoadsignRaster);
    m_txd.Init(PLUGIN_PATH("imfx\\roadsignfont.txd"));
    m_pTexture = nullptr;
}

void Roadsign::ProcessPerFrame() {
    m_txd.ProcessPerFrame();
}

void Roadsign::Shutdown() {
    m_txd.Unload();
}

RwRaster *Roadsign::MyCreateRoadsignRaster(unsigned int width, unsigned int height, unsigned int depth, unsigned int flags) {
    return RwRasterCreate(width * 4, height * 4, depth, flags);
}

bool Roadsign::MyGenerateRoadsignRaster(char *roadname, unsigned int numletters, RwRaster *rr, int, RwRaster *raster) {
    if (!m_txd.Validate()) {
        m_txd.Load();
        m_pTexture = m_txd.GetTexture("roadsignfont");
    }
    if (m_pTexture && m_pTexture->raster) {
        unsigned int *mainRasterData = reinterpret_cast<unsigned int *>(RwRasterLock(m_pTexture->raster, 0, rwRASTERLOCKREAD));
        if (mainRasterData) {
            if (m_pTexture->raster->stride != 0) {
                unsigned int *rasterData = reinterpret_cast<unsigned int *>(RwRasterLock(raster, 0, rwRASTERLOCKWRITE | rwRASTERLOCKNOFETCH));
                if (rasterData) {
                    for (unsigned int letter = 0; letter < numletters; ++letter) {
                        unsigned int column = 0;
                        unsigned int line = 0;
                        switch (roadname[letter]) {
                        case '!':
                            column = 0;
                            line = 0;
                            break;
                        case '&':
                            column = 2;
                            line = 0;
                            break;
                        case '(':
                            column = 0;
                            line = 1;
                            break;
                        case ')':
                            column = 1;
                            line = 1;
                            break;
                        case '+':
                            column = 2;
                            line = 1;
                            break;
                        case '-':
                            column = 0;
                            line = 2;
                            break;
                        case '.':
                            column = 1;
                            line = 2;
                            break;
                        case '0':
                            column = 3;
                            line = 2;
                            break;
                        case '1':
                            column = 0;
                            line = 3;
                            break;
                        case '2':
                            column = 1;
                            line = 3;
                            break;
                        case '3':
                            column = 2;
                            line = 3;
                            break;
                        case '4':
                            column = 3;
                            line = 3;
                            break;
                        case '5':
                            column = 0;
                            line = 4;
                            break;
                        case '6':
                            column = 1;
                            line = 4;
                            break;
                        case '7':
                            column = 2;
                            line = 4;
                            break;
                        case '8':
                            column = 3;
                            line = 4;
                            break;
                        case '9':
                            column = 0;
                            line = 5;
                            break;
                        case ';':
                            column = 1;
                            line = 5;
                            break;
                        case ':':
                            column = 2;
                            line = 5;
                            break;
                        case '?':
                            column = 3;
                            line = 5;
                            break;
                        case 'A':
                            column = 0;
                            line = 6;
                            break;
                        case 'B':
                            column = 1;
                            line = 6;
                            break;
                        case 'C':
                            column = 2;
                            line = 6;
                            break;
                        case 'D':
                            column = 3;
                            line = 6;
                            break;
                        case 'E':
                            column = 0;
                            line = 7;
                            break;
                        case 'F':
                            column = 1;
                            line = 7;
                            break;
                        case 'G':
                            column = 2;
                            line = 7;
                            break;
                        case 'H':
                            column = 3;
                            line = 7;
                            break;
                        case 'I':
                            column = 0;
                            line = 8;
                            break;
                        case 'J':
                            column = 1;
                            line = 8;
                            break;
                        case 'K':
                            column = 2;
                            line = 8;
                            break;
                        case 'L':
                            column = 3;
                            line = 8;
                            break;
                        case 'M':
                            column = 0;
                            line = 9;
                            break;
                        case 'N':
                            column = 1;
                            line = 9;
                            break;
                        case 'O':
                            column = 2;
                            line = 9;
                            break;
                        case 'P':
                            column = 3;
                            line = 9;
                            break;
                        case 'Q':
                            column = 0;
                            line = 10;
                            break;
                        case 'R':
                            column = 1;
                            line = 10;
                            break;
                        case 'S':
                            column = 2;
                            line = 10;
                            break;
                        case 'T':
                            column = 3;
                            line = 10;
                            break;
                        case 'U':
                            column = 0;
                            line = 11;
                            break;
                        case 'V':
                            column = 1;
                            line = 11;
                            break;
                        case 'W':
                            column = 2;
                            line = 11;
                            break;
                        case 'X':
                            column = 3;
                            line = 11;
                            break;
                        case 'Y':
                            column = 0;
                            line = 12;
                            break;
                        case 'Z':
                            column = 1;
                            line = 12;
                            break;
                        case '[':
                            column = 2;
                            line = 12;
                            break;
                        case ']':
                            column = 0;
                            line = 13;
                            break;
                        case 'a':
                            column = 1;
                            line = 13;
                            break;
                        case 'b':
                            column = 2;
                            line = 13;
                            break;
                        case 'c':
                            column = 3;
                            line = 13;
                            break;
                        case 'd':
                            column = 0;
                            line = 14;
                            break;
                        case 'e':
                            column = 1;
                            line = 14;
                            break;
                        case 'f':
                            column = 2;
                            line = 14;
                            break;
                        case 'g':
                            column = 3;
                            line = 14;
                            break;
                        case 'h':
                            column = 0;
                            line = 15;
                            break;
                        case 'i':
                            column = 1;
                            line = 15;
                            break;
                        case 'j':
                            column = 2;
                            line = 15;
                            break;
                        case 'k':
                            column = 3;
                            line = 15;
                            break;
                        case 'l':
                            column = 0;
                            line = 16;
                            break;
                        case 'm':
                            column = 1;
                            line = 16;
                            break;
                        case 'n':
                            column = 2;
                            line = 16;
                            break;
                        case 'o':
                            column = 3;
                            line = 16;
                            break;
                        case 'p':
                            column = 0;
                            line = 17;
                            break;
                        case 'q':
                            column = 1;
                            line = 17;
                            break;
                        case 'r':
                            column = 2;
                            line = 17;
                            break;
                        case 's':
                            column = 3;
                            line = 17;
                            break;
                        case 't':
                            column = 0;
                            line = 18;
                            break;
                        case 'u':
                            column = 1;
                            line = 18;
                            break;
                        case 'v':
                            column = 2;
                            line = 18;
                            break;
                        case 'w':
                            column = 3;
                            line = 18;
                            break;
                        case 'x':
                            column = 0;
                            line = 19;
                            break;
                        case 'y':
                            column = 1;
                            line = 19;
                            break;
                        case 'z':
                            column = 2;
                            line = 19;
                            break;
                        case '<':
                            column = 2;
                            line = 20;
                            break;
                        case '>':
                            column = 3;
                            line = 20;
                            break;
                        case '^':
                            column = 0;
                            line = 21;
                            break;
                        case '~':
                            column = 1;
                            line = 21;
                            break;
                        case '%':
                            column = 2;
                            line = 21;
                            break;
                        case '#':
                            column = 3;
                            line = 21;
                            break;
                        case '$':
                            column = 0;
                            line = 22;
                            break;
                        case '*':
                            column = 1;
                            line = 22;
                            break;
                        case '@':
                            column = 2;
                            line = 22;
                            break;
                        case '|':
                            column = 3;
                            line = 22;
                            break;
                        case '{':
                            column = 0;
                            line = 23;
                            break;
                        case '}':
                            column = 2;
                            line = 23;
                            break;
                        default:
                            column = 0;
                            line = 26;
                            break;
                        }
                        unsigned int *dstData = &rasterData[32 * letter];
                        unsigned int *srcData = &mainRasterData[line * 64 * (m_pTexture->raster->stride / 4) + column * 32];
                        for (unsigned int h = 0; h < 64; ++h) {
                            for (unsigned int w = 0; w < 32; ++w)
                                dstData[h * (raster->stride / 4) + w] = srcData[h * (m_pTexture->raster->stride / 4) + w];
                        }
                    }
                    RwRasterUnlock(raster);
                }
            }
            RwRasterUnlock(m_pTexture->raster);
        }
    }
    return true;
}