// The CKraidAIComponent unit. fn_71005fc028, the start step: after the base's step, refreshes the two areas
// (+0x14ce0, +0x14d30) from the boxes of the objects their links resolve to, sets up the fight's state, sends
// the animation a fixed request with mode 2, silences the layer node and hooks the listener (+0x14e10) to
// the animation. Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/UnkRefHandle.h"
#include "base/math/CVector.h"
#include "base/math/CVector4D.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game/CBossAIComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkSlots.h"
#include "game/UnkTU218.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU365.h"
#include "game/animtree/CLayerNode.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::reflection::UnkReflObject;

extern base::reflection::UnkTunable g_unk_71726f3440;
extern u64 g_unk_710164a210;
extern __attribute__((visibility("hidden"))) u64 g_unk_71726f3798;  // the layer node's key
void* fn_71005fc64c(UnkWeakTarget* target);
void fn_7100602158();

struct UnkKraidHash_l5fc028 {
    u8 pad_0x0[0x30];
    u64 hash;
};
extern __attribute__((visibility("hidden"))) UnkKraidHash_l5fc028* g_unk_71726f3768;

struct UnkKraidTunable_l5fc028 {
    u8 pad_0x0[0x30];
    s32 mode;  // 0x30
    u32 value;  // 0x34
};

// A handle to an actor, resolved from its reflected source on first use.
struct UnkKraidLink_l5fc028 {
    u8 pad_0x0[0x8];
    u8 source[0x28];
    u32 state;
    u32 pad_0x34;
    base::UnkRefHandle handle;

    UnkWeakHandle* Handle() {
        if (state == 1) {
            {
                UnkRefPtr fresh = fn_710023b580(source);
                fn_7100234f78(&handle, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
            }
            state = 2;
        }
        return reinterpret_cast<UnkWeakHandle*>(handle.ptr);
    }
};
static_assert(sizeof(UnkKraidLink_l5fc028) == 0x40, "layout");

struct UnkKraidBox_l5fc028 {
    base::math::CVector3D min;
    base::math::CVector3D max;
};

// The object a link's actor is tracked by: it reports its box.
struct UnkKraidBounded_l5fc028 {
    u8 pad_0x0[0x8];
    u8 field_0x8[0x8];
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual UnkKraidBox_l5fc028 Box(const void* arg);
};

struct UnkKraidTracker_l5fc028 {
    u8 pad_0x0[0x30];
    UnkKraidBounded_l5fc028* object;  // 0x30
};

struct UnkKraidCorners_l5fc028 {
    base::math::CVector2D min;
    base::math::CVector2D max;
};

// Takes the corners of the box of the object the link resolves to.
static inline void Refresh(UnkKraidLink_l5fc028* link, UnkKraidCorners_l5fc028* out) {
    if (link->state == 0)
        return;
    UnkWeakHandle* handle = link->Handle();
    UnkKraidTracker_l5fc028* tracker =
        reinterpret_cast<UnkKraidTracker_l5fc028*>(fn_710028aca8(handle ? handle->target : nullptr));
    if (!tracker)
        return;
    UnkKraidBounded_l5fc028* object = tracker->object;
    UnkKraidBox_l5fc028 box = object->Box(&object->field_0x8);
    UnkKraidCorners_l5fc028 corners;
    fn_710007b430(&corners.min, &box.min);
    fn_710007b430(&corners.max, &box.max);
    out->min.x = corners.min.x;
    out->min.y = corners.min.y;
    out->max.x = corners.max.x;
    out->max.y = corners.max.y;
}

struct UnkKraidNodeMap_l5fc028 {
    u8 pad_0x0[0x78];
    UnkReflObject** values;  // 0x78
    u64* keys;               // 0x80
    u32 count;               // 0x88

    UnkReflObject** Find(const u64& key) {
        for (u64 i = 0; i != count; ++i) {
            if (keys[i] == key)
                return &values[i];
        }
        return values + count;
    }
};

struct UnkKraidTree_l5fc028 {
    u8 pad_0x0[0x40];
    UnkKraidNodeMap_l5fc028* map;  // 0x40
};

struct UnkKraidTable_l5fc028 {
    u8 pad_0x0[0x78];
    void* field_0x78;
};

struct UnkKraidSlotHolder_l5fc028 {
    u8 pad_0x0[0x138];
    UnkSlots* slots;  // 0x138
};

struct UnkKraidSceneObject_l5fc028 {
    u8 pad_0x0[0x138];
    UnkKraidTable_l5fc028* field_0x138;  // 0x138
};

struct UnkKraidScene_l5fc028 {
    u8 pad_0x0[0x50];
    UnkKraidSceneObject_l5fc028* object;  // 0x50
};

struct UnkKraidOwner_l5fc028 {
    u8 pad_0x0[0x220];
    UnkKraidTree_l5fc028* tree;    // 0x220
    UnkKraidScene_l5fc028* scene;  // 0x228
};

struct UnkKraidParams_l5fc028 {
    u8 pad_0x0[0x530];
    f32 field_0x530;
    u8 pad_0x534[8];
    f32 low;   // 0x53c
    f32 high;  // 0x540
};

struct UnkKraidAI_l5fc028;

struct UnkKraidState_l5fc028 {
    f32 field_0x0;
    u8 pad_0x4[4];
    f32 field_0x8;
    u8 pad_0xc[4];
    void* field_0x10;
    u8 field_0x18;
    u8 pad_0x19[3];
    f32 field_0x1c;
    u8 pad_0x20[8];
    u8 flags[7];  // 0x28
    u8 pad_0x2f[9];
    u8 field_0x38;
    u8 field_0x39;
    u8 pad_0x3a[2];
    u32 field_0x3c;
    u8 pad_0x40[0x28];
    u32 field_0x68;
    u8 field_0x6c;
    u8 pad_0x6d[0x1b];
    UnkKraidParams_l5fc028* params;  // 0x88
    u8 pad_0x90[0x2f8 - 0x90];
};
static_assert(sizeof(UnkKraidState_l5fc028) == 0x2f8, "layout");

struct UnkKraidListener_l5fc028 {
    u8 pad_0x0[8];
    UnkKraidAI_l5fc028* owner;  // 0x8
    void (*callback)();         // 0x10
    UnkWeakHandle* target;      // 0x18
};

// The part of the component from +0x14ce0: the first corners, the second link and corners, and the listener.
struct UnkKraidArea_l5fc028 {
    UnkKraidCorners_l5fc028 first;    // 0x0
    UnkKraidLink_l5fc028 second_link;  // 0x10
    UnkKraidCorners_l5fc028 second;    // 0x50
    u8 pad_0x60[0xc0];
    u8 field_0x120;
    u8 pad_0x121[7];
    UnkKraidListener_l5fc028 listener;  // 0x128
};

struct UnkKraidAI_l5fc028 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkKraidOwner_l5fc028* owner;  // 0x30
    u8 pad_0x38[0x14970];
    UnkKraidState_l5fc028 state;         // 0x149a8
    UnkKraidLink_l5fc028 first_link;     // 0x14ca0
    UnkKraidArea_l5fc028 area;           // 0x14ce0

    void vf_0x208(bool a, bool b) {
        reinterpret_cast<void (*)(UnkKraidAI_l5fc028*, bool, bool)>(vtable[0x208 / 8])(this, a, b);
    }
};
static_assert(__builtin_offsetof(UnkKraidArea_l5fc028, listener) == 0x128, "layout");

static inline UnkLayerNode* FindLayer_l5fc028(UnkKraidTree_l5fc028* tree) {
    UnkKraidNodeMap_l5fc028* map = tree->map;
    UnkReflObject** it = map->Find(g_unk_71726f3798);
    UnkReflObject* layer = nullptr;
    if (it != map->values + map->count) {
        if (UnkReflObject* node = *it)
            layer = fn_710009d50c(node->vf_0x0(), fn_71002e43a4()) ? node : nullptr;
    }
    return reinterpret_cast<UnkLayerNode*>(layer);
}

// Smooth ramp of t over 0..1.
static inline f32 Ease(f32 t) {
    f32 r = t * t;
    if (t < 1.0f) {
        f32 u = 1.0f - t;
        f32 w = 1.0f - u * u;
        if (t < 0.0f)
            r = w;
        else
            r = t * r + u * w;
    }
    return !(r <= 0.0f) ? r : 0.0f;
}

void fn_71005fc028(UnkKraidAI_l5fc028* self) {
    fn_71003acb5c(reinterpret_cast<UnkCoreXAI_A2_b73*>(self));
    UnkKraidArea_l5fc028* area = &self->area;
    UnkKraidState_l5fc028* state = &self->state;
    state->field_0x0 = -1.0f;
    Refresh(&self->first_link, &area->first);
    Refresh(&area->second_link, &area->second);
    UnkKraidTunable_l5fc028* tunable = reinterpret_cast<UnkKraidTunable_l5fc028*>(&g_unk_71726f3440);
    if (tunable->mode == 1)
        self->vf_0x208(true, true);
    if (UnkKraidSceneObject_l5fc028* scene = self->owner->scene->object)
        fn_7100199420(scene->field_0x138->field_0x78, &g_unk_710164a210, false);
    f32 duration = state->params->field_0x530;
    state->field_0x8 = duration;
    UnkKraidSceneObject_l5fc028* object = self->owner->scene->object;
    u64 key = 0x6b084cde3ed5995e;
    u32 id = -1;
    if (object) {
        if (fn_7100199d88(object->field_0x138->field_0x78, &key, &id)) {
            f32 v = Ease(1.0f - duration / state->params->field_0x530);
            base::math::CVector4D color;
            color.x = v;
            color.y = v;
            color.z = v;
            color.w = 1.0f;
            fn_71001c2764(reinterpret_cast<UnkKraidSlotHolder_l5fc028*>(object)->slots, id, 0, &color, 1, true);
        }
    }
    u64 manager_key = g_unk_71726f3768->hash;
    u32 kind = g_unk_71727bebb8;
    state->field_0x10 = fn_71005fc64c(fn_7100debbc8(g_unk_7101d47310, &manager_key, &kind));
    state->field_0x18 = 0;
    state->field_0x39 = 0;
    state->field_0x68 = 0;
    for (u8& flag : state->flags)
        flag = 0;
    f32 low = state->params->low;
    f32 high = state->params->high;
    state->field_0x1c = low + (high - low) * static_cast<f32>((rand() & 0x7fff) * (1.0 / 32767.0));
    UnkKraidTree_l5fc028* tree = self->owner->tree;
    {
        base::reflection::UnkReflTypeIdArg mode;
        mode.id = 2;
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        fn_7101176690(tree, 0xdec1d0efc9e2415f, 0x9317bfcf28edee2b, &mode, &g_unk_710164b958, &g_unk_7172769740,
                      &mask);
    }
    state->field_0x38 = 1;
    state->field_0x6c = 0;
    state->field_0x3c = tunable->value;
    if (tree) {
        fn_71002e465c(FindLayer_l5fc028(tree), 0.0f);
        UnkKraidListener_l5fc028* listener = &area->listener;
        listener->owner = self;
        listener->callback = fn_7100602158;
        if (UnkWeakHandle* old = listener->target) {
            if (old->target)
                fn_710117b3f0(old->target, listener);
            ResetRef(&listener->target);
        }
        CAbilityGhostAura_AssignWeakHandleToRef(reinterpret_cast<UnkWeakTarget*>(tree), reinterpret_cast<UnkWeakRef*>(&listener->target));
        fn_710117b3e8(listener->target ? listener->target->target : nullptr, listener);
    }
    area->field_0x120 = 0;
}
