#pragma once

// UnkTU259: functions and globals of this translation unit that other files use (tools/tu_map.py unit 259,
// 0x71001c6960-0x71001d948c). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

struct UnkCountedResource;
struct UnkGraphicsSystem;
struct UnkGlobal_710b372a98;
struct UnkMaskedEntry_b67a1;
struct UnkRenderTargetA1;
struct UnkStruct_71001cdd74;
void fn_71001cdba4(UnkRenderTargetA1* target, void* block);
void fn_71001cb604(UnkRenderTargetA1* target, u32 a, void* b, void* c, void* d, void* e);
void fn_71001cdd18(UnkRenderTargetA1* target);
void fn_71001cdd70(UnkRenderTargetA1* target);
void fn_71001cdd74(UnkRenderTargetA1* target, void* block);
void fn_71001cde48(UnkRenderTargetA1* target);
void fn_71001cafa0(UnkStruct_71001cdd74* self, s32 a, s32 b, s32 c);
void fn_71001cc550(UnkRenderTargetA1* target, u32 a, u32 b, u32 c);
void fn_71001cc570(UnkRenderTargetA1* target);
void fn_71001ce704(UnkRenderTargetA1* target, u32 value);
void fn_71001ced08(UnkRenderTargetA1* target, u8 value);
void fn_71001d0b70(UnkRenderTargetA1* target, void* p, u32 n, s32 a, s32 b);
void fn_71001d0d10(void* self, UnkMaskedEntry_b67a1* entry);
void fn_71001d1e78(UnkRenderTargetA1* target, f32 a, f32 b, f32 c, f32 d);
extern u32 g_unk_716e554938;
void fn_71001d0638(UnkGlobal_710b372a98* gfx, void* item, void* a, u32 b);
f32 fn_71001d7320(UnkGlobal_710b372a98* self);
void fn_71001cdd6c(UnkRenderTargetA1* target, base::global::CStrId id);
void fn_71001cae0c(UnkGraphicsSystem* self, const base::global::CStrId* name, UnkCountedResource* texture, void* a);
