#pragma once

// CInventoryComponent: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2710,
// 0x710113f978-0x7101149edc). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

struct UnkInventory;
struct UnkQualifiedId;
struct UnkObject_7101141558;
struct UnkStrIdHandle;
void fn_7101141378(UnkObject_7101141558*);
void fn_71011437bc(void* service, const UnkStrIdHandle& handle);
extern void* g_unk_7101cb33c8[];  // The class's vtable.
extern u64 g_unk_7172a092b0 __attribute__((visibility("hidden")));
f32 fn_7101142928(UnkInventory*, const UnkQualifiedId&);
void fn_7101142a28(UnkInventory* inventory, const UnkQualifiedId& item, bool a, bool b, bool c, f32 amount, f32 extra);
bool fn_71011428bc(UnkInventory* inventory, const UnkQualifiedId& item);
extern base::global::CStrId g_unk_7172a092b8 __attribute__((visibility("hidden")));
extern base::global::CStrId g_unk_7172a092c0 __attribute__((visibility("hidden")));
extern base::global::CStrId g_unk_7172a092c8 __attribute__((visibility("hidden")));
extern base::global::CStrId g_unk_7172a092d0 __attribute__((visibility("hidden")));
extern base::global::CStrId g_unk_7172a092d8 __attribute__((visibility("hidden")));
extern base::global::CStrId g_unk_7172a092e0 __attribute__((visibility("hidden")));
extern base::global::CStrId g_unk_7172a092e8 __attribute__((visibility("hidden")));
extern base::global::CStrId g_unk_7172a092f0 __attribute__((visibility("hidden")));
extern base::global::CStrId g_unk_7172a092f8 __attribute__((visibility("hidden")));
extern base::global::CStrId g_unk_7172a09300 __attribute__((visibility("hidden")));
extern base::global::CStrId g_unk_7172a09308 __attribute__((visibility("hidden")));
extern base::global::CStrId g_unk_7172a09310 __attribute__((visibility("hidden")));
extern base::global::CStrId g_unk_7172a09318 __attribute__((visibility("hidden")));
extern base::global::CStrId g_unk_7172a09320 __attribute__((visibility("hidden")));
extern base::global::CStrId g_unk_7172a09328 __attribute__((visibility("hidden")));
extern base::global::CStrId g_unk_7172a09330 __attribute__((visibility("hidden")));
extern base::global::CStrId g_unk_7172a09338 __attribute__((visibility("hidden")));
extern base::global::CStrId g_unk_7172a09340 __attribute__((visibility("hidden")));
extern base::global::CStrId g_unk_7172a09348 __attribute__((visibility("hidden")));
extern base::global::CStrId g_unk_7172a09350 __attribute__((visibility("hidden")));
extern base::global::CStrId g_unk_7172a09358 __attribute__((visibility("hidden")));
extern base::global::CStrId g_unk_7172a09360 __attribute__((visibility("hidden")));
extern base::global::CStrId g_unk_7172a09368 __attribute__((visibility("hidden")));
extern base::global::CStrId g_unk_7172a09378 __attribute__((visibility("hidden")));
extern base::global::CStrId g_unk_7172a09380 __attribute__((visibility("hidden")));
