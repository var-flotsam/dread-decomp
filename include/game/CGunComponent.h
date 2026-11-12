#pragma once

// CGunComponent: functions of this translation unit that other files call (tools/tu_map.py unit 1304,
// 0x710083751c-0x71008424e4). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

struct UnkComponent;
struct UnkItem_710083e9d4;
struct UnkListener_b40;
struct UnkSamusGunComponent;
struct UnkStruct_710083e9d4;
void* fn_710083cd30(UnkComponent* component);
void fn_7100839b00(char* self, bool value);
void fn_710083b0bc(void* owner, UnkListener_b40* listener);
UnkItem_710083e9d4* fn_710083e9d4(UnkStruct_710083e9d4* self, const u64* id, bool primary);
void fn_710083ad6c(UnkSamusGunComponent* gun);
char* fn_710083cd4c(UnkSamusGunComponent* self);
void fn_710083f32c(UnkSamusGunComponent* gun, const base::global::CStrId* id, bool a, bool b, bool c);
