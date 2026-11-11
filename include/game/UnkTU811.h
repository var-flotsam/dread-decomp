#pragma once

// UnkTU811: functions of this translation unit that other files call (tools/tu_map.py unit 811,
// 0x7100537bf0-0x710056a378). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkComponent;
struct UnkEmmyAI;
struct UnkEmmyAI2_b62a3;
struct UnkEmmyAIComponent_b46a1;
struct UnkEmmyAIView;
struct UnkEmmyAI_b0068;
struct UnkEmmyAI_b61;
struct UnkEmmyActor_b55;
struct UnkEmmySearchCallback_b0068;
struct UnkSelf;
struct UnkSlidingBlockMovement;
struct UnkTutoHandle;
bool fn_71005436e8(UnkEmmyAIComponent_b46a1* self);
void fn_7100544a34(UnkEmmyActor_b55* self, char* data, bool on);
void fn_7100565ef0(UnkTutoHandle* handle, UnkTutoHandle** slot);
bool fn_710054ad2c(UnkEmmyAIView* self);
void fn_710053e5c0(UnkComponent* self);
void fn_710053f510(UnkComponent* self);
bool fn_7100543b30(UnkEmmyAIView* self);
UnkSlidingBlockMovement* fn_710054a2cc(void* entity);
bool fn_710054aecc(UnkEmmyAI2_b62a3* self);
f32 fn_7100552ee4(UnkEmmyAI_b61* ai);
void fn_7100541de4(UnkEmmyActor_b55* self);
f32 fn_7100552d94(UnkSelf* self);
f32 fn_7100552e3c(UnkEmmyAI* ai);
void fn_7100554740(void* self, void* arg);
void fn_7100556c00(UnkEmmyAI_b0068* ai, bool pending, const UnkEmmySearchCallback_b0068* a, const UnkEmmySearchCallback_b0068* b, u32 c, void* data, const UnkEmmySearchCallback_b0068& d);
void fn_710056a010(void* set, const void* value);
