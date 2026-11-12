// game::logic::collision::CCapsuleShape2D's unit (tools/tu_map.py): the capsule read from a file. The
// capsule's centre is at +0xa8, its half length at +0xb0 and its radius at +0xb4. Names and layouts are
// placeholders.
#include "base/global/CRntFile.h"
#include "game/logic/collision/CShape.h"
#include "types.h"

using base::global::CRntFile;

struct UnkPolygonCollectionShape;

struct UnkCapsuleShape2D_ovb0015 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    u8 pad_0x8[0xa0];
    f32 x;            // 0xa8
    f32 y;            // 0xac
    f32 half_length;  // 0xb0
    f32 radius;       // 0xb4
};

// Reads the shape's transform (fn_7100d6c288, declared as src/game/logic/collision/
// CPolygonCollectionShape/CPolygonCollectionShape_Read.cpp has it), then the radius and the full
// length (both zero once the file is exhausted); the centre goes back to the origin and the shape
// updates itself (slot 0x80). The radius is stored as read, without a float type.
bool fn_7100d60094(UnkCapsuleShape2D_ovb0015* self, CRntFile* file) {
    fn_7100d6c288(reinterpret_cast<UnkPolygonCollectionShape*>(self), file);
    f32 radius = 0.0f;
    f32 length = 0.0f;
    if (file->position < file->size) {
        fn_710007c714(file, &radius, 4, 1);
        fn_710007c714(file, &length, 4, 1);
    }
    __builtin_memcpy(&self->radius, &radius, sizeof(radius));
    self->x = 0.0f;
    self->y = 0.0f;
    self->half_length = length * 0.5f;
    self->vf_0x80();
    return true;
}
