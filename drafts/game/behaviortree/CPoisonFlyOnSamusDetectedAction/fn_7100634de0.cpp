// behaviortree::CPoisonFlyOnSamusDetectedAction's unit (tools/tu_map.py unit 952): the start. After the base
// start it hands the owner's navigation (+0x298) a kind-5 block carrying the side of the actor it acts against
// (-1, 0 or 1 along x). Then, from whether the AI manager's registry (g_unk_71726af468, created on first use)
// finds the owner for that actor and whether the owner already faces that side, it keeps the id of the
// reaction in +0x30 and sends it to the character class (+0x220): the turn track gets the blend descriptors.
// An owner that is not the registry's pick with no AI flag instead rolls a new wait time (+0x2c). Names and
// layouts are placeholders.
#include "../../../../src/game_s2/UnkSpitSend.h"
#include "base/UnkMatrix44.h"
#include "base/global/CRntVector.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkManager_71726af468.h"
#include "game/UnkTU1730.h"
#include "game/behaviortree/CBehavior.h"
#include "game_s2/UnkParams.h"
#include "types.h"

extern "C" int rand();

extern f32 g_unk_7101a58110;
extern f32 g_unk_7101a58114;
UnkActor* fn_710063c1f4(void* registry, UnkActor* target, bool flag);
extern void* g_unk_7101a25b10[];

struct UnkPoisonFlyParams_l634de0 {
    void** vtable;
    f32 dir_x;
    f32 dir_y;
    f32 dir_z;
    u8 dir_set;
    u8 pad_0x15[3];
    u32 field_0x18;
    u8 field_0x1c;
    u8 pad_0x1d[3];
    u64 field_0x20[2];
    u8 field_0x30;
    u8 pad_0x31[3];
    u32 field_0x34;
    u8 pad_0x38;
    u8 field_0x39;
    u8 pad_0x3a;
    u8 field_0x3b;
    u8 pad_0x3c;
    u8 field_0x3d;
};

struct UnkPoisonFlyActor_l634de0 {
    u8 pad_0x0[0x38];
    UnkMatrix44 xform;  // 0x38
    u8 pad_0x78[0x298 - 0x78];
    void* navigation;  // 0x298
};

struct UnkPoisonFlyAI_l634de0 {
    u8 pad_0x0[0x1220];
    bool field_0x1220;
};

struct UnkPoisonFlySamus_l634de0 {
    u8 pad_0x0[0x44];
    f32 x;
};

struct UnkPoisonFlyDetected_l634de0 {
    u8 pad_0x0[0x18];
    UnkActor* actor;  // 0x18
    u8 pad_0x20[0xc];
    f32 wait;  // 0x2c
    u64 reaction;  // 0x30
};

void fn_7100634de0(UnkPoisonFlyDetected_l634de0* self) {
    base::reflection::UnkReflTypeIdArg mode_a_same, mask_a_same;
    base::reflection::UnkReflTypeIdArg mode_a_turn, mask_a_turn;
    base::reflection::UnkReflTypeIdArg mode_a_other, mask_a_other;
    base::reflection::UnkReflTypeIdArg mode_b_same, mask_b_same;
    base::reflection::UnkReflTypeIdArg mode_b_turn, mask_b_turn;
    base::reflection::UnkReflTypeIdArg mode_b_other, mask_b_other;
    fn_7100394bd0(reinterpret_cast<UnkStruct_0x68*>(self));
    UnkActor* target = fn_7100d7c990(self->actor);
    UnkPoisonFlyActor_l634de0* actor = reinterpret_cast<UnkPoisonFlyActor_l634de0*>(self->actor);
    f32 toward = reinterpret_cast<UnkPoisonFlySamus_l634de0*>(target)->x -
                 reinterpret_cast<UnkPoisonFlySamus_l634de0*>(self->actor)->x;
    f32 side = (toward > 0.0f) - (toward < 0.0f);
    {
        UnkPoisonFlyParams_l634de0 params;
        params.vtable = g_unk_7101a25b10 + 2;
        params.field_0x1c = 0;
        params.field_0x20[0] = 0;
        params.field_0x20[1] = 0;
        params.field_0x30 = 0;
        params.field_0x39 = 0;
        params.field_0x3b = 0;
        params.field_0x3d = 0;
        params.field_0x34 = 5;
        params.dir_x = side;
        params.dir_y = 0.0f;
        params.dir_z = 0.0f;
        params.dir_set = 1;
        fn_71006f4b0c(actor->navigation, &params, 1);
    }
    UnkActor* owner = self->actor;
    void* animation = owner->field_0x220;
    const UnkMatrix44& m = fn_710002614c(&reinterpret_cast<UnkPoisonFlyActor_l634de0*>(owner)->xform);
    f32 facing = (m.m[2] > 0.0f) - (m.m[2] < 0.0f);
    UnkActor* actor_now = self->actor;
    UnkManager_71726af468* manager = g_unk_71726af468;
    if (!manager) {
        manager = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(manager);
        g_unk_71726af468 = manager;
    }
    UnkActor* picked = fn_710063c1f4(manager->field_0x80, target, true);
    const u64 kind = 0xf4061ff25a2163d6;
    if (actor_now == picked) {
        if (facing == side) {
            self->reaction = 0x5e8166aba0571043;
            mode_a_same.id = 0;
            mask_a_same.id = UnkAll_SpitSend().value;
            fn_7101176690(animation, 0x5e8166aba0571043, kind, &mode_a_same, &g_unk_710164b958, &g_unk_7172769740,
                          &mask_a_same);
        } else {
            self->reaction = 0x0f844a14fa1bf449;
            if (fn_710117b348(static_cast<UnkAnimationComponent*>(animation), fn_7100d2b3a8())) {
                mode_a_turn.id = 0;
                mask_a_turn.id = UnkAll_SpitSend().value;
                fn_7101176690(animation, 0x0f844a14fa1bf449, kind, &mode_a_turn, &g_unk_710164b968, &g_unk_7172769770,
                              &mask_a_turn);
            } else {
                mode_a_other.id = 0;
                mask_a_other.id = UnkAll_SpitSend().value;
                fn_7101176690(animation, 0x0f844a14fa1bf449, kind, &mode_a_other, &g_unk_710164b958, &g_unk_7172769740,
                              &mask_a_other);
            }
        }
    } else if (!static_cast<UnkPoisonFlyAI_l634de0*>(owner->field_0x240)->field_0x1220) {
        f32 base_wait = g_unk_7101a58110;
        f32 range = g_unk_7101a58114;
        self->wait = base_wait + ((f32)((rand() & 0x7fff) * (1.0 / 32767.0)) * (range + range) - range);
    } else if (facing != side) {
        self->reaction = 0x1e4b917f54b4f6f2;
        if (fn_710117b348(static_cast<UnkAnimationComponent*>(animation), fn_7100d2b3a8())) {
            mode_b_turn.id = 0;
            mask_b_turn.id = UnkAll_SpitSend().value;
            fn_7101176690(animation, 0x1e4b917f54b4f6f2, kind, &mode_b_turn, &g_unk_710164b968, &g_unk_7172769770,
                          &mask_b_turn);
        } else {
            mode_b_other.id = 0;
            mask_b_other.id = UnkAll_SpitSend().value;
            fn_7101176690(animation, 0x1e4b917f54b4f6f2, kind, &mode_b_other, &g_unk_710164b958, &g_unk_7172769740,
                          &mask_b_other);
        }
    } else {
        self->reaction = 0x1720955b012a5867;
        mode_b_same.id = 0;
        mask_b_same.id = UnkAll_SpitSend().value;
        fn_7101176690(animation, 0x1720955b012a5867, kind, &mode_b_same, &g_unk_710164b958, &g_unk_7172769740,
                      &mask_b_same);
    }
}
