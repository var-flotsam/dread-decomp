// game::logic::collision::CCollider unit: erases the element an index iterator names from a
// base::global::CRntVector layout whose 0x28-byte elements are themselves byte arrays, destroying it and
// moving the later ones down. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "types.h"

struct UnkByteArray {
    u8* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::IAllocator* allocator;

    UnkByteArray(UnkByteArray&& other)
        : data(other.data),
          count(other.count),
          capacity(other.capacity),
          step(other.step),
          field_0x14(other.field_0x14),
          flags(other.flags),
          allocator(other.allocator) {
        other.data = nullptr;
        other.count = 0;
        other.capacity = 0;
    }
    ~UnkByteArray() {
        if (data) {
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

struct UnkByteArrays {
    UnkByteArray* data;
    u32 count;
};

void fn_7100d437dc(UnkByteArrays* self, const u32* it) {
    UnkByteArray* base = reinterpret_cast<UnkByteArray*>(self);
    u64 offset = (u64)*it * sizeof(UnkByteArray);
    u32 index = (u32)(reinterpret_cast<UnkByteArray*>(reinterpret_cast<char*>(base) + offset) - base);
    if (index < self->count) {
        self->data[index].~UnkByteArray();
        for (u32 i = index; i < self->count - 1; ++i) {
            new (&self->data[i]) UnkByteArray(static_cast<UnkByteArray&&>(self->data[i + 1]));
            self->data[i + 1].~UnkByteArray();
        }
        self->count = self->count - 1;
    }
}
