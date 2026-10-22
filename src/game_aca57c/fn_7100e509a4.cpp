// Copy constructors of two records with arrays copied out of line (fn_710011d8b4,
// fn_71000cbae4) and a u16 array copied inline with its settings. The 0xa8-byte record resets
// its bounds at 0x30 (to an empty box) instead of copying them; the 0x80-byte one holds a string
// id at 0x10.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

struct UnkPart0 {
    u64 unk_0x0[5];

    UnkPart0(const UnkPart0& other);
};
void fn_710011d8b4(UnkPart0* self, const UnkPart0* other);
inline UnkPart0::UnkPart0(const UnkPart0& other) { fn_710011d8b4(this, &other); }

struct UnkPart48 {
    u64 unk_0x0[5];

    UnkPart48(const UnkPart48& other);
};
void fn_71000cbae4(UnkPart48* self, const UnkPart48* other);
inline UnkPart48::UnkPart48(const UnkPart48& other) { fn_71000cbae4(this, &other); }

// base::UnkVector<u16>'s layout.
struct UnkU16Array {
    u16* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    // Takes the settings, reserves the source's capacity and copies the elements.
    __attribute__((always_inline)) UnkU16Array(const UnkU16Array& other)
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

struct UnkBounds {
    f32 min_x;
    f32 min_y;
    f32 max_x;
    f32 max_y;

    UnkBounds() : min_x(3.4028235e38f), min_y(3.4028235e38f), max_x(-3.4028235e38f), max_y(-3.4028235e38f) {}
};

struct UnkRecordA8 {
    UnkPart0 field_0x0;
    u32 field_0x28;
    bool field_0x2c;
    bool field_0x2d;
    UnkBounds field_0x30;
    u32 field_0x40;
    UnkPart48 field_0x48;
    UnkU16Array field_0x70;
    u32 field_0x98;
    u32 field_0x9c;
    u32 field_0xa0;
    u32 field_0xa4;

    __attribute__((always_inline)) UnkRecordA8(const UnkRecordA8& other)
        : field_0x0(other.field_0x0), field_0x28(other.field_0x28), field_0x2c(other.field_0x2c), field_0x2d(true),
          field_0x40(other.field_0x40), field_0x48(other.field_0x48), field_0x70(other.field_0x70),
          field_0x98(other.field_0x98), field_0x9c(other.field_0x9c), field_0xa0(other.field_0xa0),
          field_0xa4(other.field_0xa4) {}
};
static_assert(sizeof(UnkRecordA8) == 0xa8, "UnkRecordA8 layout");

void fn_7100e509a4(UnkRecordA8* self, const UnkRecordA8& other) { new (self) UnkRecordA8(other); }

struct UnkRecord80 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    base::global::CStrId field_0x10;
    UnkPart48 field_0x18;
    UnkU16Array field_0x40;
    u32 field_0x68;
    u32 field_0x6c;
    u32 field_0x70;
    u32 field_0x74;
    u32 field_0x78;

    __attribute__((always_inline)) UnkRecord80(const UnkRecord80& other)
        : field_0x0(other.field_0x0), field_0x4(other.field_0x4), field_0x8(other.field_0x8),
          field_0xc(other.field_0xc), field_0x10(other.field_0x10), field_0x18(other.field_0x18),
          field_0x40(other.field_0x40), field_0x68(other.field_0x68), field_0x6c(other.field_0x6c),
          field_0x70(other.field_0x70), field_0x74(other.field_0x74), field_0x78(other.field_0x78) {}
};
static_assert(sizeof(UnkRecord80) == 0x80, "UnkRecord80 layout");

void fn_7100e5190c(UnkRecord80* self, const UnkRecord80& other) { new (self) UnkRecord80(other); }
