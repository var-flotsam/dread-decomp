// A method of the unit of base::file::nx::CDeviceNX: counts a use on the object `*handle` names
// and looks up its path (a base::global::CRntString built from the C string at +0x10) in the
// table at +0x10 (fn_710002e168). Also fn_710002f0dc.
#include "base/file/nx/CDeviceNX.h"
#include "base/global/CRntString.h"
#include "types.h"

typedef u32 __attribute__((may_alias)) UnkAliasU32;

struct UnkDeviceFile_710002e0f8 {
    u8 unk_0x0[0x10];
    const char* path;
    u8 unk_0x18[0x2c];
    UnkAliasU32 use_count;
};

struct UnkDevice_710002e0f8 {
    u8 unk_0x0[0x10];
    u8 table[0x31];
    bool field_0x41;
};

u32 fn_710002e168(void* table, base::global::CRntString* path);

u32 fn_710002e0f8(UnkDevice_710002e0f8* self, UnkDeviceFile_710002e0f8** handle) {
    void* table = &self->table;
    UnkAliasU32* count = &(*handle)->use_count;
    ++*count;
    const char* path = (*handle)->path;
    base::global::CRntString str(path ? path : "");
    return fn_710002e168(table, &str);
}

// The same lookup through fn_710002ed98 with more arguments (the fourth is unused and 0 is passed in
// its place); true without a lookup when the flag at +0x41 is set.
u32 fn_710002f0dc(UnkDevice_710002e0f8* self, UnkDeviceFile_710002e0f8** handle, void* a, void* unused, void* c,
                  u32 d, u32 e) {
    if (self->field_0x41)
        return 1;
    void* table = &self->table;
    UnkAliasU32* count = &(*handle)->use_count;
    ++*count;
    const char* path = (*handle)->path;
    base::global::CRntString str(path ? path : "");
    return fn_710002ed98(table, &str, a, 0, c, d, e);
}
