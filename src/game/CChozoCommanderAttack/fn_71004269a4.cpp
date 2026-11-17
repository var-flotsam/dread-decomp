// CChozoCommanderAttack unit (tools/tu_map.py unit 644): after the base reset (fn_71003609b8), when the owner's
// AI component is a CChozoCommanderAIComponent, reads the owner's current animation id (kind
// 0x71b15e29dc542a11) and sends one of two requests (fn_7100426b54, mode 1) by it. Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAnimationComponent.h"
#include "game/UnkAttack.h"
#include "game_s2/UnkChozoCommanderAttack.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

void fn_71004269a4(UnkChozoCommanderAttack* self) {
    fn_71003609b8(reinterpret_cast<UnkAttackBase*>(self));
    UnkReflObject* ai = static_cast<UnkReflObject*>(self->field_0x8->field_0x240);
    if (!ai || !fn_710009d50c(ai->vf_0x0(), fn_710044e708()))
        return;
    CStrId id(*fn_7101177ae8(static_cast<UnkAnimationComponent*>(self->field_0x8->field_0x220),
                             0x71b15e29dc542a11));
    switch (id.data->Key()) {
    case 0x8f4e8a524ebacc1b:
    case 0xec3def3d82b1cd59:
    case 0x2376ca3f9e17e420:
        fn_7100426b54(self, 0x4b93ac6c479d9d31, true);
        break;
    case 0xdc61630bd3a4ca6f:
    case 0x705923660309e254:
    case 0x3e52c28f7c89b5bd:
        fn_7100426b54(self, 0xd20a8883f919f07e, true);
        break;
    }
}
