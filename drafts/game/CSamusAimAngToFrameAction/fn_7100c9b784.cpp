// CSamusAimAngToFrameAction's unit (tools/tu_map.py unit 2210): reads the float "Angle" (degrees) from the
// owner's property bag (+0x220, the bag at +0x80), turns it into radians in [0, 2pi), sets the action's time to
// that share of the length at +0x10 +0x28 (fn_7100c967c8) and refreshes the owner's +0x228 when there is one.
// Names and layouts are placeholders.
#include <algorithm>

#include "../../../src/game_s7/UnkIter.h"
#include "../../../src/game_s7/UnkPropBagS7.h"
#include "base/global/CRntFile.h"
#include "base/global/CStrId.h"
#include "game/UnkTU56.h"
#include "types.h"

using base::global::CStrId;
using unk_prop_bag::UnkPropBag;

struct UnkActionInstance_ovb0073;
void fn_7100c967c8(UnkActionInstance_ovb0073* self, f32 t);
char* fn_71011729a8(char* p0);

// A property: where it is in the bag's stream, the hash of its name and its type.
struct UnkPropValue_lc9b784 {
    u64 offset;
    u64 key;
    s32 type;  // 'f' for a float
};

struct UnkPropOwner_lc9b784 {
    u8 pad_0x0[0x80];
    UnkPropBag bag;  // 0x80
};

struct UnkAimOwner_lc9b784 {
    u8 pad_0x0[0x220];
    UnkPropOwner_lc9b784* props;  // 0x220
    char* field_0x228;
};

struct UnkAimAnim_lc9b784 {
    u8 pad_0x0[0x28];
    f32 length;  // 0x28
};

struct UnkAimAngToFrame_lc9b784 {
    u8 pad_0x0[0x10];
    UnkAimAnim_lc9b784* anim;    // 0x10
    UnkAimOwner_lc9b784* owner;  // 0x18
};

static inline UnkPropValue_lc9b784* Values_lc9b784(UnkPropBag& bag) {
    return reinterpret_cast<UnkPropValue_lc9b784*>(bag.values.data);
}

// The property named `id`, or the end of the values when there is none.
static inline UnkPropValue_lc9b784* FindValue_lc9b784(UnkPropBag& bag, const CStrId& id) {
    UnkPropValue_lc9b784* values = Values_lc9b784(bag);
    UnkIter<UnkPropValue_lc9b784> first{values};
    UnkIter<UnkPropValue_lc9b784> last{&values[bag.values.count]};
    return std::find_if(first, last, [&id](const UnkPropValue_lc9b784& v) { return v.key == id.data->Key(); }).p;
}

void fn_7100c9b784(UnkAimAngToFrame_lc9b784* self) {
    static CStrId s_angle("Angle", true);
    f32 degrees = 0.0f;
    UnkPropBag& bag = self->owner->props->bag;
    UnkPropValue_lc9b784* it = FindValue_lc9b784(bag, s_angle);
    if (it != &Values_lc9b784(bag)[bag.values.count] && it->type == 'f') {
        fn_710007c00c(&bag.lock, it->offset, 0);
        fn_710007c70c(&bag.lock, &degrees, 4);
    }
    f32 angle = degrees * 3.141592653589793 / 180.0;
    if (angle < 0.0f)
        angle += 6.2831855f;
    fn_7100c967c8(reinterpret_cast<UnkActionInstance_ovb0073*>(self), self->anim->length * angle / 6.2831855f);
    if (char* target = self->owner->field_0x228)
        fn_71011729a8(target);
}
