// Unit 1176 (CCentralUnitComponent): saves the central unit, drops its deferred-set registration and, in
// mode 6, resets every linked door lock and opens the unit's own door (or its central door's stay-opened
// state). Names and layouts are placeholders.
#include "base/UnkDeferredSet.h"
#include "base/UnkRefHandle.h"
#include "base/global/CStrId.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/UnkTU1176.h"
#include "game/UnkTU1376.h"
#include "game/UnkTU367.h"
#include "game/UnkTU437.h"
#include "types.h"

using base::global::CStrId;

struct UnkCentralUnit_b49;
struct UnkCentralUnitDoor_b46a2;
struct UnkDoorCentral_A4_b76;
void CDoorCentralUnitLifeComponent_SetStayOpened(UnkDoorCentral_A4_b76* self);

struct UnkLinkHandle_l775fe0 {
    u8 pad_0x0[0x10];
    UnkWeakTarget* target;  // 0x10
};

// A lazily resolved link to another entity: the key is resolved to a handle on first use.
struct UnkEntityLink_l775fe0 {
    u8 pad_0x0[0x8];
    u8 key[0x28];                    // 0x8
    u32 state;                       // 0x30
    u32 pad_0x34;
    base::UnkRefHandle handle;       // 0x38: an UnkLinkHandle_l775fe0

    void Resolve() {
        if (state == 1) {
            {
                base::UnkRefPtrMember ref = fn_7100353be8(key);
                fn_710031d208(&handle, reinterpret_cast<const base::UnkRefHandle*>(&ref));
            }
            state = 2;
        }
    }
    UnkWeakTarget* Get() {
        Resolve();
        UnkLinkHandle_l775fe0* h = reinterpret_cast<UnkLinkHandle_l775fe0*>(handle.ptr);
        return h ? h->target : nullptr;
    }
};
static_assert(sizeof(UnkEntityLink_l775fe0) == 0x40, "UnkEntityLink_l775fe0 layout");

struct UnkCentralUnit_l775fe0 {
    u8 pad_0x0[0x4f8];
    UnkEntityLink_l775fe0 door;  // 0x4f8
    u8 pad_0x538[0x638 - 0x538];
    bool has_locks;                  // 0x638
    u8 pad_0x639[0x640 - 0x639];
    UnkEntityLink_l775fe0* locks;    // 0x640
    u32 lock_count;                  // 0x648
    u8 pad_0x64c[0x7c8 - 0x64c];
    base::UnkDeferredSet<void>* registered;  // 0x7c8
};

static inline void SetLockState(UnkDoorLock* lock, u32 state, CStrId id) {
    fn_71008d564c(lock, state, &id);
}

void fn_7100775fe0(UnkCentralUnit_l775fe0* self, s32 mode) {
    fn_71007717c0(reinterpret_cast<UnkCentralUnit_b49*>(self));
    if (self->registered) {
        fn_71004f3ea4(self->registered, &self->registered);
        self->registered = nullptr;
    }
    if (mode != 6)
        return;
    if (self->has_locks) {
        UnkEntityLink_l775fe0* end = self->locks + self->lock_count;
        for (UnkEntityLink_l775fe0* link = self->locks; link != end; ++link) {
            if (UnkWeakTarget* target = link->Get()) {
                if (UnkDoorLock* lock = fn_71002af860(target))
                    SetLockState(lock, 0, base::global::CStrId::Empty());
            }
        }
    }
    if (self->door.Get()) {
        if (UnkCentralUnitDoor_b46a2* central = fn_71004056fc(self->door.Get())) {
            CDoorCentralUnitLifeComponent_SetStayOpened(reinterpret_cast<UnkDoorCentral_A4_b76*>(central));
        } else if (UnkDoorLock* door = fn_71002af860(self->door.Get())) {
            OpenDoorUnlessOpen(door);
        }
    }
}
