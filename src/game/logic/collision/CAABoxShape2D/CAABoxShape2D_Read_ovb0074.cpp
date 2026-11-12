// game::logic::collision::CAABoxShape2D's unit (tools/tu_map.py): the box read from a file. After the
// shape's transform (fn_7100d6c288), its width and height (both zero once the file is exhausted)
// give the corners at +0xa8 and +0xb0, centred on the origin; then the shape updates itself (slot
// 0x80). Names and layouts are placeholders.
#include "base/global/CRntFile.h"
#include "game/logic/collision/CShape.h"
#include "types.h"

using base::global::CRntFile;

struct UnkPolygonCollectionShape;

struct UnkAABoxShape2D_ovb0074 {
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
    f32 min_x;  // 0xa8
    f32 min_y;  // 0xac
    f32 max_x;  // 0xb0
    f32 max_y;  // 0xb4
};

bool fn_7100d5e2f0(UnkAABoxShape2D_ovb0074* self, CRntFile* file) {
    fn_7100d6c288(reinterpret_cast<UnkPolygonCollectionShape*>(self), file);
    f32 width = 0.0f;
    f32 height = 0.0f;
    if (file->position < file->size) {
        fn_710007c714(file, &width, 4, 1);
        fn_710007c714(file, &height, 4, 1);
    }
    f32 min_x = width * -0.5f;
    f32 min_y = height * -0.5f;
    f32 max_x = width * 0.5f;
    f32 max_y = height * 0.5f;
    self->min_x = min_x;
    self->min_y = min_y;
    self->max_x = max_x;
    self->max_y = max_y;
    self->vf_0x80();
    return true;
}
