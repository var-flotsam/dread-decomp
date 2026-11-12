#pragma once

// reflection: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2525,
// 0x7100d8c56c-0x7100d911d8). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkAssignedRecord_a1ba4;
struct UnkCompared_0x18;
struct UnkFormatArgs;
struct UnkItem;
struct UnkScene;
struct UnkSceneItem;
struct UnkTextBuffer200;
void fn_7100d8ced8(void* self);  // Constructor.
void fn_71000a1ba4(UnkAssignedRecord_a1ba4* self, const UnkAssignedRecord_a1ba4* other);
bool fn_71000a1ed4(const UnkCompared_0x18* a, const UnkCompared_0x18* b);
void fn_7100d8e0a0(void* manager);

// Out-of-line destructor of a music preset map's values (src/game/sound/UnkMusicPresets.cpp).
void fn_7100d90304(void*);

void fn_71000a1784(UnkFormatArgs* args, UnkTextBuffer200* out);
void* fn_7100202574(UnkItem* item);
void fn_7100d8ef04(void* self, f32 dt);
void fn_7100d8f9e4(void*);
void fn_710009aae8(void* self);
void fn_71001eed18(void* self, void* item);
bool fn_71001efb44(UnkScene* scene, bool flag, const u64* id);
void fn_7100200138(UnkItem* item, bool value);
bool fn_7100200144(UnkItem* item);
void fn_710020014c(UnkItem* item, bool value);
void fn_7100200160(UnkSceneItem* item, f32 dt);
bool fn_7100202d24(UnkItem* item, void* arg);
void fn_7100adc470();
void fn_7100adc4c0();
void fn_7100b03ecc();
void fn_7100b75ea8();
void fn_7100b76544();
void fn_7100b76b28();
void fn_7100b7d05c();
void fn_7100b7db14();
void fn_7100b7e0f8();
void fn_7100d8efd0(void* self, bool a, bool b, bool c);
extern u8 g_unk_7172757530[];
extern char g_unk_7172758254;
extern char g_unk_7172758255;
extern char g_unk_7172758256;
extern char g_unk_717275dfb8;
extern char g_unk_717275dfb9;
extern char g_unk_7172766268;
extern char g_unk_7172766269;
extern char g_unk_717276626a;
extern char g_unk_717276626b;
extern char g_unk_71727663a8;
extern char g_unk_71727663a9;
extern char g_unk_71727664e8;
extern char g_unk_71727664e9;
extern char g_unk_7172766660;
extern char g_unk_7172767138;
extern char g_unk_7172767139;
extern char g_unk_717276713a;
extern char g_unk_717276713b;
extern char g_unk_7172767278;
extern char g_unk_7172767279;
extern char g_unk_71727673b8;
extern char g_unk_71727673b9;
extern char g_unk_7172767530;
void fn_710025bda0(void* self);
