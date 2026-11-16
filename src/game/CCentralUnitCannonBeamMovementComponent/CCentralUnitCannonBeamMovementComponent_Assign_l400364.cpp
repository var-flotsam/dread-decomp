// CCentralUnitCannonBeamMovementComponent's unit (tools/tu_map.py unit 607): the copy assignment of
// the 0x100-byte damage description (built by fn_71008c0c74, reset by fn_71008c0e30). The callers
// declare it as a free function (game/CCentralUnitCannonBeamMovementComponent.h).
// Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

// A handle to the description's source, assigned out of line.
struct UnkDamageHandle_l400364 {
    base::UnkRefHandle handle;
    UnkDamageHandle_l400364& operator=(const UnkDamageHandle_l400364& other) {
        fn_710031d208(&handle, &other.handle);
        return *this;
    }
};

struct UnkDamageVec3_l400364 {
    f32 x, y, z;
    UnkDamageVec3_l400364& operator=(const UnkDamageVec3_l400364& other) {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }
};

struct UnkDamageVec4_l400364 {
    f32 x, y, z, w;
    UnkDamageVec4_l400364& operator=(const UnkDamageVec4_l400364& other) {
        x = other.x;
        y = other.y;
        z = other.z;
        w = other.w;
        return *this;
    }
};

struct UnkDamageDesc_l400364 {
    virtual ~UnkDamageDesc_l400364();
    UnkDamageHandle_l400364 field_0x8;
    UnkDamageHandle_l400364 field_0x10;
    bool field_0x18;
    u32 field_0x1c;
    bool field_0x20[0xb];
    u32 field_0x2c;
    bool field_0x30;
    u64 field_0x38;
    u64 field_0x40;
    bool field_0x48;
    bool field_0x49;
    UnkDamageVec4_l400364 field_0x4c;
    u64 field_0x60;
    UnkDamageVec3_l400364 field_0x68;
    UnkDamageVec3_l400364 field_0x74;
    UnkDamageVec3_l400364 field_0x80;
    UnkDamageVec3_l400364 field_0x8c;
    u64 field_0x98;
    CStrId field_0xa0;
    CStrId field_0xa8;
    u64 field_0xb0;
    CStrId field_0xb8;
    u32 field_0xc0;
    u32 field_0xc4;
    u32 field_0xc8;
    bool field_0xcc;
    u32 field_0xd0;
    bool field_0xd4;
    bool field_0xd5;
    UnkDamageVec3_l400364 field_0xd8;
    UnkDamageVec3_l400364 field_0xe4;
    u32 field_0xf0;
    bool field_0xf4;
    bool field_0xf5;
    bool field_0xf6;
    u32 field_0xf8;

    // Member by member: the trivial runs are copied as blocks, the ids and handles by their own
    // assignments.
    UnkDamageDesc_l400364& operator=(const UnkDamageDesc_l400364& other) asm("fn_7100400364");
};
static_assert(__builtin_offsetof(UnkDamageDesc_l400364, field_0x98) == 0x98, "layout");
static_assert(__builtin_offsetof(UnkDamageDesc_l400364, field_0xd8) == 0xd8, "layout");
static_assert(sizeof(UnkDamageDesc_l400364) == 0x100, "layout");

UnkDamageDesc_l400364& UnkDamageDesc_l400364::operator=(const UnkDamageDesc_l400364& other) = default;
