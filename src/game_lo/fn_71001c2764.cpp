// Unless a global flag blocks it, forwards a request keyed by two ids to a sub-object; when it
// succeeds and `remove` is set, moves every matching entry of the object's list past its end
// (swapping it with the last entry and shrinking the count).
#include "game/UnkGlobal_710b372a98.h"
#include "types.h"

// A 16-byte value returned through x8, so not trivially copyable.
struct UnkValue_71001c2764 {
    u64 field_0x0;
    u32 field_0x8;
    u32 field_0xc;
    UnkValue_71001c2764(const UnkValue_71001c2764& o);
};
UnkValue_71001c2764 fn_7100030878(const UnkValue_71001c2764* src);

bool fn_71001996e0(void* target, u32 a, u32 b, UnkValue_71001c2764 value, u32 c);

struct UnkEntry_71001c2764 {
    u8 unk_0x0[0x28];
    u32 field_0x28;
    u32 field_0x2c;
};

struct UnkStruct_71001c2764 {
    u8 unk_0x0[0x78];
    void* field_0x78;
    u8 unk_0x80[0x308 - 0x80];
    UnkEntry_71001c2764* entries[9];
    // A union member: the original reloads the count after each pointer store.
    union {
        u32 count;
    };
};

bool fn_71001c2764(UnkStruct_71001c2764* self, u32 a, u32 b, const UnkValue_71001c2764* value, u32 c,
                   bool remove) {
    if (g_unk_710b372a98->field_0x318367c)
        return false;
    if (!fn_71001996e0(self->field_0x78, a, b, fn_7100030878(value), c))
        return false;
    if (remove) {
        for (u32 i = 0; i < self->count; ++i) {
            UnkEntry_71001c2764* e = self->entries[i];
            if (e->field_0x28 == a && e->field_0x2c == b) {
                --self->count;
                self->entries[i] = self->entries[self->count];
                self->entries[self->count] = e;
            }
        }
    }
    return true;
}
