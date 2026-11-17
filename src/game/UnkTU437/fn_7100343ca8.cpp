// In unit 437 (CAimComponent's neighbourhood): resets the aim state (targets, timers, the held
// reference at +0x970, the two weak-spot links), then, when asked, requests state 1 from the state
// machine at +0x40. Names and layouts are placeholders.
#include "../CSensorDoorComponent/UnkDoorStateMachine_b58.h"
#include "base/global/CStrId.h"
#include "types.h"

void* fn_710035379c(void* owner);  // refl_FindTyped of "ENHANCEWEAKSPOT"

struct UnkAimRefCounted_7100343ca8 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Release();
    u32 refs;

    void DecRef() {
        if (--refs == 0)
            Release();
    }
};

struct UnkAimLinkTarget_7100343ca8 {
    void SetEnabled(bool enabled) {
        typedef void (*Fn)(UnkAimLinkTarget_7100343ca8*, bool);
        (*reinterpret_cast<Fn**>(this))[0x168 / 8](this, enabled);
    }
};

struct UnkAimLink_7100343ca8 {
    u8 pad_0x0[0x10];
    void* owner;  // 0x10
};

struct UnkAim_7100343ca8 {
    u8 pad_0x0[0x40];
    UnkDoorStateMachine_b58 machine;  // 0x40
    u8 pad_0xf0[0x2b8 - 0xf0];
    u32 field_0x2b8;
    u8 pad_0x2bc[0x2d8 - 0x2bc];
    u32 field_0x2d8;
    u8 pad_0x2dc[0x330 - 0x2dc];
    u64 field_0x330;
    u64 field_0x338;
    u32 field_0x340;
    u16 field_0x344;
    u8 pad_0x346[2];
    u64 field_0x348;
    u64 field_0x350;
    u32 field_0x358;
    u8 pad_0x35c[4];
    bool field_0x360;
    bool field_0x361;
    bool field_0x362;
    u8 pad_0x363[0x4cc - 0x363];
    f32 field_0x4cc;
    f32 field_0x4d0;
    u32 field_0x4d4;
    bool field_0x4d8;
    u8 pad_0x4d9[0x580 - 0x4d9];
    u64 field_0x580;
    u8 pad_0x588[0x630 - 0x588];
    UnkAimLink_7100343ca8* link_a;  // 0x630
    UnkAimLink_7100343ca8* link_b;  // 0x638
    bool field_0x640;
    bool field_0x641;
    u8 pad_0x642[0x670 - 0x642];
    u64 field_0x670;
    bool field_0x678;
    u8 pad_0x679[3];
    u32 field_0x67c;
    u8 pad_0x680[8];
    u32 field_0x688;
    u8 pad_0x68c[4];
    bool field_0x690;
    u8 pad_0x691[0x6a0 - 0x691];
    u32 field_0x6a0;
    u8 pad_0x6a4[0x969 - 0x6a4];
    bool field_0x969;
    u8 pad_0x96a[0x970 - 0x96a];
    UnkAimRefCounted_7100343ca8* held;  // 0x970
    u64 name;                           // 0x978
    u32 field_0x980;
};
static_assert(__builtin_offsetof(UnkAim_7100343ca8, field_0x2b8) == 0x2b8, "layout");
static_assert(__builtin_offsetof(UnkAim_7100343ca8, field_0x4cc) == 0x4cc, "layout");
static_assert(__builtin_offsetof(UnkAim_7100343ca8, link_a) == 0x630, "layout");
static_assert(__builtin_offsetof(UnkAim_7100343ca8, field_0x969) == 0x969, "layout");
static_assert(__builtin_offsetof(UnkAim_7100343ca8, field_0x980) == 0x980, "layout");

void fn_7100343eb4(UnkAim_7100343ca8* self);

static inline void DisableLink(UnkAimLink_7100343ca8* link) {
    if (link && link->owner) {
        if (auto* target = static_cast<UnkAimLinkTarget_7100343ca8*>(fn_710035379c(link->owner)))
            target->SetEnabled(false);
    }
}

static inline void RequestState(UnkDoorStateMachine_b58* self, s32 id) {
    self->pending = id;
    if (self->field_0xa8)
        return;
    UnkDoorState_b58* current = self->current;
    if (!current || current->id != id) {
        if (UnkDoorState_b58* next = FindState_b58(self, id)) {
            if (current) {
                current->vf_0x18(id);
                self->vf_0x28(self->current, next);
            }
            self->previous = self->current;
            self->current = next;
            next->vf_0x10(self->previous ? self->previous->id : 5);
            self->vf_0x20(self->previous, next);
        }
    }
    self->pending = 5;
}

void fn_7100343ca8(UnkAim_7100343ca8* self, bool request) {
    self->field_0x980 = 0;
    self->field_0x2b8 = 0;
    self->field_0x360 = false;
    self->field_0x361 = false;
    self->field_0x362 = false;
    self->field_0x330 = 0;
    self->field_0x338 = 0;
    self->field_0x340 = 0;
    self->field_0x344 = 0;
    self->field_0x348 = 0;
    self->field_0x350 = 0;
    self->field_0x358 = 0;
    self->field_0x4cc = -1.0f;
    self->field_0x4d0 = -1.0f;
    self->field_0x4d4 = 0;
    self->field_0x4d8 = false;
    self->field_0x2d8 = 0;
    self->field_0x670 = 0;
    self->field_0x969 = false;
    if (self->held) {
        self->held->DecRef();
        self->held = nullptr;
    }
    self->name = fn_7100001570(g_unk_710162e2ed, 0);
    self->field_0x580 = 0;
    self->field_0x688 = 0;
    self->field_0x67c = 0;
    self->field_0x690 = false;
    self->field_0x678 = false;
    self->field_0x6a0 = 0;
    DisableLink(self->link_a);
    DisableLink(self->link_b);
    self->field_0x640 = false;
    self->field_0x641 = false;
    fn_7100343eb4(self);
    if (request)
        RequestState(&self->machine, 1);
}
