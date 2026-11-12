// CEmmyAIComponent's unit (tools/tu_map.py unit 811): fn_7100556d40. The shape (+0x158) of either
// the owner's collision object under the unit's id g_unk_71726e1f68 (when asked for and found) or
// what the owner's +0x218 component gives through slot 0x2a8; its segment (slot 0x28) is projected
// to 2D (fn_710007b430) and the first end's negated y is returned (0 without a shape). Names and
// layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkTU811.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector2D;
using base::math::CVector3D;

struct UnkEmmySegment_A3_b66 {
    CVector3D p0;
    CVector3D p1;
};

struct UnkEmmyShape_A3_b66 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual UnkEmmySegment_A3_b66 vf_0x28(void* arg);
    u8 field_0x8[0x8];
};

struct UnkEmmyShapeHolder_A3_b66 {
    u8 pad_0x0[0x158];
    UnkEmmyShape_A3_b66* shape;
};

struct UnkEmmyShapeSource_A3_b66 {
    void** vtable;
    UnkEmmyShapeHolder_A3_b66* vf_0x2a8() {
        return reinterpret_cast<UnkEmmyShapeHolder_A3_b66* (*)(UnkEmmyShapeSource_A3_b66*)>(vtable[0x2a8 / 8])(this);
    }
};

struct UnkEmmyOwner_A3_b66 {
    u8 pad_0x0[0x218];
    UnkEmmyShapeSource_A3_b66* field_0x218;
    u8 pad_0x220[0x80];
    UnkCollisionContainer* collision;  // +0x2a0
};

struct UnkEmmyHeightAI_A3_b66 {
    u8 pad_0x0[0x30];
    UnkEmmyOwner_A3_b66* owner;
};

struct UnkEmmyBox_A3_b66 {
    CVector2D a;
    CVector2D b;
};

__attribute__((always_inline)) static inline UnkEmmyBox_A3_b66 Project_A3_b66(UnkEmmyShape_A3_b66* shape) {
    UnkEmmyBox_A3_b66 box;
    UnkEmmySegment_A3_b66 seg = shape->vf_0x28(shape->field_0x8);
    fn_710007b430(&box.a, &seg.p0);
    fn_710007b430(&box.b, &seg.p1);
    return box;
}

f32 fn_7100556d40(UnkEmmyHeightAI_A3_b66* self, bool use_collision) {
    UnkCollisionObject* object;
    if (use_collision && self->owner->collision &&
        (object = fn_71008640a4(self->owner->collision, &g_unk_71726e1f68))) {
        UnkEmmyBox_A3_b66 box = Project_A3_b66(reinterpret_cast<UnkEmmyShapeHolder_A3_b66*>(object)->shape);
        return -box.a.y;
    }
    UnkEmmyShapeHolder_A3_b66* holder = self->owner->field_0x218->vf_0x2a8();
    if (!holder)
        return 0.0f;
    UnkEmmyBox_A3_b66 box = Project_A3_b66(holder->shape);
    return 0.0f - box.a.y;
}
