// CGrabComponent unit (tools/tu_map.py): the component's property handler. Writes out ("s") the
// name of the character class of the object it holds (+0xd0); other properties go to the base
// handler (fn_71010c4250). Names and layouts are placeholders.
#include "../../game_s7/UnkPropS7.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkComponent.h"
#include "types.h"

using namespace unk_prop;
using base::global::CStrId;
using base::global::UnkStrIdInline;
using base::reflection::UnkReflObject;

// A character class object: its name at +0x98.
struct UnkGrabCharClass_b3eb5c : UnkReflObject {
    u8 unk_0x8[0x90];
    CStrId name;
};
static_assert(__builtin_offsetof(UnkGrabCharClass_b3eb5c, name) == 0x98, "UnkGrabCharClass_b3eb5c layout");

// The held object's owner: the object at +0x28.
struct UnkGrabOwner_b3eb5c {
    u8 unk_0x0[0x28];
    UnkReflObject* object;
};

struct UnkGrabHeld_b3eb5c {
    u8 unk_0x0[0x10];
    UnkGrabOwner_b3eb5c* owner;
};

struct UnkGrabComponent_b3eb5c : UnkBaseComponent {
    u8 unk_0x40[0x90];
    UnkGrabHeld_b3eb5c* field_0xd0;  // what it holds
};
static_assert(__builtin_offsetof(UnkGrabComponent_b3eb5c, field_0xd0) == 0xd0, "UnkGrabComponent_b3eb5c layout");

static inline bool IsCharClass(UnkReflObject* obj) {
    return fn_710009d50c(obj->vf_0x0(), fn_71011abbd4());
}

u32 fn_710113eb5c(UnkGrabComponent_b3eb5c* self, UnkComponentProp* prop) {
    UnkProp* p = reinterpret_cast<UnkProp*>(prop);
    if (!Is(p, Key(0x1220)) || p->set)
        return fn_71010c4250(self, prop);
    UnkGrabOwner_b3eb5c* owner = self->field_0xd0 ? self->field_0xd0->owner : nullptr;
    CStrId name(UnkStrIdInline(), nullptr, false);
    if (owner && owner->object && IsCharClass(owner->object)) {
        UnkReflObject* obj = owner->object;
        name = static_cast<UnkGrabCharClass_b3eb5c*>(IsCharClass(obj) ? obj : nullptr)->name;
    }
    fn_710007cb4c(p->stream, "s", name.Text());
    return true;
}
