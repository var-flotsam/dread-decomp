// CGooplotAIComponent (tools/tu_map.py): a message handler. After the behaviour-tree AI's handler
// (fn_710036974c): with the message's flag (+8) set, hands the owner's object at +0x218 a kind-3 parameter
// block when it is in mode 3 or 2 and plays "gooplottail" on the owner's +0x280; otherwise, in mode 1, sends
// the owner's animation (+0x220) a hash by the track it holds (4, 5 or its own). Names and layouts are
// placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "game/CAnimationComponent.h"
#include "game/CAutsharpAIComponent.h"
#include "game/CBehaviorTreeAIComponent.h"
#include "game/CEnemyMovement.h"
#include "game/CFXComponent.h"
#include "types.h"

struct UnkGooplotKind3Params_l59b8d4 {
    void** vtable;
    u32 field_0x8;
    u8 field_0xc;
    u8 pad_0xd[7];
    u8 field_0x14;
    u8 pad_0x15[3];
    u8 field_0x18[13];
    u8 pad_0x25[3];
    u8 field_0x28[13];
    u8 pad_0x35[3];
    u32 kind;  // 0x38
    u8 field_0x3c;
    u8 field_0x3d;
};

struct UnkGooplotTrackHash_l59b8d4 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkGooplotAnim_l59b8d4 {
    u8 pad_0x0[0x2b8];
    UnkGooplotTrackHash_l59b8d4* field_0x2b8;
};

struct UnkGooplotOwner_l59b8d4 {
    u8 pad_0x0[0x218];
    UnkEffectSink* sink;            // 0x218
    UnkGooplotAnim_l59b8d4* anim;   // 0x220
    u8 pad_0x228[0x280 - 0x228];
    UnkWorld* world;  // 0x280
};

struct UnkGooplotAI_l59b8d4 {
    u8 pad_0x0[0x30];
    UnkGooplotOwner_l59b8d4* owner;
};

struct UnkGooplotMessage_l59b8d4 {
    u8 pad_0x0[0x8];
    bool field_0x8;
};

u32 fn_710059b8d4(UnkGooplotAI_l59b8d4* self, UnkGooplotMessage_l59b8d4* msg) {
    UnkEffectSink* sink = self->owner->sink;
    UnkGooplotAnim_l59b8d4* anim = self->owner->anim;
    u32 result = fn_710036974c(reinterpret_cast<UnkSclawkAI_b44a2*>(self),
                               reinterpret_cast<UnkSclawkMessage_b44a2*>(msg));
    switch (static_cast<int>(msg->field_0x8)) {
    case 1:
        UnkGooplotKind3Params_l59b8d4 params;
        // Mode 3 is only asked of an object that exists; mode 2 is asked either way.
        if (sink ? fn_71010cc3a4(sink) == 3 || fn_71010cc3a4(sink) == 2 : fn_71010cc3a4(nullptr) == 2) {
            params.field_0x14 = 0;
            __builtin_memset(params.field_0x18, 0, sizeof(params.field_0x18));
            __builtin_memset(params.field_0x28, 0, sizeof(params.field_0x28));
            params.field_0x8 = 0;
            params.vtable = g_unk_7101a2e208 + 2;
            params.kind = 3;
            params.field_0x3c = 0;
            params.field_0x3d = 1;
            params.field_0xc = 1;
            fn_71010c7e6c(sink, &params, 1);
        }
        if (UnkWorld* world = self->owner->world)
            fn_710114fe8c(world, fn_7100001570("gooplottail", 11), 0, 0);
        break;
    default: {
        if (!sink)
            break;
        u32 mode = fn_71010cc3a4(sink);
        if (!anim || mode != 1)
            break;
        u64 hash;
        if (fn_7101178e6c(reinterpret_cast<UnkTakumakuComponent*>(anim), 4))
            hash = 0x62ae948d387883a6;
        else if (fn_7101178e6c(reinterpret_cast<UnkTakumakuComponent*>(anim), 5))
            hash = 0xa04fe3218635d058;
        else
            hash = anim->field_0x2b8->hash;
        UnkSpitSend(anim, hash, 0xf4061ff25a2163d6);
        break;
    }
    }
    return result;
}