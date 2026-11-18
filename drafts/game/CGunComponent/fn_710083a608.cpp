// CGunComponent unit (tools/tu_map.py unit 1304): moves the selectable weapon of the first primary group with
// the highest priority (vf_0xf8) to the group's front, switching it in as the alternative weapon when the old
// front was the current one, or when there is none. Names and layouts are placeholders.
// This unit has the element accessor (GrowAt) inlined (base/global/CRntVector.h).
#define UNK_VECTOR_GROW_AT_ATTR __attribute__((always_inline))
#include "base/global/CRntVector.h"
#include "types.h"

using base::global::CRntVector;

// A weapon of a group. Only the slots this function calls are named.
struct UnkGunWeapon_l83a608 {
    virtual void vf_0x0(); virtual void vf_0x8(); virtual void vf_0x10(); virtual void vf_0x18(); virtual void vf_0x20(); virtual void vf_0x28();
    virtual bool IsSelectable();  // 0x30
    virtual void vf_0x38(); virtual void vf_0x40(); virtual void vf_0x48(); virtual void vf_0x50(); virtual void vf_0x58();
    virtual void vf_0x60(); virtual void vf_0x68(); virtual void vf_0x70(); virtual void vf_0x78(); virtual void vf_0x80();
    virtual void vf_0x88(); virtual void vf_0x90(); virtual void vf_0x98(); virtual void vf_0xa0(); virtual void vf_0xa8();
    virtual void vf_0xb0(); virtual void vf_0xb8(); virtual void vf_0xc0(); virtual void vf_0xc8(); virtual void vf_0xd0();
    virtual void vf_0xd8(); virtual void vf_0xe0(); virtual void vf_0xe8(); virtual void vf_0xf0();
    virtual s32 Priority();  // 0xf8
    virtual void vf_0x100(); virtual void vf_0x108(); virtual void vf_0x110(); virtual void vf_0x118(); virtual void vf_0x120();
    virtual void vf_0x128();
    virtual void OnFront();    // 0x130
    virtual void OnUnfront();  // 0x138

    u8 pad_0x8[0x1871 - 0x8];
    bool field_0x1871;
};

using UnkGunWeaponGroup_l83a608 = CRntVector<UnkGunWeapon_l83a608*>;

struct UnkGunComponent_l83a608 {
    virtual void vf_0x0(); virtual void vf_0x8(); virtual void vf_0x10(); virtual void vf_0x18(); virtual void vf_0x20(); virtual void vf_0x28();
    virtual void vf_0x30(); virtual void vf_0x38(); virtual void vf_0x40(); virtual void vf_0x48(); virtual void vf_0x50(); virtual void vf_0x58();
    virtual void vf_0x60(); virtual void vf_0x68(); virtual void vf_0x70(); virtual void vf_0x78(); virtual void vf_0x80(); virtual void vf_0x88();
    virtual void vf_0x90(); virtual void vf_0x98(); virtual void vf_0xa0(); virtual void vf_0xa8(); virtual void vf_0xb0(); virtual void vf_0xb8();
    virtual void vf_0xc0(); virtual void vf_0xc8(); virtual void vf_0xd0(); virtual void vf_0xd8(); virtual void vf_0xe0(); virtual void vf_0xe8();
    virtual void vf_0xf0(); virtual void vf_0xf8(); virtual void vf_0x100(); virtual void vf_0x108(); virtual void vf_0x110(); virtual void vf_0x118();
    virtual void vf_0x120(); virtual void vf_0x128(); virtual void vf_0x130(); virtual void vf_0x138(); virtual void vf_0x140(); virtual void vf_0x148();
    virtual void vf_0x150(); virtual void vf_0x158(); virtual void vf_0x160(); virtual void vf_0x168(); virtual void vf_0x170(); virtual void vf_0x178();
    virtual void vf_0x180(); virtual void vf_0x188(); virtual void vf_0x190(); virtual void vf_0x198(); virtual void vf_0x1a0();
    virtual void SetCurrent(UnkGunWeapon_l83a608* weapon, bool a, bool b);  // 0x1a8
    virtual void ReleaseCurrent(UnkGunWeapon_l83a608* weapon, bool a);       // 0x1b0

    u8 pad_0x8[0xc0];
    CRntVector<UnkGunWeaponGroup_l83a608> primary;  // 0xc8
    u8 pad_0xf0[0x28];
    UnkGunWeapon_l83a608* alt;  // 0x118
};
static_assert(__builtin_offsetof(UnkGunComponent_l83a608, alt) == 0x118, "layout");

struct UnkGunComponent_ovb0091;
void fn_710083a034(UnkGunComponent_ovb0091* self, bool flag, bool now);

void fn_710083a608(UnkGunComponent_l83a608* self) {
    UnkGunWeapon_l83a608*& front = self->primary[0].GrowAt(0);

    UnkGunWeapon_l83a608** best = nullptr;
    s32 best_priority = -1;
    for (UnkGunWeapon_l83a608** it = self->primary[0].data; it != self->primary[0].data + self->primary[0].count;
         ++it) {
        if ((*it)->IsSelectable() && (*it)->Priority() > best_priority) {
            best_priority = (*it)->Priority();
            best = it;
        }
    }

    bool was_current = false;
    if (best && front != *best) {
        if (self->alt && !self->alt->field_0x1871)
            fn_710083a034(reinterpret_cast<UnkGunComponent_ovb0091*>(self), false, false);
        was_current = front == self->alt;
        if (was_current)
            self->ReleaseCurrent(self->alt, false);
        front->OnUnfront();
        UnkGunWeapon_l83a608* old = front;
        front = *best;
        *best = old;
        front->OnFront();
    } else if (front) {
        front->OnFront();
    }

    if (was_current || !self->alt) {
        self->alt = front;
        self->SetCurrent(self->alt, false, true);
    }
}
