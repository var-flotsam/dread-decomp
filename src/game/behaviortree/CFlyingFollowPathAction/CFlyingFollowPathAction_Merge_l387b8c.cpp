// behaviortree::CFlyingFollowPathAction's unit (tools/tu_map.py unit 484): merges a set of optional parameters
// into another: each value the source has overrides this one's; `reset` first clears what this one has. The
// later group applies only when the source's mode is "any" (4) or this one's. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/UnkTU83.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CRntVector;

template <class T>
struct UnkCopiedVector;
struct UnkThreeWords;
UnkCopiedVector<UnkThreeWords>* fn_71000cbcf0(UnkCopiedVector<UnkThreeWords>* self,
                                              const UnkCopiedVector<UnkThreeWords>* other);

struct UnkThreeFloats_l387b8c {
    f32 x;
    f32 y;
    f32 z;
};

struct UnkTwoWords_l387b8c {
    u64 first;
    u64 second;
};

struct UnkFlyingPathParams_l387b8c {
    void** vtable;
    UnkThreeFloats_l387b8c field_0x8;
    bool has_0x8;  // 0x14
    u32 field_0x18;
    bool has_0x18;  // 0x1c
    UnkTwoWords_l387b8c field_0x20;
    bool has_0x20;  // 0x30
    u32 mode;  // 0x34; 4 matches any
    u64 field_0x38;
    bool has_0x38;  // 0x40
    u32 field_0x44;
    bool has_0x44;
    u32 field_0x4c;
    bool has_0x4c;
    u32 field_0x54;
    bool has_0x54;
    u32 field_0x5c;
    bool has_0x5c;
    u32 field_0x64;
    bool has_0x64;
    u32 field_0x6c;
    bool has_0x6c;
    u32 field_0x74;
    bool has_0x74;  // 0x78
    UnkThreeFloats_l387b8c field_0x7c;
    bool has_0x7c;  // 0x88
    u32 field_0x8c;
    bool has_0x8c;  // 0x90
    u8 field_0x91;
    bool has_0x91;
    u8 field_0x93;
    bool has_0x93;  // 0x94
    u32 field_0x98;
    bool has_0x98;  // 0x9c
    u8 field_0x9d;
    bool has_0x9d;
    u8 field_0x9f;
    bool has_0x9f;  // 0xa0
    UnkTwoWords_l387b8c field_0xa8;
    bool has_0xa8;  // 0xb8
    UnkTwoWords_l387b8c field_0xc0;
    bool has_0xc0;  // 0xd0
    CRntVector<UnkVec3f> path;  // 0xd8
    bool has_path;  // 0x100
    u8 field_0x101;
    bool has_0x101;
    u8 field_0x103;
    bool has_0x103;
    u8 field_0x105;
    bool has_0x105;  // 0x106
};
static_assert(__builtin_offsetof(UnkFlyingPathParams_l387b8c, mode) == 0x34, "layout");
static_assert(__builtin_offsetof(UnkFlyingPathParams_l387b8c, field_0xa8) == 0xa8, "layout");
static_assert(__builtin_offsetof(UnkFlyingPathParams_l387b8c, has_path) == 0x100, "layout");
static_assert(__builtin_offsetof(UnkFlyingPathParams_l387b8c, has_0x105) == 0x106, "layout");

// Takes the source's value when it has one.
template <class T>
static inline void Merge(T& value, bool& has, const T& other_value, const bool& other_has) {
    if (other_has) {
        value = other_value;
        has = true;
    }
}

static inline void Assign(CRntVector<UnkVec3f>* self, const CRntVector<UnkVec3f>* other) {
    fn_71000cbcf0(reinterpret_cast<UnkCopiedVector<UnkThreeWords>*>(self),
                  reinterpret_cast<const UnkCopiedVector<UnkThreeWords>*>(other));
}

void fn_7100387b8c(UnkFlyingPathParams_l387b8c* self, const UnkFlyingPathParams_l387b8c* other, bool reset) {
    if (reset) {
        self->has_0x8 = false;
        self->has_0x18 = false;
        self->has_0x20 = false;
    }
    Merge(self->field_0x8, self->has_0x8, other->field_0x8, other->has_0x8);
    Merge(self->field_0x18, self->has_0x18, other->field_0x18, other->has_0x18);
    Merge(self->field_0x20, self->has_0x20, other->field_0x20, other->has_0x20);

    if (reset) {
        self->has_0x38 = false;
        self->has_0x44 = false;
        self->has_0x4c = false;
        self->has_0x54 = false;
        self->has_0x5c = false;
        self->has_0x64 = false;
        self->has_0x6c = false;
        self->has_0x74 = false;
        self->has_0x7c = false;
        self->has_0x8c = false;
        self->has_0x91 = false;
        self->has_0x93 = false;
        self->has_0x98 = false;
        self->has_0x9d = false;
        self->has_0x9f = false;
        self->has_0xa8 = false;
        self->has_0xc0 = false;
        self->has_path = false;
        self->has_0x101 = false;
        self->has_0x103 = false;
        self->has_0x105 = false;
    }
    if (other->mode != 4 && other->mode != self->mode)
        return;

    Merge(self->field_0x38, self->has_0x38, other->field_0x38, other->has_0x38);
    Merge(self->field_0x44, self->has_0x44, other->field_0x44, other->has_0x44);
    Merge(self->field_0x4c, self->has_0x4c, other->field_0x4c, other->has_0x4c);
    Merge(self->field_0x54, self->has_0x54, other->field_0x54, other->has_0x54);
    Merge(self->field_0x5c, self->has_0x5c, other->field_0x5c, other->has_0x5c);
    Merge(self->field_0x64, self->has_0x64, other->field_0x64, other->has_0x64);
    Merge(self->field_0x6c, self->has_0x6c, other->field_0x6c, other->has_0x6c);
    Merge(self->field_0x74, self->has_0x74, other->field_0x74, other->has_0x74);
    Merge(self->field_0x7c, self->has_0x7c, other->field_0x7c, other->has_0x7c);
    Merge(self->field_0x8c, self->has_0x8c, other->field_0x8c, other->has_0x8c);
    Merge(self->field_0x91, self->has_0x91, other->field_0x91, other->has_0x91);
    Merge(self->field_0x93, self->has_0x93, other->field_0x93, other->has_0x93);
    Merge(self->field_0x98, self->has_0x98, other->field_0x98, other->has_0x98);
    Merge(self->field_0x9d, self->has_0x9d, other->field_0x9d, other->has_0x9d);
    Merge(self->field_0x9f, self->has_0x9f, other->field_0x9f, other->has_0x9f);
    Merge(self->field_0xa8, self->has_0xa8, other->field_0xa8, other->has_0xa8);
    Merge(self->field_0xc0, self->has_0xc0, other->field_0xc0, other->has_0xc0);

    // The path goes through a local copy: fetched when the source has one, then set from a copy of it.
    CRntVector<UnkVec3f> path(0xf);
    if (other->has_path) {
        Assign(&path, &other->path);
        if (other->has_path) {
            // Built in place by the array's out-of-line copy constructor.
            alignas(CRntVector<UnkVec3f>) u8 storage[sizeof(CRntVector<UnkVec3f>)];
            auto* value = reinterpret_cast<CRntVector<UnkVec3f>*>(storage);
            fn_71000cbae4(reinterpret_cast<UnkPart48*>(value), reinterpret_cast<const UnkPart48*>(&path));
            Assign(&self->path, value);
            self->has_path = true;
            value->Free();
        }
    }
    Merge(self->field_0x101, self->has_0x101, other->field_0x101, other->has_0x101);
    Merge(self->field_0x103, self->has_0x103, other->field_0x103, other->has_0x103);
    Merge(self->field_0x105, self->has_0x105, other->field_0x105, other->has_0x105);
    path.Free();
}
