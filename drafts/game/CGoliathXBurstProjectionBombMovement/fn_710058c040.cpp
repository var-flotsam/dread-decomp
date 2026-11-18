// CGoliathXBurstProjectionBombMovement's unit (tools/tu_map.py unit 840): where the bomb is placed on an
// actor: finds the surface normal under it (a cast along the actor's motion, the actor's own contact, or two
// cross-shaped casts the bomb's size wide), lets the base movement place it, pulls it back along the normal and
// turns it to stand on that surface. Names and layouts are placeholders.
#include <math.h>

#include "game/UnkGameManager.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

// What a segment cast through the collision tracker reports.
struct UnkSegmentHit_l58c040 {
    UnkRefObject* collider;
    UnkRefObject* shape;
    s32 id;
    UnkVec3f point;
    UnkVec3f field_0x20;
    UnkVec3f normal;  // 0x2c
    u32 field_0x38;
    u32 flags;  // 0x3c: bit 4, the normal is set
    u32 field_0x40;

    UnkSegmentHit_l58c040()
        : collider(nullptr), shape(nullptr), id(-1), point(0.0f, 0.0f, 0.0f), field_0x20(0.0f, 0.0f, 0.0f),
          normal(0.0f, 0.0f, 0.0f), field_0x38(0), flags(0), field_0x40(0) {}
    ~UnkSegmentHit_l58c040() {
        if (UnkRefObject* s = shape) {
            if (--s->ref_count == 0)
                s->vf_0x10();
        }
        if (UnkRefObject* c = collider) {
            if (--c->ref_count == 0)
                c->vf_0x10();
        }
    }
};

bool fn_7100d5b268(void* tracker, u32 mask, const UnkVec3f* from, const UnkVec3f* to, UnkSegmentHit_l58c040* hit,
                   bool a, bool b);

struct UnkBombMovement_ovb0078;
struct UnkBombActor_ovb0078;
void fn_7100aabf4c(UnkBombMovement_ovb0078* self, UnkBombActor_ovb0078* actor, UnkVec3f* pos,
                   UnkVec3f* offset);

struct UnkContactSource_l58c040 {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

// The actor's contact with the ground.
struct UnkContact_l58c040 {
    u8 pad_0x0[0xf9];
    bool field_0xf9;
    u8 pad_0xfa[0x2e];
    UnkContactSource_l58c040* source;  // 0x128
    u8 pad_0x130[0x24];
    UnkVec3f normal;  // 0x154
};

// The actor's motion: the direction it moves in, in the plane.
struct UnkMotion_l58c040 {
    u8 pad_0x0[0x9ec];
    f32 dir_x;
    f32 dir_y;
};

struct UnkBombActor_l58c040 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x170];
    UnkMotion_l58c040* motion;  // 0x1d8
    u8 pad_0x1e0[0x30];
    UnkContact_l58c040* contact;  // 0x210
    u8 pad_0x218[0x164];
    f32 radius;  // 0x37c
};

struct UnkGoliathBombMovement_l58c040 {
    u8 pad_0x0[0xd88];
    f32 size;  // 0xd88
    u8 pad_0xd8c[0x2];
    bool pull_back;  // 0xd8e
    bool align;      // 0xd8f
};

static inline f32 Sign(f32 v) { return (v > 0.0f) - (v < 0.0f); }

void fn_710058c040(UnkGoliathBombMovement_l58c040* self, UnkBombActor_l58c040* actor, UnkVec3f* pos,
                   UnkVec3f* rotation) {
    *pos = UnkVec3f(actor->x, actor->y, actor->z);
    UnkContact_l58c040* contact = actor ? actor->contact : nullptr;
    UnkGameManagerSub_0x608* sub = g_unk_7101d47310->field_0x608;
    void* tracker = sub ? sub->field_0x820 : nullptr;
    f32 size = self->size;
    UnkMotion_l58c040* motion = actor ? actor->motion : nullptr;

    UnkSegmentHit_l58c040 hit;
    const UnkVec3f* normal = nullptr;
    if (motion && (motion->dir_x != 0.0f || motion->dir_y != 0.0f)) {
        UnkVec3f to(size * motion->dir_x + pos->x, size * motion->dir_y + pos->y, size * 0.0f + pos->z);
        if (fn_7100d5b268(tracker, 9, pos, &to, &hit, true, true) && (hit.flags & 0x10))
            normal = &hit.normal;
    }
    if (!normal) {
        if (contact && !contact->field_0xf9 && contact->source && contact->source->field_0x10) {
            normal = &contact->normal;
        } else {
            static const UnkVec3f across(size, 0.0f, 0.0f);
            static const UnkVec3f down(0.0f, size, 0.0f);
            UnkVec3f from(pos->x - across.x, pos->y - across.y, pos->z - across.z);
            UnkVec3f to(pos->x + across.x, pos->y + across.y, pos->z + across.z);
            bool found = fn_7100d5b268(tracker, 9, &from, &to, &hit, true, true);
            if (!found) {
                from = UnkVec3f(pos->x - down.x, pos->y - down.y, pos->z - down.z);
                to = UnkVec3f(pos->x + down.x, pos->y + down.y, pos->z + down.z);
                found = fn_7100d5b268(tracker, 9, &from, &to, &hit, true, true);
            }
            if (found && (hit.flags & 0x10))
                normal = &hit.normal;
        }
    }

    f32 nx, ny, nz;
    if (normal) {
        nx = normal->x;
        ny = normal->y;
        nz = normal->z;
        fn_7100aabf4c(reinterpret_cast<UnkBombMovement_ovb0078*>(self), reinterpret_cast<UnkBombActor_ovb0078*>(actor),
                      pos, rotation);
        if (self->pull_back) {
            f32 distance = actor->radius * 0.33f;
            pos->x -= nx * distance;
            pos->y -= ny * distance;
            pos->z -= nz * distance;
        }
    } else {
        fn_7100aabf4c(reinterpret_cast<UnkBombMovement_ovb0078*>(self), reinterpret_cast<UnkBombActor_ovb0078*>(actor),
                      pos, rotation);
        nx = 0.0f;
        ny = 1.0f;
        nz = 0.0f;
    }

    if (self->align) {
        f32 inv = 1.0f / (sqrtf(nx * nx + ny * ny + nz * nz) + 0.0001f);
        f32 cosine = (nx * inv) * 0.0f + (ny * inv) * 0.9999f + (nz * inv) * 0.0f;
        cosine = !(cosine >= 1.0f) ? cosine : 1.0f;
        cosine = !(cosine < -1.0f) ? cosine : -1.0f;
        f32 angle = acosf(cosine);
        *rotation = UnkVec3f(0.0f, 0.0f, -(angle * Sign(nx)));
    } else {
        *rotation = UnkVec3f(0.0f, 0.0f, 0.0f);
    }
}
