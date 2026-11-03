// A function in an unnamed unit (tu_map unit 958, 0x7100639cf8-0x710063ce2c): finds the 0x30-byte entry
// of the array at +8 (count at +0x10) whose object at +0x20 has the key at +0x10 (a missing object counts
// as a null key). Names and layouts are placeholders.
#include "types.h"

struct UnkUnit958Object {
    u8 unk_0x0[0x10];
    void* key;
};
struct UnkUnit958Entry {
    u8 unk_0x0[0x20];
    UnkUnit958Object* object;
    u8 unk_0x28[0x8];
};
struct UnkUnit958List {
    u8 unk_0x0[0x8];
    UnkUnit958Entry* entries;
    u32 count;
};

UnkUnit958Entry* fn_710063b770(UnkUnit958List* self, void* key) {
    for (UnkUnit958Entry *it = self->entries, *end = it + self->count; it != end; ++it) {
        void* k = it->object != nullptr ? it->object->key : nullptr;
        if (k == key)
            return it;
    }
    return nullptr;
}
