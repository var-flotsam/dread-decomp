// CSwarmControllerComponent unit (tools/tu_map.py unit 1163): the base message handler for a swarm
// member. One message type starts the billboard's effect on the member; another releases the member's
// hold and, on the controller's spawn period, drops an item at the member's position. True when it
// handled the message. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CCoreXAIComponent.h"
#include "game/CDizzeanSwarmControllerComponent.h"
#include "game/CSwarmControllerComponent.h"
#include "game/UnkActor.h"
#include "game/UnkTU1159.h"
#include "game/UnkTU2560.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::reflection::CClass;

struct UnkSwarmKeyHolder_l74ddd8 {
    u8 pad_0x0[0x30];
    u64 field_0x30;
};

struct UnkSwarmBillboard_l74ddd8 {
    void** vtable;
    u8 pad_0x8[0xc8];
    UnkSwarmKeyHolder_l74ddd8* field_0xd0;
};

struct UnkSwarmChain2_l74ddd8 {
    u8 pad_0x0[0x10];
    UnkSwarmBillboard_l74ddd8* field_0x10;
};

struct UnkSwarmChain1_l74ddd8 {
    u8 pad_0x0[0x10];
    UnkSwarmChain2_l74ddd8* field_0x10;
};

struct UnkSwarmTuning_l74ddd8 {
    u8 pad_0x0[0xf0];
    f32 field_0xf0;
    f32 field_0xf4;
};

struct UnkSelf {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0xb0];
    UnkSwarmChain1_l74ddd8* field_0xe8;
    u8 pad_0xf0[0xf0];
    UnkSwarmTuning_l74ddd8* field_0x1e0;
    u8 pad_0x1e8[4];
    f32 spawn_period;  // 0x1ec
    f32 spawn_timer;   // 0x1f0
};

// What a member holds: a count of holders and whether to let go when the last one leaves.
struct UnkSwarmHold_l74ddd8 {
    u8 pad_0x0[0x24];
    u8 field_0x24;
    u8 pad_0x25[0xbb];
    u32 count;  // 0xe0

    // Drops one holder; true when none is left.
    bool Release() {
        u32 left = count;
        if (left != 0)
            count = --left;
        return left == 0;
    }
};

struct UnkTarget {
    u8 pad_0x0[0x10];
    UnkSwarmHold_l74ddd8* hold;  // 0x10
    u8 pad_0x18[0xc];
    u16 field_0x24;
    u8 pad_0x26[0x12e];
    f32 field_0x154;
    u8 pad_0x158[0x30];
    u8 field_0x188;
};

struct UnkTypeRef {
    u8 pad_0x0[0x30];
    u64 field_0x30;
};

struct UnkMessage {
    UnkTypeRef* field_0x0;
};

struct UnkOffsetObj_1159_l7452d0;
UnkVec3f fn_71007452d0(const UnkOffsetObj_1159_l7452d0* self);

struct UnkDropSpawnComp_ovb0096;
bool fn_71007c1d50(UnkDropSpawnComp_ovb0096* self);
void fn_71007c47c0(UnkDropSpawnComp_ovb0096* self, const UnkVec3f* pos);
void fn_71007c36a0(UnkDropSpawnComp_ovb0096* self, const UnkVec3f* pos);

void fn_7100748234(UnkSelf* self, UnkSwarmHold_l74ddd8* hold);

extern char g_unk_710164aa18[];

// The billboard's effect on the member (the message 0x1e69e8c28c5be72e).
static void StartEffect(UnkSelf* self, UnkTarget* target) {
    if (!self->field_0xe8 || !self->field_0xe8->field_0x10)
        return;
    UnkSwarmBillboard_l74ddd8* obj = self->field_0xe8->field_0x10->field_0x10;
    if (!obj ||
        !fn_710009d50c(reinterpret_cast<const CClass* (*)(UnkSwarmBillboard_l74ddd8*)>(obj->vtable[0])(obj),
                       fn_710072be34()))
        return;
    void* manager = g_unk_71726da048;
    if (!manager) {
        manager = fn_71000839a0(0xe0, 0x3f);
        ConstructBillboardManager(manager);
        g_unk_71726da048 = manager;
    }
    u64 key = obj->field_0xd0->field_0x30;
    if (void* effect = fn_7100e1cb50(manager, &key, g_unk_710164aa18))
        fn_7100745440(target, effect, 0);
}

bool fn_710074ddd8(UnkSelf* self, UnkTarget* target, UnkMessage* message) {
    if (!target)
        return false;
    switch (message->field_0x0->field_0x30) {
    case 0x7531238cb72381d8ull: {
        UnkSwarmHold_l74ddd8* hold = target->hold;
        target->field_0x24 = 0;
        if (hold->Release() && hold->field_0x24)
            fn_7100748234(self, hold);

        UnkSwarmTuning_l74ddd8* tuning = self->field_0x1e0;
        if (!(tuning->field_0xf4 < 0.0f) && !(tuning->field_0xf0 < 0.0f) && !(target->field_0x154 > 0.0f))
            return false;
        f32 period = self->spawn_period;
        if (!(period > -1.0f))
            return false;
        f32 timer = self->spawn_timer;
        if (period > 0.0f) {
            timer += -1.0f;
            self->spawn_timer = timer;
        }
        if (!(timer <= 0.0f))
            return false;
        self->spawn_timer = period + timer;
        UnkDropSpawnComp_ovb0096* drop = reinterpret_cast<UnkDropSpawnComp_ovb0096*>(fn_71004dad10(self->owner));
        if (!drop || !target->field_0x188)
            return true;
        UnkVec3f pos = fn_71007452d0(reinterpret_cast<const UnkOffsetObj_1159_l7452d0*>(target));
        if (fn_71007c1d50(drop))
            fn_71007c47c0(drop, &pos);
        else
            fn_71007c36a0(drop, &pos);
        return true;
    }
    case 0x1e69e8c28c5be72eull:
        StartEffect(self, target);
        return true;
    default:
        return false;
    }
}