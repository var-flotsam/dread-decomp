// From unit 280 of tools/tu_map.py (0x71001fb9ac-0x71001fe698): the two copy assignments of the
// reflected 0xff0-byte value its descriptor points at: an array of 8-byte entries at +0x0 (data,
// count, capacity, the allocator's flags at +0x18 and the allocator at +0x20; grown to the source's
// capacity when smaller, then copied bitwise), eight words at +0x28, a hundred 0x28-byte records at
// +0x48 (entry arrays, each assigned by fn_71001fdacc) and a word at +0xfe8. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

// The scene manager's entry arrays (src/engine/scene/CSceneManager.cpp), copy-assigned out of line.
struct UnkSceneEntry;
base::UnkVector<UnkSceneEntry>* fn_71001fdacc(base::UnkVector<UnkSceneEntry>* self, const base::UnkVector<UnkSceneEntry>* other);

struct UnkUnit280Record {
    u64 words[5];

    UnkUnit280Record& operator=(const UnkUnit280Record& other) {
        fn_71001fdacc(reinterpret_cast<base::UnkVector<UnkSceneEntry>*>(this),
                      reinterpret_cast<const base::UnkVector<UnkSceneEntry>*>(&other));
        return *this;
    }
};

struct UnkUnit280Array {
    u64* data;
    u32 count;     // 0x8
    u32 capacity;  // 0xc
    u32 step;      // 0x10
    bool field_0x14;
    u32 flags;                      // 0x18
    base::UnkAllocator* allocator;  // 0x20

    __attribute__((always_inline)) UnkUnit280Array& operator=(const UnkUnit280Array& other) {
        count = 0;
        u32 cap = other.capacity;
        u64* p;
        if (capacity < cap) {
            p = static_cast<u64*>(allocator->vf_0x40((u64)cap * sizeof(u64), 8, flags));
            if (data) {
                __builtin_memcpy(p, data, (u64)count * sizeof(u64));
                allocator->vf_0x48(data);
            }
            data = p;
            capacity = cap;
        } else {
            p = data;
        }
        __builtin_memcpy(p, other.data, (u64)other.count * sizeof(u64));
        count = other.count;
        return *this;
    }
};

struct UnkUnit280Value {
    UnkUnit280Array entries;
    struct {
        u32 words[8];
    } field_0x28;
    UnkUnit280Record records[100];  // 0x48
    u32 field_0xfe8;

    __attribute__((always_inline)) UnkUnit280Value& operator=(const UnkUnit280Value& other) {
        if (this != &other) {
            entries = other.entries;
            field_0x28 = other.field_0x28;
            for (u32 i = 0; i < 100; ++i)
                records[i] = other.records[i];
            field_0xfe8 = other.field_0xfe8;
        }
        return *this;
    }
};
static_assert(sizeof(UnkUnit280Value) == 0xff0, "UnkUnit280Value layout");

void* fn_71001fe078(void* dst, const void* src) {
    return &(*static_cast<UnkUnit280Value*>(dst) = *static_cast<const UnkUnit280Value*>(src));
}
void* fn_71001fe16c(void* dst, const void* src) {
    return &(*static_cast<UnkUnit280Value*>(dst) = *static_cast<const UnkUnit280Value*>(src));
}
