// CCharClassCallEmmyEvent unit (tools/tu_map.py): whether a position lies on the target's shape. The
// position (the translation column of the transform: +0x44, +0x54, +0x64) is projected to 2D
// (fn_710007b430); the chain from the second object (+0x29558, its +0x10, the target fn_71002a76a4
// gives for +0x5a8, its +0x10, the tracker fn_710028aca8 finds, its +0x30) leads to a shape that
// answers 0x10 to vtable slot 0x18, whose 0x48-byte entries are tested in turn (fn_7100116de0 with
// 0, 1 and a scale of 1). Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkBreakablePart;
struct UnkBreakableTarget;
struct UnkTracker;
struct UnkEntry;
UnkBreakableTarget** fn_71002a76a4(UnkBreakablePart* part);
UnkTracker* fn_710028aca8(void* link);
bool fn_7100116de0(UnkEntry* entry, void* arg, u32 a, u32 b, f32 scale);

struct UnkShapeEntry_cce {
    u8 data[0x48];
};

struct UnkShape_cce {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual s32 vf_0x18();

    u8 pad_0x8[0xa0];
    UnkShapeEntry_cce* entries;  // 0xa8
    u32 entry_count;             // 0xb0
};

struct UnkTracker_cce {
    u8 pad_0x0[0x30];
    UnkShape_cce* shape;
};

struct UnkLink_cce {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkRecord_cce {
    u8 pad_0x0[0x5a8];
    u8 field_0x5a8[8];
};

struct UnkRoot_cce {
    u8 pad_0x0[0x10];
    UnkRecord_cce* record;
};

struct UnkScenario_cce {
    u8 pad_0x0[0x29558];
    UnkRoot_cce* root;
};

struct UnkTransform_cce {
    u8 pad_0x0[0x44];
    f32 x;
    u8 pad_0x48[0xc];
    f32 y;
    u8 pad_0x58[0xc];
    f32 z;
};

bool fn_7100b9db84(void*, const UnkTransform_cce* transform, UnkScenario_cce* scenario) {
    f32 x = transform->x;
    f32 y = transform->y;
    f32 z = transform->z;
    CVector3D position;
    position.x = x;
    position.y = y;
    position.z = z;
    CVector2D point;
    fn_710007b430(&point, &position);
    UnkRoot_cce* root = scenario->root;
    if (!root || !root->record)
        return false;
    UnkLink_cce* link = reinterpret_cast<UnkLink_cce*>(
        *fn_71002a76a4(reinterpret_cast<UnkBreakablePart*>(root->record->field_0x5a8)));
    if (!link || !link->field_0x10)
        return false;
    UnkTracker_cce* tracker = reinterpret_cast<UnkTracker_cce*>(fn_710028aca8(link->field_0x10));
    if (!tracker)
        return false;
    UnkShape_cce* shape = tracker->shape;
    if (!shape || shape->vf_0x18() != 0x10 || shape->entry_count == 0)
        return false;
    for (UnkShapeEntry_cce* entry = shape->entries; entry != shape->entries + shape->entry_count; ++entry) {
        if (fn_7100116de0(reinterpret_cast<UnkEntry*>(entry), &point, 0, 1, 1.0f))
            return true;
    }
    return false;
}
