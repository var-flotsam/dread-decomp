// behaviortree::CBigFistChaseAction's unit (tools/tu_map.py): the chase distance. A negative configured
// distance (+0x30) is replaced by the middle of the range the AI component's character class gives
// (vf_0x90); 200 is added while the target's life object (+0x1b8) passes fn_7101161088. Names and
// layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "types.h"

using base::reflection::UnkReflObject;

bool fn_7101161088(void*);

// The AI component's character class, as the chase sees it.
struct UnkBigFistCharClass_ovb0114 {
    virtual void vf_0x0();

    void Range(UnkActor* actor, f32* max, f32* min) {
        using Fn = void (*)(UnkBigFistCharClass_ovb0114*, UnkActor*, f32*, f32*);
        (*reinterpret_cast<Fn**>(this))[0x90 / 8](this, actor, max, min);
    }
};

// The object at +0x161c of the AI component.
struct UnkBigFistQuery_ovb0114 {
    virtual void vf_0x0();

    void Range(u32 id, f32* max, f32* min) {
        using Fn = void (*)(UnkBigFistQuery_ovb0114*, u32, f32*, f32*);
        (*reinterpret_cast<Fn**>(this))[0xb8 / 8](this, id, max, min);
    }
};

struct __attribute__((packed, aligned(4))) UnkBigFistAI_ovb0114 {
    u8 pad_0x0[0x10];
    UnkReflObject* charClass;  // 0x10
    u8 pad_0x18[0x1128];
    u32 field_0x1140;
    u8 pad_0x1144[0x4d4];
    u32 field_0x1618;
    UnkBigFistQuery_ovb0114* field_0x161c;
};
static_assert(__builtin_offsetof(UnkBigFistAI_ovb0114, field_0x161c) == 0x161c, "layout");

struct UnkBigFistChase_ovb0114 {
    u8 pad_0x0[0x18];
    UnkActor* owner;  // 0x18
    u8 pad_0x20[0x10];
    f32 distance;  // 0x30
};

static inline UnkBigFistAI_ovb0114* AI(UnkActor* actor) {
    return static_cast<UnkBigFistAI_ovb0114*>(actor->field_0x240);
}

f32 fn_71003e4dc4(UnkBigFistChase_ovb0114* self) {
    f32 distance = self->distance;
    f32 max, min;
    if (distance < 0.0f) {
        if (UnkBigFistAI_ovb0114* ai = AI(self->owner)) {
            UnkReflObject* obj = ai->charClass;
            UnkBigFistCharClass_ovb0114* charClass = reinterpret_cast<UnkBigFistCharClass_ovb0114*>(
                fn_710009d50c(obj->vf_0x0(), fn_7100408b2c()) ? obj : nullptr);
            charClass->Range(self->owner, &max, &min);
            distance = (max + min) * 0.5f;
        }
    }
    UnkBigFistAI_ovb0114* ai = AI(self->owner);
    if (ai->field_0x1618 && ai->field_0x161c) {
        max = 0.0f;
        min = 0.0f;
        ai->field_0x161c->Range(ai->field_0x1140, &max, &min);
    }
    if (fn_7101161088(fn_7100d7c990(self->owner)->field_0x1b8))
        distance += 200.0f;
    return distance;
}
