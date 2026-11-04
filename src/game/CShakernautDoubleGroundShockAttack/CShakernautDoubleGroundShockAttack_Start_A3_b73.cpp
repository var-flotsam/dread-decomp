// CShakernautDoubleGroundShockAttack's unit (tools/tu_map.py): the attack's start: after the base
// (fn_71003608ac) clears +0x30 and +0x34, sets +0x34 to 2 when the first value of the pair at +0x14810 of the
// owner's (+0x8) AI component (+0x240, a CShakernautAIComponent) is positive (else 1); keeps the owner's
// character class (+0x220) at +0x58, and sets +0x30 to 2 when the id it holds under the usual track kind
// (fn_7101177ae8) is one of four ids (g_unk_71727057c0..d8), else 1; then hooks the callback fn_71006a5d94
// (link at +0x38, handle at +0x50) up to the class, as
// src/game/CChozoWarriorXGlaiveSpinAttack/UnkAttackTrackLink_b54.cpp, and clears +0x60. Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkAttackBase;
void fn_71003608ac(UnkAttackBase* self);
void fn_7100316018(UnkWeakTarget* self, UnkWeakHandle** out);
void fn_710117b3e8(UnkWeakTarget* target, void* link);
void fn_71006a5d94();
struct UnkAnimationComponent;
base::global::UnkStringData** fn_7101177ae8(UnkAnimationComponent*, u64);

extern CStrId g_unk_71727057c0[4] __attribute__((visibility("hidden")));

struct UnkShakernautTrackLink_A3_b73 {
    u8 pad_0x0[0x8];
    void* context;
    void (*callback)();
};

struct UnkShakernautAttack_A3_b73 {
    u8 pad_0x0[0x8];
    char* owner;
    u8 pad_0x10[0x20];
    u32 field_0x30;
    u32 field_0x34;
    UnkShakernautTrackLink_A3_b73 link;  // 0x38
    UnkWeakHandle* handle;               // 0x50
    UnkWeakTarget* anim;                 // 0x58
    u32 field_0x60;
};

void fn_71006a441c(UnkShakernautAttack_A3_b73* self) {
    fn_71003608ac(reinterpret_cast<UnkAttackBase*>(self));
    self->field_0x30 = 0;
    self->field_0x34 = 0;
    UnkReflObject* ai = *reinterpret_cast<UnkReflObject**>(self->owner + 0x240);
    UnkReflObject* shakernaut = fn_710009d50c(ai->vf_0x0(), fn_71006ad0a4()) ? ai : nullptr;
    f32* pair = reinterpret_cast<f32*>(reinterpret_cast<char*>(shakernaut) + 0x14810);
    self->field_0x34 = pair[0] + pair[1] * 0.0f > 0.0f ? 2 : 1;
    self->anim = *reinterpret_cast<UnkWeakTarget**>(self->owner + 0x220);
    CStrId id(*reinterpret_cast<CStrId*>(
        fn_7101177ae8(reinterpret_cast<UnkAnimationComponent*>(self->anim), 0xf4061ff25a2163d6)));
    const CStrId* ids = g_unk_71727057c0;
    self->field_0x30 = ((id.data == ids[1].data) | (id.data == ids[0].data) | (id.data == ids[2].data) |
                        (id.data == ids[3].data))
                           ? 2
                           : 1;
    UnkShakernautTrackLink_A3_b73* link = &self->link;
    link->context = self;
    link->callback = fn_71006a5d94;
    UnkWeakTarget* track = self->anim;
    if (UnkWeakHandle* handle = self->handle) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, link);
        if (UnkWeakHandle* p = self->handle) {
            if (--p->ref_count == 0)
                p->vf_0x10();
            self->handle = nullptr;
        }
    }
    if (track) {
        asm("" : "+r"(self));
        fn_7100316018(track, &self->handle);
        fn_710117b3e8(self->handle ? self->handle->target : nullptr, link);
    }
    self->field_0x60 = 0;
}
