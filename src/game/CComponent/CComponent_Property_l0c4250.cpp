// CComponent's unit (tools/tu_map.py): the base component's property handler. Reads or writes the
// flag at +0x3c ("b") and writes the owning character's class name ("s"); other keys go to
// fn_710024fc10. Names and layouts are placeholders.
#include <string.h>

#include "../../game_s7/UnkPropS7.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkComponent.h"

using namespace unk_prop;
using base::global::UnkStringData;
using base::reflection::UnkReflObject;

// The actor component's property handler.
u32 fn_710024fc10(UnkBaseComponent* self, UnkComponentProp* prop);

// A character class object: its name at +0x98.
struct UnkCharClass_c4250 : UnkReflObject {
    u8 unk_0x8[0x90];
    UnkStringData* name;  // a CStrId's data
};
static_assert(__builtin_offsetof(UnkCharClass_c4250, name) == 0x98, "UnkCharClass_c4250 layout");

// The component's owner (UnkBaseComponent::field_0x30): the object at +0x28.
struct UnkOwner_c4250 {
    u8 unk_0x0[0x28];
    UnkReflObject* object;
};

static inline UnkOwner_c4250* Owner(UnkBaseComponent* self) {
    return static_cast<UnkOwner_c4250*>(self->field_0x30);
}

static inline bool IsCharClass(UnkReflObject* obj) {
    return fn_710009d50c(obj->vf_0x0(), fn_71011abbd4());
}

u32 fn_71010c4250(UnkBaseComponent* self, UnkComponentProp* prop) {
    UnkProp* p = reinterpret_cast<UnkProp*>(prop);
    if (!p->set && Is(p, Key(0xe88)))
        return fn_710007cb4c(p->stream, "b", self->unk_0x38[4]) != 0;
    if (p->set && Is(p, Key(0xe88)))
        return fn_710007cf30(p->stream, "b", &self->unk_0x38[4]) != 0;
    if (Is(p, Key(0xaa8)) && !p->set) {
        char buf[0x80];
        if (self->field_0x30 && Owner(self)->object && IsCharClass(Owner(self)->object)) {
            UnkReflObject* obj = Owner(self)->object;
            UnkCharClass_c4250* cls = static_cast<UnkCharClass_c4250*>(IsCharClass(obj) ? obj : nullptr);
            ++cls->name->reads;
            const char* text = cls->name->text;
            if (!text)
                text = "";
            memcpy(buf, text, strlen(text) + 1);
        } else {
            buf[0] = '\0';
        }
        fn_710007cb4c(p->stream, "s", buf);
        return true;
    }
    return fn_710024fc10(self, prop);
}
