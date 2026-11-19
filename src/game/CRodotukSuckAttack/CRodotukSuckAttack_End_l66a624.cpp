// CRodotukSuckAttack's unit (tools/tu_map.py unit 990): the attack's end. After the shared attack
// reset (fn_71003609b8), a pending player-info effect (+0x68) is stopped, the attack's own reset runs
// (fn_710066a760), the owner's timeline (+0x2e8) gets an id with the all-flags mask, and the AI
// component's reset (fn_71010fa790) and the outlines at +0x70 follow. Names and layouts are
// placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/CPlayerInfo.h"
#include "game/CRodotukSuckAttack.h"
#include "game/UnkActor.h"
#include "game/UnkAttack.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2702.h"
#include "game/UnkTimelineCalls.h"
#include "game/logic/collision/CPolygonCollectionShape.h"
#include "types.h"

struct UnkPlayerInfo_ovb0071;

struct UnkRodotukKey_l66a624;
extern UnkRodotukKey_l66a624 g_unk_710164a4d8;  // reached through the GOT

struct UnkRodotukSuckAttack_l66a624 {
    void** vtable;
    UnkActor* owner;  // 0x8
    u8 pad_0x10[0x58];
    bool effect_pending;  // 0x68
    u8 pad_0x69[0x7];
    u8 outlines[8];  // 0x70
};

void fn_710066a624(UnkRodotukSuckAttack_l66a624* self) {
    fn_71003609b8(reinterpret_cast<UnkAttackBase*>(self));
    if (self->effect_pending) {
        fn_7100f0dd3c(reinterpret_cast<UnkPlayerInfo_ovb0071*>(g_unk_7101d47310->field_0x38));
        self->effect_pending = false;
    }
    fn_710066a760(self);
    if (auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8)) {
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_710164a4d8), &mask.id, false, -1.0f);
    }
    fn_71010fa790(self->owner->field_0x240);
    fn_7100d633ec(reinterpret_cast<UnkTileOutlines*>(self->outlines));
}
