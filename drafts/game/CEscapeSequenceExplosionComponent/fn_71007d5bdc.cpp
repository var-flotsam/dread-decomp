// CEscapeSequenceExplosionComponent unit (tools/tu_map.py): after the base step, registers the component
// with the game manager under the string id of its explosion kind (+0x40). Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "game/CComponent.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::global::CStrId;

extern CStrId g_unk_717271e2d0 __attribute__((visibility("hidden")));
extern CStrId g_unk_717271e2d8 __attribute__((visibility("hidden")));
extern CStrId g_unk_717271e2e0 __attribute__((visibility("hidden")));

struct UnkEscapeExplosion_l7d5bdc {
    u8 pad_0x0[0x40];
    s32 kind;  // 0x40
};

// Kinds 0, 1 and 2 name the unit's three ids; any other kind is taken as kind 0.
static inline const CStrId& KindId(s32 kind) {
    switch (kind) {
    case 0:
        return g_unk_717271e2d0;
    case 1:
        return g_unk_717271e2d8;
    case 2:
        return g_unk_717271e2e0;
    default:
        return g_unk_717271e2d0;
    }
}

void fn_71007d5bdc(UnkEscapeExplosion_l7d5bdc* self) {
    fn_71010c4230(reinterpret_cast<UnkHandleOwner_71010c4230*>(self));
    UnkGameManager* manager = g_unk_7101d47310;
    CStrId id(KindId(self->kind));
    fn_7100de9540(manager, reinterpret_cast<const u64*>(&id), reinterpret_cast<UnkWeakTarget*>(self));
}
