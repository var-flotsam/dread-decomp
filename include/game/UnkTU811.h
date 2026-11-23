#pragma once

// UnkTU811: functions and globals of this translation unit that other files use (tools/tu_map.py unit 811,
// 0x7100537bf0-0x710056a378). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/global/CRntVector.h"
#include "types.h"

struct UnkHandleAI_b0193;
struct UnkEmmyAI_b60;
struct UnkRecord_54715c;
struct UnkEmmyForestAI_A2_b70;
struct UnkLuaDoorTarget;
struct UnkListedItem;
struct UnkActor;
struct UnkEmmyRelaxAI_b59;
struct UnkComponent;
struct UnkEmmyAI;
struct UnkEmmyAI2_b57a3;
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
UnkSlidingBlockMovement* fn_710054a2cc(void* entity);  // The entity's typed component lookup.
bool fn_710054aecc(UnkEmmyAI2_b62a3* self);
f32 fn_7100552ee4(UnkEmmyAI_b61* ai);
void fn_7100541de4(UnkEmmyActor_b55* self);
f32 fn_7100552d94(UnkSelf* self);
f32 fn_7100552e3c(UnkEmmyAI* ai);
void fn_7100554740(void* self, void* arg);
void fn_7100556c00(UnkEmmyAI_b0068* ai, bool pending, const UnkEmmySearchCallback_b0068* a, const UnkEmmySearchCallback_b0068* b, u32 c, void* data, const UnkEmmySearchCallback_b0068& d);
void fn_710056a010(void* set, const void* value);
extern char g_unk_7101a48bc0[];
extern void* g_unk_7101a48c20[];
extern char g_unk_71726e20a8[] __attribute__((visibility("hidden")));
void fn_7100546a30(UnkEmmyRelaxAI_b59* self, u32 state, UnkActor* other, void* arg);
extern base::global::CStrId g_unk_71726e1f68 __attribute__((visibility("hidden")));
base::global::CRntVector<UnkListedItem*>* fn_7100552d7c();
void fn_71005479a0(UnkEmmyRelaxAI_b59* self);
// Hands the game manager the string id for the barrier state (the barrier or no-barrier name, or the default).
void fn_710054095c(UnkEmmyAI2_b57a3* self);
void fn_7100540dd0(char* self);
UnkLuaDoorTarget* fn_710055b104(void* object);
void fn_7100547428(UnkEmmyActor_b55* self, s32 state, UnkActor* target, bool b);
bool fn_710054782c(UnkEmmyRelaxAI_b59* self);
void fn_710054d16c(UnkEmmyForestAI_A2_b70* self, f32 dt);
void fn_7100541fd4(UnkEmmyAI_b60* self);
UnkRecord_54715c* fn_710054715c(UnkRecord_54715c* self, const UnkRecord_54715c* other);
void fn_7100552324(UnkEmmyRelaxAI_b59* self);
struct UnkEmmyAI_l541974;  // the E.M.M.I. AI component as its stop reads it
void fn_7100541974(UnkEmmyAI_l541974* self);  // stops the AI; derived E.M.M.I.s call it first
void fn_7100541e64(UnkHandleAI_b0193* self);
