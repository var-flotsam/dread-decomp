// SNavMeshPath unit: grows the record table so that index exists (new records get the default values),
// then makes a temporary copy of that record and destroys it. Names and layouts are placeholders.
#include "game/UnkNavMeshPath.h"
#include "game/UnkObject_0x100.h"
#include "game/UnkTU376.h"
#include "types.h"

struct __attribute__((packed)) UnkRecord_l2ac720 {
    void** vtable;
    u16 field_0x8;
    u8 field_0xa;
    u8 pad_0xb;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u8 field_0x18[0x100];  // an UnkObject_0x100 at an unaligned address
    u8 field_0x118;
    u8 field_0x119;
    u8 pad_0x11a[6];
};

struct __attribute__((packed)) UnkTable_l2ac720 {
    void** vtable;
    u32 count;
    UnkRecord_l2ac720 records[250];
};

static UnkObject_0x100* obj(u8* p) { return reinterpret_cast<UnkObject_0x100*>(p); }
static const UnkObject_0x100* cobj(const u8* p) { return reinterpret_cast<const UnkObject_0x100*>(p); }

void fn_71002ac720(UnkTable_l2ac720* self, u32 index) {
    u32 wanted = index + 1;
    if (wanted > self->count) {
        u32 n = index - self->count + 1;
        UnkRecord_l2ac720* r = &self->records[self->count];
        do {
            r->vtable = g_unk_7101a21120 + 2;
            r->field_0x8 = 0xffff;
            r->field_0xa = 0xff;
            r->field_0xc = 0;
            r->field_0x10 = 0;
            r->field_0x14 = 0;
            fn_71002b57bc(obj(r->field_0x18));
            r->field_0x118 = 0;
            r->field_0x119 = 0;
            ++r;
        } while (--n != 0);
        self->count = wanted;
    }
    const UnkRecord_l2ac720* from = &self->records[index];
    UnkRecord_l2ac720 copy;
    copy.vtable = g_unk_7101a21120 + 2;
    copy.field_0x8 = from->field_0x8;
    copy.field_0xa = from->field_0xa;
    copy.field_0xc = from->field_0xc;
    copy.field_0x10 = from->field_0x10;
    copy.field_0x14 = from->field_0x14;
    fn_71002b58c0(obj(copy.field_0x18), cobj(from->field_0x18));
    copy.field_0x118 = from->field_0x118;
    copy.field_0x119 = from->field_0x119;
    copy.vtable = g_unk_7101a21120 + 2;
    copy.field_0x118 = 1;
    fn_71002b6094(obj(copy.field_0x18));
}
