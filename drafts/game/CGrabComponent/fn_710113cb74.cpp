// CGrabComponent unit (tools/tu_map.py): where a grabbed actor is held. In mode 2 at the grabber's
// transform moved to the actor's position (raised by the grabber's node height); in mode 1 at the actor's
// animated grab point, or else its node's transform. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/reflection/ReflectedAttributes.h"
#include "game/CAnimationComponent.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkTU1134.h"
#include "types.h"

using base::global::CStrId;

struct UnkAnimActionSetsOwner;
struct UnkAnimationComponent;
struct UnkEntity;
struct UnkMtx44_b46a2 {
    f32 m[16];
};

bool fn_7101179e68(void* anim, UnkCutsceneTimeline* timeline, u64 key, UnkMatrix44* out, u32 b);
f32 fn_710117808c(UnkAnimationComponent* self, u64 key);

struct UnkGrabNode_l13cb74 {
    u8 pad_0x0[0x60];
    f32 height;  // 0x60
};

struct UnkGrabMode_l13cb74 {
    u8 pad_0x0[0xc4];
    u32 mode;  // 0xc4
};

struct UnkGrabActor_l13cb74 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38 (its translation column at +0x44, +0x54, +0x64)
    u8 pad_0x78[0x1a8];
    void* anim;                  // 0x220
    UnkGrabNode_l13cb74* node;   // 0x228
    u8 pad_0x230[0x30];
    UnkGrabMode_l13cb74* grab;   // 0x260
};
static_assert(__builtin_offsetof(UnkGrabActor_l13cb74, anim) == 0x220, "layout");
static_assert(__builtin_offsetof(UnkGrabActor_l13cb74, grab) == 0x260, "layout");

struct UnkGrabLink_l13cb74 {
    u8 pad_0x0[0x10];
    UnkGrabActor_l13cb74* target;  // 0x10
};

struct UnkGrabComponent_l13cb74 {
    u8 pad_0x0[0x30];
    UnkGrabActor_l13cb74* owner;  // 0x30
    u8 pad_0x38[0x90];
    UnkGrabLink_l13cb74* link;  // 0xc8
    u8 pad_0xd0[0x38];
    CStrId linkedAction;  // 0x108
    CStrId action;        // 0x110
};
static_assert(__builtin_offsetof(UnkGrabComponent_l13cb74, action) == 0x110, "layout");

static inline u64 GrabBoneKey() {
    return base::reflection::refl_GetStrIdTable<0x7100031894>().ids[0x1a7].data->Key();
}

void fn_710113cb74(UnkGrabComponent_l13cb74* self, UnkGrabActor_l13cb74* target, void* animated, UnkMatrix44* out) {
    if (!target->grab)
        return;

    switch (target->grab->mode) {
    case 2: {
        UnkCopyMatrix(out, fn_710002614c(&self->owner->transform));
        UnkGrabActor_l13cb74* owner = self->owner;
        f32 x = target->transform.m[3];
        f32 z = target->transform.m[11];
        UnkGrabNode_l13cb74* node = owner->node;
        f32 y = node ? target->transform.m[7] + node->height : out->m[7];
        out->m[3] = x;
        out->m[7] = y;
        out->m[11] = z;
        break;
    }
    case 1: {
        bool placed = false;
        if (void* anim = target->anim) {
            if (animated) {
                const CStrId* id;
                if (self->link && self->link->target == target)
                    id = &self->linkedAction;
                else
                    id = self->action.data == &g_unk_7101d47108->empty ? &self->linkedAction : &self->action;
                void* key = reinterpret_cast<void*>(id->data->Key());
                placed = fn_7101179de0(static_cast<UnkAnimActionSetsOwner*>(anim), key, GrabBoneKey(), 0.0f,
                                       reinterpret_cast<UnkMtx44_b46a2*>(out), 2);
            } else {
                const u64 kGrabTrack = 0xf4061ff25a2163d6;
                if (UnkCutsceneTimeline* timeline = fn_710070d4e8(fn_71011771e0(anim, kGrabTrack))) {
                    u64 bone = GrabBoneKey();
                    fn_710117808c(static_cast<UnkAnimationComponent*>(anim), kGrabTrack);
                    placed = fn_7101179e68(anim, timeline, bone, out, 2);
                }
            }
        }
        if (placed) {
            UnkCopyMatrix(out, fn_71000127c8(fn_710002614c(&target->transform), *out));
        } else if (UnkGrabNode_l13cb74* node = target->node) {
            u64 bone = GrabBoneKey();
            fn_71011735e8(reinterpret_cast<UnkEntity*>(node), &bone, out, 0);
        }
        break;
    }
    }
}
