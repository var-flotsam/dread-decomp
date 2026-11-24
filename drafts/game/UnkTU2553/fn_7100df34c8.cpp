// CGameManager::CTunableGameManager's unit (tools/tu_map.py unit 2553): whether the tracked actor in use can
// take the player's place: unless the player's collider overlaps something, it probes for ground below the
// player's position (plus the movement offset) and asks the tracked actor's movement whether it fits there,
// trying once more with the other offset when asked. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkGameManager.h"
#include "game/UnkXformNode.h"
#include "game/logic/collision/UnkBvhTree.h"
#include "types.h"

struct UnkCharacterMovement;
struct UnkCollisionHit;
struct UnkMorphMovement;

__attribute__((visibility("hidden"))) extern base::global::CStrId g_unk_71727bee20;

struct UnkMorphVec3 {
    f32 x;
    f32 y;
    f32 z;

    UnkMorphVec3() {}
    UnkMorphVec3(const UnkMorphVec3& other) : x(other.x), y(other.y), z(other.z) {}
};

UnkMorphVec3 fn_7100943118(UnkMorphMovement* self, bool flag);
bool fn_7100d5bfbc(UnkTree* self, u32 mask, UnkBvhCollider* shape, void* arg, u32 arg2);
bool fn_7100977e50(UnkCharacterMovement* self, f32 max_dot, f32 extra, f32 step, f32 back, u32 mask,
                   const base::math::CVector3D* pos, base::math::CVector3D* out_point, UnkCollisionHit* out_hit,
                   const base::math::CVector3D* dir);

// What the tracked actor's movement hands out: its collision shape.
struct UnkProbeBody_ldf34c8 {
    u8 pad_0x0[0x158];
    UnkShape* shape;
};

// The tracked actor's movement (+0x200).
struct UnkProbeMovement_ldf34c8;
struct UnkProbeMovementVtbl_ldf34c8 {
    void* pad_0x0[0x33];
    // The ground below `from` within `distance`; `hit` says whether there is any.
    UnkVec3f (*vf_0x198)(UnkProbeMovement_ldf34c8* self, const UnkVec3f* from, f32 distance, bool* hit, void* arg);
    void* pad_0x1a0[0xc];
    // Whether the movement fits at `pos` (it may move it).
    bool (*vf_0x200)(UnkProbeMovement_ldf34c8* self, UnkVec3f* pos, void* arg, u32 mask, void* arg2, u32 kind);
    void* pad_0x208[0x14];
    UnkProbeBody_ldf34c8* (*vf_0x2a8)(UnkProbeMovement_ldf34c8* self);
};
struct UnkProbeMovement_ldf34c8 {
    const UnkProbeMovementVtbl_ldf34c8* vtbl;
    u8 pad_0x8[0x3c8];
    u32 mask;  // 0x3d0

    UnkVec3f Ground(const UnkVec3f& from, f32 distance, bool* hit, void* arg) {
        return vtbl->vf_0x198(this, &from, distance, hit, arg);
    }
    bool Fits(UnkVec3f* pos, void* arg, u32 mask_, void* arg2, u32 kind) {
        return vtbl->vf_0x200(this, pos, arg, mask_, arg2, kind);
    }
    UnkProbeBody_ldf34c8* Body() { return vtbl->vf_0x2a8(this); }
    // The bounds of the movement's shape where it stands.
    UnkAABB3 Bounds() {
        UnkShape* shape = Body()->shape;
        return shape->vf_0x28(shape->transform);
    }
};
static_assert(__builtin_offsetof(UnkProbeMovement_ldf34c8, mask) == 0x3d0, "layout");

// The player's movement (+0x210).
struct UnkPlayerMovement_ldf34c8;
struct UnkPlayerMovementVtbl_ldf34c8 {
    void* pad_0x0[0x55];
    UnkBvhCollider* (*vf_0x2a8)(UnkPlayerMovement_ldf34c8* self);
};
struct UnkPlayerMovement_ldf34c8 {
    const UnkPlayerMovementVtbl_ldf34c8* vtbl;
    u8 pad_0x8[0x3a0];
    bool field_0x3a8;  // on the ground
};
static_assert(__builtin_offsetof(UnkPlayerMovement_ldf34c8, field_0x3a8) == 0x3a8, "layout");

struct UnkProbeActor_ldf34c8 {
    u8 pad_0x0[0x44];
    f32 x;
    u8 pad_0x48[0xc];
    f32 y;
    u8 pad_0x58[0xc];
    f32 z;
    u8 pad_0x68[0x198];
    UnkProbeMovement_ldf34c8* field_0x200;
    u8 pad_0x208[0x8];
    UnkPlayerMovement_ldf34c8* field_0x210;
    u8 pad_0x218[0x88];
    UnkCollisionContainer* field_0x2a0;
    u8 pad_0x2a8[0xdc];
    f32 field_0x384;
    u8 pad_0x388[0x4];
    f32 field_0x38c;

    UnkVec3f Position() const { return UnkVec3f(x, y, z); }
};
static_assert(__builtin_offsetof(UnkProbeActor_ldf34c8, field_0x2a0) == 0x2a0, "layout");
static_assert(__builtin_offsetof(UnkProbeActor_ldf34c8, field_0x38c) == 0x38c, "layout");

struct UnkProbeHandle_ldf34c8 {
    u8 pad_0x0[0x10];
    UnkProbeActor_ldf34c8* target;
};

// The game manager's tracked actors.
struct UnkProbeManager_ldf34c8 {
    u8 pad_0x0[0x2998];
    UnkProbeHandle_ldf34c8* field_0x2998;
    UnkProbeHandle_ldf34c8* field_0x29a0;  // the player
    u8 pad_0x29a8[0x10];
    UnkProbeHandle_ldf34c8* field_0x29b8;  // the one in use
};
static_assert(__builtin_offsetof(UnkProbeManager_ldf34c8, field_0x29b8) == 0x29b8, "layout");

static inline UnkProbeActor_ldf34c8* TargetOf(UnkProbeHandle_ldf34c8* handle) {
    return handle ? handle->target : nullptr;
}

static inline UnkVec3f operator+(const UnkMorphVec3& a, const UnkVec3f& b) {
    return UnkVec3f(a.x + b.x, a.y + b.y, a.z + b.z);
}
static inline UnkVec3f operator+(const UnkVec3f& a, const UnkVec3f& b) {
    return UnkVec3f(a.x + b.x, a.y + b.y, a.z + b.z);
}


// Moves `pos` by a movement's offset, in place.
static inline void MoveBy(UnkVec3f& pos, const UnkMorphVec3& by) {
    pos.x = by.x + pos.x;
    pos.y = by.y + pos.y;
    pos.z = by.z + pos.z;
}

bool fn_7100df34c8(UnkGameManager* manager, void* a, void* b, void* c, void* d, bool retry, bool offset) {
    auto* self = reinterpret_cast<UnkProbeManager_ldf34c8*>(manager);
    UnkVec3f* out_position = static_cast<UnkVec3f*>(a);
    bool* out_on_ground = static_cast<bool*>(b);
    bool* out_snapped = static_cast<bool*>(c);
    bool* out_raised = static_cast<bool*>(d);

    if (TargetOf(self->field_0x29b8) == TargetOf(self->field_0x2998))
        return false;
    UnkProbeActor_ldf34c8* current = TargetOf(self->field_0x2998);
    UnkProbeActor_ldf34c8* player = self->field_0x29a0->target;
    UnkPlayerMovement_ldf34c8* player_move = player->field_0x210;
    UnkProbeMovement_ldf34c8* move = current->field_0x200;

    if (UnkBvhCollider* collider = player_move->vtbl->vf_0x2a8(player_move)) {
        UnkGameManagerSub_0x608* collision = g_unk_7101d47310->field_0x608;
        if (!collision)
            return false;
        UnkTree* tree = static_cast<UnkTree*>(collision->field_0x820);
        if (!tree)
            return false;
        if (fn_7100d5bfbc(tree, 0x100000, collider, nullptr, 0))
            return false;
    }

    UnkVec3f pos;
    pos = player->Position();
    bool grounded = player_move->field_0x3a8;
    if (grounded || offset)
        MoveBy(pos, fn_7100943118(reinterpret_cast<UnkMorphMovement*>(player_move), false));
    move->Body();
    if (!player_move->field_0x3a8)
        fn_71008640a4(current->field_0x2a0, &g_unk_71727bee20);

    bool hit;
    UnkVec3f ground = move->Ground(pos + UnkVec3f(0.0f, 5.0f, 0.0f), 5.0f - move->Bounds().min.y, &hit, nullptr);
    bool raised;
    bool snapped;
    bool on_ground;
    if (!hit) {
        raised = false;
        snapped = false;
        on_ground = false;
    } else if (grounded) {
        // Settle on the ground found, moving `pos` there.
        UnkCharacterMovement* body = reinterpret_cast<UnkCharacterMovement*>(move->Body());
        u32 mask = move->mask;
        UnkVec3f down(0.0f, -1.0f, 0.0f);
        snapped = fn_7100977e50(body, -0.3f, 20.0f, 3.4028235e38f, 0.0f, mask,
                                reinterpret_cast<const base::math::CVector3D*>(&pos),
                                reinterpret_cast<base::math::CVector3D*>(&ground), nullptr,
                                reinterpret_cast<const base::math::CVector3D*>(&down));
        raised = false;
        on_ground = true;
    } else {
        // In the air: keep the two actors' height difference.
        pos = player->Position();
        pos.y = (current->field_0x384 - player->field_0x384) + (current->field_0x38c - player->field_0x38c) + pos.y;
        snapped = false;
        raised = true;
        on_ground = false;
    }
    f32 limit = pos.y - 0.001f;
    bool fits = move->Fits(&pos, nullptr, move->mask, nullptr, 3);
    // Raised into the air but pushed below where it started: it does not fit.
    if (raised & fits & (limit > pos.y))
        fits = false;

    if (retry && !fits) {
        pos = player->Position();
        if (player_move->field_0x3a8)
            MoveBy(pos, fn_7100943118(reinterpret_cast<UnkMorphMovement*>(player_move), true));
        ground = move->Ground(pos + UnkVec3f(0.0f, 5.0f, 0.0f), current->field_0x38c + 5.0f, &hit, nullptr);
        if (hit) {
            UnkCharacterMovement* body = reinterpret_cast<UnkCharacterMovement*>(move->Body());
            u32 mask = move->mask;
            UnkVec3f down(0.0f, -1.0f, 0.0f);
            snapped = fn_7100977e50(body, -0.3f, 20.0f, 3.4028235e38f, 0.0f, mask,
                                    reinterpret_cast<const base::math::CVector3D*>(&pos),
                                    reinterpret_cast<base::math::CVector3D*>(&ground), nullptr,
                                    reinterpret_cast<const base::math::CVector3D*>(&down));
            on_ground = true;
        } else {
            on_ground = false;
            snapped = false;
        }
        fits = move->Fits(&pos, nullptr, move->mask, nullptr, 3);
        raised = false;
    }

    if (out_position)
        *out_position = pos;
    if (out_on_ground)
        *out_on_ground = on_ground;
    if (out_snapped)
        *out_snapped = snapped;
    if (out_raised)
        *out_raised = raised;
    return fits;
}
