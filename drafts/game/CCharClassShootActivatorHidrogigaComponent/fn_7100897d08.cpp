// CCharClassShootActivatorHidrogigaComponent's unit (tools/tu_map.py): while enabled (+0x602), charges
// the activator: the charge (+0x4bc) grows by its step (+0x4b4, five times as fast when the shot's
// flags hold any of 0x08000400) up to the maximum (+0x4b0; 99% of it while the Hydrogiga's state is not 2),
// when both the owner's and the linked object's entries are found and neither is busy; then plays the
// charge timeline and notifies itself (vtable slot 0x1f0). Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "base/global/CRntVector.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CActorComponent.h"
#include "game/CTeamManager/CTunableTeamManager.h"
#include "game/UnkActor.h"
#include "game/UnkBufferStringPool.h"
#include "game/UnkTU437.h"
#include "game/UnkTU883.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

using base::reflection::UnkReflObject;

extern u64 g_unk_710164af00;

// The Hydrogiga's AI component: the phase object at +0x14cb8 (its state at +0x10).
struct UnkHydrogigaPhase_l897d08 {
    u8 pad_0x0[0x10];
    s32 state;
};

struct UnkHydrogigaAI_l897d08 : UnkReflObject {
    u8 pad_0x8[0x14cb0];
    UnkHydrogigaPhase_l897d08* phase;  // 0x14cb8
};
static_assert(__builtin_offsetof(UnkHydrogigaAI_l897d08, phase) == 0x14cb8, "layout");

struct UnkBossActor_l897d08 {
    u8 pad_0x0[0x240];
    UnkReflObject* ai;  // 0x240
};

struct UnkBossHolder_l897d08 {
    u8 pad_0x0[0x10];
    UnkBossActor_l897d08* actor;  // 0x10
};

// The linked object the handle refers to; its id is read atomically.
struct UnkLinked_l897d08 : base::UnkRefCounted {
    u64 id;  // 0x10

    u64 Id() const { return __atomic_load_n(&id, __ATOMIC_RELAXED); }
};

// What fn_71005c1008 finds for an id.
struct UnkEntry_l897d08 {
    u8 pad_0x0[0x44];
    bool busy;  // 0x44
};

struct UnkShot_l897d08 {
    u8 pad_0x0[0xb4];
    u32 flags;  // 0xb4
};

struct UnkHidrogigaActivator_l897d08 {
    virtual void vf_0();
    virtual void vf_1();
    virtual void vf_2();
    virtual void vf_3();
    virtual void vf_4();
    virtual void vf_5();
    virtual void vf_6();
    virtual void vf_7();
    virtual void vf_8();
    virtual void vf_9();
    virtual void vf_10();
    virtual void vf_11();
    virtual void vf_12();
    virtual void vf_13();
    virtual void vf_14();
    virtual void vf_15();
    virtual void vf_16();
    virtual void vf_17();
    virtual void vf_18();
    virtual void vf_19();
    virtual void vf_20();
    virtual void vf_21();
    virtual void vf_22();
    virtual void vf_23();
    virtual void vf_24();
    virtual void vf_25();
    virtual void vf_26();
    virtual void vf_27();
    virtual void vf_28();
    virtual void vf_29();
    virtual void vf_30();
    virtual void vf_31();
    virtual void vf_32();
    virtual void vf_33();
    virtual void vf_34();
    virtual void vf_35();
    virtual void vf_36();
    virtual void vf_37();
    virtual void vf_38();
    virtual void vf_39();
    virtual void vf_40();
    virtual void vf_41();
    virtual void vf_42();
    virtual void vf_43();
    virtual void vf_44();
    virtual void vf_45();
    virtual void vf_46();
    virtual void vf_47();
    virtual void vf_48();
    virtual void vf_49();
    virtual void vf_50();
    virtual void vf_51();
    virtual void vf_52();
    virtual void vf_53();
    virtual void vf_54();
    virtual void vf_55();
    virtual void vf_56();
    virtual void vf_57();
    virtual void vf_58();
    virtual void vf_59();
    virtual void vf_60();
    virtual void vf_61();
    virtual void OnCharged();  // 0x1f0
    u8 pad_0x8[0x28];
    u64 owner;  // 0x30
    u8 pad_0x38[0x478];
    f32 max_charge;  // 0x4b0
    f32 step;  // 0x4b4
    u8 pad_0x4b8[0x4];
    f32 charge;  // 0x4bc
    u8 pad_0x4c0[0xc8];
    u8 source[0x28];  // 0x588
    u32 state;  // 0x5b0
    u8 pad_0x5b4[4];
    base::UnkRefHandle link;  // 0x5b8
    u8 pad_0x5c0[0x42];
    bool enabled;  // 0x602
};
static_assert(__builtin_offsetof(UnkHidrogigaActivator_l897d08, link) == 0x5b8, "layout");
static_assert(__builtin_offsetof(UnkHidrogigaActivator_l897d08, enabled) == 0x602, "layout");

static inline char* Manager_l897d08() {
    char* manager = g_unk_71726b2398;
    if (!manager) {
        manager = static_cast<char*>(fn_71000839a0(0x108, 0x3f));
        fn_71011a664c(manager);
        g_unk_71726b2398 = manager;
    }
    return manager;
}

bool fn_7100897d08(UnkHidrogigaActivator_l897d08* self, UnkShot_l897d08* shot) {
    if (!self->enabled)
        return false;
    f32 charge = self->charge;
    f32 max_charge = self->max_charge;
    if (UnkBossHolder_l897d08* holder = *reinterpret_cast<UnkBossHolder_l897d08**>(Manager_l897d08() + 0x100)) {
        if (UnkBossActor_l897d08* actor = holder->actor) {
            if (UnkReflObject* ai = actor->ai) {
                if (fn_710009d50c(ai->vf_0x0(), fn_71005ba098())) {
                    UnkHydrogigaPhase_l897d08* phase = static_cast<UnkHydrogigaAI_l897d08*>(ai)->phase;
                    if (!phase || phase->state != 2)
                        max_charge *= 0.99f;
                }
            }
        }
    }

    UnkEntry_l897d08* own = reinterpret_cast<UnkEntry_l897d08*>(fn_71005c1008(self->owner));
    if (self->state == 1) {
        {
            base::UnkRefPtrMember handle = fn_7100353be8(self->source);
            fn_710031d208(&self->link, reinterpret_cast<base::UnkRefHandle*>(&handle));
        }
        self->state = 2;
    }
    UnkEntry_l897d08* linked = nullptr;
    if (UnkLinked_l897d08* target = static_cast<UnkLinked_l897d08*>(self->link.ptr)) {
        if (target->Id())
            linked = reinterpret_cast<UnkEntry_l897d08*>(fn_71005c1008(target->Id()));
    }
    if (!own && !linked)
        return false;

    bool active = fn_710024fbc8(self);
    if (!(charge < max_charge) || !active)
        return false;
    if (!own->busy && !linked->busy) {
        f32 step = self->step;
        f32 slow = charge + step;
        f32 fast = charge + step * 5.0f;
        f32 next = (shot->flags & 0x08000400) == 0 ? slow : fast;
        f32 clamped = next >= max_charge ? max_charge : next;
        self->charge = next <= 0.0f ? 0.0f : clamped;
        fn_7100d7c828(reinterpret_cast<UnkActor*>(self->owner), &g_unk_710164af00, false);
        self->OnCharged();
    }
    return false;
}
