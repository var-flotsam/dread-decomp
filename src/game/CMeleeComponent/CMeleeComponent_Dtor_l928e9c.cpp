// CMeleeComponent's unit: the component's destructor: the string ids, the three arrays (the first one's
// entries point to slots that are cleared), the listener at +0xa8, the "MELEE_INPUT" input listener at +0x40,
// then the component base's destructor. Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAbilityLayout.h"
#include "game/CInputComponent.h"
#include "game/UnkComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

extern void* g_unk_7101a83750[];

struct UnkMeleeLink_l928e9c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Release();  // 0x10
    u32 ref_count;           // 0x8
    UnkWeakTarget* owner;    // 0x10
};

// A listener on a counted link; it detaches itself from the link's owner on destruction.
struct UnkMeleeListener_l928e9c {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkMeleeLink_l928e9c* link;  // 0x18

    ~UnkMeleeListener_l928e9c() {
        vtable = &g_unk_7101a22d48[2];
        if (UnkMeleeLink_l928e9c* l = link) {
            if (UnkWeakTarget* owner = l->owner) {
                fn_710117b3f0(owner, this);
                l = link;
                if (!l)
                    return;
            }
            if (--l->ref_count == 0)
                l->Release();
        }
    }
};

struct UnkMeleeInputOwner_l928e9c {
    u8 pad_0x0[0x1d8];
    void* input;  // 0x1d8
};

struct UnkMeleeComponent_l928e9c {
    void** vtable;
    u8 pad_0x8[0x38];
    UnkAbilityInputListener input;  // 0x40
    UnkBaseComponent* target;       // 0x88; the component whose actor's input component has the listener
    u8 pad_0x90[0x18];
    UnkMeleeListener_l928e9c listener;  // 0xa8
    u8 pad_0xc8[8];
    CStrId name;  // 0xd0
    CStrId field_0xd8;
    u32 field_0xe0;
    u8 field_0xe4[0x80];
    u32 field_0x164;
    u8 field_0x168[0x80];
    bool field_0x1e8;
    u8 pad_0x1e9[7];
    CStrId field_0x1f0;
    base::global::CRntVector<void**> lists[3];  // 0x1f8
    bool field_0x270;
    u8 pad_0x271[7];
    CStrId field_0x278;
};
static_assert(__builtin_offsetof(UnkMeleeComponent_l928e9c, listener) == 0xa8, "layout");
static_assert(__builtin_offsetof(UnkMeleeComponent_l928e9c, field_0x1f0) == 0x1f0, "layout");
static_assert(__builtin_offsetof(UnkMeleeComponent_l928e9c, field_0x278) == 0x278, "layout");

// The input component of the target's actor.
static inline void* InputOf(UnkBaseComponent* component) {
    return static_cast<UnkMeleeInputOwner_l928e9c*>(component->field_0x30)->input;
}

void fn_7100928e9c(UnkMeleeComponent_l928e9c* self) {
    self->vtable = g_unk_7101a83750 + 2;
    self->field_0x278.~CStrId();
    base::global::CRntVector<void**>& links = self->lists[0];
    for (void*** it = links.data, ***end = links.data + links.count; it != end; ++it)
        **it = nullptr;
    for (base::global::CRntVector<void**>& list : self->lists)
        list.Clear();
    self->lists[2].Free();
    self->lists[1].Free();
    self->lists[0].Free();
    self->field_0x1f0.~CStrId();
    self->field_0x164 = 0;
    self->field_0xe0 = 0;
    self->field_0xd8.~CStrId();
    self->name.~CStrId();
    self->listener.~UnkMeleeListener_l928e9c();
    self->input.vtable = g_unk_7101a22858 + 2;
    if (self->target && self->target->field_0x30 && InputOf(self->target))
        fn_7100872908(InputOf(self->target), &self->input);
    self->target = nullptr;
    self->input.vtable = g_unk_7101a22878 + 2;
    if (self->input.entries) {
        fn_7100083c30(static_cast<u64*>(self->input.entries) - 1);
        self->input.entries = nullptr;
    }
    fn_710007ecd4(reinterpret_cast<base::global::CRntString*>(&self->input.name));
    fn_71010c4194(reinterpret_cast<UnkBaseComponent*>(self));
}
