// Picks the entry of the pool at +0x68 this component will use (the index in the global at +0x24
// when it is in range, else a random one) and remembers its index and the value at +0x30 of it
// (0 when there is none). Then asks the owner's object at +0x240 whether it is of the component's
// class and, if so, readies it (fn_71004d1fe8, flag at 0x15834) and copies the global's value at
// +0x28 to the second global.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::CClass;

struct UnkEntry {
    u8 pad_0x0[0x30];
    f32 field_0x30;
};

struct UnkPool {
    u8 pad_0x0[0x128];
    u32 count;
};

struct UnkGlobal {
    u8 pad_0x0[0x24];
    s32 field_0x24;
    u32 field_0x28;
};

struct UnkGlobal2 {
    u8 pad_0x0[0x40];
    u32 field_0x40;
};

struct UnkActor {
    void** vtable;
    const CClass* vf_0x0(f32 value) { return reinterpret_cast<const CClass* (*)(UnkActor*, f32)>(vtable[0])(this, value); }
    u8 pad_0x8[0x1582c];
    u8 field_0x15834;
};

struct UnkOwner {
    u8 pad_0x0[0x240];
    UnkActor* field_0x240;
};

struct UnkSelf {
    u8 pad_0x0[8];
    UnkOwner* field_0x8;
    u8 pad_0x10[0x58];
    UnkPool* field_0x68;
    union {
        u64 field_0x70;
        struct {
            u32 pad_0x70;
            f32 field_0x74;
        };
    };
    u8 field_0x78;
    u8 pad_0x79[3];
    s32 field_0x7c;
    s32 field_0x80;
    u8 field_0x84;
    u8 pad_0x85[3];
    u32 field_0x88;
    u8 field_0x8c;
    u8 pad_0x8d[3];
    u32 field_0x90;
};

extern UnkGlobal g_unk_71726d6438;
extern UnkGlobal2 g_unk_7172790a08;

void fn_71004c1944(UnkSelf* self);
UnkEntry* fn_71004ba10c(UnkPool* pool, u32 index, u32 flag);
int fn_71012395a0();
void fn_71004d1fe8(UnkActor* actor);

void fn_71004cb554(UnkSelf* self) {
    fn_71004c1944(self);
    self->field_0x8c = 0;
    self->field_0x90 = 0;
    self->field_0x84 = 0;
    self->field_0x88 = 0;
    self->field_0x70 = 0;
    self->field_0x78 = 0;
    s32 index = g_unk_71726d6438.field_0x24;
    u32 count = self->field_0x68->count;
    u32 picked = 0;
    if (index >= 0 && index < (s32)count)
        picked = index;
    else if (count != 0)
        picked = (fn_71012395a0() & 0x7fff) % count;
    self->field_0x7c = picked;
    self->field_0x80 = 0;
    UnkEntry* entry = fn_71004ba10c(self->field_0x68, picked, 0);
    f32 value = entry ? entry->field_0x30 : 0.0f;
    UnkOwner* owner = self->field_0x8;
    self->field_0x74 = value;
    UnkActor* actor = owner->field_0x240;
    if (actor) {
        const CClass* type = actor->vf_0x0(value);
        if (fn_710009d50c(type, fn_71004cf318())) {
            fn_71004d1fe8(actor);
            actor->field_0x15834 = 1;
            g_unk_7172790a08.field_0x40 = g_unk_71726d6438.field_0x28;
        }
    }
}
