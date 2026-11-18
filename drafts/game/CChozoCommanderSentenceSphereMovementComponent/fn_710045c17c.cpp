// CChozoCommanderSentenceSphereMovementComponent's unit: a per-frame update (fn_710045c17c). While held
// (state 0) the owner sits on the holder's point plus an offset; once the owner's parts are ready it sends its
// timeline an id and spawns its effect, once; and its height is eased back to zero. Names and layouts are
// placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CRntVector.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CAutclastChargeAttack.h"
#include "game/CCollisionComponent.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkModelManager.h"
#include "game/UnkMovementComponent.h"
#include "game/UnkSwitchObject.h"
#include "game/UnkTU134.h"
#include "game/UnkTU2739.h"
#include "game/UnkTU54.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkXformNode.h"
#include "../../game_s2/UnkSpitSend.h"
#include "types.h"

struct UnkMatrix44Words {
    u32 m[16];

    UnkMatrix44Words() {}
    UnkMatrix44Words(const UnkMatrix44Words& other);
};

UnkMatrix44Words fn_7100076dc4(const UnkMatrix44Words* src);

extern const u64 g_unk_7101649750;
extern const u64 g_unk_7101649738;
__attribute__((visibility("hidden"))) extern const UnkVec3f g_unk_7101649740;
__attribute__((visibility("hidden"))) extern const u64 g_unk_71726ca0f8;

// The effect object the model manager hands out, as its virtual calls see it (as in
// src/game/CMeleeComponent/CMeleeComponent_Checks_ovb0041.cpp).
struct UnkFxObject_ovb0041;
struct UnkFxObjectVtable_ovb0041 {
    void* unk_0x0[2];
    void (*set_position)(UnkFxObject_ovb0041* self, const UnkVec3f* v);  // 0x10
    void (*set_rotation)(UnkFxObject_ovb0041* self, const UnkVec3f* v);  // 0x18
    void (*set_scale)(UnkFxObject_ovb0041* self, const UnkVec3f* v);     // 0x20
    void* unk_0x28[13];
    void (*set_loop)(UnkFxObject_ovb0041* self, bool loop);               // 0x90
    void (*set_speed)(UnkFxObject_ovb0041* self, f32 speed);              // 0x98
};
struct UnkFxObject_ovb0041 {
    UnkFxObjectVtable_ovb0041* vtable;
};

struct UnkSphereOwnerState_l45c17c {
    u8 pad_0x0[0x70];
    s32 mode;  // 0x70
};

struct UnkSphereOwner_l45c17c {
    void** vtable;
    u8 pad_0x8[0x30];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x124];
    UnkSphereOwnerState_l45c17c* state;  // 0x1b8
    u8 pad_0x1c0[0xc0];
    void* field_0x280;  // 0x280
    u8 pad_0x288[0x18];
    UnkStruct_7100861148* collision;  // 0x2a0
    u8 pad_0x2a8[0x40];
    UnkTimelineComponent* timeline;  // 0x2e8
    u8 pad_0x2f0[0xc0];
    bool moved;  // 0x3b0

    void vf_0xd0(const UnkVec3f* position, bool teleport) {
        reinterpret_cast<void (*)(UnkSphereOwner_l45c17c*, const UnkVec3f*, bool)>(vtable[0xd0 / 8])(
            this, position, teleport);
    }
};
static_assert(__builtin_offsetof(UnkSphereOwner_l45c17c, state) == 0x1b8, "layout");
static_assert(__builtin_offsetof(UnkSphereOwner_l45c17c, collision) == 0x2a0, "layout");
static_assert(__builtin_offsetof(UnkSphereOwner_l45c17c, timeline) == 0x2e8, "layout");
static_assert(__builtin_offsetof(UnkSphereOwner_l45c17c, moved) == 0x3b0, "layout");

struct UnkSphereAnchor_l45c17c {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x194];
    UnkEntity* model;  // 0x228
};
static_assert(__builtin_offsetof(UnkSphereAnchor_l45c17c, model) == 0x228, "layout");

struct UnkSphereHolder_l45c17c {
    u8 pad_0x0[0x10];
    UnkSphereAnchor_l45c17c* anchor;  // 0x10
};

struct UnkSentenceSphere_l45c17c {
    u8 pad_0x0[0x30];
    UnkSphereOwner_l45c17c* owner;  // 0x30
    u8 pad_0x38[0x1f8];
    UnkSphereHolder_l45c17c* holder;  // 0x230
    u8 pad_0x238[0x3a0];
    s32 state;       // 0x5d8
    f32 size;        // 0x5dc
    f32 base_size;   // 0x5e0
    u8 pad_0x5e4[4];
    bool spawned;  // 0x5e8
};
static_assert(__builtin_offsetof(UnkSentenceSphere_l45c17c, state) == 0x5d8, "layout");
static_assert(__builtin_offsetof(UnkSentenceSphere_l45c17c, spawned) == 0x5e8, "layout");

static inline UnkModelManager* ModelManager() {
    if (!g_unk_71726b0cd8) {
        UnkModelManager* m = static_cast<UnkModelManager*>(fn_71000839a0(0x1e8, 0x3f));
        fn_71011a870c(m);
        g_unk_71726b0cd8 = m;
    }
    return g_unk_71726b0cd8;
}

// Spawns the effect at `path` with the given placement, looping, played backwards.
static inline void SpawnEffect(UnkModelManager* manager, const void* path, UnkVec3f position, UnkVec3f rotation,
                               UnkVec3f scale) {
    UnkFxObject_ovb0041* fx = fn_71003b7db0(manager, path, false, true);
    if (!fx)
        return;
    fx->vtable->set_position(fx, &position);
    fx->vtable->set_rotation(fx, &rotation);
    fx->vtable->set_scale(fx, &scale);
    fn_7100146dac(reinterpret_cast<UnkSwitchObject*>(fx), true);
    fx->vtable->set_loop(fx, true);
    fx->vtable->set_speed(fx, -1.0f);
    fn_71011a9bb4(manager, fx);
}

// The address of a temporary, for the callees that take their matrix by pointer.
template <class T>
static inline const T* AddressOf(const T& value) {
    return &value;
}

void fn_710045c17c(UnkSentenceSphere_l45c17c* self, f32 dt) {
    fn_71010d1a90(reinterpret_cast<UnkMovementComponent*>(self), dt);

    if (self->state == 0) {
        UnkVec3f position = self->holder->anchor->xform.GetTranslation();
        UnkMatrix44 m = {};
        m.m[15] = 1.0f;
        m.m[10] = 1.0f;
        m.m[5] = 1.0f;
        m.m[0] = 1.0f;
        if (fn_71011735e8(self->holder->anchor->model, &g_unk_7101649738, &m, 0)) {
            position = UnkVec3f(m.m[3], m.m[7], m.m[11]);
            UnkVec3f offset = fn_7100078170(
                reinterpret_cast<const UnkMat4f*>(AddressOf(fn_7100076dc4(reinterpret_cast<const UnkMatrix44Words*>(&m)))),
                &g_unk_7101649740);
            position.x = offset.x + position.x;
            position.y = offset.y + position.y;
            position.z = offset.z + position.z;
        }
        self->owner->vf_0xd0(&position, true);
    }

    if (!self->spawned) {
        UnkSphereOwner_l45c17c* owner = self->owner;
        UnkTimelineComponent* timeline;
        UnkStruct_7100861148* collision;
        if (owner->state && (timeline = owner->timeline) && (collision = owner->collision) &&
            owner->state->mode == 2) {
            self->spawned = true;
            {
                base::reflection::UnkReflTypeIdArg mask;
                mask.id = UnkAll_SpitSend().value;
                fn_7100a2cb7c(timeline, &g_unk_7101649750, &mask.id, false, -1.0f);
            }
            fn_7100861148(collision);
            if (self->owner->field_0x280) {
                f32 scale = self->size / self->base_size;
                SpawnEffect(ModelManager(), &g_unk_71726ca0f8, self->owner->xform.GetTranslation(),
                            UnkVec3f(0.0f, 0.0f, self->owner->xform.GetAngle().z), UnkVec3f(scale, scale, scale));
            }
        }
    }

    // Ease the height back to zero at 200 units a second.
    UnkSphereOwner_l45c17c* owner = self->owner;
    UnkVec3f t = owner->xform.GetTranslation();
    f32 z = t.z;
    if (0.0f <= z + 0.001f && z - 0.001f <= 0.0f)
        return;
    f32 step = dt * 200.0f;
    if (z > 0.0f) {
        z = z - step;
        if (z <= 0.0f)
            z = 0.0f;
    } else {
        z = step + z;
        if (z >= 0.0f)
            z = 0.0f;
    }
    owner->xform.SetTranslation(UnkVec3f(t.x, t.y, z));
    owner->moved = true;
}
