// In unit 437 (CAimComponent's neighbourhood): the aim component's destructor body. Drops the counted
// references, the reference list and the string ids, detaches the two listeners and the input listener,
// resets the four embedded sub-objects and the one at +0x40, then runs the base destructor.
// Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAbilityOpticCamouflage.h"
#include "game/CGunComponent.h"
#include "game/CInputComponent.h"
#include "game/UnkComponent.h"
#include "game/UnkTU39.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

extern void* g_unk_7101a23ab0[];
extern void* g_unk_7101a23d60[];
extern void* g_unk_7101a23db8[];

// Reference counted through a virtual release (count at +0x8, vf_0x10 deletes).
struct UnkAimRef_l3437a4 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;
};

// Lowers a reference count; true when it reaches zero.
inline bool DropRef(s32& count) { return --count == 0; }

static inline void Release(UnkAimRef_l3437a4* p) {
    if (p && DropRef(p->ref_count))
        p->vf_0x10();
}

// An owning reference: releases its object when destroyed.
struct UnkAimRefPtr_l3437a4 {
    UnkAimRef_l3437a4* ptr;

    ~UnkAimRefPtr_l3437a4() { Release(ptr); }
};

// A listener registered with the owner of its counted link (+0x18).
struct UnkAimLink_l3437a4 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;  // 0x8
    void* owner;    // 0x10
};

struct UnkAimListener_l3437a4 {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkAimLink_l3437a4* link;  // 0x18
};

// An embedded sub-object (0x70 bytes) with its own vtable.
struct UnkAimSub_l3437a4 {
    void* vtable;
    u8 pad_0x8[0x10];
    base::reflection::UnkReflValue field_0x18;
    void* field_0x30[8];

    void Destroy() {
        field_0x30[7] = nullptr;
        field_0x30[6] = nullptr;
        field_0x30[5] = nullptr;
        field_0x30[4] = nullptr;
        field_0x30[3] = nullptr;
        field_0x30[2] = nullptr;
        field_0x30[1] = nullptr;
        field_0x30[0] = nullptr;
        vtable = g_unk_7101a23d60 + 2;
        fn_71000a1c60(&field_0x18);
    }
};

struct UnkAimSub2_l3437a4 {
    void* vtable;
    u8 pad_0x8[0x8];
    void* field_0x10;
    void* field_0x18;
    u32 field_0x20;

    void Destroy() {
        vtable = g_unk_7101a23db8 + 2;
        field_0x10 = nullptr;
        field_0x18 = nullptr;
        field_0x20 = 0;
    }
};

struct UnkAimInputOwner_l3437a4 {
    u8 pad_0x0[0x1d8];
    void* input;  // 0x1d8
};

struct UnkAimTarget_l3437a4 {
    u8 pad_0x0[0x30];
    UnkAimInputOwner_l3437a4* owner;  // 0x30
};

// The input listener: registered with the target's input component.
struct UnkAimInput_l3437a4 {
    void** vtable;
    base::global::CRntString name;  // 0x8
    u8 pad_0x38[0x8];
    void* entries;                  // 0x40
    UnkAimTarget_l3437a4* target;   // 0x48
};

struct UnkAim_l3437a4 : UnkBaseComponent {
    UnkAimSub2_l3437a4 field_0x40;
    u8 pad_0x68[0xf0 - 0x68];
    UnkAimSub_l3437a4 field_0xf0;
    UnkAimSub_l3437a4 field_0x160;
    UnkAimSub_l3437a4 field_0x1d0;
    UnkAimSub_l3437a4 field_0x240;
    u8 pad_0x2b0[0x2e0 - 0x2b0];
    UnkAimInput_l3437a4 input;  // 0x2e0
    u8 pad_0x330[0x368 - 0x330];
    u8 field_0x368[0xb0];
    u8 field_0x418[0xc8];
    base::global::CStrId field_0x4e0;
    base::global::CStrId field_0x4e8;
    u8 pad_0x4f0[0x530 - 0x4f0];
    UnkAimListener_l3437a4 listener_a;  // 0x530
    UnkAimListener_l3437a4 listener_b;  // 0x550
    u8 pad_0x570[0x5c0 - 0x570];
    base::global::CStrId field_0x5c0;
    UnkAimRef_l3437a4* field_0x5c8;
    u8 pad_0x5d0[0x630 - 0x5d0];
    UnkAimRef_l3437a4* field_0x630;
    UnkAimRef_l3437a4* field_0x638;
    u8 pad_0x640[0x648 - 0x640];
    base::global::CRntVector<UnkAimRefPtr_l3437a4> refs;  // 0x648
    u8 pad_0x670[0x680 - 0x670];
    UnkAimRef_l3437a4* field_0x680;
    u8 pad_0x688[0x6e8 - 0x688];
    u8 field_0x6e8[0xb0];
    u8 field_0x798[0xb0];
    u32 field_0x848;
    u8 pad_0x84c[0x970 - 0x84c];
    UnkAimRef_l3437a4* field_0x970;
};
static_assert(__builtin_offsetof(UnkAim_l3437a4, input) == 0x2e0, "layout");
static_assert(__builtin_offsetof(UnkAim_l3437a4, field_0x4e0) == 0x4e0, "layout");
static_assert(__builtin_offsetof(UnkAim_l3437a4, listener_b) == 0x550, "layout");
static_assert(__builtin_offsetof(UnkAim_l3437a4, refs) == 0x648, "layout");
static_assert(__builtin_offsetof(UnkAim_l3437a4, field_0x6e8) == 0x6e8, "layout");
static_assert(__builtin_offsetof(UnkAim_l3437a4, field_0x970) == 0x970, "layout");

void fn_71003437a4(UnkAim_l3437a4* self) {
    self->vtable = g_unk_7101a23ab0 + 2;
    Release(self->field_0x970);
    self->field_0x848 = 0;
    fn_710002f67c(self->field_0x798);
    fn_710002f67c(self->field_0x6e8);
    Release(self->field_0x680);
    self->refs.Free();
    Release(self->field_0x638);
    Release(self->field_0x630);
    Release(self->field_0x5c8);
    self->field_0x5c0.Reset();

    UnkAimListener_l3437a4* b = &self->listener_b;
    b->vtable = g_unk_7101a22d48 + 2;
    if (UnkAimLink_l3437a4* link = b->link) {
        if (link->owner) {
            fn_710117b3f0(static_cast<UnkWeakTarget*>(link->owner), b);
            link = b->link;
        }
        if (link)
            if (--link->ref_count == 0)
                link->vf_0x10();
    }
    UnkAimListener_l3437a4* a = &self->listener_a;
    a->vtable = g_unk_7101a23280 + 2;
    if (UnkAimLink_l3437a4* link = a->link) {
        if (link->owner) {
            fn_710083b0bc(link->owner, reinterpret_cast<UnkListener_b40*>(a));
            link = a->link;
        }
        if (link)
            if (--link->ref_count == 0)
                link->vf_0x10();
    }

    self->field_0x4e8.Reset();
    self->field_0x4e0.Reset();
    fn_7100030a50(reinterpret_cast<UnkMoveSlot_0xb0*>(self->field_0x418));
    fn_7100030a50(reinterpret_cast<UnkMoveSlot_0xb0*>(self->field_0x368));

    self->input.vtable = g_unk_7101a22858 + 2;
    if (self->input.target && self->input.target->owner && self->input.target->owner->input)
        fn_7100872908(self->input.target->owner->input, &self->input);
    self->input.target = nullptr;
    self->input.vtable = g_unk_7101a22878 + 2;
    if (self->input.entries) {
        fn_7100083c30(static_cast<u64*>(self->input.entries) - 1);
        self->input.entries = nullptr;
    }
    CRntString_Destroy(&self->input.name);

    self->field_0x240.Destroy();
    self->field_0x1d0.Destroy();
    self->field_0x160.Destroy();
    self->field_0xf0.Destroy();
    self->field_0x40.Destroy();
    fn_71010c4194(self);
}
