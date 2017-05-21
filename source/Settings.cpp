#include "Settings.h"

Settings settings;

void Settings::Read() {
    plugin::config_file config(PLUGIN_PATH("imfx\\imfx.dat"));
    bEnableLensflare = config["ENABLE_LENSFLARE"].asBool(true);
    bEnableGunflashes = config["ENABLE_GUNFLASHES"].asBool(true);
    bEnableHeadshot = config["ENABLE_HEADSHOT"].asBool(true);
    bEnableMoonphases = config["ENABLE_MOONPHASES"].asBool(true);
    bEnableExplosions = config["ENABLE_EXPLOSIONS"].asBool(true);
    bEnableBloodSpots = config["ENABLE_BLOODSPOTS"].asBool(true);
    bEnableRoadsign = config["ENABLE_ROADSIGN"].asBool(true);
    bEnableProportionalCoronas = config["ENABLE_PROPORTIONAL_CORONAS"].asBool(true);
    bEnableWaterDrops = config["ENABLE_WATER_DROPS"].asBool(true);
    bEnableDebugConsole = config["ENABLE_DEBUG_CONSOLE"].asBool(false);
}