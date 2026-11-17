// Unit 1102 (tools/tu_map.py, hint CXParasiteAIComponent): the X parasite component's start. It registers the
// component with the game manager, runs the base start, levels and turns the owner, picks a choice, publishes its
// values and a far-away random point in the blackboard, shows or hides the owner's shape and effect by state,
// adds the owner to the manager's list and starts the owner's current animation at a random frame. Names and
// layouts are placeholders.
#include <math.h>
#include <stdlib.h>

#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CAbilityShinespark.h"
#include "game/CAnimationComponent.h"
#include "game/CBlackboard.h"
#include "game/CFXComponent.h"
#include "game/CXParasiteAIComponent.h"
#include "game/UnkBehaviorTree.h"
#include "game/UnkGameManager.h"
#include "game/UnkSwitchObject.h"
#include "game/UnkTU2553.h"
#include "game/UnkTypedValue.h"
#include "game/UnkXParasiteAI.h"
#include "game/UnkXformNode.h"
#include "game/behaviortree/CBehavior.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector3D;
using base::reflection::UnkReflTypeIdArg;
using base::reflection::UnkReflTypeOf;

u32 XParasiteAI_PickWeightedChoice(UnkXParasiteAI* self);
void fn_71006e75e8(UnkXParasiteAI* self, bool on);

extern const u64 g_unk_717270b8b0 __attribute__((visibility("hidden")));
extern UnkPropertyName g_unk_717270b8c8 __attribute__((visibility("hidden")));
extern UnkPropertyName g_unk_717270b8d0 __attribute__((visibility("hidden")));
extern UnkPropertyName g_unk_717270b8d8 __attribute__((visibility("hidden")));

struct UnkXParasiteKeyed_8208;
extern UnkXParasiteKeyed_8208* g_unk_717270b910[2] __attribute__((visibility("hidden")));
struct UnkXParasiteKeyed_8208 {
    u8 pad_0x0[0x30];
    u64 key;  // 0x30
};

// The component's tunables (+0x146c8) and the two values copied from them, which UnkXParasiteAI does not
// have yet.
struct UnkXParasiteTunable_l6e6ee0 {
    u8 pad_0x0[0x530];
    f32 field_0x530;
    f32 field_0x534;
    u32 field_0x538;
    u32 field_0x53c;
};

struct UnkXParasiteTuned_l6e6ee0 {
    u8 pad_0x0[0x146c8];
    UnkXParasiteTunable_l6e6ee0* tunable;  // 0x146c8
    u8 pad_0x146d0[0x8];
    u32 field_0x146d8;
    u32 field_0x146dc;
};

static inline UnkXParasiteTuned_l6e6ee0* Tuned(UnkXParasiteAI* self) {
    return reinterpret_cast<UnkXParasiteTuned_l6e6ee0*>(self);
}

// The owner's animation component (+0x220): its current animation's id at +0x2b8.
struct UnkXParasiteAnim_l6e6ee0 {
    u8 pad_0x0[0x2b8];
    CStrId current;  // 0x2b8
};

struct UnkXParasiteAnimEntry_l6e6ee0 {
    u8 pad_0x0[0x24];
    s32 frames;  // 0x24
};

// Where a started animation begins.
struct UnkXParasiteAnimStart_l6e6ee0 {
    u64 field_0x0;
    f32 frame;

    explicit UnkXParasiteAnimStart_l6e6ee0(f32 f) : field_0x0(0), frame(f) {}
};

// An id the game manager's lists hold (the owner's at +0x2a0).
struct UnkXParasiteActorId_l6e6ee0 {
    u32 field_0x0;
    u32 field_0x4;
};

struct UnkXParasiteOwner_l6e6ee0 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x220 - 0x94];
    UnkXParasiteAnim_l6e6ee0* animation;  // 0x220
    UnkXParasiteHolder* model;            // 0x228
    u8 pad_0x230[0x50];
    void* parts;  // 0x280
    u8 pad_0x288[0x18];
    UnkXParasiteActorId_l6e6ee0 id;  // 0x2a0
    u8 pad_0x2a8[0x108];
    u8 field_0x3b0;
    u8 pad_0x3b1[0xf];
    bool field_0x3c0;
};
static_assert(__builtin_offsetof(UnkXParasiteOwner_l6e6ee0, id) == 0x2a0, "layout");
static_assert(__builtin_offsetof(UnkXParasiteOwner_l6e6ee0, field_0x3c0) == 0x3c0, "layout");

static inline UnkXParasiteOwner_l6e6ee0* Owner(UnkXParasiteAI* self) {
    return reinterpret_cast<UnkXParasiteOwner_l6e6ee0*>(self->owner);
}

// The game manager's list of actor ids (fn_7100defcd4's +0x2b0).
struct UnkXParasiteIds_l6e6ee0 {
    u32 count;
    UnkXParasiteActorId_l6e6ee0 ids[0x20];  // 0x4
};

struct UnkXParasiteIdList_l6e6ee0 {
    u8 pad_0x0[0x848];
    UnkXParasiteIds_l6e6ee0 owners;  // 0x848
};

struct UnkXParasiteGameOwner_l6e6ee0 {
    u8 pad_0x0[0x2b0];
    UnkXParasiteIdList_l6e6ee0* ids;  // 0x2b0
};

struct UnkXParasitePart_l6e6ee0 {
    u8 pad_0x0[0xb];
    u8 id;  // 0xb
};

// Stores `value` under `name` in the component's section of its blackboard, unless the blackboard is locked.
template <class T>
static inline void Publish(CBlackboard* blackboard, UnkPropertyName* name, T* value) {
    if (CBlackboard::CSection* section = fn_71010472e0(blackboard, g_unk_71726b7cb8)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = UnkReflTypeOf<T>::Get();
            v.value = value;
            fn_7100332c80(section, name, &v);
        }
    }
}

static inline void PublishFloat(CBlackboard* blackboard, UnkPropertyName* name, f32 value) {
    Publish(blackboard, name, &value);
}

static inline f32 Random01() { return (f32)((rand() & 0x7fff) * (1.0 / 32767.0)); }

static inline f32 RandomRange(f32 lo, f32 hi) { return Random01() * (hi - lo) + lo; }

// Hides the owner's part of the state's effect (as XParasiteAI_SetEffectActive does turning it off).
static inline void HideEffect(UnkXParasiteAI* self) {
    if (self->field_0x14724 == 4)
        return;
    void* parts = Owner(self)->parts;
    if (!parts)
        return;
    auto* part = reinterpret_cast<UnkXParasitePart_l6e6ee0*>(
        fn_7101152b90(static_cast<UnkWorld*>(parts), g_unk_717270b910[0]->key));
    if (!part)
        return;
    if (!fn_710114e668(static_cast<UnkObject*>(parts), part->id))
        return;
    fn_7100146e60(reinterpret_cast<UnkSwitchObject*>(part), false);
    fn_710115596c(static_cast<UnkRinkaControl_ruc*>(parts), part);
}

void fn_71006e6ee0(UnkXParasiteAI* self) {
    fn_7100de9540(g_unk_7101d47310, &g_unk_717270b8b0, reinterpret_cast<UnkWeakTarget*>(self));
    fn_7100369650(reinterpret_cast<char*>(self));

    Owner(self)->xform.m[2][3] = 0.0f;
    Owner(self)->field_0x3b0 = 1;
    Owner(self)->xform.SetAngle(UnkVec3f(0.0f, 1.5707964f, 0.0f));
    Owner(self)->field_0x3c0 = true;

    if (self->field_0x14724 == 5)
        self->field_0x14724 = XParasiteAI_PickWeightedChoice(self);
    Tuned(self)->field_0x146d8 = Tuned(self)->tunable->field_0x538;
    Tuned(self)->field_0x146dc = Tuned(self)->tunable->field_0x53c;

    Publish(&self->blackboard, &g_unk_717270b8d8, &self->field_0x146e8);
    PublishFloat(&self->blackboard, &g_unk_717270b8c8, Tuned(self)->tunable->field_0x530);
    PublishFloat(&self->blackboard, &g_unk_717270b8d0, Tuned(self)->tunable->field_0x534);

    // A random point far away on the owner's plane.
    f32 angle = RandomRange(0.0f, 6.2831855f);
    UnkXform& xform = Owner(self)->xform;
    f32 x = xform.m[0][3];
    f32 y = xform.m[1][3];
    f32 z = xform.m[2][3];
    CVector3D target;
    target.x = x + cosf(angle) * 10000.0f;
    target.y = y + sinf(angle) * 10000.0f;
    target.z = z + 0.0f;
    Publish(&self->blackboard, reinterpret_cast<UnkPropertyName*>(g_unk_717270b870), &target);

    if (self->state == 8 || (self->state == 0 && self->field_0x14798 && self->field_0x14798->target)) {
        if (UnkXParasiteHolder* model = Owner(self)->model) {
            UnkXParasiteShape* shape = model->shape;
            if (shape->field_0x11c)
                shape->field_0x114 = true;
        }
        HideEffect(self);
    } else {
        fn_71006e75e8(self, true);
    }

    auto* game = reinterpret_cast<UnkXParasiteGameOwner_l6e6ee0*>(fn_7100defcd4(g_unk_7101d47310));
    if (game && game->ids) {
        UnkXParasiteIds_l6e6ee0& list = game->ids->owners;
        if (list.count != 0x20) {
            list.ids[list.count] = Owner(self)->id;
            ++list.count;
        }
    }

    // Restarts the current animation at a random frame.
    if (UnkXParasiteAnim_l6e6ee0* animation = Owner(self)->animation) {
        u64 key = animation->current.data->Key();
        auto* entry = static_cast<UnkXParasiteAnimEntry_l6e6ee0*>(
            fn_7101176d34(reinterpret_cast<UnkStruct_7101176ea8*>(animation), reinterpret_cast<void*>(key)));
        if (entry) {
            s32 frames = entry->frames;
            u32 mode = fn_7101177ae8(reinterpret_cast<UnkAnimationComponent*>(animation), 0xf4061ff25a2163d6)
                                   ->data->Key() == key
                           ? 2
                           : 0;
            f32 frame = frames ? (f32)((rand() & 0x7fff) % frames) : 0.0f;
            UnkXParasiteAnimStart_l6e6ee0 start(frame);
            UnkReflTypeIdArg mask;
            mask.id = UnkAll_SpitSend().value;
            fn_7101176690(animation, key, 0xf4061ff25a2163d6, reinterpret_cast<UnkReflTypeIdArg*>(&mode),
                          &g_unk_710164b958, &start, &mask);
        }
    }
}
