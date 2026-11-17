// CChozoWarriorAIComponent's unit (tools/tu_map.py): the warrior's damage check. After the boss base's
// answer (fn_71003adaf8) it handles shinespark hits (a timeline mask and a 2.5 s wait), impacts of kind
// 0x8000 (the tweaked impact track's name) and hits by the kind at g_unk_71726d1260, which it may block and
// answer with an animation. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflStatics.h"
#include "../../game_s2/UnkSpitSend.h"
#include "game/CBossAIComponent.h"
#include "game/CChozoWarriorAIComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU1370.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

struct UnkChozoWarriorKey_l499a88;
extern UnkChozoWarriorKey_l499a88 g_unk_71016499e8;  // reached through the GOT

struct UnkChozoWarriorTuning_l499a88 {
    u8 pad_0x0[0x2c];
    f32 field_0x2c;
    f32 field_0x30;
    bool field_0x34;
    f32 field_0x38;
};

extern UnkChozoWarriorTuning_l499a88 g_unk_71726d0e00;

struct UnkChozoWarriorLife_l499a88 {
    u8 pad_0x0[0x54];
    f32 field_0x54;
};

struct UnkChozoWarriorOwner_l499a88 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
    u8 pad_0x78[0x1b8 - 0x78];
    UnkChozoWarriorLife_l499a88* field_0x1b8;
    u8 pad_0x1c0[0x220 - 0x1c0];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x2e8 - 0x228];
    void* field_0x2e8;
};

struct UnkChozoWarriorHitObject_l499a88 {
    u8 pad_0x0[0x20];
    UnkChozoWarriorKind_b67a2* kind;
};

struct UnkChozoWarriorDamage_l499a88 {
    u8 pad_0x0[0x60];
    UnkChozoWarriorHitObject_l499a88* source;
    u8 pad_0x68[0x78 - 0x68];
    base::math::CVector3D direction;  // 0x78
    u8 pad_0x84[0xb4 - 0x84];
    u32 flags;  // 0xb4
};

struct UnkChozoWarriorAI_l499a88 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkChozoWarriorOwner_l499a88* owner;  // 0x30
    u8 pad_0x38[0x14999 - 0x38];
    bool field_0x14999;
    bool field_0x1499a;
    u8 pad_0x1499b[0x149a0 - 0x1499b];
    bool field_0x149a0;
    u8 pad_0x149a1[0x14b38 - 0x149a1];
    f32 cooldown;  // 0x14b38
    bool field_0x14b3c;
    u64 field_0x14b40;
    f32 wait;  // 0x14b48

    bool vf_0x838(f32 value) {
        return reinterpret_cast<bool (*)(UnkChozoWarriorAI_l499a88*, f32)>(vtable[0x838 / 8])(this, value);
    }
};
static_assert(__builtin_offsetof(UnkChozoWarriorAI_l499a88, wait) == 0x14b48, "");

bool fn_710110c83c(void* object);

// A hit by the kind at g_unk_71726d1260: blocks it when the warrior may, and answers a hit from behind.
static inline u32 Hit(UnkChozoWarriorAI_l499a88* self, UnkChozoWarriorDamage_l499a88* damage, u32 flags) {
    UnkChozoWarriorHitObject_l499a88* source = damage->source;
    if (!source || source->kind != g_unk_71726d1260)
        return (flags >> 17) & 2;
    if (!(flags & 0x0800040c))
        return 2;
    if (!self->vf_0x838(g_unk_71726d0e00.field_0x2c))
        return 2;
    if (!(damage->flags & 4))
        self->field_0x149a0 = false;
    if (!self->field_0x14999 || !g_unk_71726d0e00.field_0x34)
        return 1;
    if (self->cooldown > 0.0f)
        return 1;
    const UnkMatrix44& m = fn_710002614c(&self->owner->transform);
    if (m.m[2] * damage->direction.x + m.m[6] * damage->direction.y + m.m[10] * damage->direction.z < 0.0f) {
        if (base::reflection::refl_EmitU32(self->owner->animation, 0x972422f600dd3f97, 0xf4061ff25a2163d6, 0,
                                           &g_unk_710164b958, &g_unk_7172769740))
            self->cooldown = g_unk_71726d0e00.field_0x38;
    }
    return 1;
}

// An impact (flag 0x8000): the life part takes it, unless the animation ignores death; else the
// tweaked impact track names the reaction.
static inline void Impact(UnkChozoWarriorAI_l499a88* self, UnkChozoWarriorDamage_l499a88* damage) {
    UnkChozoWarriorOwner_l499a88* owner = self->owner;
    UnkAnimationComponent* animation = owner->animation;
    if (!animation)
        return;
    UnkChozoWarriorLife_l499a88* life = owner->field_0x1b8;
    if (!life)
        return;
    if (fn_710117b348(animation, fn_7100d09418())) {
        fn_71008c1538(reinterpret_cast<UnkRequest*>(damage), life->field_0x54);
    } else {
        self->field_0x14b3c = true;
        self->field_0x14b40 = 0x9b2147806cdf8f00;
        UnkTrackQuery query;
        query.cls = fn_7100c624f8();
        query.result = nullptr;
        query.value = -1.0f;
        fn_710117b08c(animation, &query);
        if (UnkTrackRecord* record = query.result) {
            base::global::UnkStringData* name = record->sources.data;
            if (name != &g_unk_7101d47108->empty)
                self->field_0x14b40 = name->Key();
        }
    }
}

u32 fn_7100499a88(UnkChozoWarriorAI_l499a88* self, UnkChozoWarriorDamage_l499a88* damage) {
    u32 result = fn_71003adaf8(reinterpret_cast<UnkQuetzoaXAI_b58*>(self),
                               reinterpret_cast<UnkQuetzoaXMessage_b58*>(damage));
    if (result)
        return result;
    if (!fn_710110c83c(self))
        return 6;
    if (self->field_0x1499a)
        return 2;
    u32 flags = damage->flags;
    if (flags & 0x20) {
        auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8);
        if (timeline) {
            base::reflection::UnkReflTypeIdArg mask;
            mask.id = UnkAll_SpitSend().value;
            fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_71016499e8), &mask.id, false, -1.0f);
            self->wait = 2.5f;
            return 0;
        }
    } else if (flags & 0x8000) {
        Impact(self, damage);
    } else {
        return Hit(self, damage, flags);
    }
    return 0;
}
