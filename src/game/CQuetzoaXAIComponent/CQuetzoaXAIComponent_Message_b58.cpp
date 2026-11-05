// CQuetzoaXAIComponent unit (tools/tu_map.py): a message handler override. After the base handler
// (fn_71003adaf8), a message with flag 0x8000 (+0xb4), when the owner (+0x30) has its character class
// (+0x220), effect sink (+0x218) and an actor to act against (fn_7100d7c990), gives the sink the
// largest-float block (as fn_7100703358 in src/game/UnkUnit1123/UnkUnit1123_b38.cpp), sends the class
// hash 0x9b214780df6c8f00 with mode 1 (as src/game_s2/UnkSpitSend.h sends), and turns the owner (vtable slot
// 0xd8) a quarter turn by the sign of the other actor's facing. Messages without 0x240 whose flags miss
// 0x2800041c, or arrive while +0x14bc8 is clear, answer 2. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/UnkMatrix44.h"
#include "game/UnkActor.h"
#include "types.h"

struct UnkVec3_b58a4 {
    f32 x;
    f32 y;
    f32 z;
};

template <class T>
struct UnkOpt_b58a4 {
    T value;
    bool has;
};

// The block sent to the effect sink (vtable g_unk_7101a24b80, kind 0).
struct UnkSinkParams_b58a4 {
    void** vtable;
    UnkOpt_b58a4<u32> field_0x8;
    UnkOpt_b58a4<f32> field_0x10;
    UnkOpt_b58a4<UnkVec3_b58a4> field_0x18;
    UnkOpt_b58a4<UnkVec3_b58a4> field_0x28;
    u32 kind;  // 0x38
};

extern void* g_unk_7101a24b80[];

struct UnkEffectSink;
void fn_71010c7e6c(UnkEffectSink* sink, void* params, u32 flags);

struct UnkQuetzoaXOwner_b58;
struct UnkQuetzoaXOwnerVtbl_b58 {
    void* slots[0xd8 / 8];
    void (*turn)(UnkQuetzoaXOwner_b58* self, const UnkVec3_b58a4* angles, u32 flag);
};
struct UnkQuetzoaXOwner_b58 {
    UnkQuetzoaXOwnerVtbl_b58* vtable;
};

struct UnkQuetzoaXAI_b58 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x14bc8 - 0x38];
    bool field_0x14bc8;
};

struct UnkQuetzoaXMessage_b58 {
    u8 pad_0x0[0xb4];
    u32 flags;
};

u32 fn_71003adaf8(UnkQuetzoaXAI_b58* self, UnkQuetzoaXMessage_b58* msg);

u32 fn_710063f944(UnkQuetzoaXAI_b58* self, UnkQuetzoaXMessage_b58* msg) {
    u32 result = fn_71003adaf8(self, msg);
    u32 flags = msg->flags;
    if (flags & 0x8000) {
        UnkActor* owner = self->owner;
        void* anim = owner->field_0x220;
        UnkEffectSink* sink = static_cast<UnkEffectSink*>(owner->field_0x218);
        UnkActor* other = fn_7100d7c990(owner);
        if (anim && sink && other) {
            UnkSinkParams_b58a4 params;
            params.vtable = g_unk_7101a24b80 + 2;
            params.field_0x8.has = false;
            params.field_0x10.has = false;
            params.kind = 0;
            __builtin_memset(&params.field_0x18, 0, 13);
            __builtin_memset(&params.field_0x28, 0, 13);
            params.field_0x10.value = 3.4028235e+38f;
            params.field_0x10.has = true;
            fn_71010c7e6c(sink, &params, 0);
            base::reflection::UnkReflTypeIdArg mode;
            mode.id = 1;
            base::reflection::UnkReflTypeIdArg mask;
            mask.id = UnkAll_SpitSend().value;
            fn_7101176690(anim, 0x9b214780df6c8f00, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740,
                          &mask);
            UnkQuetzoaXOwner_b58* turner = reinterpret_cast<UnkQuetzoaXOwner_b58*>(self->owner);
            const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(other->field_0x38));
            f32 yaw = m.m[2] >= 0.0f ? -1.5707964f : 1.5707964f;
            UnkVec3_b58a4 angles;
            angles.x = 0.0f;
            angles.z = 0.0f;
            angles.y = yaw;
            turner->vtable->turn(turner, &angles, 1);
        }
    } else if (!(flags & 0x240)) {
        if (!(flags & 0x2800041c) || !self->field_0x14bc8)
            result = 2;
    }
    return result;
}
