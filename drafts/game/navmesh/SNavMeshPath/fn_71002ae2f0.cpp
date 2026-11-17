// navmesh::SNavMeshPath: copy assignment of a counted array of up to 250 path points (0x120 bytes
// each, from +0x4): assigns the points both arrays hold, then copy-constructs the rest one by one.
// Names and layouts are placeholders.
#include "game/UnkNavMeshPath.h"
#include "game/UnkObject_0x100.h"
#include "game/UnkTU376.h"
#include "types.h"

struct __attribute__((packed)) UnkRecord {
    void** vtable;
    u16 field_0x8;
    u8 field_0xa;
    u8 pad_0xb;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u8 field_0x18[sizeof(UnkObject_0x100)];  // an UnkObject_0x100 at an unaligned address
    u16 field_0x118;
    u8 pad_0x11a[6];
};

static inline void AssignRecord(UnkRecord* to, const UnkRecord* from) {
    u16 field_0x8 = from->field_0x8;
    u8 field_0xa = from->field_0xa;
    to->field_0xa = field_0xa;
    to->field_0x8 = field_0x8;
    to->field_0xc = from->field_0xc;
    to->field_0x10 = from->field_0x10;
    to->field_0x14 = from->field_0x14;
    fn_71002b5c90(reinterpret_cast<UnkObject_0x100*>(to->field_0x18),
                  reinterpret_cast<const UnkObject_0x100*>(from->field_0x18));
    to->field_0x118 = from->field_0x118;
}

static inline void CopyRecord(UnkRecord* to, const UnkRecord* from) {
    to->vtable = g_unk_7101a21120 + 2;
    u16 field_0x8 = from->field_0x8;
    u8 field_0xa = from->field_0xa;
    to->field_0xa = field_0xa;
    to->field_0x8 = field_0x8;
    to->field_0xc = from->field_0xc;
    to->field_0x10 = from->field_0x10;
    to->field_0x14 = from->field_0x14;
    fn_71002b58c0(reinterpret_cast<UnkObject_0x100*>(to->field_0x18),
                  reinterpret_cast<const UnkObject_0x100*>(from->field_0x18));
    to->field_0x118 = from->field_0x118;
}

struct __attribute__((packed)) UnkPointArray {
    u32 count;
    UnkRecord records[250];

    void PushBack(const UnkRecord& record) {
        UnkRecord* data = records;
        CopyRecord(data + count, &record);
        ++count;
    }
};

UnkPointArray* fn_71002ae2f0(UnkPointArray* self, const UnkPointArray* other) {
    if (self != other) {
        if (self->count > other->count) {
            for (u64 i = 0; i < other->count; ++i)
                AssignRecord(&self->records[i], &other->records[i]);
        } else {
            for (u64 i = 0; i < self->count; ++i)
                AssignRecord(&self->records[i], &other->records[i]);
        }
        for (u64 i = self->count; i < other->count; ++i) {
            const UnkRecord& record = other->records[i];
            self->PushBack(record);
        }
        self->count = other->count;
    }
    return self;
}
