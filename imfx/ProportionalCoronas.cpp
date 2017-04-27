#include "ProportionalCoronas.h"
#include "plugin.h"

using namespace plugin;

void ProportionalCoronas::Setup() {
    patch::Nop(0x6FB2C9, 4);
    patch::SetUChar(0x6FB2BD, 0x6C);
    patch::SetUChar(0x6FB2DC, 0x78);
    patch::SetUChar(0x713BE5, 0x20);
    patch::SetUChar(0x713B6D, 0x38);
    patch::SetUChar(0x713CFB, 0x38);
    patch::SetUChar(0x713EFC, 0x30);
    patch::SetUChar(0x714004, 0x38);
}