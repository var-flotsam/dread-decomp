// Copy constructor of a table of up to 250 records (0x120 bytes each, from +0xc) followed by a
// small block at +0x1194c: copies the records one by one, then the block, whose last member is a
// counted reference (released first when it was set, then replaced by the copy of the other's).
#include "types.h"

struct __attribute__((packed)) UnkRecord {
    void** vtable;
    u16 field_0x8;
    u8 field_0xa;
    u8 pad_0xb;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u8 field_0x18[256];
    u16 field_0x118;
    u8 pad_0x11a[6];
};

// Accessed through this: the original reloads the block's pointer after the increment.
typedef u32 __attribute__((may_alias)) a_u32;

struct UnkCounted {
    void** vtable;
    u32 count;
    void vf_0x10() { reinterpret_cast<void (*)(UnkCounted*)>(vtable[2])(this); }
};

struct __attribute__((packed)) UnkBlock {
    u8 field_0x0;
    u8 pad_0x1[3];
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
    UnkCounted* field_0x1c;
};

struct __attribute__((packed)) UnkTable {
    void** vtable;
    u32 count;
    UnkRecord records[250];
    UnkBlock block;
};

extern void* g_unk_7101a21168[];
extern void* g_unk_7101a21120[];
void fn_71002b58c0(u8* to, const u8* from);

void fn_7100518ef4(UnkTable* self, const UnkTable* other) {
    self->vtable = g_unk_7101a21168 + 2;
    u32 count = other->count;
    if (count != 0) {
        u64 i = 0;
        do {
            UnkRecord* to = &self->records[i];
            const UnkRecord* from = &other->records[i];
            to->vtable = g_unk_7101a21120 + 2;
            u16 field_0x8 = from->field_0x8;
            u8 field_0xa = from->field_0xa;
            to->field_0xa = field_0xa;
            to->field_0x8 = field_0x8;
            to->field_0xc = from->field_0xc;
            to->field_0x10 = from->field_0x10;
            to->field_0x14 = from->field_0x14;
            fn_71002b58c0(to->field_0x18, from->field_0x18);
            to->field_0x118 = from->field_0x118;
            count = other->count;
            ++i;
        } while (i < count);
    }
    self->count = count;
    self->block.field_0x0 = other->block.field_0x0;
    self->block.field_0x4 = other->block.field_0x4;
    self->block.field_0x8 = other->block.field_0x8;
    self->block.field_0xc = other->block.field_0xc;
    self->block.field_0x10 = other->block.field_0x10;
    self->block.field_0x14 = other->block.field_0x14;
    self->block.field_0x18 = other->block.field_0x18;
    self->block.field_0x1c = nullptr;
    UnkCounted* shared = other->block.field_0x1c;
    if (shared) {
        ++*reinterpret_cast<a_u32*>(&shared->count);
        if (UnkCounted* old = self->block.field_0x1c) {
            if (--*reinterpret_cast<a_u32*>(&old->count) == 0)
                old->vf_0x10();
        }
        self->block.field_0x1c = shared;
    }
}
