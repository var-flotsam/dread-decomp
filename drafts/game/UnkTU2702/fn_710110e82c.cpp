// Unit 2702 (hint CAIComponent): lists the owner with the game manager, turns it to face along the
// attack's kind (left or right, up or down, away from or towards its target) keeping its up axis, then
// sends the animation value 2 (hooking the callback fn_71004db7f8 to the tracked object when it is
// taken) and sets the timeline value. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "base/math/CVectorOps.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU54.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::math::CVector3D;

struct UnkTU2553Manager_le023e8;
struct UnkTU2553ListedObject;
void fn_7100e023e8(UnkTU2553Manager_le023e8* self, UnkTU2553ListedObject* obj);
void fn_71000769f0(UnkMatrix44* out, const void* a, const void* b, const void* c, const void* d);
void fn_71004db7f8();

extern const CVector3D g_unk_710164cf5c __attribute__((visibility("hidden")));  // the origin
extern const u64 g_unk_710164cf80;  // the timeline id

struct UnkMovement_l10e82c {
    u8 pad_0x0[0x1c08];
    u8 flags_0x1c08[2];  // byte flags; [1] is cleared here
};

struct UnkAttack_l10e82c {
    u8 pad_0x0[0x46c];
    s32 kind;  // 0x46c: 1 to 4 pick the facing
};

// The callback hooked to the tracked object.
struct UnkTrackLink_l10e82c {
    void* node;            // 0x0
    void* context;         // 0x8
    void (*callback)();    // 0x10
    UnkWeakHandle* handle;  // 0x18
};

struct UnkSource_l10e82c {
    u8 pad_0x0[0x14724];
    u32 field_0x14724;
};

struct UnkSelf_l10e82c {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x11a8];
    u8 field_0x11e0;
    u8 pad_0x11e1[0x3f3];
    u32 field_0x15d4;
    u8 pad_0x15d8[0x38];
    UnkAttack_l10e82c* attack;  // 0x1610
    u8 pad_0x1618[0x12e80];
    UnkTrackLink_l10e82c link;  // 0x14498
};
static_assert(__builtin_offsetof(UnkSelf_l10e82c, attack) == 0x1610, "UnkSelf_l10e82c layout");
static_assert(__builtin_offsetof(UnkSelf_l10e82c, link) == 0x14498, "UnkSelf_l10e82c layout");

static inline const UnkMatrix44& Matrix(UnkActor* actor) {
    return fn_710002614c(reinterpret_cast<const UnkMatrix44*>(actor->field_0x38));
}

// Column `i` of the actor's matrix: its axis i.
static inline void GetAxis(UnkActor* actor, int i, UnkVec3f* out) {
    const UnkMatrix44& m = Matrix(actor);
    *out = UnkVec3f(m.m[i], m.m[4 + i], m.m[8 + i]);
}

static inline CVector3D Position(const UnkActor* actor) {
    return CVector3D{actor->field_0x44, actor->field_0x54, actor->field_0x64};
}

static inline f32 Dot(const UnkVec3f& a, const UnkVec3f& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static inline UnkVec3f Cross(const UnkVec3f& a, const UnkVec3f& b) {
    return UnkVec3f(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

static inline UnkVec3f ToVec3f(const CVector3D& v) {
    return UnkVec3f(v.x, v.y, v.z);
}

void fn_710110e82c(UnkSelf_l10e82c* self, UnkSource_l10e82c* source) {
    fn_7100e023e8(reinterpret_cast<UnkTU2553Manager_le023e8*>(g_unk_7101d47310),
                  reinterpret_cast<UnkTU2553ListedObject*>(self->owner));
    if (UnkMovement_l10e82c* movement = static_cast<UnkMovement_l10e82c*>(self->owner->field_0x218))
        movement->flags_0x1c08[1] = 0;

    UnkVec3f up;
    GetAxis(self->owner, 1, &up);
    UnkVec3f forward;
    GetAxis(self->owner, 2, &forward);

    if (UnkActor* target = fn_7100d7c990(self->owner)) {
        // How far the up axis leans along x: past 0.75 the facing is chosen along y.
        f32 lean = __builtin_fabsf(Dot(up, UnkVec3f(1.0f, 0.0f, 0.0f)));
        switch (self->attack->kind) {
        case 1:
            if (lean > 0.75f)
                forward = UnkVec3f(0.0f, target->field_0x54 >= self->owner->field_0x54 ? 1.0f : -1.0f, 0.0f);
            else
                forward = UnkVec3f(target->field_0x44 >= self->owner->field_0x44 ? 1.0f : -1.0f, 0.0f, 0.0f);
            break;
        case 2:
            forward = ToVec3f(base::math::Normalized(Position(target) - Position(self->owner)));
            break;
        case 3:
            if (lean > 0.75f)
                forward = UnkVec3f(0.0f, target->field_0x54 >= self->owner->field_0x54 ? -1.0f : 1.0f, 0.0f);
            else
                forward = UnkVec3f(target->field_0x44 >= self->owner->field_0x44 ? -1.0f : 1.0f, 0.0f, 0.0f);
            break;
        case 4:
            forward = ToVec3f(base::math::Normalized(Position(self->owner) - Position(target)));
            break;
        }
    }

    UnkVec3f right = Cross(up, forward);
    forward = Cross(right, up);
    UnkMatrix44 m;
    fn_71000769f0(&m, &right, &up, &forward, &g_unk_710164cf5c);
    UnkActor* owner = self->owner;
    UnkVec3f angles = fn_7100076acc(reinterpret_cast<const UnkXform*>(&m));
    reinterpret_cast<void (*)(UnkActor*, const UnkVec3f*, bool)>(owner->vtable[0xd8 / 8])(owner, &angles, true);

    owner = self->owner;
    if (owner->field_0x220) {
        if (void* animation = owner->field_0x2e8) {
            base::reflection::UnkReflTypeIdArg value;
            value.id = 2;
            base::reflection::UnkReflTypeIdArg send_mask;
            send_mask.id = base::reflection::UnkReflTypeId<base::reflection::UnkReflTypeIdTag>::id();
            if (fn_7101176690(animation, 0x3951592af38cb532ull, 0xf4061ff25a2163d6ull, &value, &g_unk_710164b968,
                              &g_unk_7172769740, &send_mask)) {
                UnkTrackLink_l10e82c* link = &self->link;
                UnkWeakHandle* handle = link->handle;
                link->context = self;
                link->callback = fn_71004db7f8;
                UnkWeakTarget* track = static_cast<UnkWeakTarget*>(self->owner->field_0x220);
                if (handle) {
                    if (UnkWeakTarget* t = handle->target)
                        fn_710117b3f0(t, link);
                    if (UnkWeakHandle* p = link->handle) {
                        if (--p->ref_count == 0)
                            p->vf_0x10();
                        link->handle = nullptr;
                    }
                }
                if (track) {
                    CAbilityGhostAura_AssignWeakHandleToRef(track, &link->handle);
                    fn_710117b3e8(link->handle ? link->handle->target : nullptr, link);
                }
            }
            base::reflection::UnkReflTypeIdArg mask;
            mask.id = base::reflection::UnkReflTypeId<base::reflection::UnkReflTypeIdTag>::id();
            fn_7100a2cb7c(static_cast<UnkTimelineComponent*>(animation), &g_unk_710164cf80, &mask.id, false, -1.0f);
            self->field_0x11e0 = 0;
        }
    }
    if (source)
        self->field_0x15d4 = source->field_0x14724;
}
