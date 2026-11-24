// CEnemyMovement unit: starts the movement. Runs the base step and the virtual at 0x338, takes the
// flag at +0xf9, hooks the movement's link up to the weak target of the owner's sub-object (+0x220)
// and, unless the game manager says otherwise, reports the flag at +0x1c09 through the virtual at 0x1e8
// before setting it. Names and layouts are placeholders.
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game/CCharacterMovement.h"
#include "game/CMovementComponent.h"
#include "game/UnkGameManager.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

u32 fn_71010ce780(char* p0, char* p1);

struct UnkEnemyMovementOwner_l0c90c0 {
    u8 pad_0x0[0x220];
    UnkWeakTarget* field_0x220;
};

struct UnkEnemyMovementLink_l0c90c0 {
    u8 pad_0x0[8];
    void* owner;
    u32 (*callback)(char*, char*);
};

struct UnkEnemyMovement_l0c90c0 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkEnemyMovementOwner_l0c90c0* owner;  // 0x30
    u8 pad_0x38[0xf9 - 0x38];
    u8 field_0xf9;
    u8 pad_0xfa[0x1be8 - 0xfa];
    UnkEnemyMovementLink_l0c90c0 link;  // 0x1be8
    UnkWeakHandle* handle;              // 0x1c00
    u8 pad_0x1c08;
    u8 field_0x1c09;

    void vf_0x338() { reinterpret_cast<void (*)(UnkEnemyMovement_l0c90c0*)>(vtable[0x338 / 8])(this); }
    void vf_0x1e8(u8 flag) { reinterpret_cast<void (*)(UnkEnemyMovement_l0c90c0*, u8)>(vtable[0x1e8 / 8])(this, flag); }
};
static_assert(__builtin_offsetof(UnkEnemyMovement_l0c90c0, handle) == 0x1c00, "layout");
static_assert(__builtin_offsetof(UnkEnemyMovement_l0c90c0, field_0x1c09) == 0x1c09, "layout");

void fn_71010c90c0(UnkEnemyMovement_l0c90c0* self) {
    fn_71009796dc(reinterpret_cast<UnkMove_b152*>(self));
    self->vf_0x338();
    self->field_0xf9 = fn_71010d0e38(reinterpret_cast<UnkMovement_71010d0e38*>(self));
    if (UnkWeakTarget* track = self->owner->field_0x220) {
        self->link.owner = self;
        self->link.callback = fn_71010ce780;
        if (UnkWeakHandle* handle = self->handle) {
            if (UnkWeakTarget* target = handle->target)
                fn_710117b3f0(target, &self->link);
            if (UnkWeakHandle* p = self->handle) {
                if (--p->ref_count == 0)
                    p->vf_0x10();
                self->handle = nullptr;
            }
        }
        CAbilityGhostAura_AssignWeakHandleToRef(track, &self->handle);
        fn_710117b3e8(self->handle ? self->handle->target : nullptr, &self->link);
    }
    if (!fn_7100deabe4(g_unk_7101d47310) && !fn_7100deac04(g_unk_7101d47310)) {
        self->vf_0x1e8(self->field_0x1c09);
        self->field_0x1c09 = 1;
    }
}
