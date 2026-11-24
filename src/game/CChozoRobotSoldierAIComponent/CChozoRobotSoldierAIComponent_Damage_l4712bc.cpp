// CChozoRobotSoldierAIComponent's unit (tools/tu_map.py unit 694): the cannon shot's damage set-up for a
// request. After the base step, when the owner's target is this soldier's own part, the request takes the
// owner's facing; it is marked (+0xc8) and, for a charged shot, gets the charged (or elite) damage name and
// tunable value. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "game/CCharClassAttackComponent.h"
#include "game/UnkTU1370.h"
#include "game/UnkTU2702.h"
#include "game/UnkTU55.h"
#include "types.h"

using base::global::CStrId;

struct UnkRobotSoldierTarget_l4712bc {
    u8 pad_0x0[0x1a0];
    void* target;  // 0x1a0
};

struct UnkRobotSoldierOwner_l4712bc {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
    u8 pad_0x78[0x200];
    UnkRobotSoldierTarget_l4712bc* targeting;  // 0x278
};

struct UnkRobotSoldierAI_l4712bc {
    u8 pad_0x0[0x30];
    UnkRobotSoldierOwner_l4712bc* owner;  // 0x30
    u8 pad_0x38[0x14a18 - 0x38];
    u8 part[0x108];  // 0x14a18
    bool charged;  // 0x14b20
    u8 pad_0x14b21[0x27790 - 0x14b21];
    bool elite;  // 0x27790
};

struct UnkRobotSoldierRequest_l4712bc {
    u8 pad_0x0[0x49];
    u8 flags[3];  // 0x49: [0] says the direction below is set
    f32 dir_x;  // 0x4c
    f32 dir_y;  // 0x50
    f32 dir_z;  // 0x54
    u8 pad_0x58[0x60];
    CStrId id;  // 0xb8
    u8 pad_0xc0[0x8];
    u32 field_0xc8;  // 0xc8
};

// The damages of the attack tunable (g_unk_7172716160).
struct UnkRobotSoldierDamageTunable_l4712bc {
    u8 pad_0x0[0xd0];
    f32 damage_charged;  // 0xd0
    u8 pad_0xd4[0x18];
    f32 damage_charged_elite;  // 0xec
};

struct UnkRobotSoldierDir_l4712bc {
    f32 x;
    f32 y;
    f32 z;
};

void CChozoRobotSoldierAIComponent_ApplyChargedCannonShotDamageToRequest(UnkRobotSoldierAI_l4712bc* self, void* arg, UnkRobotSoldierRequest_l4712bc* request) {
    fn_7101105c90(reinterpret_cast<UnkKraidAI2*>(self), arg, reinterpret_cast<UnkKraidRequest*>(request));
    if (UnkRobotSoldierTarget_l4712bc* targeting = self->owner->targeting) {
        void* target = targeting->target;
        if (target && target == self->part) {
            request->flags[0] = 1;
            const UnkMatrix44& m = fn_710002614c(&self->owner->transform);
            f32 x = m.m[2];
            f32 y = m.m[6];
            f32 z = m.m[10];
            UnkRobotSoldierDir_l4712bc dir = {x, y, z};
            fn_710007b444(&request->dir_x, reinterpret_cast<const UnkVec3*>(&dir));
            request->dir_y = 1.0f;
        }
    }
    if (request->field_0xc8 == 0)
        request->field_0xc8 = 1;
    if (!self->charged)
        return;
    const UnkRobotSoldierDamageTunable_l4712bc* tunable =
        reinterpret_cast<const UnkRobotSoldierDamageTunable_l4712bc*>(&g_unk_7172716160);
    UnkRequest* damage = reinterpret_cast<UnkRequest*>(request);
    if (self->elite) {
        static CStrId s_id("fDamageChozoRobotSoldierCannonShotChargedElite", true);
        fn_71008c1538(damage, tunable->damage_charged_elite);
        request->id = s_id;
    } else {
        static CStrId s_id("fDamageChozoRobotSoldierCannonShotCharged", true);
        fn_71008c1538(damage, tunable->damage_charged);
        request->id = s_id;
    }
}
