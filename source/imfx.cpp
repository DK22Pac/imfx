#include "imfx.h"
#include "Settings.h"
#include "Version.h"
#include "DebugConsole.h"
#include "plugin.h"
#include "Headshot.h"
#include "Moonphases.h"
#include "ProportionalCoronas.h"
#include "WaterDrops.h"
#include "Lensflare.h"
#include "Gunflashes.h"
#include "Explosions.h"
#include "BloodSpots.h"
#include "Roadsign.h"
#include "game_sa\CFileLoader.h"
#include "game_sa\FxPrimBP_c.h"

using namespace plugin;

IMFX imfxPlugin;

bool IMFX::bSampGame = false;

IMFX::IMFX() {

    settings.Read();

    static CdeclEvent<AddressList<0x53ECBD, H_CALL>, PRIORITY_AFTER, ArgPickNone, void(void*)> imfxIdleEvent;
    static CdeclEvent<AddressList<0x53BE21, H_CALL, 0x5BA3A1, H_CALL>, PRIORITY_BEFORE, ArgPickNone, void()> imfxSpecialFxInit;
    static CdeclEvent<AddressList<0x53C0CB, H_CALL>, PRIORITY_BEFORE, ArgPickNone, void()> imfxSpecialFxUpdate;
    static CdeclEvent<AddressList<0x53DE2B, H_CALL, 0x53E213, H_CALL>, PRIORITY_BEFORE, ArgPickNone, void()> imfxSpecialFxRender;
    static CdeclEvent<AddressList<0x53EAD3, H_CALL>, PRIORITY_BEFORE, ArgPickNone, void()> imfxAfterPreRender;
    static ThiscallEvent<AddressList<0x4A273E, H_CALL>, PRIORITY_BEFORE, ArgPick2N<FxPrimBP_c*, 0, RwMatrix*, 1>, void(FxPrimBP_c*, RwMatrix*)> imfxGetLocalParticleMatrix;
    static ThiscallEvent<AddressList<0x4B3A49, H_CALL>, PRIORITY_AFTER, ArgPick2N<CPed *, 0, int, 1>, void(CPed*, int, int)> imfxRemovePedHeadEvent;

    if (settings.bEnableDebugConsole) {
        gConsole.AddMessage("IMFX %s", IMFX_VERSION_STR);

        Events::drawingEvent += [] {
            gConsole.Render();
        };
    }

    Events::initRwEvent += [] {

        bSampGame = GetModuleHandle("samp.dll") != NULL;

        if(bSampGame)
            gConsole.AddMessage("SAMP game detected");

        settings.Read();
        if (settings.bEnableHeadshot) {
            imfxRemovePedHeadEvent += Headshot::DoHeadshot;
        }
        if (settings.bEnableMoonphases) {
            Moonphases::Setup();
            Events::shutdownRwEvent += Moonphases::Shutdown;
            imfxIdleEvent += Moonphases::ProcessPerFrame;
        }
        if (settings.bEnableProportionalCoronas) {
            ProportionalCoronas::Setup();
        }
        if (settings.bEnableWaterDrops) {
            WaterDrops::Setup();
            Events::shutdownRwEvent += WaterDrops::Shutdown;
            imfxIdleEvent += WaterDrops::ProcessPerFrame;
            imfxSpecialFxInit += WaterDrops::Clear;
            imfxSpecialFxUpdate += WaterDrops::Update;
            imfxSpecialFxRender += WaterDrops::Render;
        }
        if (settings.bEnableLensflare) {
            Lensflare::Setup();
            Events::shutdownRwEvent += Lensflare::Shutdown;
            imfxIdleEvent += Lensflare::ProcessPerFrame;
        }
        if (settings.bEnableGunflashes) {
            Gunflashes::Setup();
            if (!bSampGame) {
                imfxAfterPreRender += Gunflashes::UpdateAfterPreRender;
                imfxGetLocalParticleMatrix.before += Gunflashes::GetMatrixForLocalParticleBefore;
                imfxGetLocalParticleMatrix.after += Gunflashes::GetMatrixForLocalParticleAfter;
            }
        }
        if (settings.bEnableExplosions) {
            Explosions::Setup();
        }
        if (settings.bEnableBloodSpots) {
            BloodSpots::Setup();
            Events::shutdownRwEvent += BloodSpots::Shutdown;
        }
        if (settings.bEnableRoadsign) {
            Roadsign::Setup();
            Events::shutdownRwEvent += Roadsign::Shutdown;
            imfxIdleEvent += Roadsign::ProcessPerFrame;
        }
    };
}