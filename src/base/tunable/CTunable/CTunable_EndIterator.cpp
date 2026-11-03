// From unit 111 (base::tunable::CTunable): the end position of a keyed array (values of 0x28 bytes
// at +0, keys of 8 at +8, the count at +0x10) as an iterator whose kind word at +0x30 is 2. The
// words between are left unset. Names are placeholders.
#include "types.h"

struct UnkTunableValue_0x28 {
    u8 data[0x28];
};

struct UnkTunableKeyedArray {
    UnkTunableValue_0x28* values;
    u64* keys;
    u32 count;
};

struct UnkTunableIterator {
    u64* key;
    UnkTunableValue_0x28* value;
    u8 pad_0x10[0x20];
    u32 kind;

    UnkTunableIterator(u64* k, UnkTunableValue_0x28* v) {
        kind = 2;
        key = k;
        value = v;
    }
};

UnkTunableIterator fn_71001336f0(const UnkTunableKeyedArray* self) {
    return UnkTunableIterator(self->keys + self->count, self->values + self->count);
}
