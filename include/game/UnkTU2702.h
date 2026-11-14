#pragma once

// UnkTU2702: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2702,
// 0x71010f41a0-0x71011173fc). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/reflection/CClass.h"
#include "types.h"

struct UnkAIBB_b0144;
struct UnkPropertyName;
struct UnkAIComponent;
struct UnkAabb;
struct UnkBigFistAI3_ovb0050;
struct UnkBigFistKind_ovb0050;
struct UnkBigFistLimit_ovb0050;
struct UnkBoundsComponent;
struct UnkBtAI_A2_b76;
struct UnkChozoRobotSoldierAI_b66a2;
struct UnkChozoWarriorAI_b66a2;
struct UnkCompared_0x48;
struct UnkComponent_7100286798;
struct UnkDaivoAIComponent_b46a1;
struct UnkDaivoArg_b46a1;
struct UnkEmmyCaveAIStart_b49;
struct UnkEntry;
struct UnkEntryVector;
struct UnkGoliathAI_b55;
struct UnkGoliathAI_b58a3;
struct UnkGoliathHitInfo_b55;
struct UnkGoliathHitTarget_b55;
struct UnkKraidAI2;
struct UnkKraidRequest;
struct UnkNailongAI_b46a2;
struct UnkNailongMsg_b46a2;
struct UnkQuetzoaAIComponent;
struct UnkQuetzoaOther_ovb0134;
struct UnkQuetzoaXAI_ovb0134;
struct UnkScorpiusAI_b69a4;
class UnkSelf_7100369c44;
struct UnkSluggerAI;
struct UnkSluggerEvent;
struct UnkWarLotusAI;
void fn_710110cca4(void*, void*);
void fn_71010ff2b8(char* self);
void fn_7101105c90(UnkKraidAI2* self, void* arg, UnkKraidRequest* request);
void fn_7101105c94(UnkWarLotusAI* self);
u32 fn_71011061ac(UnkGoliathAI_b55* self, u32* out, void* arg2, UnkGoliathHitInfo_b55* info, UnkGoliathHitTarget_b55** hit);
bool fn_71010fc68c(UnkGoliathAI_b58a3* ai, f32* out);  // False when it has no value for `out` (its callers test it).
bool fn_71010fc778(void*, f32*);
u32 fn_7101105e44(UnkSluggerAI* self, UnkSluggerEvent* event);
void fn_7101108124(UnkQuetzoaAIComponent* self, void* arg);
void* fn_7101109df4(UnkScorpiusAI_b69a4* ai, void* id);
u32 fn_7101111dc8(UnkNailongAI_b46a2* self, UnkNailongMsg_b46a2* msg);
bool fn_71010fc284(UnkChozoWarriorAI_b66a2* self, f32* out);
bool fn_71010fc31c(UnkChozoWarriorAI_b66a2* self, f32* out);
void fn_710110f8ac(void* self, void* object);
void fn_71010f813c(UnkBtAI_A2_b76* self);
void fn_71010fa828(UnkAIComponent* object);
void* fn_71010fb398(UnkComponent_7100286798* self);
bool fn_71010fc39c(void*, f32*);
bool fn_71010fc864(UnkBigFistAI3_ovb0050* self, UnkBigFistKind_ovb0050* kind, UnkBigFistLimit_ovb0050* min, UnkBigFistLimit_ovb0050* max);
void fn_71011000e0(UnkSelf_7100369c44* self, f32 dt);
void fn_7101103aa8(UnkEmmyCaveAIStart_b49* self, f32 speed);
void fn_71011061d0(UnkBoundsComponent* self, UnkAabb* out);
void fn_7101108004(UnkAIBB_b0144* self, void* arg);
bool fn_7101108c9c(UnkDaivoAIComponent_b46a1* self, UnkDaivoArg_b46a1* arg);
void fn_710110bb4c(UnkQuetzoaXAI_ovb0134* self, UnkQuetzoaOther_ovb0134* other, u32 id, bool flag);
bool fn_710110f3cc(UnkChozoRobotSoldierAI_b66a2* self, u16 id, u32 a2, void* a3, void* a4, void* a5, u32 a6, u32 a7);
void fn_710110fae4(void* emmy, void* block);
void fn_7101112ab0(void* self);
bool fn_71011136b8(const UnkCompared_0x48* a, const UnkCompared_0x48* b);
void fn_7101114588(UnkEntryVector* v, const UnkEntry* value);
extern u8* g_unk_7172a062e0 __attribute__((visibility("hidden")));
void fn_710111094c(base::reflection::UnkReflObject* ai, u32 value);
extern base::reflection::CType g_unk_7172a05520;
extern UnkPropertyName g_unk_7172a06338 __attribute__((visibility("hidden")));
