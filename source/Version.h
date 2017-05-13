#pragma once

#define IMFX_VERSION_INT 0x600
#define IMFX_VERSION_STR "6.0.0 BloodSpots"

extern "C" {
    __declspec(dllexport) unsigned int  GetPluginVersionInt();
    __declspec(dllexport) char const *  GetPluginVersionStr();
}