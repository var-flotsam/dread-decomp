// CAIAimAngToFrameAction's unit (tools/tu_map.py unit 1733): reads the float "Angle" (0 when it is not a number)
// from the owner's property bag (+0x220, the bag at +0x80), shifts it by half the aim range (the AI component's
// +0x1610 +0x3ac, else 2pi), sets the action's time to that share of the length at +0x10 +0x28 (fn_7100c95f74)
// and refreshes the owner's +0x228 when there is one. Names and layouts are placeholders.
#include <algorithm>

#include "../../game_s7/UnkIter.h"
#include "../../game_s7/UnkPropBagS7.h"
#include "base/global/CRntFile.h"
#include "base/global/CStrId.h"
#include "game/UnkTU56.h"
#include "types.h"

using base::global::CStrId;
using unk_prop_bag::UnkPropBag;

struct UnkActionInstance_ovb0073;
void fn_7100c95f74(UnkActionInstance_ovb0073* self, u32 a, bool b, f32 t);
char* fn_71011729a8(char* p0);

// A property: where it is in the bag's stream, the hash of its name and its type.
struct UnkPropValue_lb90594 {
    u64 offset;
    u64 key;
    s32 type;  // 'f' for a float
};

struct UnkPropOwner_lb90594 {
    u8 pad_0x0[0x80];
    UnkPropBag bag;  // 0x80
};

struct UnkAimRange_lb90594 {
    u8 pad_0x0[0x3ac];
    f32 range;  // 0x3ac
};

struct UnkAimAI_lb90594 {
    u8 pad_0x0[0x1610];
    UnkAimRange_lb90594* aim;  // 0x1610
};

struct UnkAimOwner_lb90594 {
    u8 pad_0x0[0x220];
    UnkPropOwner_lb90594* props;  // 0x220
    char* field_0x228;
    u8 pad_0x230[0x10];
    UnkAimAI_lb90594* ai;  // 0x240
};

struct UnkAimAnim_lb90594 {
    u8 pad_0x0[0x28];
    f32 length;  // 0x28
};

struct UnkAimAngToFrame_lb90594 {
    u8 pad_0x0[0x10];
    UnkAimAnim_lb90594* anim;    // 0x10
    UnkAimOwner_lb90594* owner;  // 0x18
};

static inline UnkPropValue_lb90594* Values_lb90594(UnkPropBag& bag) {
    return reinterpret_cast<UnkPropValue_lb90594*>(bag.values.data);
}

// The property whose name hashes to `key`, or the end of the values when there is none.
static inline UnkPropValue_lb90594* FindValue_lb90594(UnkPropBag& bag, u64 key) {
    UnkPropValue_lb90594* values = Values_lb90594(bag);
    UnkIter<UnkPropValue_lb90594> first{values};
    UnkIter<UnkPropValue_lb90594> last{&values[bag.values.count]};
    return std::find_if(first, last, [key](const UnkPropValue_lb90594& v) { return v.key == key; }).p;
}

void fn_7100b90594(UnkAimAngToFrame_lb90594* self) {
    static CStrId s_angle("Angle", true);
    f32 angle = 0.0f;
    UnkPropBag& bag = self->owner->props->bag;
    UnkPropValue_lb90594* it = FindValue_lb90594(bag, s_angle.data->Key());
    if (it != &Values_lb90594(bag)[bag.values.count] && it->type == 'f') {
        fn_710007c00c(&bag.lock, it->offset, 0);
        fn_710007c70c(&bag.lock, &angle, 4);
        if (__builtin_isnan(angle))
            angle = 0.0f;
    }
    UnkAimAI_lb90594* ai = self->owner->ai;
    f32 range = ai && ai->aim ? ai->aim->range : 6.2831855f;
    angle += range * 0.5f;
    fn_7100c95f74(reinterpret_cast<UnkActionInstance_ovb0073*>(self), 0, true, angle * self->anim->length / range);
    if (char* target = self->owner->field_0x228)
        fn_71011729a8(target);
}
