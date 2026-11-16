// CLifeComponent's unit (tools/tu_map.py unit 2715): the life component's base constructor (fn_710115f7bc):
// the component base, the vtable, cleared life values, the root name "DC_Root", an empty string, cleared
// state and seven empty arrays (two groups of three with a flag, then one). Names and layouts are placeholders.
#include <new>

#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "game/CComponent.h"
#include "game/CLifeComponent.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CRntString;
using base::global::CRntVector;

extern void* g_unk_7101cb3db0[];

// A value and whether it is set.
struct UnkLifeValue_l15f7bc {
    u32 value;
    bool set;
};

// Two words, cleared together.
struct UnkLifePair_l15f7bc {
    u32 field_0x0;
    u32 field_0x4;
};

// Ten words, cleared together.
struct UnkLifeBlock_l15f7bc {
    u32 words[10];
};

// Three arrays and a flag.
struct UnkLifeArrays_l15f7bc {
    CRntVector<void*> lists[3];
    bool field_0x78;
};

struct UnkLifeComponent_l15f7bc {
    void** vtable;
    u8 pad_0x8[0x40];
    u64 field_0x48;
    f32 field_0x50;
    f32 field_0x54;
    UnkLifeValue_l15f7bc values[3];  // 0x58
    u32 field_0x70;
    UnkLifePair_l15f7bc field_0x74;
    u8 pad_0x7c[0x4];
    CRntString root_name;  // 0x80
    u8 field_0xb0[0x30];
    bool field_0xe0;
    UnkLifeBlock_l15f7bc field_0xe4;
    bool field_0x10c;
    u32 field_0x110;
    u8 pad_0x114[0x4];
    UnkLifeArrays_l15f7bc arrays[2];  // 0x118
    CRntVector<void*> field_0x218;
};
static_assert(__builtin_offsetof(UnkLifeComponent_l15f7bc, root_name) == 0x80, "layout");
static_assert(__builtin_offsetof(UnkLifeComponent_l15f7bc, field_0x10c) == 0x10c, "layout");
static_assert(__builtin_offsetof(UnkLifeComponent_l15f7bc, arrays) == 0x118, "layout");
static_assert(__builtin_offsetof(UnkLifeComponent_l15f7bc, field_0x218) == 0x218, "layout");

void fn_710115f7bc(void* object) {
    UnkLifeComponent_l15f7bc* self = static_cast<UnkLifeComponent_l15f7bc*>(object);
    fn_71010c4148(static_cast<UnkStateObject*>(object));
    self->vtable = &g_unk_7101cb3db0[2];
    self->field_0x48 = 0;
    self->field_0x50 = -1.0f;
    self->field_0x54 = -1.0f;
    for (UnkLifeValue_l15f7bc& value : self->values) {
        value.value = 0;
        value.set = false;
    }
    self->field_0x70 = 0;
    self->field_0x74 = {};
    new (&self->root_name) CRntString("DC_Root");
    fn_710007e6c4(self->field_0xb0);
    self->field_0xe0 = true;
    self->field_0xe4 = {};
    self->field_0x10c = false;
    self->field_0x110 = 0;
    for (UnkLifeArrays_l15f7bc& group : self->arrays) {
        for (CRntVector<void*>& list : group.lists)
            new (&list) CRntVector<void*>(0xf);
        group.field_0x78 = false;
    }
    new (&self->field_0x218) CRntVector<void*>(0xf);
}
