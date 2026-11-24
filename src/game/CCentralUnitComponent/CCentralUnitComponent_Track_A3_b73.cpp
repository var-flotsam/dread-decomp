// A central unit component handler (tools/tu_map.py: the CCentralUnitComponent unit): on the first pass
// (state 1 at +0x468) takes the handle fn_7100353be8 builds from +0x440 into the slot at +0x470 and moves to
// state 2; then, when the slot's target is `other` and that object tracks something (+0x220), re-hooks the
// link at +0x768 (handle at +0x780) to it, as src/game/CChozoWarriorXGlaiveSpinAttack/UnkAttackTrackLink_b54.cpp
// without setting the callback. Returns 1. Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game/UnkTU437.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using UnkRef = base::UnkRefPtrMember;

// The slot's weak handle (base::UnkRefCounted's layout); its target is at +0x10.
struct UnkCentralUnitTarget_A3_b73 : base::UnkRefCounted {
    void* field_0x10;
};

struct UnkCentralUnitTrack_A3_b73 {
    u8 pad_0x0[0x440];
    u8 field_0x440[0x28];
    u32 state;  // 0x468
    u8 pad_0x46c[4];
    base::UnkRefHandle target;  // 0x470
    u8 pad_0x478[0x768 - 0x478];
    u8 link[0x18];           // 0x768
    UnkWeakRef handle;   // 0x780
};

u32 fn_7100773b74(UnkCentralUnitTrack_A3_b73* self, void* other) {
    if (self->state == 1) {
        {
            base::UnkRefPtrMember handle = fn_7100353be8(self->field_0x440);
            fn_710031d208(&self->target, reinterpret_cast<base::UnkRefHandle*>(&handle));
        }
        self->state = 2;
    }
    if (auto* target = static_cast<UnkCentralUnitTarget_A3_b73*>(self->target.ptr)) {
        if (target->field_0x10) {
            asm("" : "+r"(target));
            if (target->field_0x10 == other) {
                asm("" : "+r"(target));
                char* obj = static_cast<char*>(target->field_0x10);
                if (UnkWeakTarget* track = *reinterpret_cast<UnkWeakTarget**>(obj + 0x220)) {
                    void* link = self->link;
                    if (UnkWeakHandle* handle = self->handle.handle) {
                        if (UnkWeakTarget* t = handle->target)
                            fn_710117b3f0(t, link);
                        if (UnkWeakHandle* p = self->handle.handle) {
                            if (--p->ref_count == 0)
                                p->vf_0x10();
                            self->handle.handle = nullptr;
                        }
                    }
                    asm("" : "+r"(self));
                    CAbilityGhostAura_AssignWeakHandleToRef(track, &self->handle);
                    fn_710117b3e8(self->handle.handle ? self->handle.handle->target : nullptr, link);
                }
            }
        }
    }
    return 1;
}
