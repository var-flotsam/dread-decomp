// Looks up an entry by key in a parallel key/value array, then forwards the request with the entry's field_0x70.
#include "types.h"

struct UnkEntry_7101176690 {
    char pad[0x70];
    void* field_0x70;
};

struct UnkMap_7101176690 {
    char pad[0xc8];
    UnkEntry_7101176690** values;
    u64* keys;
    u32 count;

    UnkEntry_7101176690** Find(u64 key) {
        for (u32 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &values[i];
        return values + count;
    }
};

struct UnkStruct_7101176690 {
    char pad[0x40];
    UnkMap_7101176690* field_0x40;
};

struct alignas(8) UnkFlags_7101176690 { u32 value; };
struct UnkS16 { u64 a, b; };
struct UnkS12 { u64 a; u32 b; };

void* fn_7101176d34(UnkStruct_7101176690* self, void* arg);
bool fn_7101177044(UnkStruct_7101176690* self, void* r, s32 idx, void* data, const UnkFlags_7101176690& flags,
                   const UnkS16& a, const UnkS12& b, const UnkFlags_7101176690& c);

bool fn_7101176690(UnkStruct_7101176690* self, void* arg, u64 key, const u32& flags, const UnkS16& a,
                   const UnkS12& b, const u32& c) {
    void* r = fn_7101176d34(self, arg);
    UnkMap_7101176690* m = self->field_0x40;
    UnkEntry_7101176690** it = m->Find(key);
    UnkEntry_7101176690** end = m->values + m->count;
    void* data = (it != end && *it) ? (*it)->field_0x70 : nullptr;
    return fn_7101177044(self, r, -1, data, UnkFlags_7101176690{flags}, a, b, UnkFlags_7101176690{c});
}
