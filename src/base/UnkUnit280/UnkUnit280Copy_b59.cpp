// From unit 280 of tools/tu_map.py (0x71001fb9ac-0x71001fe698): the copy constructors (reflection
// slots 0x30 and 0x38) of the reflected 0xff0-byte value whose assignments are in
// UnkUnit280Assign.cpp, and the copy constructor of its 0x28-byte entry arrays (fn_71001fe260):
// reserve the source's capacity, then append each entry with the out-of-line PushBack. Names and
// layouts are placeholders.
#include "base/UnkVector.h"
#include "game/UnkTU280.h"
#include "types.h"

// The scene manager's entry arrays (src/engine/scene/CSceneManager.cpp).
struct UnkSceneEntry;

// An entry (0x28 bytes), moved field by field when the array grows.
typedef u64 __attribute__((may_alias)) UnkAliasU64_b59;
typedef u32 __attribute__((may_alias)) UnkAliasU32_b59;
struct UnkUnit280Entry_b59 {
    u64 field_0x0;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
    u32 field_0x1c;
    u32 field_0x20;
    bool field_0x24;

    UnkUnit280Entry_b59(UnkUnit280Entry_b59&& o) {
        char* d = reinterpret_cast<char*>(this);
        u64 v0 = o.field_0x0;
        *reinterpret_cast<UnkAliasU32_b59*>(d + 0x8) = o.field_0x8;
        *reinterpret_cast<UnkAliasU64_b59*>(d + 0x0) = v0;
        *reinterpret_cast<UnkAliasU32_b59*>(d + 0xc) = o.field_0xc;
        *reinterpret_cast<UnkAliasU32_b59*>(d + 0x10) = o.field_0x10;
        *reinterpret_cast<UnkAliasU32_b59*>(d + 0x14) = o.field_0x14;
        *reinterpret_cast<UnkAliasU32_b59*>(d + 0x18) = o.field_0x18;
        *reinterpret_cast<UnkAliasU32_b59*>(d + 0x1c) = o.field_0x1c;
        *reinterpret_cast<UnkAliasU32_b59*>(d + 0x20) = o.field_0x20;
        *reinterpret_cast<u8*>(d + 0x24) = o.field_0x24;
    }
};
static_assert(sizeof(UnkUnit280Entry_b59) == 0x28, "UnkUnit280Entry_b59 layout");

struct UnkUnit280Record_b59 {
    UnkUnit280Entry_b59* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) UnkUnit280Record_b59(const UnkUnit280Record_b59& o)
        : data(nullptr), count(0), capacity(0), step(o.step), field_0x14(o.field_0x14), flags(o.flags),
          allocator(o.allocator) {
        u32 cap = o.capacity;
        if (cap != 0) {
            UnkUnit280Entry_b59* p =
                static_cast<UnkUnit280Entry_b59*>(allocator->vf_0x40((u64)cap * sizeof(UnkUnit280Entry_b59), 8, flags));
            if (data) {
                UnkUnit280Entry_b59* src = data;
                UnkUnit280Entry_b59* dst = p;
                for (u32 i = 0; i < count; ++i, ++dst, ++src) {
                    asm("" : "+r"(dst));
                    new (dst) UnkUnit280Entry_b59(static_cast<UnkUnit280Entry_b59&&>(*src));
                }
                allocator->vf_0x48(data);
            }
            data = p;
            capacity = cap;
        }
        for (const UnkUnit280Entry_b59 *it = o.data, *end = it + o.count; it != end; ++it)
            fn_71001fe390(reinterpret_cast<base::UnkVector<UnkSceneEntry>*>(this),
                          reinterpret_cast<const UnkSceneEntry*>(it));
    }
};

void fn_71001fe260(void* dst, const void* src) {
    new (dst) UnkUnit280Record_b59(*static_cast<const UnkUnit280Record_b59*>(src));
}

// The 0xff0-byte value (UnkUnit280Assign.cpp's UnkUnit280Value): its 8-byte entry array is copied
// bitwise after reserving the source's capacity, its records by fn_71001fe260.
struct UnkUnit280RecordRef_b59 {
    u64 words[5];

    UnkUnit280RecordRef_b59(const UnkUnit280RecordRef_b59& o) { fn_71001fe260(this, &o); }
};

struct UnkUnit280Array_b59 {
    u64* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) UnkUnit280Array_b59(const UnkUnit280Array_b59& o)
        : data(nullptr), count(0), capacity(0), step(o.step), field_0x14(o.field_0x14), flags(o.flags),
          allocator(o.allocator) {
        u32 cap = o.capacity;
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
        __builtin_memcpy(p, o.data, (u64)o.count * sizeof(u64));
        count = o.count;
    }
};

struct UnkUnit280Value_b59 {
    UnkUnit280Array_b59 entries;
    struct {
        u32 words[8];
    } field_0x28;
    UnkUnit280RecordRef_b59 records[100];  // 0x48
    u32 field_0xfe8;
};
static_assert(sizeof(UnkUnit280Value_b59) == 0xff0, "UnkUnit280Value_b59 layout");

void fn_71001fde84(void* dst, const void* src) {
    new (dst) UnkUnit280Value_b59(*static_cast<const UnkUnit280Value_b59*>(src));
}
void fn_71001fdf7c(void* dst, const void* src) {
    new (dst) UnkUnit280Value_b59(*static_cast<const UnkUnit280Value_b59*>(src));
}
