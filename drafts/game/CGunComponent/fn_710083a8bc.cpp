// CGunComponent unit (tools/tu_map.py unit 1304): moves the highest-priority usable weapon of the first
// weapon group to the group's front slot, deactivating the old front one, and makes it the current
// weapon when the old one was current or none is. Names and layouts are placeholders.
#include <utility>

#include "base/global/CRntVector.h"
#include "types.h"

// A weapon of a group. Only the slots this function calls are named.
struct UnkGunWeapon_l83a8bc {
    virtual void vf_0x0(); virtual void vf_0x8(); virtual void vf_0x10(); virtual void vf_0x18(); virtual void vf_0x20(); virtual void vf_0x28();
    virtual bool IsSelectable();  // 0x30
    virtual void vf_0x38(); virtual void vf_0x40(); virtual void vf_0x48(); virtual void vf_0x50(); virtual void vf_0x58(); virtual void vf_0x60();
    virtual void vf_0x68(); virtual void vf_0x70(); virtual void vf_0x78(); virtual void vf_0x80(); virtual void vf_0x88(); virtual void vf_0x90();
    virtual void vf_0x98(); virtual void vf_0xa0(); virtual void vf_0xa8(); virtual void vf_0xb0(); virtual void vf_0xb8(); virtual void vf_0xc0();
    virtual void vf_0xc8(); virtual void vf_0xd0(); virtual void vf_0xd8(); virtual void vf_0xe0(); virtual void vf_0xe8(); virtual void vf_0xf0();
    virtual s32 GetPriority();  // 0xf8
    virtual void vf_0x100(); virtual void vf_0x108(); virtual void vf_0x110(); virtual void vf_0x118(); virtual void vf_0x120(); virtual void vf_0x128();
    virtual void Activate();    // 0x130
    virtual void Deactivate();  // 0x138
};

using UnkGunWeaponGroup_l83a8bc = base::global::CRntVector<UnkGunWeapon_l83a8bc*>;

struct UnkGunComponent_l83a8bc {
    virtual void vf_0x0(); virtual void vf_0x8(); virtual void vf_0x10(); virtual void vf_0x18(); virtual void vf_0x20(); virtual void vf_0x28();
    virtual void vf_0x30(); virtual void vf_0x38(); virtual void vf_0x40(); virtual void vf_0x48(); virtual void vf_0x50(); virtual void vf_0x58();
    virtual void vf_0x60(); virtual void vf_0x68(); virtual void vf_0x70(); virtual void vf_0x78(); virtual void vf_0x80(); virtual void vf_0x88();
    virtual void vf_0x90(); virtual void vf_0x98(); virtual void vf_0xa0(); virtual void vf_0xa8(); virtual void vf_0xb0(); virtual void vf_0xb8();
    virtual void vf_0xc0(); virtual void vf_0xc8(); virtual void vf_0xd0(); virtual void vf_0xd8(); virtual void vf_0xe0(); virtual void vf_0xe8();
    virtual void vf_0xf0(); virtual void vf_0xf8(); virtual void vf_0x100(); virtual void vf_0x108(); virtual void vf_0x110(); virtual void vf_0x118();
    virtual void vf_0x120(); virtual void vf_0x128(); virtual void vf_0x130(); virtual void vf_0x138(); virtual void vf_0x140(); virtual void vf_0x148();
    virtual void vf_0x150(); virtual void vf_0x158(); virtual void vf_0x160(); virtual void vf_0x168(); virtual void vf_0x170(); virtual void vf_0x178();
    virtual void vf_0x180(); virtual void vf_0x188(); virtual void vf_0x190(); virtual void vf_0x198(); virtual void vf_0x1a0();
    virtual void SetCurrent(UnkGunWeapon_l83a8bc* weapon, bool a, bool b);  // 0x1a8
    virtual void ReleaseCurrent(UnkGunWeapon_l83a8bc* weapon, bool a);       // 0x1b0

    u8 pad_0x8[0xe8];
    base::global::CRntVector<UnkGunWeaponGroup_l83a8bc> groups;  // 0xf0
    u8 pad_0x118[0x8];
    UnkGunWeapon_l83a8bc* current;  // 0x120

    UnkGunWeaponGroup_l83a8bc& FirstGroup() { return groups.data[0]; }
};
static_assert(__builtin_offsetof(UnkGunComponent_l83a8bc, current) == 0x120, "layout");

void fn_710083a8bc(UnkGunComponent_l83a8bc* self) {
    UnkGunWeapon_l83a8bc*& front = self->FirstGroup().GrowAt(0);

    UnkGunWeapon_l83a8bc** best = nullptr;
    s32 best_priority = -1;
    for (UnkGunWeapon_l83a8bc** it = self->FirstGroup().data;
         it != self->FirstGroup().data + self->FirstGroup().count; ++it) {
        if ((*it)->IsSelectable() && (*it)->GetPriority() > best_priority) {
            best_priority = (*it)->GetPriority();
            best = it;
        }
    }

    bool was_current = false;
    if (!best || *best == front) {
        if (front)
            front->Activate();
    } else {
        was_current = front == self->current;
        if (was_current)
            self->ReleaseCurrent(front, false);
        front->Deactivate();
        std::swap(front, *best);
        front->Activate();
    }

    if (was_current || !self->current) {
        self->current = front;
        self->SetCurrent(front, false, true);
    }
}
