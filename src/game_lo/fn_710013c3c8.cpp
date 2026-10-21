// Looks up the 0x80-byte entry stored under a (copied) string id in the keyed array at +0x30
// and, when there is one, passes it and `arg` to fn_710013ffbc.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkEntry_710013c3c8 {
    u8 data[0x80];
};

void fn_710013ffbc(UnkEntry_710013c3c8* entry, void* arg);

struct UnkStruct_710013c3c8 {
    u8 pad_0x0[0x30];
    UnkEntry_710013c3c8* values;
    CStrId* keys;
    u32 count;

    UnkEntry_710013c3c8* End() { return &values[count]; }
    UnkEntry_710013c3c8* Find(const CStrId& key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i].data == key.data)
                return &values[i];
        return End();
    }
};

void fn_710013c3c8(UnkStruct_710013c3c8* self, const CStrId* name, void* arg) {
    CStrId key(*name);
    UnkEntry_710013c3c8* e = self->Find(key);
    if (e != self->End())
        fn_710013ffbc(e, arg);
}
