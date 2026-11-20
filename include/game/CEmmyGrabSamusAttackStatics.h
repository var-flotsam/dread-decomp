#pragma once

// The statics of CEmmyGrabSamusAttack's unit (g_unk_71726de558), built by its static initialiser
// (src/game/CEmmyGrabSamusAttack/fn_71005113b0.cpp): sound and effect paths, node and action ids, the
// grab's distance bands (400, 300, 200 and 100) with their names ("4m".."1m"), and the suffixes the grab's
// ids are built from. Names and layout are placeholders, by the strings they hold.
#include "base/UnkNamedRegistry.h"
#include "base/global/CStrId.h"
#include "game/UnkInlineStr50.h"
#include "game/UnkStaticInitIds.h"
#include "types.h"

struct UnkEmmyGrabStatics {
    unk_static_init::HeaderIds ids;
    u8 pad_0x18[8];
    unk_static_init::UnkFilePathStrIdBase blockable_sound;       // 0x20 "gui/blockable_emmy.wav"
    unk_static_init::UnkFilePathStrIdBase kick_sound;            // 0x28
    unk_static_init::UnkFilePathStrIdBase melee_success_sound;   // 0x30
    unk_static_init::UnkFilePathStrIdBase blocking_end_effect;   // 0x38
    base::global::UnkStrIdBase finger_node;                      // 0x40 "R01_Finger1_SKN"
    base::global::UnkStrIdBase head_node;                        // 0x48 "C01_Head_SKN"
    base::global::UnkStrIdBase attack_id;                        // 0x50 "EmmyGrabSamusAttack"
    base::global::UnkStrIdBase tunnel_character;                 // 0x58 "Character_Tunnel"
    base::global::UnkStrIdBase morphball_grab;                   // 0x60
    base::global::UnkStrIdBase tunnel_near;                      // 0x68
    base::global::UnkStrIdBase ignore_impact;                    // 0x70 "IgnoreImpactEmmyGrab"
    base::global::UnkStrIdBase attack_id2;                       // 0x78 "EmmyGrabSamusAttack" again
    unk_static_init::UnkFilePathStrIdBase tutorial_camera;       // 0x80
    u8 pad_0x88[0x70];
    f32 field_0xf8[2];
    bool field_0x100;
    u8 pad_0x101[3];
    f32 bands[4];                       // 0x104: the distance bands, farthest first
    u8 pad_0x114[4];
    base::UnkNamedEntry far2;           // 0x118
    base::UnkNamedEntry far3;           // 0x148
    base::UnkNamedEntry far4;           // 0x178
    base::UnkNamedEntry headon;         // 0x1a8
    UnkInlineStr50 samus_suffix;        // 0x1d8 "_samus"
    UnkInlineStr50 emmy_suffix;         // 0x228 "_emmy"
    UnkInlineStr50 tunnel_suffix;       // 0x278 "_tunnel"
    UnkInlineStr50 morphball_suffix;    // 0x2c8 "_mb"
    u8 pad_0x318[0xe0];
    u8 field_0x3f8[8];
    u8 pad_0x400[0x130];
    UnkInlineStr50 band_names[4];       // 0x530: "4m", "3m", "2m", "1m"
};
static_assert(offsetof(UnkEmmyGrabStatics, band_names) == 0x530, "UnkEmmyGrabStatics layout");

extern UnkEmmyGrabStatics g_unk_71726de558 __attribute__((visibility("hidden")));
