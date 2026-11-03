// A movement component's floor height: from the owner's polygon collection (type 16) when it has
// the polygon the component refers to, plus the height of the owner's shape; else its own value.
#include "base/UnkMatrix44.h"
#include "types.h"

struct UnkShape;
struct UnkPool_0x48;
struct UnkStruct_0x48;
UnkStruct_0x48* fn_710012431c(UnkPool_0x48* pool, u32 handle);
UnkMatrix44 fn_7100d36a2c(UnkShape* shape);

struct UnkPolygon_c4 {
    u8 pad_0x0[0x28];
    f32 height;
};

struct UnkCollection_c4 {
    virtual void vf_0();
    virtual void vf_1();
    virtual void vf_2();
    virtual s32 GetType();
    u8 pad_0x8[0xa0];
};

struct UnkOwnerShape_c4 {
    u8 pad_0x0[0x158];
    UnkCollection_c4* collection;
};

struct UnkOwnerRef_c4 {
    u8 pad_0x0[0x10];
    UnkOwnerShape_c4* shape;
};

struct UnkHeightOut_c4 {
    u8 pad_0x0[0x8];
    f32 height;
};

struct UnkMovementComponent_c4 {
    u8 pad_0x0[0x100];
    f32 height;
    u8 pad_0x104[0x178 - 0x104];
    UnkOwnerRef_c4* owner;
    u8 pad_0x180[0x188 - 0x180];
    u32 polygon;
    u8 pad_0x18c[0x194 - 0x18c];
    f32 default_height;
};

void fn_71010d1ca4(UnkMovementComponent_c4* self, UnkHeightOut_c4* out) {
    if (!self->owner || !self->owner->shape)
        return;
    UnkCollection_c4* collection = self->owner->shape->collection;
    UnkCollection_c4* polygons = collection->GetType() == 16 ? collection : nullptr;
    if (polygons) {
        UnkPolygon_c4* polygon = reinterpret_cast<UnkPolygon_c4*>(
            fn_710012431c(reinterpret_cast<UnkPool_0x48*>(reinterpret_cast<u8*>(polygons) + 0xa8), self->polygon));
        if (polygon) {
            UnkMatrix44 m = fn_7100d36a2c(reinterpret_cast<UnkShape*>(self->owner ? self->owner->shape : nullptr));
            out->height = m.m[11] + polygon->height;
        }
    } else {
        out->height = self->default_height;
    }
    self->height = out->height;
}
