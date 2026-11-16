// Unit 2200 (hint CCutSceneDef): the move assignment of the record fn_7100c8c7b4 copy-assigns: its ids are
// taken over and its four arrays (takes, shots, path ids, path entries) take over the other's storage.
// Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkTU2200.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntVector;
using base::global::CStrId;

struct UnkVec3f_lc8cdf4 {
    f32 x;
    f32 y;
    f32 z;

    UnkVec3f_lc8cdf4& operator=(const UnkVec3f_lc8cdf4& other) {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }
};

// A string id whose move releases the old id (leaving the empty one) before it takes the other's.
struct UnkStrId_lc8cdf4 : CStrId {
    UnkStrId_lc8cdf4& operator=(UnkStrId_lc8cdf4&& other) {
        ReleaseAndTake(other);
        return *this;
    }
};

// A 0x40-byte take, destroyed out of line.
struct UnkTake_lc8cdf4 {
    u8 unk_0x0[0x40];

    ~UnkTake_lc8cdf4() { fn_7100c8a490(this); }
};

// A 0xa0-byte shot, destroyed out of line.
struct UnkShot_lc8cdf4 {
    u8 unk_0x0[0xa0];

    ~UnkShot_lc8cdf4() { fn_7100c886c0(this); }
};

struct UnkPathEntry_lc8cdf4 {
    CFilePathStrId path;
    u64 value;
};

struct UnkRecord_lc8cdf4 {
    u32 field_0x0;
    UnkStrId_lc8cdf4 field_0x8;
    CFilePathStrId field_0x10;
    UnkVec3f_lc8cdf4 field_0x18;
    UnkStrId_lc8cdf4 field_0x28;
    UnkStrId_lc8cdf4 field_0x30;
    UnkStrId_lc8cdf4 field_0x38;
    UnkStrId_lc8cdf4 field_0x40;
    UnkStrId_lc8cdf4 field_0x48;
    UnkStrId_lc8cdf4 field_0x50;
    u64 field_0x58;
    u32 field_0x60;
    bool field_0x64;
    CRntVector<UnkTake_lc8cdf4> takes;  // 0x68
    CRntVector<UnkShot_lc8cdf4> shots;  // 0x90
    CRntVector<CFilePathStrId> paths;  // 0xb8
    CRntVector<UnkPathEntry_lc8cdf4> entries;  // 0xe0
    u32 field_0x108;

    UnkRecord_lc8cdf4& operator=(UnkRecord_lc8cdf4&& other) asm("fn_7100c8cdf4");  // fn_7100c8cdf4
};
static_assert(__builtin_offsetof(UnkRecord_lc8cdf4, takes) == 0x68, "takes");
static_assert(__builtin_offsetof(UnkRecord_lc8cdf4, field_0x108) == 0x108, "field_0x108");

UnkRecord_lc8cdf4& UnkRecord_lc8cdf4::operator=(UnkRecord_lc8cdf4&& other) = default;
