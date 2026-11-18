// Unit 2517 of tools/tu_map.py (a 2D segment collision shape): a clone: a new 0xc0-byte shape (operator new,
// fn_7100083b60) built by the shape base constructor from the source's transform (fn_7100d6c034, from +0x8),
// the class's vtable, an empty segment (+0xa8, its length unset) assigned the source's (fn_7100125b44) and a
// cleared flag (+0xbc). Names and layouts are placeholders.
#include <new>

#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "game/logic/collision/CShape.h"
#include "game/malloc.h"
#include "types.h"

using base::math::CVector2D;

extern char g_unk_7101aa8ce0[];  // the class's vtable

// The segment as the shape keeps it: its two points and the cached length (-1 until computed).
struct UnkShapeSegment_ld69508 {
    CVector2D start;
    CVector2D end;
    f32 length;

    UnkShapeSegment_ld69508() : start{0.0f, 0.0f}, end{0.0f, 0.0f}, length(-1.0f) {}
    UnkShapeSegment_ld69508& operator=(const UnkShapeSegment_ld69508& other) {
        fn_7100125b44(reinterpret_cast<UnkSegment2D*>(this), reinterpret_cast<const UnkSegment2D*>(&other));
        return *this;
    }
};

// The shape base: its copy constructor is fn_7100d6c034 (from the source's transform).
struct UnkShapeBase_ld69508 {
    void* vtable;
    u8 transform[0xa0];  // 0x8

    explicit UnkShapeBase_ld69508(const u8* source_transform) {
        fn_7100d6c034(reinterpret_cast<UnkShape2D_b46a3*>(this), source_transform);
    }
};

struct UnkSegmentShape_ld69508 : UnkShapeBase_ld69508 {
    UnkShapeSegment_ld69508 segment;  // 0xa8
    bool field_0xbc;

    UnkSegmentShape_ld69508(const u8* source_transform, const UnkShapeSegment_ld69508& source_segment)
        : UnkShapeBase_ld69508(source_transform) {
        vtable = g_unk_7101aa8ce0 + 0x10;
        segment = source_segment;
        field_0xbc = false;
    }
};
static_assert(__builtin_offsetof(UnkSegmentShape_ld69508, field_0xbc) == 0xbc, "layout");

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
UnkSegmentShape_ld69508* fn_7100d69508(const UnkSegmentShape_ld69508* other) {
    auto* self = static_cast<UnkSegmentShape_ld69508*>(__builtin_operator_new(0xc0));
    new (self) UnkSegmentShape_ld69508(other->transform, other->segment);
    return self;
}
