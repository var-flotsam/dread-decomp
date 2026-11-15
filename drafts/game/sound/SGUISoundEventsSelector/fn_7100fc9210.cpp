// sound::SGUISoundEventsSelector's unit: how well a selector's events fit a GUI display object. Each
// event is tested from the last to the first against the object and then its parents; every parent
// stepped over scales that event's share by 0.8. Names and layouts are placeholders.
#include "base/animation/CBlendSpaceResource.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTU83.h"
#include "types.h"

struct UnkDisplayParent_lfc9210 : base::reflection::UnkReflObject {
    u8 pad_0x8[0x118];
    UnkDisplayParent_lfc9210* parent;  // 0x120
    u8 pad_0x128[8];
    u64 id;  // 0x130
};
static_assert(__builtin_offsetof(UnkDisplayParent_lfc9210, id) == 0x130, "layout");

struct UnkDisplayTarget_lfc9210 {
    const base::reflection::CType* type;
    UnkDisplayParent_lfc9210* object;
};

struct UnkSelectorEvent_lfc9210 {
    u32 kind;
    u64 id;  // 0x8
};

struct UnkSelectorScore_lfc9210 {
    u8 pad_0x0[0xd0];
    UnkSelectorEvent_lfc9210* events;
    u32 count;
};

static inline bool MatchesType(UnkDisplayParent_lfc9210* object, const UnkSelectorEvent_lfc9210* event) {
    if (event->kind != 0)
        return false;
    const base::reflection::CType* type =
        reinterpret_cast<const base::reflection::CType*>(fn_71000a5118(g_unk_7101d4c8d0, &event->id));
    return type && fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(object->vf_0x0()), type);
}

f32 fn_7100fc9210(UnkSelectorScore_lfc9210* self, UnkDisplayTarget_lfc9210* target) {
    const base::reflection::CType* type = target->type;
    if (!type)
        return 0.0f;
    if (!fn_710009d50c(type, reinterpret_cast<const base::reflection::CType*>(fn_71011c3868())))
        return 0.0f;
    UnkDisplayParent_lfc9210* object = target->object;
    f32 score = 1.0f;
    bool found = object != nullptr;
    if (object && self->count) {
        UnkSelectorEvent_lfc9210* it = self->events + self->count;
        do {
            f32 share = 1.0f;
            --it;
            while (!(it->kind == 1 ? object->id == it->id : MatchesType(object, it))) {
                share *= 0.8f;
                object = object->parent;
                if (!object)
                    return 0.0f;
            }
            score += share;
        } while (it != self->events);
        found = true;
    }
    return found ? score : 0.0f;
}
