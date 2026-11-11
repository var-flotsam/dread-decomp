// CBossAIComponent's unit (tools/tu_map.py unit 549): the lazily resolved weak link of a boss's
// door-lock entries (fn_71003b0c4c) and the walks over those entries that notify or lock the doors,
// the colour-slot timer and its update, the active/inactive timers that hand the camera back, the
// slot blink and the death handling. Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/math/CVector4D.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CBossAIComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkCameraCtrl.h"
#include "game/UnkGameManager.h"
#include "game/UnkSlots.h"
#include "game/UnkTU0.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector4D;
using base::global::CRntString;

// A counted reference to a weak handle; assigning another one refers to the handle of its target.
struct UnkRef {
    UnkWeakHandle* ptr;

    __attribute__((always_inline)) UnkRef& operator=(const UnkRef& other) {
        if (this != &other)
            CopyWeakRef(&ptr, other.ptr);
        return *this;
    }
    ~UnkRef() {
        if (ptr && --ptr->ref_count == 0)
            ptr->vf_0x10();
    }
};

UnkRef fn_7100353be8(void*);

struct UnkBossLazyLink_ovb0102 {
    u8 pad_0x0[0x8];
    u8 source[0x28];  // +0x8
    u32 state;        // +0x30
    u8 pad_0x34[4];
    UnkRef handle;    // +0x38
};

struct UnkZiplineLink_b49;
struct UnkZiplineLinkTarget_b49;

// The link's weak handle, resolved from the source on first use (state 1 -> 2).
UnkZiplineLinkTarget_b49** fn_71003b0c4c(UnkZiplineLink_b49* link) {
    UnkBossLazyLink_ovb0102* self = reinterpret_cast<UnkBossLazyLink_ovb0102*>(link);
    switch (self->state) {
    case 0:
        return reinterpret_cast<UnkZiplineLinkTarget_b49**>(&self->handle);
    case 1:
        self->handle = fn_7100353be8(self->source);
        self->state = 2;
        return reinterpret_cast<UnkZiplineLinkTarget_b49**>(&self->handle);
    case 2:
        return reinterpret_cast<UnkZiplineLinkTarget_b49**>(&self->handle);
    default:
        return reinterpret_cast<UnkZiplineLinkTarget_b49**>(&self->handle);
    }
    return reinterpret_cast<UnkZiplineLinkTarget_b49**>(&self->handle);
}

struct UnkZiplineLink_b49;
struct UnkZiplineLinkTarget_b49;
struct UnkDoorLock;
UnkDoorLock* fn_71002af860(UnkWeakTarget*);
void fn_71008d564c(UnkDoorLock* door, u32 value, const CStrId* id);
void fn_71008d586c(UnkDoorLock*, bool, u32, CStrId*);

struct UnkBossDoorLink_ovb0102 {
    u8 pad_0x0[0x40];
};

struct UnkBossDoorOwner_ovb0102 {
    u8 pad_0x0[0x30];
    CStrId field_0x30;
};

struct UnkBossDoorAI_ovb0102 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkBossDoorOwner_ovb0102* owner;  // +0x30
    u8 pad_0x38[0x14690];
    base::UnkVector<UnkBossDoorLink_ovb0102> doors;  // +0x146c8
};

// The door locks the boss's links resolve to, each told the owner's id through `notify`.
template <class Notify>
static inline void ForEachDoor(UnkBossDoorAI_ovb0102* self, Notify notify) {
    for (UnkBossDoorLink_ovb0102 *it = self->doors.data, *end = it + self->doors.count; it != end; ++it) {
        UnkWeakHandle* handle =
            *reinterpret_cast<UnkWeakHandle**>(fn_71003b0c4c(reinterpret_cast<UnkZiplineLink_b49*>(it)));
        if (!handle || !handle->target)
            continue;
        if (UnkDoorLock* door = fn_71002af860(handle->target)) {
            CStrId id = self->owner->field_0x30;
            notify(door, &id);
        }
    }
}

struct UnkCameraOwner;

// Tells every linked door lock (through the lazily resolved links at +0x146c8) the owner's id.
void fn_71003ad174(UnkCameraOwner* owner, u32 arg) {
    ForEachDoor(reinterpret_cast<UnkBossDoorAI_ovb0102*>(owner),
                [](UnkDoorLock* door, const CStrId* id) { fn_71008d564c(door, 0, id); });
}

// Locks or unlocks every linked door lock, in the owner's name.
void fn_71003b0054(UnkBossDoorAI_ovb0102* self, bool locked) {
    ForEachDoor(self, [locked](UnkDoorLock* door, CStrId* id) { fn_71008d586c(door, locked, 0, id); });
}

struct UnkBossSlotTarget_ovb0102 {
    u8 pad_0x0[0x138];
    UnkSlots* slots;  // +0x138
};

struct UnkBossLinkSub_ovb0102 {
    u8 pad_0x0[0x50];
    UnkBossSlotTarget_ovb0102* target;  // +0x50
};

struct UnkBossLink_ovb0102 {
    u8 pad_0x0[0x228];
    UnkBossLinkSub_ovb0102* field_0x228;
};

struct UnkBossFlags_ovb0102 {
    u8 pad_0x0[0x50c];
    bool field_0x50c;
    u8 pad_0x50d[0x3];
    bool field_0x510;
};

struct UnkBossSlotEntry_ovb0102 {
    u32 field_0x0;
    u32 id;
    CVector4D value;
};

struct UnkBossAI_ovb0102 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkBossLink_ovb0102* link;  // +0x30
    u8 pad_0x38[0x147dc];
    u32 entry_count;                      // +0x14814
    UnkBossSlotEntry_ovb0102 entries[8];  // +0x14818
    u8 pad_0x148d8[0xc];
    f32 field_0x148e4;
    u32 field_0x148e8;
    u8 pad_0x148ec[0x1c];
    UnkBossFlags_ovb0102* field_0x14908;
};
static_assert(__builtin_offsetof(UnkBossAI_ovb0102, field_0x14908) == 0x14908, "UnkBossAI_ovb0102 layout");

extern const CVector4D g_unk_71016491c0 __attribute__((visibility("hidden")));

static inline void Apply(UnkBossSlotTarget_ovb0102* target, u32 id, u32 kind, const CVector4D* value) {
    fn_71001c2764(target->slots, id, kind, value, true, true);
}

// Sets the timer at +0x148e4; when it was zero, first clears +0x148e8 and requests kind 1 of every
// stored slot id on the linked target. False (nothing done) while the flags object forbids it.
bool fn_71003b0a28(UnkBossAI_ovb0102* self, f32 value) {
    UnkBossFlags_ovb0102* flags = self->field_0x14908;
    if (flags->field_0x50c || !flags->field_0x510)
        return false;
    if (self->field_0x148e4 == 0.0f) {
        self->field_0x148e8 = 0;
        if (UnkBossLinkSub_ovb0102* sub = self->link->field_0x228) {
            if (UnkBossSlotTarget_ovb0102* target = sub->target) {
                for (u32 i = 0; i < self->entry_count;) {
                    UnkBossSlotEntry_ovb0102* e = &self->entries[i++];
                    Apply(target, e->id, 1, &g_unk_71016491c0);
                }
            }
        }
    }
    self->field_0x148e4 = value;
    return true;
}

struct UnkBossColorFlags_ovb0102 {
    u8 pad_0x0[0x510];
    bool field_0x510;
};

struct UnkBossColorLinkSub_ovb0102 {
    u8 pad_0x0[0x50];
    void* target;  // +0x50
};

struct UnkBossColorLink_ovb0102 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // +0x220
    UnkBossColorLinkSub_ovb0102* field_0x228;
};

struct UnkBossColorAI_ovb0102 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkBossColorLink_ovb0102* link;  // +0x30
    u8 pad_0x38[0x147d4];
    f32 value;  // +0x1480c
    f32 timer;  // +0x14810
    u8 pad_0x14814[0xf4];
    UnkBossColorFlags_ovb0102* flags;  // +0x14908

    f32 vf_0x820() { return reinterpret_cast<f32 (*)(UnkBossColorAI_ovb0102*)>(vtable[0x820 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkBossColorAI_ovb0102, flags) == 0x14908, "UnkBossColorAI_ovb0102 layout");

// Applies the new value and the timer to the linked object.
void fn_71003adc50(UnkBossColorAI_ovb0102* self, UnkBossColorLink_ovb0102* link, f32 value, f32 timer);

// Counts the timer at +0x14810 down by `dt` (or clears it without `counting`), then, when the value
// from vf_0x820 changed, stores it and applies it (a drop restarts the timer at 0.75 while counting).
void fn_71003adb60(UnkBossColorAI_ovb0102* self, f32 dt, bool counting) {
    if (counting) {
        f32 t = self->timer;
        if (!(t > 0.0f))
            goto counted;
        t -= dt;
        self->timer = t;
        if (!(t <= 0.0f))
            goto counted;
    }
    self->timer = 0.0f;
counted:
    if (!self->flags->field_0x510)
        return;
    UnkAnimationComponent* animation = self->link->animation;
    if (!animation || fn_710117b348(animation, fn_7100d07fd8()))
        return;
    UnkBossColorLinkSub_ovb0102* sub = self->link->field_0x228;
    if (!sub || !sub->target)
        return;
    f32 value = self->vf_0x820();
    f32 old = self->value;
    if (value != old) {
        self->value = value;
        if (old > value && counting)
            self->timer = 0.75f;
        fn_71003adc50(self, self->link, value, self->timer);
    }
}

struct UnkSelf;
void fn_71003ad8e4(UnkSelf* self);

struct UnkBossAITimers_ovb0102 {
    u8 pad_0x0[0x147f4];
    bool active;    // +0x147f4
    u8 pad_0x147f5[3];
    f32 active_time;  // +0x147f8
    f32 total_active_time;  // +0x147fc
    f32 inactive_time;  // +0x14800
};

// Whether the AI is active this frame (fn_710110c7f8 on the component, the second argument clear).
bool fn_710110c7f8(UnkBossAITimers_ovb0102* self, bool arg);

// Advances the active/inactive timers by `dt`. Becoming active runs fn_71003ad8e4; becoming
// inactive while the boss camera is in control asks the scenario to activate the default camera.
void fn_71003ad7ec(UnkBossAITimers_ovb0102* self, f32 dt) {
    bool active = fn_710110c7f8(self, false);
    f32 was = self->active_time;
    self->active = active;
    if (active) {
        self->active_time = was + dt;
        self->total_active_time += dt;
        self->inactive_time = 0.0f;
        if (was == 0.0f)
            fn_71003ad8e4(reinterpret_cast<UnkSelf*>(self));
    } else {
        self->inactive_time += dt;
        self->active_time = 0.0f;
        if (was > 0.0f) {
            UnkCameraCtrl* camera = fn_71010e2080(fn_7100defcec(g_unk_7101d47310));
            if (camera && fn_710009d50c(camera->vf_0x0(), fn_710074eca8()))
                fn_7100000790("Scenario.ActivateDefaultCamera", "b", false);
        }
    }
}

struct UnkBossBlinkTarget_ovb0102 {
    u8 pad_0x0[0x138];
    UnkSlots* slots;  // +0x138
};

struct UnkBossBlinkLinkSub_ovb0102 {
    u8 pad_0x0[0x50];
    UnkBossBlinkTarget_ovb0102* target;  // +0x50
};

struct UnkBossBlinkLife_ovb0102 {
    void** vtable;
    u8 pad_0x8[0x114];
    f32 blink_period;  // +0x11c

    const base::reflection::CType* vf_0x0() {
        return reinterpret_cast<const base::reflection::CType* (*)(UnkBossBlinkLife_ovb0102*)>(vtable[0])(this);
    }
};

struct UnkBossBlinkLifeSlot_ovb0102 {
    u8 pad_0x0[0x10];
    UnkBossBlinkLife_ovb0102* life;  // +0x10
};

struct UnkBossBlinkLink_ovb0102 {
    u8 pad_0x0[0x1d0];
    UnkBossBlinkLifeSlot_ovb0102* field_0x1d0;
    u8 pad_0x1d8[0x50];
    UnkBossBlinkLinkSub_ovb0102* field_0x228;
};

struct UnkBossBlinkEntry_ovb0102 {
    u32 field_0x0;
    u32 id;
    CVector4D value;
};

struct UnkBossBlinkAI_ovb0102 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkBossBlinkLink_ovb0102* link;  // +0x30
    u8 pad_0x38[0x147dc];
    u32 entry_count;                       // +0x14814
    UnkBossBlinkEntry_ovb0102 entries[8];  // +0x14818
    u8 pad_0x148d8[0xc];
    f32 timer;    // +0x148e4
    f32 elapsed;  // +0x148e8
};

extern const CVector4D g_unk_71016491b0 __attribute__((visibility("hidden")));
extern const CVector4D g_unk_71016491c0 __attribute__((visibility("hidden")));

static inline void Apply(UnkBossBlinkTarget_ovb0102* target, u32 id, u32 kind, const CVector4D* value) {
    fn_71001c2764(target->slots, id, kind, value, true, true);
}

// Requests kind 1 of every stored slot id on the linked target with `value`.
static inline void ApplyAll(UnkBossBlinkAI_ovb0102* self, UnkBossBlinkLinkSub_ovb0102* sub, const CVector4D* value) {
    if (sub) {
        if (UnkBossBlinkTarget_ovb0102* target = sub->target) {
            for (u32 i = 0; i < self->entry_count;) {
                UnkBossBlinkEntry_ovb0102* e = &self->entries[i++];
                Apply(target, e->id, 1, value);
            }
        }
    }
}

// Runs the blink set up by fn_71003b0a28: while the timer at +0x148e4 runs, the slots alternate
// between the two values every blink period of the owner's enemy life component; when it runs
// out, they are cleared.
void fn_71003ae858(UnkBossBlinkAI_ovb0102* self, f32 dt) {
    if (!(self->timer > 0.0f))
        return;
    self->elapsed += dt;
    if (UnkBossBlinkLifeSlot_ovb0102* slot = self->link->field_0x1d0) {
        UnkBossBlinkLife_ovb0102* life = slot->life;
        UnkBossBlinkLife_ovb0102* enemy = fn_710009d50c(life->vf_0x0(), fn_71008b3b04()) ? life : nullptr;
        s32 phase = (s32)(self->elapsed / enemy->blink_period);
        if (phase & 1)
            ApplyAll(self, self->link->field_0x228, &g_unk_71016491b0);
        else
            ApplyAll(self, self->link->field_0x228, &g_unk_71016491c0);
    }
    f32 t = self->timer - dt;
    self->timer = t;
    if (!(t <= 0.0f))
        return;
    self->timer = 0.0f;
    self->elapsed = 0.0f;
    ApplyAll(self, self->link->field_0x228, &g_unk_71016491b0);
}

struct UnkModeFlags {
    u8 unk_0x0[0xb8];
    bool field_0xb8;
};
extern UnkModeFlags* g_unk_71726bc570;

bool fn_7100dfbe3c(UnkGameManager*);
void fn_7100dfaf88(UnkGameManager* self, void* name, const CRntString& tag, bool flag);
void fn_710007f564(void*, const char*);

// The pooled data behind an id as its text accessor sees it: the text at +0x10 and a count of the
// reads at +0x44 (a union member: the original reloads the id after the increment).
struct UnkBossDeathIdText_ovb0102 {
    u8 pad_0x0[0x10];
    CRntString str;
    u8 pad_0x40[0x4];
    union {
        s32 reads;
    };
};

static inline const CRntString& IdText(const CStrId& id) {
    ++reinterpret_cast<UnkBossDeathIdText_ovb0102*>(id.data)->reads;
    return reinterpret_cast<UnkBossDeathIdText_ovb0102*>(id.data)->str;
}

struct UnkBossDeathEntity_ovb0102 {
    u8 pad_0x0[0x30];
    CStrId name;  // +0x30
};

struct UnkBossDeathTargetHolder_ovb0102 : base::UnkRefCounted {
    UnkBossDeathEntity_ovb0102* target;  // +0x10
};

struct UnkBossDeathSettings_ovb0102 {
    u8 pad_0x0[0x50c];
    bool field_0x50c;
};

struct UnkBossDeathAI_ovb0102 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkBossDeathEntity_ovb0102* owner;  // +0x30
    u8 pad_0x38[0x148d0];
    UnkBossDeathSettings_ovb0102* settings;  // +0x14908
    u8 pad_0x14910[0x50];
    u8 field_0x14960[0x28];
    u32 state;  // +0x14988
    u8 pad_0x1498c[4];
    base::UnkRefHandle handle;  // +0x14990
    bool field_0x14998;
};
static_assert(__builtin_offsetof(UnkBossDeathAI_ovb0102, field_0x14998) == 0x14998, "layout");

// The base death handling.
void fn_710110e274(UnkBossDeathAI_ovb0102* self);

// The target behind the handle at +0x14990, resolved from +0x14960 on first use.
static inline UnkBossDeathTargetHolder_ovb0102* Holder(UnkBossDeathAI_ovb0102* self) {
    if (self->state == 1) {
        {
            UnkRefPtr reference = fn_710023b580(self->field_0x14960);
            fn_7100234f78(&self->handle, reinterpret_cast<const base::UnkRefHandle*>(&reference));
        }
        self->state = 2;
    }
    return static_cast<UnkBossDeathTargetHolder_ovb0102*>(self->handle.ptr);
}

// The boss's death: after the base handling (and unless the settings say otherwise), outside the mode
// flag at +0xb8 the "<owner>_dead" event is raised for the target when the game manager allows it and
// +0x14998 is set; then the scenario's OnBossDeath runs with the owner's name.
void fn_71003afd68(UnkBossDeathAI_ovb0102* self) {
    fn_710110e274(self);
    if (self->settings->field_0x50c)
        return;
    if (!g_unk_71726bc570->field_0xb8) {
        UnkBossDeathTargetHolder_ovb0102* holder = Holder(self);
        if (holder && holder->target && fn_7100dfbe3c(g_unk_7101d47310) && self->field_0x14998) {
            CRntString name(IdText(self->owner->name));
            fn_710007f564(&name, "_dead");
            UnkGameManager* manager = g_unk_7101d47310;
            fn_7100dfaf88(manager, &name, IdText(Holder(self)->target->name), false);
        }
    }
    const char* text = IdText(self->owner->name).data;
    fn_7100000790("CurrentScenario.OnBossDeath", "s", text ? text : g_unk_710162e2ed);
}
