// CCooldownXBossLavaDropsAttack's unit (tools/tu_map.py): starts the attack. After the attack base's start it
// asks the state machine at +0x68 for state 1; then, when the owner's AI component is a
// CCooldownXBossAIComponent and the owner has a character class, copies the drop position, rebinds the
// listener at +0x328 to the character class, picks the drop pattern and sends the four lava tracks, each in
// the variant its pattern flag selects. Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game/CCooldownXBossAttack.h"
#include "game/UnkActor.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::reflection::UnkReflObject;
using base::reflection::UnkReflTypeIdArg;

extern u64 g_unk_7101649ad0;  // reached through the GOT
extern u64 g_unk_7101649ad8;  // reached through the GOT

void fn_71004c94a4();

struct UnkLavaDropsMachine_l4c65c4;

struct UnkLavaDropsState_l4c65c4;
struct UnkLavaDropsStateVtable_l4c65c4 {
    void* unk_0x0[2];
    void (*entered)(UnkLavaDropsState_l4c65c4* self, s32 previous);  // 0x10
    void (*left)(UnkLavaDropsState_l4c65c4* self, s32 next);         // 0x18
};

struct UnkLavaDropsState_l4c65c4 {
    UnkLavaDropsStateVtable_l4c65c4* vtable;
    UnkLavaDropsMachine_l4c65c4* machine;
    u32 id;
};

struct UnkLavaDropsMachineVtable_l4c65c4 {
    void* unk_0x0[4];
    void (*entered)(UnkLavaDropsMachine_l4c65c4* self, UnkLavaDropsState_l4c65c4* previous,
                    UnkLavaDropsState_l4c65c4* next);  // 0x20
    void (*left)(UnkLavaDropsMachine_l4c65c4* self, UnkLavaDropsState_l4c65c4* current,
                 UnkLavaDropsState_l4c65c4* next);  // 0x28
};

struct UnkLavaDropsMachine_l4c65c4 {
    UnkLavaDropsMachineVtable_l4c65c4* vtable;
    void* owner;
    UnkLavaDropsState_l4c65c4* current;
    UnkLavaDropsState_l4c65c4* previous;
    u32 count;
    u32 states[0x20];  // UnkLavaDropsState_l4c65c4* entries, 4-byte aligned
    UnkLavaDropsState_l4c65c4** States() { return reinterpret_cast<UnkLavaDropsState_l4c65c4**>(states); }
    u32 pending;
    bool busy;
    bool field_0xa9;

    UnkLavaDropsState_l4c65c4* Find(u32 id) {
        UnkLavaDropsState_l4c65c4** begin = States();
        UnkLavaDropsState_l4c65c4** end = begin + count;
        UnkLavaDropsState_l4c65c4** p = begin;
        for (; p != end; ++p)
            if ((*p)->id == id)
                break;
        return p != end ? *p : nullptr;
    }

    // Switches to the state `id` unless the machine is busy; `pending` holds it meanwhile.
    void Request(u32 id) {
        pending = id;
        if (busy)
            return;
        UnkLavaDropsState_l4c65c4* old = current;
        if (!old || old->id != id) {
            if (UnkLavaDropsState_l4c65c4* next = Find(id)) {
                if (old) {
                    old->vtable->left(old, id);
                    vtable->left(this, current, next);
                }
                previous = current;
                current = next;
                s32 previous_id = previous ? previous->id : 5;
                next->vtable->entered(next, previous_id);
                vtable->entered(this, previous, next);
            }
        }
        pending = 5;
    }
};
static_assert(sizeof(UnkLavaDropsMachine_l4c65c4) == 0xb0, "layout");

struct UnkLavaDropsListener_l4c65c4 {
    void** vtable;
    void* object;
    void (*function)();
};

struct UnkLavaDropsPatternInfo_l4c65c4 {
    u8 pad_0x0[0xc];
    u32 field_0xc;
};

struct UnkLavaDropsPattern_l4c65c4 {
    u8 pad_0x0[0x8];
    u32 flags[4];  // 0x8: one per track
    UnkLavaDropsPatternInfo_l4c65c4* info;  // 0x18
};

UnkLavaDropsPattern_l4c65c4* fn_71004ae918(void* patterns, u32 id);

struct UnkLavaDropsSetup_l4c65c4 {
    u32 pattern;  // 0x0
    u8 pad_0x4[0xcc];
    UnkVec3f position;  // 0xd0
};

struct UnkLavaDropsAI_l4c65c4 {
    u8 pad_0x0[0x15750];
    UnkLavaDropsSetup_l4c65c4 setup;
};

struct UnkLavaDropsAttack_l4c65c4 {
    u8 pad_0x0[0x8];
    UnkActor* owner;
    u8 pad_0x10[0x58];
    UnkLavaDropsMachine_l4c65c4 machine;  // 0x68
    void* patterns;  // 0x118
    u8 pad_0x120[0x1d0];
    UnkVec3f position;  // 0x2f0
    u32 field_0x2fc;
    u32 field_0x300;
    u8 pad_0x304[0x4];
    UnkLavaDropsPattern_l4c65c4* pattern;  // 0x308
    u32 flags[4];  // 0x310
    u8 pad_0x320[0x8];
    UnkLavaDropsListener_l4c65c4 listener;  // 0x328
    UnkWeakHandle* handle;  // 0x340
};
static_assert(__builtin_offsetof(UnkLavaDropsAttack_l4c65c4, handle) == 0x340, "layout");

// Sends the lava track `kind` in the variant `flag` selects.
static inline void SendTrack(void* anim, u64 kind, u32 flag, UnkReflTypeIdArg& mode, UnkReflTypeIdArg& mask) {
    mode.id = 0;
    const u64& hash = flag == 0 ? g_unk_7101649ad0 : g_unk_7101649ad8;
    mask.id = UnkAll_SpitSend().value;
    CAnimationComponent_SendActionToTrack(anim, hash, kind, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
}

void fn_71004c65c4(UnkLavaDropsAttack_l4c65c4* self) {
    fn_71004c1944(reinterpret_cast<UnkSelf*>(self));
    self->machine.Request(1);
    UnkReflObject* object = static_cast<UnkReflObject*>(self->owner->field_0x240);
    if (!object)
        return;
    object = fn_710009d50c(object->vf_0x0(), fn_71004cf318()) ? object : nullptr;
    if (!object)
        return;
    void* anim = self->owner->field_0x220;
    if (!anim)
        return;
    UnkLavaDropsSetup_l4c65c4& setup = reinterpret_cast<UnkLavaDropsAI_l4c65c4*>(object)->setup;
    self->position = setup.position;
    self->listener.object = self;
    self->listener.function = fn_71004c94a4;
    if (UnkWeakHandle* handle = self->handle) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, &self->listener);
        ResetRef(&self->handle);
    }
    CAbilityGhostAura_AssignWeakHandleToRef(static_cast<UnkWeakTarget*>(anim), &self->handle);
    fn_710117b3e8(self->handle ? self->handle->target : nullptr, &self->listener);
    self->field_0x2fc = 0;
    self->pattern = fn_71004ae918(self->patterns, setup.pattern);
    self->field_0x300 = self->pattern->info->field_0xc;

    UnkReflTypeIdArg mode1, mask1, mode2, mask2, mode3, mask3, mode4, mask4;
    self->flags[0] = self->pattern->flags[0];
    SendTrack(anim, 0x913c5999cc2002f8, self->flags[0], mode1, mask1);
    self->flags[1] = self->pattern->flags[1];
    SendTrack(anim, 0xfc0afa18dd48957d, self->flags[1], mode2, mask2);
    self->flags[2] = self->pattern->flags[2];
    SendTrack(anim, 0x8b59361142c681d2, self->flags[2], mode3, mask3);
    self->flags[3] = self->pattern->flags[3];
    SendTrack(anim, 0x8bb4f4733ec20d8c, self->flags[3], mode4, mask4);
}
