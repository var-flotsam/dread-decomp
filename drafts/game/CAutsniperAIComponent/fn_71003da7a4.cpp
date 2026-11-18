// CAutsniperAIComponent unit (tools/tu_map.py unit 579). fn_71003da7a4, the start: the behaviour tree's
// start (fn_7100369650) and fn_71003daa70; hands the default effect block to the owner's enemy movement and
// gives it the state-change callback (fn_71003db6e0), which the blend at +0x27230 keeps too; clears the
// blend. With an animation at the owner's +0x220: stops layer 11, plays layer 6, forwards the blend weight to
// the layer node and links the listener at +0x27260 (callback fn_71003db714) to the animation again.
// Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game/CEnemyMovement.h"
#include "game/UnkBehaviorTree.h"
#include "game/UnkXformNode.h"
#include "game/animtree/CLayerNode.h"
#include "game/behaviortree/CSetAnimDependsOnDirAction.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::reflection::UnkReflObject;

extern __attribute__((visibility("hidden"))) u64 g_unk_71726bfcb0;

struct UnkLayerNode;
struct UnkEffectSink;
struct UnkAutsniperAI_ovb0003;
struct UnkAutsniperAI_l3da7a4;

void fn_71003daa70(UnkAutsniperAI_l3da7a4* self, bool flag);
void fn_71003db6e0(UnkAutsniperAI_ovb0003* self, s32 from, s32 to);
void fn_71003db714(UnkAutsniperAI_l3da7a4* self);

// The block handed to the owner's +0x218 (fn_71010c7e6c); the vtable tells the kinds apart.
struct UnkEffectParams_b0117 {
    void** vtable;
    u8 pad_0x8[4];
    u8 field_0xc;
    u8 pad_0xd[7];
    u8 field_0x14;
    u8 pad_0x15[3];
    u8 field_0x18[13];
    u8 pad_0x25[3];
    u8 field_0x28[13];
    u8 pad_0x35[3];
    u32 field_0x38;
    u8 pad_0x3c;
    u8 field_0x3d;
    u8 field_0x3e;
    u8 field_0x3f;
    u8 pad_0x40;
    u8 field_0x41;
    u8 pad_0x42[2];
    u8 field_0x44[13];
    u8 field_0x51;
    u8 field_0x52;
};

// An object and the function to call on it.
struct UnkCallback_l3da7a4 {
    void* object;
    void* function;
};

struct UnkNodeMap_l3da7a4 {
    u8 pad_0x0[0x78];
    UnkReflObject** values;  // 0x78
    u64* keys;               // 0x80
    u32 count;               // 0x88
};

struct UnkAnimation_l3da7a4 {
    u8 pad_0x0[0x40];
    UnkNodeMap_l3da7a4* map;  // 0x40
};

struct UnkAutsniperMover_l3da7a4 {
    u8 pad_0x0[0x19d0];
    UnkCallback_l3da7a4 on_state_change;  // 0x19d0
};

struct UnkAutsniperShape_l3da7a4 {
    void** vtable;
    UnkVec3f vf_0x360() { return reinterpret_cast<UnkVec3f (*)(UnkAutsniperShape_l3da7a4*)>(vtable[0x360 / 8])(this); }
};

struct UnkAutsniperOwner_l3da7a4 {
    u8 pad_0x0[0x1f0];
    UnkReflObject* movement;  // 0x1f0
    u8 pad_0x1f8[0x218 - 0x1f8];
    UnkAutsniperShape_l3da7a4* field_0x218;
    UnkActorTarget* animation;  // 0x220
};

struct UnkAutsniperBlend_l3da7a4 {
    void* params;                    // 0x0
    UnkCallback_l3da7a4 on_change;   // 0x8
    bool field_0x18;
    u8 pad_0x19[3];
    s32 state;    // 0x1c
    f32 weight;   // 0x20
    f32 elapsed;  // 0x24
};

// A listener: the object and the function the animation calls.
struct UnkAutsniperLink_l3da7a4 {
    u8 pad_0x0[0x8];
    UnkCallback_l3da7a4 callback;  // 0x8
};

struct UnkAutsniperAI_l3da7a4 {
    u8 pad_0x0[0x30];
    UnkAutsniperOwner_l3da7a4* owner;  // 0x30
    u8 pad_0x38[0x62 - 0x38];
    bool field_0x62;
    u8 pad_0x63[0x27230 - 0x63];
    UnkAutsniperBlend_l3da7a4 blend;  // 0x27230
    u32 field_0x27258;
    u8 pad_0x2725c[4];
    UnkAutsniperLink_l3da7a4 link;   // 0x27260
    UnkWeakHandle* handle;           // 0x27278
};
static_assert(__builtin_offsetof(UnkAutsniperAI_l3da7a4, handle) == 0x27278, "layout");

static inline UnkReflObject** Find(UnkNodeMap_l3da7a4* map, const u64& key) {
    for (u64 i = 0; i != map->count; ++i) {
        if (map->keys[i] == key) {
            return &map->values[i];
        }
    }
    return map->values + map->count;
}

// Forwards the weight to the animation's layer node under the unit's key, when it is a CLayerNode.
static inline void ForwardToLayer(UnkAnimation_l3da7a4* animation, const f32& value) {
    UnkNodeMap_l3da7a4* map = animation->map;
    UnkReflObject** it = Find(map, g_unk_71726bfcb0);
    if (it == map->values + map->count)
        return;
    UnkReflObject* node = *it;
    if (!node)
        return;
    if (!fn_710009d50c(node->vf_0x0(), fn_71002df4ec()))
        return;
    if (!fn_710009d50c(node->vf_0x0(), fn_71002e43a4()))
        return;
    fn_71002e465c(reinterpret_cast<UnkLayerNode*>(node), value);
}

void fn_71003da7a4(UnkAutsniperAI_l3da7a4* self) {
    fn_7100369650(reinterpret_cast<char*>(self));
    fn_71003daa70(self, true);
    // The shape's extent, asked for and not used.
    UnkVec3f extent = self->owner->field_0x218->vf_0x360();

    UnkReflObject* movement = self->owner->movement;
    if (movement)
        movement = fn_710009d50c(movement->vf_0x0(), fn_71010c74e0()) ? movement : nullptr;
    UnkEffectParams_b0117 effect;
    effect.field_0xc = 0;
    effect.field_0x14 = 0;
    __builtin_memset(effect.field_0x18, 0, 13);
    __builtin_memset(effect.field_0x28, 0, 13);
    effect.vtable = g_unk_7101a24db8 + 2;
    effect.field_0x3d = 0;
    effect.field_0x3f = 0;
    effect.field_0x41 = 0;
    effect.field_0x52 = 0;
    __builtin_memset(effect.field_0x44, 0, 13);
    effect.field_0x38 = 1;
    fn_71010c7e6c(reinterpret_cast<UnkEffectSink*>(movement), &effect, 1);

    void* on_change = reinterpret_cast<void*>(fn_71003db6e0);
    self->blend.on_change.object = self;
    self->blend.on_change.function = on_change;
    UnkAutsniperMover_l3da7a4* mover = reinterpret_cast<UnkAutsniperMover_l3da7a4*>(movement);
    mover->on_state_change.function = on_change;
    mover->on_state_change.object = self;
    self->field_0x62 = true;
    self->blend.elapsed = 0.0f;
    self->blend.weight = 0.0f;
    self->blend.state = 0;
    self->blend.field_0x18 = false;

    if (UnkActorTarget* animation = self->owner->animation) {
        fn_7101178e08(animation, 11, false, false, -1.0f);
        fn_710117662c(animation, 6, 0, true, true, -1.0f);
        ForwardToLayer(reinterpret_cast<UnkAnimation_l3da7a4*>(animation), self->blend.weight);

        UnkAutsniperLink_l3da7a4* link = &self->link;
        link->callback.object = self;
        link->callback.function = reinterpret_cast<void*>(fn_71003db714);
        UnkActorTarget* target = self->owner->animation;
        if (UnkWeakHandle* handle = self->handle) {
            if (handle->target)
                fn_710117b3f0(handle->target, link);
            if (UnkWeakHandle* old = self->handle) {
                UnkWeakHandle* held = old;
                if (--held->ref_count == 0)
                    held->vf_0x10();
                self->handle = nullptr;
            }
        }
        if (target) {
            fn_7100316018(reinterpret_cast<UnkWeakTarget*>(target), &self->handle);
            fn_710117b3e8(self->handle ? self->handle->target : nullptr, link);
        }
    }
    self->field_0x27258 = 0;
}
