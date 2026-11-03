// Whether a keyed array (at +0x50) of 0x20-byte values holds the id at +0x30 of one of its key
// objects: fn_7101152b24's lookup (src/game/CFXComponent/fn_7101152b24.cpp) inlined and tested
// against null. CFXComponent's unit. Names and layouts are placeholders.
#include "types.h"

struct UnkFXHasValue_0x20 {
    u64 words[4];
};

struct UnkFXHasKeyObject {
    u8 pad_0x0[0x30];
    u64 id;
};

struct UnkFXHasValuesByKeyObject {
    UnkFXHasValue_0x20* values;
    UnkFXHasKeyObject** keys;
    u32 count;

    UnkFXHasValue_0x20* End() { return &values[count]; }
    UnkFXHasValue_0x20* Find(u64 id) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i]->id == id)
                return &values[i];
        return End();
    }
    UnkFXHasValue_0x20* Get(u64 id) {
        UnkFXHasValue_0x20* p = Find(id);
        return p == End() ? nullptr : p;
    }
};

struct UnkFXHasValuesOwner {
    u8 pad_0x0[0x50];
    UnkFXHasValuesByKeyObject values;
};

bool fn_7101156c00(UnkFXHasValuesOwner* self, u64 id) { return self->values.Get(id) != nullptr; }
