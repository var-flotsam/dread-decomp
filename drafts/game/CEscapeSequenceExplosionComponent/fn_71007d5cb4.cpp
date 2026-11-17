// CEscapeSequenceExplosionComponent unit (tools/tu_map.py): after the base step, takes the component off the
// game manager's list for its explosion kind (+0x40): kinds 1 and 2 name the unit's second and third static ids,
// any other the first. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "types.h"

using base::global::CStrId;

extern CStrId g_unk_717271e2d0 __attribute__((visibility("hidden")));
extern CStrId g_unk_717271e2d8 __attribute__((visibility("hidden")));
extern CStrId g_unk_717271e2e0 __attribute__((visibility("hidden")));

struct UnkSpawnGroupKey;

struct UnkEscapeExplosion_l7d5cb4 {
    u8 pad_0x0[0x40];
    s32 kind;  // 0x40
};

static inline CStrId KindId(s32 kind) {
    switch (kind) {
    case 1:
        return g_unk_717271e2d8;
    case 2:
        return g_unk_717271e2e0;
    default:
        return g_unk_717271e2d0;
    }
}

void fn_71007d5cb4(UnkEscapeExplosion_l7d5cb4* self) {
    fn_71010c4230(reinterpret_cast<UnkHandleOwner_71010c4230*>(self));
    UnkGameManager* manager = g_unk_7101d47310;
    CStrId id = KindId(self->kind);
    fn_7100de9730(manager, reinterpret_cast<UnkSpawnGroupKey*>(&id), self);
}
