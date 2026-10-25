// Shrink-to-fit of a container of two parallel arrays (0x118-byte records and u32 values)
// sharing a count and capacity: both are reallocated to exactly `count`, the records moved over
// (each source destroyed by fn_7100ea7034, also here), the values copied and the old arrays freed.
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::UnkAllocator;
using base::global::UnkStringData;

// A string id whose move leaves the pool's empty string behind.
struct UnkMovedStrId {
    UnkStringData* data;

    UnkMovedStrId(UnkMovedStrId&& other) : data(other.data) { other.data = &g_unk_7101d47108->empty; }
    // Decrements and tests in two steps (the original reloads the id after the decrement).
    __attribute__((always_inline)) ~UnkMovedStrId() {
        if (base::global::UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

// A 0x30-byte string moved by fn_710007eab4.
struct UnkMovedString {
    u64 unk_0x0[6];

    UnkMovedString(UnkMovedString&& other);
    ~UnkMovedString();
};
void fn_710007eab4(UnkMovedString* dst, UnkMovedString* src);
__attribute__((always_inline)) inline UnkMovedString::~UnkMovedString() { fn_710007ecd4(reinterpret_cast<base::global::CRntString*>(this)); }
__attribute__((always_inline)) inline UnkMovedString::UnkMovedString(UnkMovedString&& other) { fn_710007eab4(this, &other); }

// A string with its own inline storage, copied (not moved) by fn_710007eaa0.
extern void* g_unk_7101a1b5a0[];
struct UnkStrStorage {
    void* vtable;
    u8 unk_0x8[0x20];
    void* field_0x28;
    u32 field_0x30;
};
struct UnkStrBuffer {
    u8 unk_0x0[0x30];
    UnkStrStorage storage;

    UnkStrBuffer(const UnkStrBuffer& other);
    ~UnkStrBuffer();
};
void fn_710007e6e4(UnkStrBuffer* self, void* storage);
void fn_710007ed40(UnkStrBuffer* self);  // releases the storage
__attribute__((always_inline)) inline UnkStrBuffer::~UnkStrBuffer() {
    fn_710007ed40(this);
    fn_710007ecd4(reinterpret_cast<base::global::CRntString*>(this));
}
void fn_710007eaa0(UnkStrBuffer* self, const UnkStrBuffer* other);
__attribute__((always_inline)) inline UnkStrBuffer::UnkStrBuffer(const UnkStrBuffer& other) {
    fn_710007e6e4(this, &storage);
    storage.vtable = g_unk_7101a1b5a0 + 2;
    storage.field_0x28 = nullptr;
    storage.field_0x30 = 0;
    fn_710007eaa0(this, &other);
}

// Plain values copied as a whole.
struct UnkPod40 {
    u32 field_0x0[5];
};
struct UnkPod90 {
    u32 field_0x0[3];
};
struct UnkPod110 {
    u16 field_0x0[3];
};

struct UnkRecord118 {
    u64 field_0x0;
    UnkMovedStrId field_0x8;
    UnkMovedStrId field_0x10;
    UnkMovedStrId field_0x18;
    u32 field_0x20;
    u32 field_0x24;
    u32 field_0x28;
    u32 field_0x2c;
    u32 field_0x30;
    u32 field_0x34;
    u32 field_0x38;
    u32 unk_0x3c;
    UnkPod40 field_0x40;
    UnkMovedString field_0x58;
    u32 field_0x88;
    u32 unk_0x8c;
    UnkPod90 field_0x90;
    UnkStrBuffer field_0xa0;
    u32 field_0x108;
    u32 unk_0x10c;
    UnkPod110 field_0x110;

    __attribute__((always_inline)) UnkRecord118(UnkRecord118&& other)
        : field_0x0(other.field_0x0), field_0x8(static_cast<UnkMovedStrId&&>(other.field_0x8)),
          field_0x10(static_cast<UnkMovedStrId&&>(other.field_0x10)),
          field_0x18(static_cast<UnkMovedStrId&&>(other.field_0x18)), field_0x20(other.field_0x20),
          field_0x24(other.field_0x24), field_0x28(other.field_0x28), field_0x2c(other.field_0x2c),
          field_0x30(other.field_0x30), field_0x34(other.field_0x34), field_0x38(other.field_0x38),
          field_0x40(other.field_0x40), field_0x58(static_cast<UnkMovedString&&>(other.field_0x58)),
          field_0x88(other.field_0x88), field_0x90(other.field_0x90), field_0xa0(other.field_0xa0),
          field_0x108(other.field_0x108), field_0x110(other.field_0x110) {}
    __attribute__((always_inline)) ~UnkRecord118() {}
};
static_assert(sizeof(UnkRecord118) == 0x118, "UnkRecord118 layout");

__attribute__((noinline)) void fn_7100ea7034(UnkRecord118* record);

// The record's destructor.
void fn_7100ea7034(UnkRecord118* record) { record->~UnkRecord118(); }

struct UnkRecord118Table {
    UnkRecord118* records;
    u32* values;
    u32 count;
    u32 capacity;
    u32 field_0x18;
    u32 flags;
    UnkAllocator* allocator;
};

void fn_7100eae77c(UnkRecord118Table* self) {
    if (self->count == self->capacity)
        return;
    UnkRecord118* records = static_cast<UnkRecord118*>(
        self->allocator->vf_0x40((u64)self->count * sizeof(UnkRecord118), 8, self->flags));
    u32* values = static_cast<u32*>(self->allocator->vf_0x40((u64)self->count * sizeof(u32), 8, self->flags));
    if (UnkRecord118* old = self->records) {
        UnkRecord118* dst = records;
        for (u32 i = 0; i < self->count; ++i, ++old, ++dst) {
            new (dst) UnkRecord118(static_cast<UnkRecord118&&>(*old));
            fn_7100ea7034(old);
        }
        self->allocator->vf_0x48(self->records);
    }
    if (self->values) {
        __builtin_memcpy(values, self->values, (u64)self->count * sizeof(u32));
        self->allocator->vf_0x48(self->values);
    }
    self->records = records;
    self->values = values;
    self->capacity = self->count;
}
