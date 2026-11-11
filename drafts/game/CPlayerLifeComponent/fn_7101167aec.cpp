// DRAFT (trial 3, attempt 1 of 3 was the best: 97.1%, KIND operations, 5 differing instructions, same size).
// This text is that attempt. What still differs:
// 1. Timer test: the original keeps cset le / cset pl, tests `answer`, then orr (second, first) + cbz.
//    This form gives the inverted pair (cset hi / cset mi, and, tbnz). Tried and worse (28.9%, the test
//    splits into branches after the `answer` test): the two flags as locals joined by `|` into a third
//    local, and the two flags as locals joined by `||` inside the if. Untried: a form where the `ready`
//    value is not inverted, e.g. an inline helper returning the bool, or `if (answer && ready)` nested
//    around a separate `if (answer->vf_0x680(...))`.
// 2. Normalise: the original has x * scale, y * scale, scale * z; this form has scale * y. Likely cause:
//    `normal` starts its lifetime between the sqrtf call and the reloads of push. Building it through an
//    inline helper returning UnkVec3f (`UnkVec3f normal = Normalized(push);`) was written in attempts 2
//    and 3 but its effect was hidden behind the earlier difference, so it is unconfirmed.
// CPlayerLifeComponent (unit 2716): reacts to a hit. Picks the hit direction, lets the attacker's
// component answer the hit, passes the resulting push to the player's movement and, for hit kinds
// 4 and 5, sends the game manager an event with the push's direction.
// Names and layouts are placeholders.
#include <math.h>

#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkHit;

// The player's movement (owner +0x200): takes the push.
struct UnkHitMovement {
    void** vtable;

    void vf_0x390(UnkVec3f* push, UnkHit* hit) {
        reinterpret_cast<void (*)(UnkHitMovement*, UnkVec3f*, UnkHit*)>(vtable[0x390 / 8])(this, push, hit);
    }
};

// Owner +0x208: says whether the owner's own angles give the hit direction.
struct UnkHitFacing {
    void** vtable;

    bool vf_0x3b8() { return reinterpret_cast<bool (*)(UnkHitFacing*)>(vtable[0x3b8 / 8])(this); }
};

// The attacker's component that answers a hit.
struct UnkHitAnswer {
    void** vtable;

    bool vf_0x680(UnkHit* hit, base::global::CFilePathStrId* path) {
        return reinterpret_cast<bool (*)(UnkHitAnswer*, UnkHit*, base::global::CFilePathStrId*)>(
            vtable[0x680 / 8])(this, hit, path);
    }
};

struct UnkHitOwner {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x16c];
    UnkHitMovement* field_0x200;
    UnkHitFacing* field_0x208;
};
static_assert(__builtin_offsetof(UnkHitOwner, field_0x200) == 0x200, "UnkHitOwner layout");

struct UnkHitAttacker {
    u8 pad_0x0[0x240];
    UnkHitAnswer* field_0x240;
};

struct UnkHitSource {
    u8 pad_0x0[0x10];
    UnkHitAttacker* field_0x10;
};

struct UnkHit {
    u8 pad_0x0[8];
    UnkHitSource* field_0x8;
    u8 pad_0x10[8];
    bool field_0x18;
    u8 pad_0x19[3];
    s32 field_0x1c;
    u8 pad_0x20[2];
    bool field_0x22;
    u8 pad_0x23[0xa5];
    u32 field_0xc8;
    u8 pad_0xcc[8];
    bool field_0xd4;
};
static_assert(__builtin_offsetof(UnkHit, field_0xc8) == 0xc8, "UnkHit layout");

struct CPlayerLifeComponent {
    u8 pad_0x0[0x30];
    UnkHitOwner* field_0x30;
    u8 pad_0x38[0x428];
    f32 field_0x460;  // time since the attacker last answered a hit
};
static_assert(__builtin_offsetof(CPlayerLifeComponent, field_0x460) == 0x460, "CPlayerLifeComponent layout");

struct UnkVec;

UnkVec3f fn_7101158fa0(CPlayerLifeComponent* self, UnkHit* hit);
UnkVec3f fn_71010d8590(UnkHitMovement* movement, UnkVec3f* direction, UnkHit* hit);
void fn_7101100d0c(UnkHitAnswer* answer, UnkHit* hit, base::global::CFilePathStrId* path);
void fn_7100e038b0(UnkGameManager* dispatcher, void* key, UnkVec* vec, u64 arg);

extern char g_unk_7172a0a198[] __attribute__((visibility("hidden")));  // the event's key

void fn_7101167aec(CPlayerLifeComponent* self, UnkHit* hit) {
    UnkHitOwner* owner = self->field_0x30;
    UnkHitMovement* movement = owner->field_0x200;
    if (!movement)
        return;

    UnkVec3f direction = owner->xform.GetAngle();
    UnkHitFacing* facing = self->field_0x30->field_0x208;
    if (!facing || !facing->vf_0x3b8())
        direction = fn_7101158fa0(self, hit);

    if (hit->field_0x8 && hit->field_0x8->field_0x10) {
        UnkHitAnswer* answer = hit->field_0x8->field_0x10->field_0x240;
        base::global::CFilePathStrId path(g_unk_7101d470e8);
        if (hit->field_0xd4 && (hit->field_0x1c != 2 || hit->field_0x18)) {
            f32 time = self->field_0x460;
            bool ready = time <= -0.001f || !(time < 0.2f);
            if (answer && ready && answer->vf_0x680(hit, &path)) {
                fn_7101100d0c(answer, hit, &path);
                self->field_0x460 = 0.0f;
            }
        }
    }

    UnkVec3f push = fn_71010d8590(movement, &direction, hit);
    movement->vf_0x390(&push, hit);

    if (!hit->field_0x22 && (hit->field_0xc8 & ~1u) == 4) {
        UnkGameManager* manager = g_unk_7101d47310;
        f32 scale = 1.0f / (sqrtf(push.x * push.x + push.y * push.y + push.z * push.z) + 0.0001f);
        UnkVec3f normal(push.x * scale, push.y * scale, push.z * scale);
        fn_7100e038b0(manager, g_unk_7172a0a198, reinterpret_cast<UnkVec*>(&normal), 0);
    }
}
