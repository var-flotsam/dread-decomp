// Unit 220's setup function (tools/tu_map.py; no class hint): creates the two pooled objects (a
// 0x60-byte buffer and a 0x48-byte one) of the global billboard drawable, fills the buffer and the
// second object, and hands both to the drawable (src/game/UnkTU252/UnkTU252_Setup_ovb0221.cpp has the
// same calls on a holder). Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "game/UnkTU174.h"
#include "game/UnkTU182.h"
#include "game/UnkTU221.h"
#include "types.h"

struct UnkObj_7100161b90;
struct UnkObj_71001541a0;
struct UnkGfxBuffer252;
struct UnkBillboardDrawable;
struct UnkStruct_710019ab0c;

void fn_710019b184(UnkBillboardDrawable* self, u32 index, u32 part, const UnkMatrix44* matrix);
void fn_7100154380(UnkObj_71001541a0* self, u32 a, u32 b);
void fn_7100154270(UnkObj_71001541a0* self);
void fn_71001542f0(UnkObj_71001541a0* self, u32 a, u32 b, const void* data);
void fn_71001542d0(UnkObj_71001541a0* self);
extern UnkMatrix44 g_unk_7101a1a680;
extern char g_unk_7101a1e0d8[];
extern char g_unk_7101a1e138[];
extern u32 g_unk_7101a1e180;

struct UnkBillboardTarget_ovb0259 {
    u8 pad_0x0[0xc];
    u32 field_0xc;
};

struct UnkBillboardGlobal_ovb0259 {
    UnkObj_71001541a0* field_0x0;
    UnkObj_7100161b90* field_0x8;
    u8 pad_0x10[8];
    UnkBillboardTarget_ovb0259* field_0x18;
};
extern UnkBillboardGlobal_ovb0259 g_unk_716e352530;

struct UnkPair_ovb0259 {
    UnkObj_7100161b90* buffer;  // 0x0
    UnkObj_71001541a0* second;  // 0x8
};
extern UnkPair_ovb0259 g_unk_716e352670 __attribute__((visibility("hidden")));

void fn_710019a16c() {
    UnkPair_ovb0259* pair = &g_unk_716e352670;
    alignas(8) u32 first = 1;
    fn_7100161960(&pair->buffer, 0x60, nullptr, &first);
    alignas(8) u32 second = 1;
    fn_7100154020(&pair->second, 0x48, &second, nullptr, 1, 2);
    UnkBillboardGlobal_ovb0259* drawable = &g_unk_716e352530;
    fn_710019b184(reinterpret_cast<UnkBillboardDrawable*>(drawable), 0, 0, &g_unk_7101a1a680);
    fn_7100161d70(reinterpret_cast<UnkGfxBuffer252*>(pair->buffer));
    UnkGfxBuffer252* buffer = reinterpret_cast<UnkGfxBuffer252*>(pair->buffer);
    fn_7100161e60(buffer, 0, 0, 0, fn_71001622f0(3, 3), g_unk_7101a1e0d8, 0x60, 0);
    fn_7100161dc0(reinterpret_cast<UnkGfxBuffer252*>(pair->buffer));
    fn_7100161d60(reinterpret_cast<UnkGfxBuffer252*>(pair->buffer));
    fn_7100154380(pair->second, 1, 2);
    fn_7100154270(pair->second);
    fn_71001542f0(pair->second, 0, 0x48, g_unk_7101a1e138);
    fn_71001542d0(pair->second);
    drawable->field_0x8 = pair->buffer;
    fn_710019ab0c(reinterpret_cast<UnkStruct_710019ab0c*>(drawable));
    drawable->field_0x0 = pair->second;
    drawable->field_0x18->field_0xc = g_unk_7101a1e180;
    fn_710019ab0c(reinterpret_cast<UnkStruct_710019ab0c*>(drawable));
}
