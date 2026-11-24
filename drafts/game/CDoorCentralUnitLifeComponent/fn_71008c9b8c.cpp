// CDoorCentralUnitLifeComponent's start: links the +0xc10 handle to the owner's component, requests state 2 of the
// machine at +0x420 and the shield request when the manager says so, then walks the central unit list.
// Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CAutsharpAIComponent.h"
#include "game/CBossAIComponent.h"
#include "game/CDroppableLifeComponent.h"
#include "game/UnkBroadphaseObject.h"
#include "game/UnkCentralUnitList.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkTU0.h"
#include "game/UnkModeFlags.h"
#include "game/UnkStateMachineFind.h"
#include "game/UnkTU1176.h"
#include "game/UnkTU2547.h"
#include "game/UnkTU794.h"
#include "game/UnkTU811.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

struct UnkTutoHandle;
void fn_7100565d38(UnkTutoHandle* handle, UnkTutoHandle** slot);
void fn_71008d0250(void* self);
void fn_71008ca0b8(void* self, bool closed);
bool fn_7100dae840(UnkEmmyRegistry_b55* registry);
bool fn_7100ddb8cc(UnkDoorRegistry_b61* registry);
void* fn_71000839a0(u64 size, u32 flags);

struct UnkSmState_l8c9b8c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);
    virtual void vf_0x18(s32 next);
    u64 field_0x8;
    u32 id;
};

typedef UnkSmState_l8c9b8c* __attribute__((aligned(4))) UnkSmStatePtr_l8c9b8c;

struct UnkSmMachine_l8c9b8c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkSmState_l8c9b8c* previous, UnkSmState_l8c9b8c* next);
    virtual void vf_0x28(UnkSmState_l8c9b8c* current, UnkSmState_l8c9b8c* next);
    u64 field_0x8;
    UnkSmState_l8c9b8c* current;
    UnkSmState_l8c9b8c* previous;
    u32 count;
    UnkSmStatePtr_l8c9b8c states[16];
    u32 pending;
    bool busy;
};

template <u32 None>
static inline void SwitchState_l8c9b8c(UnkSmMachine_l8c9b8c* self, u32 id, UnkSmState_l8c9b8c* current) {
    if (!current || current->id != id) {
        UnkSmState_l8c9b8c* found = FindStateById(self->states, self->count, id);
        UnkSmState_l8c9b8c* next;
        u32 enter;
        if (found) {
            next = found;
            enter = true;
        } else {
            if (id != 0)
                goto done;
            enter = false;
            next = nullptr;
        }
        if (current) {
            current->vf_0x18(id);
            self->vf_0x28(self->current, next);
        }
        self->previous = self->current;
        self->current = found;
        if (enter) {
            typedef void (*EnteredFn)(UnkSmState_l8c9b8c*, s32);
            s32 previous_id = self->previous ? self->previous->id : None;
            EnteredFn entered = (*reinterpret_cast<EnteredFn**>(found))[2];
            entered(next, previous_id);
            self->vf_0x20(self->previous, next);
        }
    }
done:
    self->pending = None;
}

struct UnkDoorOwner_l8c9b8c {
    u8 pad_0x0[0x220];
    void* field_0x220;
    u8 pad_0x228[0x2a0 - 0x228];
    base::reflection::UnkReflObject* field_0x2a0;
};

struct UnkDoorLife_l8c9b8c {
    u8 pad_0x0[0x30];
    UnkDoorOwner_l8c9b8c* owner;
    u8 pad_0x38[0x420 - 0x38];
    UnkSmMachine_l8c9b8c machine;
    u8 pad_0x4cc[0xc08 - 0x4cc];
    s32 field_0xc08;
    u8 pad_0xc0c[4];
    UnkTutoHandle* handle;
    u8 pad_0xc18[0xc48 - 0xc18];
    u8 field_0xc48;
    u8 field_0xc49;
};

extern base::global::CStrId g_unk_7172730f68;
extern const u8 g_unk_710164b080[] __attribute__((visibility("hidden")));
extern const u8 g_unk_710164b088[] __attribute__((visibility("hidden")));

void fn_71008c9b8c(UnkDoorLife_l8c9b8c* self) {
    fn_71008d0250(self);
    if (char* part = reinterpret_cast<char*>(fn_71005012bc(reinterpret_cast<UnkDoorLifeOwner_b56*>(self->owner)))) {
        if (self->handle) {
            fn_7100565ef0(self->handle, &self->handle);
            self->handle = nullptr;
        }
        self->handle = reinterpret_cast<UnkTutoHandle*>(part + 0x68);
        fn_7100565d38(self->handle, &self->handle);
    }
    if (!self->field_0xc48 || g_unk_71726bc570->field_0xb8) {
        // falls through to the common path
    } else {
        self->field_0xc49 = 1;
        self->machine.pending = 1;
        if (!self->machine.busy)
            SwitchState_l8c9b8c<0x7fffffff>(&self->machine, 1, self->machine.current);
    }
    fn_71008ca0b8(self, true);
    if (fn_7100dae840(g_unk_71726af0a8)) {
        UnkDoorRegistry_b61* registry = g_unk_71726bee10;
        if (!registry) {
            registry = static_cast<UnkDoorRegistry_b61*>(fn_71000839a0(4, 0x3f));
            fn_7100ddb6dc(registry);
            g_unk_71726bee10 = registry;
        }
        if (fn_7100ddb8cc(registry)) {
            if (void* target = self->owner->field_0x220)
                UnkSpitSend(target, 0, 0xf4061ff25a2163d6);
            self->machine.pending = 2;
            if (!self->machine.busy)
                SwitchState_l8c9b8c<0x7fffffff>(&self->machine, 2, self->machine.current);
            if (self->owner->field_0x2a0) {
                if (void* entry = fn_71008640a4(reinterpret_cast<UnkCollisionContainer*>(self->owner->field_0x2a0),
                                                 &g_unk_7172730f68))
                    fn_7100d35a98(reinterpret_cast<UnkEntry*>(entry), false);
            }
        }
    }
    UnkCentralUnitList* list = fn_7100772c98();
    for (u32 i = 0; i < list->count; ++i) {
        char* unit = reinterpret_cast<char*>(list->data[i]);
        u64 id;
        CStrId_Construct(&id, "PRP_CV_CentralUnitProto", true);
        if (unit) {
            (void)fn_71003b0c4c(reinterpret_cast<UnkZiplineLink_b49*>(unit + 0x438));
        }
    }
    if (self->field_0xc08 == 1)
        fn_7100d7c828(reinterpret_cast<UnkActor*>(self->owner), reinterpret_cast<const u64*>(g_unk_710164b080), false);
}
