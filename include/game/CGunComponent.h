#pragma once

// CGunComponent: functions and globals of this translation unit that other files use (tools/tu_map.py unit 1304,
// 0x710083751c-0x71008424e4). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "types.h"

struct UnkGunComponent;
struct UnkGunComponent_l838f20;  // the component as its destructor reads it
void fn_7100838f20(UnkGunComponent_l838f20* self);  // the destructor; derived guns end in it
struct UnkGunBeam_ovb0070;
struct UnkSamusGun_ovb0070;
struct UnkComponent;
struct UnkComponent_710035131c;
struct UnkItem_710083e9d4;
struct UnkListener_b40;
struct UnkSamusGunComponent_b50;
struct UnkStruct_710083e9d4;
void* fn_710083cd30(UnkComponent_710035131c* component);
void fn_7100839b00(char* self, bool value);
void fn_710083b0bc(void* owner, UnkListener_b40* listener);
UnkItem_710083e9d4* fn_710083e9d4(UnkStruct_710083e9d4* self, const u64* id, bool primary);
void fn_710083ad6c(UnkSamusGunComponent_b50* gun);
char* fn_710083cd4c(UnkSamusGunComponent_b50* self);
bool fn_710083f32c(UnkSamusGunComponent_b50* gun, const base::global::CStrId* id, bool a, bool b, bool c);
void fn_710083d27c(UnkSamusGunComponent_b50* gun, bool flag);
bool fn_710084088c(UnkSamusGunComponent_b50*);
void fn_7100840f50(void*, void**);
const base::global::CStrId* fn_71008407fc(UnkGunComponent* self);
UnkGunBeam_ovb0070* fn_7100840828(UnkSamusGun_ovb0070* self);
base::reflection::UnkReflObject* fn_71008407d8(UnkGunComponent* self);
extern void* g_unk_7101a77bc0[];
extern void* g_unk_7101a77dc8[];
