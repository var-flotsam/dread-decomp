// Unit 2200 (hint CCutSceneDef): the copy assignment of a record of string ids, a path id, a vector, plain
// values and four arrays (takes, shots, path ids, path entries), each array assigned by its own operator.
// Names and layouts are placeholders.
#include "base/UnkResPathVec.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkElem40Array_b42a3.h"
#include "game/UnkElem40_b42a3.h"
#include "game/UnkElemA0Array_b42a3.h"
#include "game/UnkPathArray_b239.h"
#include "game/UnkTU2200.h"
#include "types.h"

#include <new>

using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkElemA0_b42a3;

struct UnkPathEntry_b239;

UnkElemA0Array_b42a3* fn_7100c8cb88(UnkElemA0Array_b42a3* self, const UnkElemA0Array_b42a3* other);
UnkPathArray_b239* fn_7100c8cc78(UnkPathArray_b239* self, const UnkPathArray_b239* other);

struct UnkVec3f_c8c7b4 {
    f32 x;
    f32 y;
    f32 z;

    UnkVec3f_c8c7b4& operator=(const UnkVec3f_c8c7b4& other) {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }
};

// The takes: the old ones are destroyed, then the other's are appended after reserving its capacity.
struct UnkTakeList_c8c7b4 : UnkElem40Array_b42a3 {
    UnkTakeList_c8c7b4& operator=(const UnkTakeList_c8c7b4& other) {
        if (this != &other) {
            for (u64 i = 0; i < count; ++i)
                fn_7100c8a490(&data[i]);
            count = 0;
            fn_7100c8b758(this, other.capacity);
            for (const UnkElem40_b42a3 *it = other.data, *end = it + other.count; it != end; ++it)
                fn_7100c8b894(this, it);
        }
        return *this;
    }
};

struct UnkShotList_c8c7b4 : UnkElemA0Array_b42a3 {
    UnkShotList_c8c7b4& operator=(const UnkShotList_c8c7b4& other) {
        fn_7100c8cb88(this, &other);
        return *this;
    }
};

struct UnkPathIdList_c8c7b4 : UnkResPathVec {
    UnkPathIdList_c8c7b4& operator=(const UnkPathIdList_c8c7b4& other) {
        fn_71000d0958(this, &other);
        return *this;
    }
};

struct UnkPathList_c8c7b4 : UnkPathArray_b239 {
    UnkPathList_c8c7b4& operator=(const UnkPathList_c8c7b4& other) {
        fn_7100c8cc78(this, &other);
        return *this;
    }
};

struct UnkRecord_c8c7b4 {
    u32 field_0x0;
    CStrId field_0x8;
    CFilePathStrId field_0x10;
    UnkVec3f_c8c7b4 field_0x18;
    CStrId field_0x28;
    CStrId field_0x30;
    CStrId field_0x38;
    CStrId field_0x40;
    CStrId field_0x48;
    CStrId field_0x50;
    u64 field_0x58;
    u32 field_0x60;
    bool field_0x64;
    UnkTakeList_c8c7b4 field_0x68;
    UnkShotList_c8c7b4 field_0x90;
    UnkPathIdList_c8c7b4 field_0xb8;
    UnkPathList_c8c7b4 field_0xe0;
    u32 field_0x108;

    UnkRecord_c8c7b4& operator=(const UnkRecord_c8c7b4& other) asm("fn_7100c8c7b4");  // fn_7100c8c7b4
};
static_assert(__builtin_offsetof(UnkRecord_c8c7b4, field_0x68) == 0x68, "field_0x68");
static_assert(__builtin_offsetof(UnkRecord_c8c7b4, field_0x108) == 0x108, "field_0x108");

UnkRecord_c8c7b4& UnkRecord_c8c7b4::operator=(const UnkRecord_c8c7b4& other) = default;
