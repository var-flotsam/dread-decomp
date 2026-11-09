// CGrappleBeamComponent unit (tools/tu_map.py, unit 1277): moves the owner (+0x30) to the hit's position (the
// translation column of the transform of +0x18 of the object at +0x530), or to the point the grappled owner's
// component at +0x228 gives for the singleton's id at +0xd58 (fn_710117364c), and marks its transform dirty
// (+0x3b0). Names and layouts are placeholders.
#include "base/reflection/UnkReflStatics.h"
#include "types.h"

struct UnkGrappleAnswer_b50;
void fn_710117364c(void* target, const u64* id, UnkGrappleAnswer_b50* out);

struct UnkGrappleVec3_ovb0047 {
    f32 x, y, z;
};

struct UnkGrappleIdHolder_ovb0047 {
    u8 pad_0x0[0x30];
    u64 id;
};

struct UnkGrappleXform_ovb0047 {
    u8 pad_0x0[0x38];
    f32 transform[3][4];  // 0x38

    UnkGrappleVec3_ovb0047 Position() const { return {transform[0][3], transform[1][3], transform[2][3]}; }
};

struct UnkGrappleHitInfo_ovb0047 {
    u8 pad_0x0[0x18];
    UnkGrappleXform_ovb0047* xform;
};

struct UnkGrappleActor_ovb0047 {
    u8 pad_0x0[0x38];
    f32 transform[3][4];  // 0x38
    u8 pad_0x68[0x3b0 - 0x68];
    bool transform_dirty;  // 0x3b0

    void SetPosition(const UnkGrappleVec3_ovb0047& p) {
        transform[0][3] = p.x;
        transform[1][3] = p.y;
        transform[2][3] = p.z;
        transform_dirty = true;
    }
};

struct UnkGrappleOwner2_ovb0047 {
    u8 pad_0x0[0x228];
    void* field_0x228;
};

struct UnkGrappleLink2_ovb0047 {
    u8 pad_0x0[0x10];
    UnkGrappleOwner2_ovb0047* owner;
};

struct UnkGrappleBeamPos_ovb0047 {
    u8 pad_0x0[0x30];
    UnkGrappleActor_ovb0047* actor;  // 0x30
    u8 pad_0x38[0x230 - 0x38];
    UnkGrappleLink2_ovb0047* link;  // 0x230
    u8 pad_0x238[0x530 - 0x238];
    UnkGrappleHitInfo_ovb0047* hit;  // 0x530
};

void fn_71008191e4(UnkGrappleBeamPos_ovb0047* self) {
    UnkGrappleVec3_ovb0047 pos = self->hit->xform->Position();
    if (void* target = self->link->owner->field_0x228) {
        u64 id = (*reinterpret_cast<UnkGrappleIdHolder_ovb0047**>(
                      reinterpret_cast<char*>(&base::reflection::refl_GetSingleton()) + 0xd58))->id;
        fn_710117364c(target, &id, reinterpret_cast<UnkGrappleAnswer_b50*>(&pos));
    }
    self->actor->SetPosition(pos);
}
