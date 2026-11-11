#pragma once

// CAnimationComponent: functions of this translation unit that other files call (tools/tu_map.py unit 2720,
// 0x7101174bd0-0x710117f300). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

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
void fn_710117abfc(void* target, u32 a1, void* a2, u64 id);
void fn_710117ad18(UnkActionSwitcherAnimation_b48* self, bool flag, f32 value);
void fn_7101176a88(UnkActorTarget* target, u32 value);
bool fn_7101178524(UnkAnimFlagsHolder_b47a3* self, u32 bit);
bool fn_7101179870(UnkStateSet_1441* self);
void fn_710117774c(UnkShakernautCtrl_b53* ctrl);
void fn_7101178010(void* target, u64 id, bool flag, f32 time);
void fn_7101179878(UnkAnimActionSetsOwner* anim, void* entry, u32 a, u64 key, UnkMtx44_b46a2* out, u32 b, f32 c);
void fn_710117ab64(UnkAnimationComponent* self, u32 value, const void* arg);
