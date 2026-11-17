// CEmmyLabAIComponent's unit (tools/tu_map.py unit 787): the update. After the base update it tells
// fn_71004f7570 whether the E.M.M.I. has moved off its marked place (or still is, for a moment), saves its
// position, and puts the two lab effects at its head and body. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "game/CEnemyMovement.h"
#include "game/CFXComponent.h"
#include "game/CGrabComponent.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkObject_0x100.h"
#include "game/UnkTU376.h"
#include "game/UnkXformNode.h"
#include "types.h"

// A navigation path: the cells it starts and ends in (-1 for none).
struct UnkEmmyLabPath_l4f710c {
    u8 pad_0x0[2];
    s16 from;  // 0x2
    u8 pad_0x4[0x1e - 0x4];
    s16 to;  // 0x1e
    u8 pad_0x20[0x4c - 0x20];
    bool blocked;  // 0x4c
    u8 pad_0x4d[0x100 - 0x4d];

    UnkEmmyLabPath_l4f710c() { fn_71002b57bc(Object()); }
    ~UnkEmmyLabPath_l4f710c() { fn_71002b6094(Object()); }
    UnkEmmyLabPath_l4f710c& operator=(const UnkEmmyLabPath_l4f710c& other) {
        fn_71002b5c90(Object(), other.Object());
        return *this;
    }
    UnkObject_0x100* Object() { return reinterpret_cast<UnkObject_0x100*>(this); }
    const UnkObject_0x100* Object() const { return reinterpret_cast<const UnkObject_0x100*>(this); }
};
static_assert(sizeof(UnkEmmyLabPath_l4f710c) == 0x100, "layout");

struct UnkEmmyLabMovement_l4f710c {
    u8 pad_0x0[0x1870];
    UnkEmmyLabPath_l4f710c path;  // 0x1870
    bool has_path;                // 0x1970
};

struct UnkEmmyLabOwner_l4f710c {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x218 - 0x94];
    UnkEmmyLabMovement_l4f710c* movement;  // 0x218
    u8 pad_0x220[0x8];
    UnkEntity* entity;  // 0x228
    u8 pad_0x230[0x260 - 0x230];
    void* grab;  // 0x260
    u8 pad_0x268[0x280 - 0x268];
    UnkWorld* world;  // 0x280
};
static_assert(__builtin_offsetof(UnkEmmyLabOwner_l4f710c, world) == 0x280, "layout");

// The lab effect: slot 0x10 places it, slot 0x18 turns it.
struct UnkEmmyLabFx_l4f710c {
    void** vtable;

    void SetPosition(const UnkVec3f* pos) {
        reinterpret_cast<void (*)(UnkEmmyLabFx_l4f710c*, const UnkVec3f*)>(vtable[0x10 / 8])(this, pos);
    }
    void SetAngle(const UnkVec3f* angle) {
        reinterpret_cast<void (*)(UnkEmmyLabFx_l4f710c*, const UnkVec3f*)>(vtable[0x18 / 8])(this, angle);
    }
};

// Where the E.M.M.I. was marked, and how long it has stood still since.
struct UnkEmmyLabMark_l4f710c {
    f32 x, y, z;
    bool marked;   // 0xc
    bool waiting;  // 0xd
    f32 timer;     // 0x10
};

// The lab state the update reads: two flags of the search and the mark.
struct UnkEmmyLabState_l4f710c {
    bool field_0x0;
    u8 pad_0x1[0x27d0 - 0x1];
    u32 field_0x27d0;
    u8 pad_0x27d4[0x2fe0 - 0x27d4];
    UnkEmmyLabMark_l4f710c mark;  // 0x2fe0
    bool visible;                 // 0x2ff4
};

struct UnkEmmyLabAI_l4f710c {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkEmmyLabOwner_l4f710c* owner;  // 0x30
    u8 pad_0x38[0x27720 - 0x38];
    UnkEmmyLabState_l4f710c state;  // 0x27720

    bool vf_0x2f8() { return reinterpret_cast<bool (*)(UnkEmmyLabAI_l4f710c*)>(vtable[0x2f8 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkEmmyLabAI_l4f710c, state.mark) == 0x2a700, "layout");

void fn_71005421d4(UnkEmmyLabAI_l4f710c* self, f32 dt);
void fn_71004f7570(UnkEmmyLabAI_l4f710c* self, bool flag);

// Whether the movement follows a path between two cells that is not blocked.
static inline bool OnOpenPath(UnkEmmyLabMovement_l4f710c* movement) {
    UnkEmmyLabPath_l4f710c path;
    if (movement->has_path)
        path = movement->path;
    if (!movement->has_path || path.from == -1 || path.to == -1)
        return false;
    return !path.blocked;
}

// Places an effect and turns it to the owner's angle, tilted back.
static inline void TurnFx(UnkEmmyLabAI_l4f710c* self, UnkEmmyLabFx_l4f710c* fx) {
    UnkVec3f angle = self->owner->xform.GetAngle();
    UnkVec3f tilted(angle.x + 0.0f, angle.y + -1.57f, angle.z + 0.0f);
    fx->SetAngle(&tilted);
}

// Whether the marked E.M.M.I. has left its mark (or is held on an open path), unless it is grabbed,
// searching or waiting for its movement.
static inline bool HasMoved(UnkEmmyLabAI_l4f710c* self) {
    UnkEmmyLabMark_l4f710c* mark = &self->state.mark;
    if (!mark->marked)
        return false;
    UnkEmmyLabOwner_l4f710c* owner = self->owner;
    f32 mx = self->state.mark.x;
    f32 px = owner->xform.m[0][3];
    f32 py = owner->xform.m[1][3];
    f32 pz = owner->xform.m[2][3];
    f32 my = mark->y;
    f32 mz = mark->z;
    if (fn_710113d83c(owner->grab))
        return false;
    if (!self->state.field_0x0 && self->state.field_0x27d0 == 1)
        return false;
    if (!mark->waiting && fn_71010cab0c(self->owner->movement))
        return false;
    f32 dx = px - mx;
    f32 dy = py - my;
    f32 dz = pz - mz;
    UnkEmmyLabMovement_l4f710c* movement = self->owner->movement;
    f32 dist2 = dx * dx + dy * dy + dz * dz;
    bool stuck = (fn_71010cc3a4(movement) == 4 && OnOpenPath(movement)) || fn_71010cab0c(self->owner->movement);
    return stuck | (dist2 > 0.01f);
}

void fn_71004f710c(UnkEmmyLabAI_l4f710c* self, f32 dt) {
    UnkEmmyLabMark_l4f710c* mark = &self->state.mark;
    fn_71005421d4(self, dt);
    bool busy = self->vf_0x2f8();
    self->state.visible = !busy;
    bool show;
    if (busy) {
        show = false;
    } else {
        bool moved = HasMoved(self);
        if (!moved && mark->waiting) {
            mark->timer += dt;
            show = mark->timer < 0.1f;
        } else {
            mark->timer = 0.0f;
            show = moved;
        }
    }
    fn_71004f7570(self, show);

    UnkEmmyLabOwner_l4f710c* owner = self->owner;
    f32 x = owner->xform.m[0][3];
    f32 y = owner->xform.m[1][3];
    f32 z = owner->xform.m[2][3];
    mark->x = x;
    mark->y = y;
    mark->z = z;
    if (!owner->world)
        return;

    UnkMatrix44 m = {};
    m.m[15] = 1.0f;
    m.m[10] = 1.0f;
    m.m[5] = 1.0f;
    m.m[0] = 1.0f;
    u64 key = 0x5207ae3cd258578cull;
    fn_71011735e8(owner->entity, &key, &m, 0);

    if (auto* fx = reinterpret_cast<UnkEmmyLabFx_l4f710c*>(fn_7101152b90(self->owner->world, 0x26537573178d0ddeull))) {
        {
            UnkVec3f pos(m.m[3] + m.m[0] * 120.0f, m.m[7] + m.m[4] * 120.0f, m.m[11] + m.m[8] * 120.0f);
            fx->SetPosition(&pos);
        }
        TurnFx(self, fx);
    }
    if (auto* fx = reinterpret_cast<UnkEmmyLabFx_l4f710c*>(fn_7101152b90(self->owner->world, 0xc0b0764986c8e573ull))) {
        {
            UnkVec3f pos(m.m[3], m.m[7], m.m[11]);
            fx->SetPosition(&pos);
        }
        TurnFx(self, fx);
    }
}
