// Translation unit 958 (tools/tu_map.py, no class name; the attack-offset registry): the smallest squared
// 2D distance (x and y of the transform translations at +0x44/+0x54/+0x64, projected by fn_710007b430) from
// `ref` to the other actors of the list at +0x40; 0 without `ref`, the float maximum with an empty list.
// As UnkUnit958_Closest_b67a4.cpp. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkNearestActor_A4_b71 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
};

struct UnkNearestEntry_A4_b71 {
    u8 pad_0x0[0x10];
    UnkNearestActor_A4_b71* actor;  // 0x10
};

struct UnkNearestNode_A4_b71 {
    UnkNearestEntry_A4_b71* entry;
    UnkNearestNode_A4_b71* next;
};

struct UnkNearestRegistry_A4_b71 {
    u8 pad_0x0[0x40];
    UnkNearestNode_A4_b71* list;  // 0x40
};

static inline __attribute__((always_inline)) void Pos2D(CVector2D* r, const UnkNearestActor_A4_b71* a) {
    f32 x = a->x, y = a->y, z = a->z;
    CVector3D v = {x, y, z};
    fn_710007b430(r, &v);
}

f32 fn_710063c10c(UnkNearestRegistry_A4_b71* self, const UnkNearestActor_A4_b71* ref) {
    if (!ref)
        return 0.0f;
    CVector2D p;
    Pos2D(&p, ref);
    f32 best = 3.4028235e38f;
    for (UnkNearestNode_A4_b71* n = self->list; n; n = n->next) {
        UnkNearestActor_A4_b71* a = n->entry ? n->entry->actor : nullptr;
        if (a == ref)
            continue;
        CVector2D q;
        Pos2D(&q, a);
        f32 dx = p.x - q.x;
        f32 dy = p.y - q.y;
        f32 d = dx * dx + dy * dy;
        if (d < best)
            best = d;
    }
    return best;
}
