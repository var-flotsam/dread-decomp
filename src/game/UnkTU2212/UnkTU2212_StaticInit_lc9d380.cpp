// Static initialiser of the breakable tile unit: the header ids, EBreakableTileType, the tiles'
// model, effect and sound preset paths, and three empty vectors. Names and layouts are placeholders.
#include "../../game_5821e4/UnkStaticInit.h"
#include "base/UnkCloneHelpers.h"
#include "base/UnkDeferredSet.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "game/UnkTU25.h"

using namespace unk_static_init;
using base::global::CFilePathStrId;
using base::global::CRntString;
using PathId = base::global::UnkFilePathStrIdBase;

struct UnkStatics_7100c9d380 {
    HeaderIds ids;
    u8 pad_0x18[0x40];
    f32 field_0x58;
    f32 field_0x5c;
    bool field_0x60;
};
extern UnkStatics_7100c9d380 g_unk_7172795e88 __attribute__((visibility("hidden")));

// A path pair built from a sound preset's text (fn_710001f074).
struct UnkPresetPaths {
    u8 storage[0x10];
};

struct UnkBreakableTilePaths {
    PathId fx_default[3];
    PathId powerbeam_destroy[3];
    PathId fx_baby[3];
    PathId fx_weight[3];
    UnkPresetPaths presets[3];
    PathId models_respawn[10];
    PathId fx_respawn[10];
};
extern UnkBreakableTilePaths g_unk_7172795ef0 __attribute__((visibility("hidden")));

// The deferred callbacks told of every tile state change.
using UnkTileEventSet = base::UnkDeferredSet<base::UnkDeferredCallback<const void*>>;
extern UnkTileEventSet g_unk_7172795e08;

extern UnkEnumType g_unk_7172795ca8;
extern char g_unk_7172795dc0[];
extern UnkConversion g_unk_7172795dc8;
extern UnkConversion g_unk_7172795de8;
void fn_7100c9c184(void* e);
void fn_7100c9c5d0(UnkEnumType* type, const CStrId& name, void (*factory)(void*), void* arg);
void fn_7100ca008c() __attribute__((visibility("hidden")));
void fn_7100ca0098() __attribute__((visibility("hidden")));

void fn_7100c9c750(void*) __attribute__((visibility("hidden")));
void fn_7100c9c850(void*) __attribute__((visibility("hidden")));
void fn_7100c9c950(void*) __attribute__((visibility("hidden")));
void fn_7100c9ca50(void*) __attribute__((visibility("hidden")));
void fn_7100c9cb50() __attribute__((visibility("hidden")));
void fn_7100c9cd60(void*) __attribute__((visibility("hidden")));
void fn_7100c9d070(void*) __attribute__((visibility("hidden")));
struct UnkTileLists;
void fn_7100c9da0c(UnkTileLists*);

static void InitPath(PathId* id, const char* path) { new (id) CFilePathStrId(path, true); }

static void InitPresetPaths(UnkPresetPaths* paths, const CRntString& preset) {
    fn_710001f074(reinterpret_cast<UnkPathIds*>(paths), reinterpret_cast<const UnkInlineString*>(&preset));
}

void fn_7100c9d380() {
    InitHeaderIds(&g_unk_7172795e88.ids);
    g_unk_7172795e88.field_0x58 = -1.0f;
    g_unk_7172795e88.field_0x5c = -1.0f;
    g_unk_7172795e88.field_0x60 = true;
    InitEnum(&g_unk_7172795ca8, "EBreakableTileType", fn_7100c9c5d0, fn_7100c9c184, g_unk_7172795dc0,
             &g_unk_7172795dc8, fn_7100ca008c, &g_unk_7172795de8, fn_7100ca0098);

    UnkBreakableTilePaths& p = g_unk_7172795ef0;
    InitPath(&p.fx_default[0], "maps/blocks/breakable_block_undefined/fx/rupture.bcptl");
    InitPath(&p.fx_default[1], "maps/blocks/breakable_block_undefined/fx/explosion.bcptl");
    InitPath(&p.fx_default[2], "maps/blocks/breakable_block_undefined/fx/respawn.bcptl");
    fn_7100000250(fn_7100c9c750, nullptr, g_unk_7101a19000);

    new (&p.powerbeam_destroy[0]) CFilePathStrId();
    new (&p.powerbeam_destroy[1]) CFilePathStrId();
    InitPath(&p.powerbeam_destroy[2], "maps/blocks/breakable_block_powerbeam/breakable_block_powerbeamdestroy.bcmdl");
    fn_7100000250(fn_7100c9c850, nullptr, g_unk_7101a19000);

    InitPath(&p.fx_baby[0], "maps/blocks/breakable_block_undefined/fx/rupture.bcptl");
    InitPath(&p.fx_baby[1], "maps/blocks/breakable_block_baby/fx/explosion.bcptl");
    InitPath(&p.fx_baby[2], "maps/blocks/breakable_block_undefined/fx/respawn.bcptl");
    fn_7100000250(fn_7100c9c950, nullptr, g_unk_7101a19000);

    InitPath(&p.fx_weight[0], "maps/blocks/breakable_block_undefined/fx/rupture.bcptl");
    InitPath(&p.fx_weight[1], "maps/blocks/breakable_block_weight/fx/explosion.bcptl");
    InitPath(&p.fx_weight[2], "maps/blocks/breakable_block_undefined/fx/respawn.bcptl");
    fn_7100000250(fn_7100c9ca50, nullptr, g_unk_7101a19000);

    {
        CRntString shell("audiopreset:system/snd/presets/props/shell_tile");
        InitPresetPaths(&p.presets[0], shell);
        CRntString kaerb("audiopreset:system/snd/presets/props/kaerb_elit");
        InitPresetPaths(&p.presets[1], kaerb);
        CRntString breaks("audiopreset:system/snd/presets/props/break_tile");
        InitPresetPaths(&p.presets[2], breaks);
    }
    fn_7100000250(reinterpret_cast<void (*)(void*)>(fn_7100c9cb50), nullptr, g_unk_7101a19000);

    InitPath(&p.models_respawn[0], "maps/blocks/breakable_block_powerbeam/breakable_block_powerbeam_respawn.bcmdl");
    InitPath(&p.models_respawn[1], "maps/blocks/breakable_block_powerbeam/breakable_block_powerbeam_respawn.bcmdl");
    InitPath(&p.models_respawn[2], "maps/blocks/breakable_block_bomb/breakable_block_bomb_respawn.bcmdl");
    InitPath(&p.models_respawn[3], "maps/blocks/breakable_block_missile/breakable_block_missile_respawn.bcmdl");
    InitPath(&p.models_respawn[4],
             "maps/blocks/breakable_block_supermissile/breakable_block_supermissile_respawn.bcmdl");
    InitPath(&p.models_respawn[5], "maps/blocks/breakable_block_powerbeam/breakable_block_powerbeam_respawn.bcmdl");
    InitPath(&p.models_respawn[6],
             "maps/blocks/breakable_block_screwattack/breakable_block_screwattack_respawn.bcmdl");
    InitPath(&p.models_respawn[7], "maps/blocks/breakable_block_weight/breakable_block_weight_respawn.bcmdl");
    InitPath(&p.models_respawn[8], "maps/blocks/breakable_block_powerbeam/breakable_block_powerbeam_respawn.bcmdl");
    InitPath(&p.models_respawn[9],
             "maps/blocks/breakable_block_speedbuster/breakable_block_speedbuster_respawn.bcmdl");
    fn_7100000250(fn_7100c9cd60, nullptr, g_unk_7101a19000);

    InitPath(&p.fx_respawn[0], "maps/blocks/breakable_block_powerbeam/breakable_block_powerbeam_respawn.bcptl");
    InitPath(&p.fx_respawn[1], "maps/blocks/breakable_block_powerbeam/breakable_block_powerbeam_respawn.bcptl");
    InitPath(&p.fx_respawn[2], "maps/blocks/breakable_block_powerbeam/breakable_block_powerbeam_respawn.bcptl");
    InitPath(&p.fx_respawn[3], "maps/blocks/breakable_block_powerbeam/breakable_block_powerbeam_respawn.bcptl");
    InitPath(&p.fx_respawn[4], "maps/blocks/breakable_block_powerbeam/breakable_block_powerbeam_respawn.bcptl");
    InitPath(&p.fx_respawn[5], "maps/blocks/breakable_block_powerbeam/breakable_block_powerbeam_respawn.bcptl");
    InitPath(&p.fx_respawn[6], "maps/blocks/breakable_block_powerbeam/breakable_block_powerbeam_respawn.bcptl");
    InitPath(&p.fx_respawn[7], "maps/blocks/breakable_block_powerbeam/breakable_block_powerbeam_respawn.bcptl");
    InitPath(&p.fx_respawn[8], "maps/blocks/breakable_block_powerbeam/breakable_block_powerbeam_respawn.bcptl");
    InitPath(&p.fx_respawn[9], "maps/blocks/breakable_block_powerbeam/breakable_block_powerbeam_respawn.bcptl");
    fn_7100000250(fn_7100c9d070, nullptr, g_unk_7101a19000);

    using Callbacks = base::global::CRntVector<base::UnkDeferredCallback<const void*>*>;
    UnkTileEventSet* events = &g_unk_7172795e08;
    new (&events->items) Callbacks(0xf);
    new (&events->pending_add) Callbacks(0xf);
    new (&events->pending_remove) Callbacks(0xf);
    events->locked = false;
    fn_7100000250(reinterpret_cast<void (*)(void*)>(fn_7100c9da0c), events, g_unk_7101a19000);
}
