// Unit 2200 (hint CCutSceneDef): the copy constructor of the record of string ids, a path id, a vector,
// plain values and four arrays (takes, shots, path ids, path entries) whose copy assignment is fn_7100c8c7b4.
// Names and layouts are placeholders.
#include <new>

#include "base/UnkResPathVec.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkElem40Array_b42a3.h"
#include "game/UnkTU2200.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkElem40_b42a3 {
    CStrId field_0x0;
    u32 field_0x8;
    CFilePathStrId field_0x10;
    CStrId field_0x18;
    CStrId field_0x20;
    CStrId field_0x28;
    u64 field_0x30;
    u32 field_0x38;
};

struct UnkElemA0_b42a3;

struct UnkElemA0Array_b42a3 {
    UnkElemA0_b42a3* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

struct UnkPathEntry_b239;

struct UnkPathArray_b239 {
    UnkPathEntry_b239* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 field_0x18;
    u64 field_0x20;
};

void fn_7100c8b688(UnkElemA0Array_b42a3* self, const UnkElemA0Array_b42a3* other);
void fn_7100c8c108(UnkPathArray_b239* self, const UnkPathArray_b239* other);

struct UnkVec3f_lc8b3d4 {
    f32 x;
    f32 y;
    f32 z;

    UnkVec3f_lc8b3d4(const UnkVec3f_lc8b3d4& other) : x(other.x), y(other.y), z(other.z) {}
};

// The takes: the other's capacity is reserved, then each take appended.
struct UnkTakeList_lc8b3d4 : UnkElem40Array_b42a3 {
    UnkTakeList_lc8b3d4(const UnkTakeList_lc8b3d4& other) {
        data = nullptr;
        count = 0;
        capacity = 0;
        step = other.step;
        field_0x14 = other.field_0x14;
        flags = other.flags;
        allocator = other.allocator;
        fn_7100c8b758(this, other.capacity);
        for (const UnkElem40_b42a3 *it = other.data, *end = it + other.count; it != end; ++it)
            fn_7100c8b894(this, it);
    }
};

struct UnkShotList_lc8b3d4 : UnkElemA0Array_b42a3 {
    UnkShotList_lc8b3d4(const UnkShotList_lc8b3d4& other) { fn_7100c8b688(this, &other); }
};

// The path ids: the other's capacity is reserved, then each id appended, growing by the step.
struct UnkPathIdList_lc8b3d4 {
    CFilePathStrId* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    void Reserve(u32 n) { fn_7100018450(reinterpret_cast<UnkResPathVec*>(this), n); }
    void PushBack(const CFilePathStrId& id) {
        u32 n = count;
        u32 new_capacity = capacity;
        if (new_capacity <= n && step != 0)
            new_capacity = (n / step + 1) * step;
        if ((s32)n >= (s32)new_capacity)
            for (;;) {
            }
        Reserve(new_capacity);
        new (&data[count]) CFilePathStrId(id);
        ++count;
    }
    UnkPathIdList_lc8b3d4(const UnkPathIdList_lc8b3d4& other)
        : data(nullptr), count(0), capacity(0), step(other.step), field_0x14(other.field_0x14),
          flags(other.flags), allocator(other.allocator) {
        Reserve(other.capacity);
        for (const CFilePathStrId *p = other.data, *end = p + other.count; p != end; ++p)
            PushBack(*p);
    }
};

struct UnkPathList_lc8b3d4 : UnkPathArray_b239 {
    UnkPathList_lc8b3d4(const UnkPathList_lc8b3d4& other) { fn_7100c8c108(this, &other); }
};

struct UnkRecord_lc8b3d4 {
    u32 field_0x0;
    CStrId field_0x8;
    CFilePathStrId field_0x10;
    UnkVec3f_lc8b3d4 field_0x18;
    CStrId field_0x28;
    CStrId field_0x30;
    CStrId field_0x38;
    CStrId field_0x40;
    CStrId field_0x48;
    CStrId field_0x50;
    u64 field_0x58;
    u32 field_0x60;
    bool field_0x64;
    UnkTakeList_lc8b3d4 field_0x68;
    UnkShotList_lc8b3d4 field_0x90;
    UnkPathIdList_lc8b3d4 field_0xb8;
    UnkPathList_lc8b3d4 field_0xe0;
    u32 field_0x108;

    UnkRecord_lc8b3d4(const UnkRecord_lc8b3d4& other) asm("fn_7100c8b3d4");  // fn_7100c8b3d4
};
static_assert(__builtin_offsetof(UnkRecord_lc8b3d4, field_0x68) == 0x68, "field_0x68");
static_assert(__builtin_offsetof(UnkRecord_lc8b3d4, field_0x108) == 0x108, "field_0x108");

UnkRecord_lc8b3d4::UnkRecord_lc8b3d4(const UnkRecord_lc8b3d4& other) = default;
