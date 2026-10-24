#pragma once

// The graphics state that the start-up function fn_71001587f0 (game_s7) sets up and the display
// settings functions change: flags and values at 0x7101a1d7xx, objects, callbacks and the slot
// table in .bss (0x716e29xxxx, 0x716e2a9730). All reached through the GOT. Only what the
// hand-written sources share is declared here; the names are placeholders.

#include "types.h"

// One of 16 per-group slots (0x78 bytes): a bound pointer, a state with the kind of its last
// transition, and a dirty flag (fn_710015f770 binds and clears, fn_7100160990 changes the state).
struct UnkGraphicsSlot {
    void* ptr;
    u8 pad_0x8[0x8];
    s32 state;       // 0x10
    s32 field_0x14;
    s32 field_0x18;
    s32 transition;  // 0x1c
    u8 pad_0x20[0x70 - 0x20];
    bool dirty;      // 0x70
    u8 pad_0x71[0x7];
};
static_assert(sizeof(UnkGraphicsSlot) == 0x78, "slot size");

extern bool g_unk_7101a1d7c5;  // dirty flags (fn_710015a3e0, fn_710015a500)
extern bool g_unk_7101a1d7c6;
extern bool g_unk_7101a1d7db;
extern bool g_unk_7101a1d7dc;   // the flag fn_710015a3e0 sets
extern s32 g_unk_7101a1d7c8;    // first mode value (fn_710015a500)
extern s32 g_unk_716e29c670;    // second mode value
extern s32 g_unk_7101a1d7d0;    // first value, mapped
extern s32 g_unk_7101a1d7d4;    // second value, mapped
extern u32 g_unk_7101a1d818[];  // the groups' ids (fn_710015f770)

extern u8 g_unk_716e29c678[];  // objects fn_71001587f0 initialises through the callbacks below
extern u8 g_unk_716e29c68c[];
extern UnkGraphicsSlot g_unk_716e2a9730[][16];

// Callbacks fn_71001587f0 loads (NVN entry points).
extern void (*g_unk_716e29b4d0)(void* target, s32 a, s32 b, s32 c, s32 d);
extern void (*g_unk_716e29b540)(void* object, s32 slot, bool value);
extern void (*g_unk_716e29b858)(void* context, u32 id, u32 index, void* arg);
