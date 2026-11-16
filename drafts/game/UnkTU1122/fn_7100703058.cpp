// Unit 1122: whether the actor's path end it faces lies within 2 units (squared distance up to 4) of a
// stored point; true when the path is empty. Names and layouts are placeholders.
#include <new>

#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "game/CEnemyMovement.h"
#include "types.h"

using base::math::CVector2D;

struct UnkPathVertex_l703058 {
    CVector2D pos;
    f32 field_0x8;
};

struct UnkPathOwner_l703058 {
    u8 pad_0x0[0x68];
    UnkPathVertex_l703058* vertices;  // 0x68
    u32 count;                        // 0x70
    u8 pad_0x74[0x20];
    u8 closed;  // 0x94

    u32 EdgeCount() const { return count - (!closed & (count != 0)); }
    void Edge(UnkSegment2D* seg, u32 i) const {
        new (seg) UnkSegment2D(vertices[i].pos, (count - 1 == i ? vertices : &vertices[i + 1])->pos);
    }
};

struct UnkMovement_l703058 {
    u8 pad_0x0[0x8e8 + 0x8c];
    CVector2D point;  // 0x974
};

struct UnkPathActor_l703058 {
    u8 pad_0x0[0x218];
    UnkMovement_l703058* field_0x218;
    u8 pad_0x220[0x20];
    UnkPathOwner_l703058* field_0x240;
};

struct UnkPathUser_l703058 {
    u8 pad_0x0[0x30];
    UnkPathActor_l703058* owner;  // 0x30
    u8 pad_0x38[0xb0];
    u8 field_0xe8;
    u8 field_0xe9;
};

bool fn_7100703058(UnkPathUser_l703058* self) {
    bool backwards = self->field_0xe9 ? self->field_0xe8 : false;
    UnkMovement_l703058* movement = self->owner->field_0x218;
    u32 mode = fn_71010cc3a4(movement);
    UnkPathOwner_l703058* path = self->owner->field_0x240;
    u32 edges = path->EdgeCount();
    if (edges == 0)
        return true;
    const CVector2D* target = mode == 1 ? &movement->point : nullptr;
    UnkSegment2D seg;
    f32 dx;
    f32 dy;
    if (backwards) {
        path->Edge(&seg, edges - 1);
        dx = target->x - seg.end.x;
        dy = target->y - seg.end.y;
    } else {
        path->Edge(&seg, 0);
        dx = target->x - seg.start.x;
        dy = target->y - seg.start.y;
    }
    return !(dx * dx + dy * dy > 4.0f);
}
