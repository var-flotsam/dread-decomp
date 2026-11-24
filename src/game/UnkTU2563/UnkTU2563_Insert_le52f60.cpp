// The CMinimapData unit (unit 2563): inserting a copy of a 0x60-byte record (an array of 3D vectors, an array of
// u16 and four words) into an array of them. Names and layouts are placeholders.
#include <new>
#include "base/UnkFreedVectorPair.h"
#include "game/UnkTU83.h"
#include "types.h"

namespace {

// base::global::CRntVector<u16>'s layout.
struct UnkU16Array_le52f60 {
    u16* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::IAllocator* allocator;

    // Takes the settings, reserves the source's capacity and copies the elements.
    UnkU16Array_le52f60(const UnkU16Array_le52f60& other)
        : data(nullptr), count(0), capacity(0), step(other.step), field_0x14(other.field_0x14),
          flags(other.flags), allocator(other.allocator) {
        u32 n = other.capacity;
        if (capacity < n) {
            u16* p = static_cast<u16*>(allocator->vf_0x40((u64)n * sizeof(u16), 8, flags));
            if (data) {
                __builtin_memcpy(p, data, (u64)count * sizeof(u16));
                allocator->vf_0x48(data);
            }
            data = p;
            capacity = n;
        }
        __builtin_memcpy(data, other.data, (u64)other.count * sizeof(u16));
        count = other.count;
    }
};

// A base::global::CRntVector of 3D vectors, copied out of line.
struct UnkPoints {
    u8 raw[0x28];

    UnkPoints(const UnkPoints& other) {
        fn_71000cbae4(reinterpret_cast<UnkPart48*>(this), reinterpret_cast<const UnkPart48*>(&other));
    }
};

// The record as its copy constructor sees it.
struct UnkRecord_le52f60 {
    UnkPoints points;
    UnkU16Array_le52f60 indices;
    u32 field_0x50;
    u32 field_0x54;
    u32 field_0x58;
    u32 field_0x5c;

    UnkRecord_le52f60(const UnkRecord_le52f60& other)
        : points(other.points), indices(other.indices), field_0x50(other.field_0x50),
          field_0x54(other.field_0x54), field_0x58(other.field_0x58), field_0x5c(other.field_0x5c) {}
};
static_assert(sizeof(UnkRecord_le52f60) == 0x60, "UnkRecord_le52f60 layout");

}  // namespace

// Inserts a copy of a record at `index`: an append goes to the PushBack; otherwise the array grows by its step
// through the out-of-line Reserve and the later records move up one.
void fn_7100e52f60(UnkPairArray* self, const UnkFreedVectorPair* value, u32 index) {
    u32 n = self->count;
    if (n == index) {
        PushBackRecord60Copy(self, value);
        return;
    }
    u32 new_capacity = self->capacity;
    if (new_capacity <= n && self->step != 0)
        new_capacity = (n / self->step + 1) * self->step;
    fn_7100e52b58(self, new_capacity);
    for (u32 i = self->count - 1; i >= index; --i) {
        UnkFreedVectorPair* q = &self->data[i];
        new (q + 1) UnkFreedVectorPair(static_cast<UnkFreedVectorPair&&>(*q));
        self->data[i].~UnkFreedVectorPair();
        if (i == 0)
            break;
    }
    new (&self->data[index]) UnkRecord_le52f60(*reinterpret_cast<const UnkRecord_le52f60*>(value));
    ++self->count;
}
