// Unit 94 of tools/tu_map.py: resets a slot holder (the id, a cleared word, the flag at +0x5468)
// and stores a copy of the reflected value `value` in the entry fn_71000f0f38 finds or makes for
// its type. Names and layouts are placeholders.
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflObjectRead.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflValue;

struct UnkUnit94Entry {
    u8 pad_0x0[0x8];
    UnkReflValue value;
};

struct UnkUnit94Holder {
    u32 id;
    u32 field_0x4;
    u8 pad_0x8[0x5468 - 0x8];
    bool field_0x5468;
};

UnkUnit94Entry* fn_71000f0f38(UnkUnit94Holder* self, const char* type);

void fn_71000f0568(UnkUnit94Holder* self, UnkReflValue* value, u32 id) {
    self->id = id;
    self->field_0x4 = 0;
    self->field_0x5468 = true;
    UnkReflValue* slot = &fn_71000f0f38(self, value->type)->value;
    const char* type = value->type;
    const void* data;
    if (!type)
        data = nullptr;
    else if (value->field_0x14 == 1 || *reinterpret_cast<const u64*>(type + 0x18) > 8)
        data = value->field_0x0;
    else
        data = value;
    fn_71000a1c60(slot);
    fn_71000a18d4(slot, reinterpret_cast<const CType*>(type), data, true);
}
