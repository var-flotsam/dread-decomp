// CPoisonFlyAIComponent unit (tools/tu_map.py): a message handler override. After the base AI
// component's handler (fn_710036974c), the lazily created manager's part at +0x80 forgets the owner
// (fn_710063a748, as in src/game/CPoisonFlyDiveAttack/CPoisonFlyDiveAttack_3.cpp). When the
// message's flag (+8) is set, a parameter block (vtable 0x7101a2e208, as in
// src/game/CChozoCommanderAIComponent/fn_7100453eec.cpp) goes to the owner's object at +0x218;
// otherwise the flag at +0x121e is stored in the blackboard section (as fn_71006395fc in
// CPoisonFlyAIComponent_b36.cpp). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CAutsharpAIComponent.h"
#include "game/CBehaviorTreeAIComponent.h"
#include "game/CBlackboard.h"
#include "game/CEnemyMovement.h"
#include "game/UnkActor.h"
#include "game/UnkManager_71726af468.h"
#include "game/UnkTUProperty.h"
#include "types.h"

// A typed value as the section setter takes it (as in src/game/CXParasiteAIComponent.cpp).
struct UnkTypedValue {
    const base::reflection::CType* type;
    void* value;
};
void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);

// As src/game/CSclawkAIComponent/CSclawkAIComponent_b44.cpp declares it.
struct UnkSclawkAI_b44a2;
struct UnkSclawkMessage_b44a2;

void fn_710063a748(u8* part, UnkActor* actor);

struct UnkEffectSink;

extern base::global::CStrId g_unk_71726b7cb8;                                    // the blackboard section
extern UnkPropertyName g_unk_71726fa598 __attribute__((visibility("hidden")));  // the property in it

struct UnkPoisonFlyOwner_b55 {
    u8 pad_0x0[0x218];
    UnkEffectSink* field_0x218;
};

struct UnkPoisonFlyAI_b55 {
    u8 pad_0x0[0x30];
    UnkPoisonFlyOwner_b55* owner;
    u8 pad_0x38[0x11e6];
    bool field_0x121e;
    u8 pad_0x121f[0x41];
    CBlackboard field_0x1260;
};

struct UnkPoisonFlyMessage_b55 {
    u8 pad_0x0[8];
    bool field_0x8;
};

struct UnkPoisonFlyParams_b55 {
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
    u32 field_0x38;
    u8 field_0x3c;
    u8 field_0x3d;
};

u32 fn_71006394d0(UnkPoisonFlyAI_b55* self, UnkPoisonFlyMessage_b55* msg) {
    u32 result = fn_710036974c(reinterpret_cast<UnkSclawkAI_b44a2*>(self),
                               reinterpret_cast<UnkSclawkMessage_b44a2*>(msg));
    UnkManager_71726af468* manager = g_unk_71726af468;
    if (!manager) {
        manager = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 3));
        fn_7100da7e2c(manager);
        g_unk_71726af468 = manager;
    }
    fn_710063a748(manager->pad_0x0 + 0x80, reinterpret_cast<UnkActor*>(self->owner));
    if (msg->field_0x8) {
        UnkEffectSink* sink = self->owner->field_0x218;
        UnkPoisonFlyParams_b55 params;
        params.field_0x14 = 0;
        __builtin_memset(params.field_0x18, 0, 13);
        __builtin_memset(params.field_0x28, 0, 13);
        params.field_0x8 = 0;
        params.vtable = g_unk_7101a2e208 + 2;
        params.field_0x38 = 3;
        params.field_0x3c = 0;
        params.field_0x3d = 1;
        params.field_0xc = 1;
        fn_71010c7e6c(sink, &params, 1);
    } else {
        CBlackboard* blackboard = &self->field_0x1260;
        UnkTypedValue v;
        bool copy = self->field_0x121e;
        if (CBlackboard::CSection* s = fn_71010472e0(blackboard, g_unk_71726b7cb8)) {
            if (!blackboard->field_0xe8) {
                v.type = fn_710000d228();
                v.value = &copy;
                fn_7100332c80(s, &g_unk_71726fa598, &v);
            }
        }
    }
    return result;
}
