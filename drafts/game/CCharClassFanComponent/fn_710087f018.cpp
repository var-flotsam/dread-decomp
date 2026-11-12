// CCharClassFanComponent: reads the node's z angle, rotates a reference vector by it and passes the
// scaled half-vector's projection on. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "game/UnkXformNode.h"

struct UnkMat4f;
UnkVec3f fn_7100078170(const UnkMat4f* m, const UnkVec3f* v);
void fn_7100a48fe0(void* self, f32 angle, f32 a, f32 b, const base::math::CVector2D* dir);
extern const UnkVec3f g_unk_710164ae30 __attribute__((visibility("hidden")));

struct UnkFanNode {
    u8 pad_0x0[0x38];
    UnkXform xform_0x38;
};

struct UnkFanComponent {
    u8 pad_0x0[0x30];
    UnkFanNode* node_0x30;
    u8 pad_0x38[0xac0 - 0x38];
    f32 field_0xac0;
    u8 pad_0xac4[0xacc - 0xac4];
    f32 field_0xacc;
    UnkVec3f field_0xad0;
};
static_assert(__builtin_offsetof(UnkFanComponent, field_0xad0) == 0xad0, "layout");
static_assert(__builtin_offsetof(UnkFanNode, xform_0x38.flags) == 0x90, "layout");

void fn_710087f018(UnkFanComponent* self) {
    UnkFanNode* node = self->node_0x30;
    if (!node)
        return;
    f32 angle = node->xform_0x38.GetAngle().z;
    UnkMatrix44 rot = fn_7100078d8c(angle);
    base::math::CVector2D proj;
    {
        UnkVec3f half;
        UnkVec3f dir = fn_7100078170(reinterpret_cast<const UnkMat4f*>(&rot), &g_unk_710164ae30);
        f32 s = self->field_0xac0;
        f32 x = s * dir.x;
        f32 y = s * dir.y;
        f32 z = s * dir.z;
        half.x = x * 0.5f;
        half.y = y * 0.5f;
        half.z = z * 0.5f;
        fn_710007b430(&proj, reinterpret_cast<const base::math::CVector3D*>(&half));
    }
    UnkVec3f moved = fn_7100078170(reinterpret_cast<const UnkMat4f*>(&rot), &g_unk_710164ae30);
    self->field_0xad0 = moved;
    fn_7100a48fe0(self, angle, self->field_0xac0, self->field_0xacc, &proj);
}
