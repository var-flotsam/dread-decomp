// The lookup in a keyed array (at +0x50) of 0x20-byte values by the id at +0x30 of each key's
// object; null when no key matches. CFXComponent's unit; the shape of fn_7100e6095c
// (src/game/CActorComponent/fn_71002519a8.cpp). Names and layouts are placeholders.
#include "types.h"

struct UnkFXValue_0x20 {
    u64 words[4];
};

struct UnkFXKeyObject {
    u8 pad_0x0[0x30];
    u64 id;
};

struct UnkFXValuesByKeyObject {
    UnkFXValue_0x20* values;
    UnkFXKeyObject** keys;
    u32 count;

    UnkFXValue_0x20* End() { return &values[count]; }
    UnkFXValue_0x20* Find(u64 id) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i]->id == id)
                return &values[i];
        return End();
    }
};

struct UnkFXValuesOwner {
    u8 pad_0x0[0x50];
    UnkFXValuesByKeyObject values;
};

UnkFXValue_0x20* fn_7101152b24(UnkFXValuesOwner* self, u64 id) {
    UnkFXValue_0x20* p = self->values.Find(id);
    return p == self->values.End() ? nullptr : p;
}
