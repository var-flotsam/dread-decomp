// behaviortree::CSwitchBetweenAIModesAction (tools/tu_map.py unit 449): the node's start. After the base
// routine (fn_7100394bd0) it caches the owner's (+0x18) AI (+0x240) and animation (+0x220) and stops when either is
// missing; with the flag at +0x6c set, an owner sink (+0x218) in state 4 (fn_71010cc3a4) only sets the done flag
// (+0x90). Otherwise it clears the state bytes, sets the enter id's (+0x58) hash on the owner's timeline (+0x2e8)
// to the all-flags mask and removes the exit id's (+0x60), hooks the callback fn_710036c338 (link at +0x98,
// handle at +0xb0) up to the animation as src/game/CChozoWarriorXGlaiveSpinAttack/UnkAttackTrackLink_b54.cpp,
// copies the mode name (+0x30 -> +0x70) and, when the name is empty, asks the AI (slot 0x610) to switch with
// the two values at +0x48 and the copied name. Names and layouts are placeholders.
#include "../../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "game/CEnemyMovement.h"
#include "game/UnkActor.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStrId;

struct UnkSelf;
void fn_7100394bd0(UnkSelf*);
void fn_7100316018(UnkWeakTarget* self, UnkWeakHandle** out);
void fn_710117b3e8(UnkWeakTarget* target, void* link);
void fn_710036c338();

struct UnkSwitchModesAI_A2_b75 {
    void** vtable;
};

struct UnkSwitchModesLink_A2_b75 {
    u8 pad_0x0[0x8];
    void* context;
    void (*callback)();
};

struct UnkSwitchModesAction_A2_b75 {
    u8 pad_0x0[0x18];
    UnkActor* owner;  // 0x18
    u8 pad_0x20[0x10];
    CStrId name;  // 0x30
    u8 pad_0x38[0x10];
    u32 field_0x48;
    u32 field_0x4c;
    u8 pad_0x50[0x8];
    UnkStrId enter;  // 0x58
    UnkStrId exit;   // 0x60
    u16 field_0x68;
    u8 field_0x6a;
    u8 pad_0x6b;
    bool check_sink;  // 0x6c
    u8 pad_0x6d[0x3];
    CStrId current;  // 0x70
    u8 pad_0x78[0x2];
    u8 field_0x7a;
    u8 pad_0x7b[0x5];
    UnkSwitchModesAI_A2_b75* ai;  // 0x80
    void* anim;                   // 0x88
    bool done;                    // 0x90
    u8 pad_0x91[0x7];
    UnkSwitchModesLink_A2_b75 link;  // 0x98
    UnkWeakHandle* handle;           // 0xb0
};

void fn_710036b578(UnkSwitchModesAction_A2_b75* self) {
    fn_7100394bd0(reinterpret_cast<UnkSelf*>(self));
    UnkActor* owner = self->owner;
    if (!(self->ai = static_cast<UnkSwitchModesAI_A2_b75*>(owner->field_0x240)))
        return;
    if (!(self->anim = owner->field_0x220))
        return;
    self->done = false;
    if (self->check_sink && owner->field_0x218 && fn_71010cc3a4(owner->field_0x218) == 4) {
        self->done = true;
        return;
    }
    self->field_0x68 = 0;
    self->field_0x6a = 0;
    self->field_0x7a = 0;
    UnkTimelineComponent* timeline =
        self->owner ? static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8) : nullptr;
    base::reflection::UnkReflTypeIdArg mask;
    u64 id;
    if ((timeline != nullptr) & (self->enter.data != &g_unk_7101d47108->empty)) {
        id = *reinterpret_cast<u64*>(reinterpret_cast<u8*>(self->enter.data) + 0x30);
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, &id, &mask.id, false, -1.0f);
    }
    if ((timeline != nullptr) & (self->exit.data != &g_unk_7101d47108->empty)) {
        id = *reinterpret_cast<u64*>(reinterpret_cast<u8*>(self->exit.data) + 0x30);
        fn_7100a2b7b8(timeline, &id);
    }
    UnkSwitchModesLink_A2_b75* link = &self->link;
    link->context = self;
    link->callback = fn_710036c338;
    UnkWeakTarget* track = static_cast<UnkWeakTarget*>(self->owner->field_0x220);
    if (UnkWeakHandle* handle = self->handle) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, link);
        if (UnkWeakHandle* p = self->handle) {
            if (--p->ref_count == 0)
                p->vf_0x10();
            self->handle = nullptr;
        }
    }
    if (track) {
        asm("" : "+r"(self));
        fn_7100316018(track, &self->handle);
        fn_710117b3e8(self->handle ? self->handle->target : nullptr, link);
    }
    self->current = self->name;
    if (self->name.data == &g_unk_7101d47108->empty) {
        if (UnkSwitchModesAI_A2_b75* ai = self->ai)
            reinterpret_cast<void (*)(UnkSwitchModesAI_A2_b75*, u32, u32, CStrId*)>(ai->vtable[0x610 / 8])(
                ai, self->field_0x48, self->field_0x4c, &self->current);
    }
}
