// CAbilityShinespark's unit (tools/tu_map.py unit 433): sends the owner actor's animation component the
// shinespark launch animation. Unless one was already chosen (+0x438), it is picked by the launch angle, a value
// of type 'f' in degrees in the component's property bag; then the weak handle at +0x3f0 is re-linked to the
// component and the choice is cleared. Names and layouts are placeholders.
#include <algorithm>

#include "../../game_s2/UnkSpitSend.h"
#include "../../game_s7/UnkIter.h"
#include "../../game_s7/UnkPropBagS7.h"
#include "base/global/CRntFile.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game/UnkTU56.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;
using unk_prop_bag::UnkPropBag;

__attribute__((visibility("hidden"))) extern CStrId g_unk_71726b09c0;  // the launch angle's name

// A value in the bag: where it is in the bag's stream, the hash of its name and its type.
struct UnkBagValue_l3308b0 {
    u64 offset;
    u64 key;
    s32 type;  // 'f' for a float
};

struct UnkAnimation_l3308b0 {
    u8 pad_0x0[0x80];
    UnkPropBag bag;  // 0x80
};

struct UnkMovement_l3308b0 {
    u8 pad_0x0[0xf9];
    bool field_0xf9;
};

struct UnkActor_l3308b0 {
    u8 pad_0x0[0x1f8];
    UnkMovement_l3308b0* movement;  // 0x1f8
    u8 pad_0x200[0x20];
    UnkAnimation_l3308b0* animation;  // 0x220
};

struct UnkOwner_l3308b0 {
    u8 pad_0x0[0x30];
    UnkActor_l3308b0* actor;
};

struct UnkShinespark_l3308b0 {
    u8 pad_0x0[0xc0];
    UnkOwner_l3308b0* owner;
    u8 pad_0xc8[0x3d8 - 0xc8];
    u8 link[0x18];             // 0x3d8
    UnkWeakHandle* animation;  // 0x3f0
    u8 pad_0x3f8[0x438 - 0x3f8];
    u64 launch;  // 0x438, -1 when not chosen
};

// The value named `id`, or the end of the values when there is none.
static inline UnkBagValue_l3308b0* FindValue_l3308b0(UnkPropBag& bag, const CStrId& id) {
    UnkBagValue_l3308b0* values = reinterpret_cast<UnkBagValue_l3308b0*>(bag.values.data);
    UnkIter<UnkBagValue_l3308b0> first{values};
    UnkIter<UnkBagValue_l3308b0> last{&values[bag.values.count]};
    return std::find_if(first, last, [&id](const UnkBagValue_l3308b0& v) { return v.key == id.data->Key(); }).p;
}

// The float named `id`, or 0 when the bag has none.
static inline f32 ReadFloat_l3308b0(UnkPropBag& bag, const CStrId& id) {
    f32 value = 0.0f;
    UnkBagValue_l3308b0* it = FindValue_l3308b0(bag, id);
    if (it != &reinterpret_cast<UnkBagValue_l3308b0*>(bag.values.data)[bag.values.count] && it->type == 'f') {
        CRntFile_Seek(&bag.lock, it->offset, 0);
        fn_710007c70c(&bag.lock, &value, 4);
    }
    return value;
}

void fn_71003308b0(UnkShinespark_l3308b0* self) {
    UnkActor_l3308b0* actor = self->owner->actor;
    UnkAnimation_l3308b0* animation = actor->animation;
    u64 launch = self->launch;
    if (launch == (u64)-1) {
        f32 angle = ReadFloat_l3308b0(animation->bag, g_unk_71726b09c0) * 3.14159265358979323846 / 180.0;
        if (angle >= 0.785398163f)
            launch = 0x283fab3df14e21d8;
        else if (angle >= 0.0f)
            launch = 0xe8b06712beee9d34;
        else if (angle != -0.785398163f)
            launch = 0x001f8c3a783b3429;
        else
            launch = actor->movement->field_0xf9 ? 0x03028bdbb837ab80 : 0xf2987a821f6613b3;
    }
    UnkSpitSend(animation, launch, 0xf4061ff25a2163d6);
    if (UnkWeakHandle* handle = self->animation) {
        if (handle->target)
            fn_710117b3f0(handle->target, self->link);
        ResetRef(&self->animation);
    }
    if (animation) {
        fn_7100316018(reinterpret_cast<UnkWeakTarget*>(animation), &self->animation);
        fn_710117b3e8(self->animation ? self->animation->target : nullptr, self->link);
    }
    self->launch = (u64)-1;
}
