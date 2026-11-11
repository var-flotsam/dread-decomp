#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/global/UnkStringPoolBody.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::global::CStrId;

extern "C" int rand();

struct UnkActionTimeline_b0265;
void fn_7101137a38(UnkActionTimeline_b0265* timeline, const CStrId* action, bool flag, u32 value, f32 time);
struct UnkWeakTarget;
extern u32 g_unk_71727bebb8;
UnkWeakTarget* fn_7100debbc8(UnkGameManager* manager, const u64* key, const u32* kind);

struct UnkOtherActionActor {
    u8 pad_0x0[0x220];
    void* animation;  // 0x220
    u8 pad_0x228[0x30];
    UnkActionTimeline_b0265* timeline;  // 0x258
};

struct UnkActionValue_b0265 {
    u64 field_0x0;
    f32 value;
};

struct UnkOtherEntityRef {
    u8 pad_0x0[0x30];
    u64 key;  // 0x30
};

struct UnkSetActionToOtherEntityEvent {
    u8 pad_0x0[0x10];
    UnkOtherEntityRef* entity;  // 0x10
    bool flag;                  // 0x18
    s32 value;                  // 0x1c
    CStrId action;              // 0x20
    bool direct;                // 0x28
    f32 chance;                 // 0x2c
};

static inline f32 Random(f32 lo, f32 hi) {
    return lo + (f32)((rand() & 0x7fff) * (1.0 / 32767.0)) * (hi - lo);
}

static inline bool DirectTimeline(UnkOtherActionActor& actor, UnkSetActionToOtherEntityEvent& event) {
    return actor.timeline && event.direct;
}

void fn_7100bcfe4c(UnkSetActionToOtherEntityEvent* self) {
    if (fn_7100deac04(g_unk_7101d47310))
        return;
    alignas(8) u32 kind;
    UnkOtherActionActor* actor;
    {
        u64 key;
        UnkOtherEntityRef* entity = self->entity;
        u32 kind_value = g_unk_71727bebb8;
        UnkGameManager* manager = g_unk_7101d47310;
        key = entity->key;
        kind = kind_value;
        actor = reinterpret_cast<UnkOtherActionActor*>(fn_7100debbc8(manager, &key, &kind));
    }
    if (!actor)
        return;
    if (self->action.data == &g_unk_7101d47108->empty)
        return;
    if (self->chance < 1.0f && !(Random(0.0f, 1.0f) <= self->chance))
        return;
    if (DirectTimeline(*actor, *self)) {
        fn_7101137a38(actor->timeline, &self->action, self->flag, self->value, -1.0f);
        return;
    }
    if (void* animation = actor->animation) {
        u32 flag = self->flag;
        u64 hash = reinterpret_cast<base::global::UnkStringEntry*>(self->action.data)->buffer.key;
        base::reflection::UnkReflTypeIdArg mode;
        mode.id = flag;
        f32 v = (f32)self->value;
        UnkActionValue_b0265 value;
        value.field_0x0 = 0;
        value.value = v;
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = base::reflection::refl_GetEnumNot8<0x7101d57cb8, 0x7101d57cc8>();
        fn_7101176690(animation, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &value, &mask);
    }
}
