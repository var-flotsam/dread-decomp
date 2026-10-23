// An object with an UnkXform at +0x84 (include/game/UnkXformNode.h). fn_71001bc8a4 sets its
// rotation angles when the object is active (+0x81); fn_71001bc740 moves it, first turning it
// to face the new position when +0x80 is set (direction to angles by fn_710007a7f0, plus 1.57
// on x). Both mark it dirty (+0x82).
#include "game/UnkXformNode.h"

extern "C" f32 sqrtf(f32);

struct UnkXformHolder_0x84 {
    u8 pad_0x0[0x80];
    bool face_0x80;
    bool active_0x81;
    u8 dirty_0x82;
    u8 pad_0x83;
    UnkXform xform;
};

void fn_71001bc8a4(UnkXformHolder_0x84* self, const UnkVec3f* v) {
    if (self->active_0x81) {
        self->xform.SetAngle(*v);
        self->dirty_0x82 = 1;
    }
}

UnkVec3f fn_710007a7f0(UnkVec3f dir);  // direction to rotation angles; by value, passed by address (non-trivial copy)

void fn_71001bc740(UnkXformHolder_0x84* self, const UnkVec3f* v) {
    if (self->face_0x80) {
        UnkXform* x = &self->xform;
        UnkVec3f t = x->GetTranslation();
        f32 dx = v->x - t.x;
        f32 dy = v->y - t.y;
        f32 dz = v->z - t.z;
        f32 inv = 1.0f / (sqrtf(dx * dx + dy * dy + dz * dz) + 0.0001f);
        UnkVec3f angle = fn_710007a7f0(UnkVec3f(dx * inv, dy * inv, dz * inv));
        if (self->active_0x81) {
            x->SetAngle(UnkVec3f(angle.x + 1.57f, angle.y + 0.0f, angle.z + 0.0f));
            self->dirty_0x82 = 1;
        }
    }
    self->xform.SetTranslation(*v);
    self->dirty_0x82 = 1;
}
