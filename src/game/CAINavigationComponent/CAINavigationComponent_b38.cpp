// Navigation component handler: when the flags allow it and the current target value holds an
// object of the expected class with its flag at +0x14 set, sends the navigation target a query
// whose direction is the third column of the argument's matrix.
#include "base/UnkMatrix44.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game_s2/UnkParams.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflValue;

extern void* g_unk_7101a24b58[];
extern base::reflection::CClass g_unk_717270d1c0;

struct UnkQuery {
    void** vtable;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u8 field_0x14;
    u8 pad_0x15[7];
    u8 field_0x1c;
    u8 pad_0x1d[3];
    u64 field_0x20;
    u64 field_0x28;
    u8 field_0x30;
    u8 pad_0x31[3];
    u32 field_0x34;

    UnkQuery() {
        vtable = g_unk_7101a24b58 + 2;
        __builtin_memset(&field_0x8, 0, 13);
        field_0x1c = 0;
        field_0x20 = 0;
        field_0x28 = 0;
        field_0x30 = 0;
        field_0x34 = 0;
    }
};

struct UnkNavValue {
    u8 pad_0x0[0x14];
    u8 field_0x14;
};

struct UnkNavEntry {
    u8 pad_0x0[0x18];
    UnkReflValue value;
};

struct UnkNavState {
    u8 pad_0x0[0x10];
    UnkNavEntry* entry;
};

struct UnkNavSelf {
    u8 pad_0x0[0xb0];
    UnkNavState** state;
};

static inline u32 AsU32(const f32& value) {
    u32 bits;
    __builtin_memcpy(&bits, &value, 4);
    return bits;
}

void fn_71006f4d84(UnkNavSelf* self, const UnkMatrix44* xform, const u8* flags) {
    if (!(flags[0] & 2) || !self->state)
        return;
    UnkNavState* state = *self->state;
    if (!state || !state->entry)
        return;
    UnkReflValue* v = &state->entry->value;
    const CType* type = reinterpret_cast<const CType*>(v->type);
    if (!type || !fn_710009d50c(type, reinterpret_cast<const CType*>(&g_unk_717270d1c0)))
        return;
    if (!v->type)
        return;
    const UnkNavValue* value = base::reflection::refl_ArgValue<UnkNavValue>(v);
    if (!value || !value->field_0x14)
        return;
    UnkQuery query;
    const UnkMatrix44& m = fn_710002614c(xform);
    u32 x = AsU32(m.m[2]);
    u32 y = AsU32(m.m[6]);
    u32 z = AsU32(m.m[10]);
    query.field_0x8 = x;
    query.field_0xc = y;
    query.field_0x10 = z;
    query.field_0x14 = 1;
    fn_71006f4b0c(self, &query, 0);
}
