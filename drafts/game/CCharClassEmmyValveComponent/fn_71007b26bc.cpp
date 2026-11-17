// CCharClassEmmyValveComponent unit (tools/tu_map.py, unit 1212): fn_71007b26bc caches the owner's two valve
// collision objects and animation component, starts the valve's animation for its current state and then
// restores the valve from the game manager's blackboard. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "game/CAnimationComponent.h"
#include "game/CCharClassEmmyValveComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkGameManager.h"
#include "types.h"

extern const base::global::CStrId g_unk_717271a2b8 __attribute__((visibility("hidden")));

f32 fn_7101177e24(UnkAnimationComponent* self, u64 key);
void fn_710117ab48(UnkAnimationComponent* self, bool flag);

struct UnkValveAnimEntry_l7b26bc {
    u8 pad_0x0[0x48];
    u64 hash;  // 0x48
};

struct UnkValveOwner_l7b26bc {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x2a0 - 0x228];
    UnkCollisionContainer* collision;  // 0x2a0
};

struct UnkValve_l7b26bc {
    void** vtable;
    u8 pad_0x8[0x30 - 0x8];
    UnkValveOwner_l7b26bc* owner;  // 0x30
    u8 pad_0x38[0x240 - 0x38];
    UnkAnimationComponent* animation;  // 0x240
    UnkCollisionObject* field_0x248;   // 0x248
    UnkCollisionObject* field_0x250;   // 0x250

    void vf_0x140(CBlackboard* blackboard) {
        reinterpret_cast<void (*)(UnkValve_l7b26bc*, CBlackboard*)>(vtable[0x140 / 8])(this, blackboard);
    }
};

void fn_71007b26bc(UnkValve_l7b26bc* self) {
    if (!self->owner)
        return;
    UnkCollisionContainer* collision = self->owner->collision;
    if (!collision)
        return;
    self->field_0x248 = fn_71008640a4(collision, &g_unk_717271a2c0);
    self->field_0x250 = fn_71008640a4(collision, &g_unk_717271a2b8);
    self->animation = self->owner->animation;
    if (!self->animation)
        return;
    if (fn_710117ae38(self->animation, 0x6a17390fc912b900, false)) {
        UnkSpitSend(self->animation, 0xe43bbd07b9e307b9, 0xf4061ff25a2163d6);
        fn_7101177e24(self->animation, 0xf4061ff25a2163d6);
        fn_710117ab48(self->animation, false);
    } else if (fn_710117ae38(self->animation, 0xa0e33be014f44bd7, false)) {
        UnkAnimationComponent* animation = self->animation;
        UnkValveAnimEntry_l7b26bc* entry =
            static_cast<UnkValveAnimEntry_l7b26bc*>(fn_7101177d2c(animation, 0xf4061ff25a2163d6));
        UnkSpitSend(animation, entry->hash, 0xf4061ff25a2163d6);
    }
    self->vf_0x140(&g_unk_7101d47310->field_0x968);
}
