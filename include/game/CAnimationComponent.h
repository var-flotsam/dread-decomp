#pragma once

// CAnimationComponent: functions of this translation unit that other files call (tools/tu_map.py unit 2720,
// 0x7101174bd0-0x710117f300). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/reflection/CClass.h"
#include "base/global/CStrId.h"
#include "types.h"

struct UnkFlags_7101176ea8;
struct UnkS12;
struct UnkS16;
struct UnkTrack;
struct UnkWeakTarget;
struct UnkActionSwitcherAnimation_b48;
struct UnkActorTarget;
struct UnkAnimActionSetsOwner;
struct UnkAnimFlagsHolder_b47a3;
struct UnkAnimationComponent;
struct UnkCls_710117af10;
struct UnkMtx44_b46a2;
struct UnkRequest_7101176ea8;
struct UnkShakernautCtrl_b53;
struct UnkShakernautId_b53;
struct UnkStateSet_1441;
struct UnkStruct_7101176ea8;
struct UnkStruct_710117af10;
struct UnkTakumakuComponent;
void fn_71011785a8(UnkShakernautCtrl_b53*, const UnkShakernautId_b53*, bool, bool);
void fn_7101178564(UnkShakernautCtrl_b53* self, const UnkShakernautId_b53* id, bool a, bool b);
bool fn_7101178e6c(UnkTakumakuComponent* component, u32 kind);
bool fn_710117af10(UnkStruct_710117af10*, UnkCls_710117af10**, bool);
bool fn_7101176ea8(UnkStruct_7101176ea8* self, const UnkRequest_7101176ea8* req, u64 key);
void* fn_7101176e0c(UnkStruct_7101176ea8* self, u64 a, u64 b);
void fn_710117abfc(void* target, bool, void* a2, u64 id);
void fn_710117ad18(UnkActionSwitcherAnimation_b48* self, bool flag, f32 value);
void fn_7101176a88(UnkActorTarget* target, bool);
bool fn_7101178524(UnkAnimFlagsHolder_b47a3* self, u32 bit);
bool fn_7101179870(UnkStateSet_1441* self);
void fn_710117774c(UnkShakernautCtrl_b53* ctrl);
void fn_7101178010(void* target, u64 id, bool flag, f32 time);
void fn_7101179878(UnkAnimActionSetsOwner* anim, void* entry, u32 a, u64 key, UnkMtx44_b46a2* out, u32 b, f32 c);
void fn_710117ab64(UnkAnimationComponent* self, bool, const void* arg);
void fn_710117b3e8(UnkWeakTarget* target, void* link);
void* fn_7101177d2c(UnkAnimationComponent* self, u64 key);
bool fn_7101179c3c(UnkAnimationComponent* self, u64 key);
UnkTrack* fn_71011771e0(void* handle, u64 key);
bool fn_71011782ec(UnkAnimationComponent* self, u64 key);
void* fn_7101176d34(UnkStruct_7101176ea8* self, void* arg);  // The entry for a key (a name's hash).
s32 fn_7101177da8(void* object, u64 hash);
bool fn_710117aaac(void* cls, const base::reflection::CClass* track_class, f32 frames);
bool fn_7101177044(UnkStruct_7101176ea8* self, void* r, s32 idx, void* data, UnkFlags_7101176ea8 flags, const UnkS16& a, const UnkS12& b, UnkFlags_7101176ea8 c);
const base::global::CStrId* fn_7101177ae8(UnkAnimationComponent* self, u64 kind);
