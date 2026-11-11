// CTakumakuAIComponent's unit (tools/tu_map.py unit 1079): the layer weight forwarded to the anim tree
// (fn_71006d6038), the timeline id picked for an event (fn_71006d5990) and the sideways reach of an
// animation's motion (fn_71006d4fdc). Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAnimationComponent.h"
#include "game/UnkActor.h"
#include "game/UnkTU1728.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "game/animtree/CLayerNode.h"
#include "types.h"

// ---- fn_71006d6038: the layer weight ----
extern __attribute__((visibility("hidden"))) u64 g_unk_7172709b50;

using base::reflection::UnkReflObject;

// As src/game/CAutsniperAIComponent/CAutsniperAIComponent_LayerLookup_b55.cpp declares it.
struct UnkLayerNode;

struct UnkTakumakuNodeMap_b0012 {
    u8 pad_0x0[0x78];
    UnkReflObject** values;  // 0x78
    u64* keys;               // 0x80
    u32 count;               // 0x88
};

struct UnkTakumakuTree_b0012 {
    u8 pad_0x0[0x40];
    UnkTakumakuNodeMap_b0012* map;  // 0x40
};

struct UnkTakumakuOwner_b0012 {
    u8 pad_0x0[0x220];
    UnkTakumakuTree_b0012* tree;  // 0x220
};

struct UnkTakumakuLayer_b0012 {
    u8 pad_0x0[0x30];
    UnkTakumakuOwner_b0012* owner;  // 0x30
};

static inline UnkReflObject** Find(UnkTakumakuNodeMap_b0012* map, const u64& key) {
    for (u64 i = 0; i != map->count; ++i) {
        if (map->keys[i] == key)
            return &map->values[i];
    }
    return map->values + map->count;
}

// Looks up the anim-tree node under the key g_unk_7172709b50 in the owner's tree and, when it is a
// CLayerNode, forwards the value to it (fn_71002e465c).
void fn_71006d6038(UnkTakumakuLayer_b0012* self, f32 value) {
    UnkTakumakuOwner_b0012* owner = self->owner;
    if (!owner)
        return;
    UnkTakumakuTree_b0012* tree = owner->tree;
    if (!tree)
        return;
    UnkTakumakuNodeMap_b0012* map = tree->map;
    UnkReflObject** it = Find(map, g_unk_7172709b50);
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

// ---- fn_71006d5990: the timeline id for an event ----

using base::global::CStrId;

// As src/game/CTakumakuAIComponent/CTakumakuAIComponent_b35.cpp declares it.
struct UnkTakumakuComponent;

extern __attribute__((visibility("hidden"))) CStrId g_unk_7172709b58[2];
extern __attribute__((visibility("hidden"))) CStrId g_unk_7172709b68;
extern __attribute__((visibility("hidden"))) CStrId g_unk_7172709b70;

struct UnkTakumakuMovement_b0012 {
    u8 pad_0x0[0x6f8];
    s32 field_0x6f8;
};

struct UnkTakumakuAI3_b0012 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
};

// Starts with the empty id; with the animation's kind 12 (fn_7101178e6c), event 11 picks one of two ids
// by whether the target is behind the owner's facing (+0x8 column of its matrix), event 6 one of two
// by the movement's mode (+0x218, its +0x6f8).
void fn_71006d5990(UnkTakumakuAI3_b0012* self, s32 event, void* arg, CStrId* out) {
    *out = g_unk_7101d470e0;
    UnkActor* owner = self->owner;
    if (!owner)
        return;
    UnkTakumakuComponent* component = static_cast<UnkTakumakuComponent*>(owner->field_0x220);
    UnkActor* target = fn_7100d7c990(owner);
    UnkTakumakuMovement_b0012* movement =
        self->owner ? static_cast<UnkTakumakuMovement_b0012*>(self->owner->field_0x218) : nullptr;
    if (event == 11 && component) {
        if (!fn_7101178e6c(component, 12))
            return;
        UnkActor* actor = self->owner;
        f32 tx = target->field_0x44;
        f32 ty = target->field_0x54;
        f32 tz = target->field_0x64;
        f32 ax = actor->field_0x44;
        f32 ay = actor->field_0x54;
        f32 az = actor->field_0x64;
        f32 dx = tx - ax;
        f32 dy = ty - ay;
        f32 dz = tz - az;
        const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(actor->field_0x38));
        f32 fx = m.m[2];
        f32 fy = m.m[6];
        f32 fz = m.m[10];
        f32 dot = dx * fx + dy * fy + dz * fz;
        *out = dot < -0.001f ? g_unk_7172709b58[1] : g_unk_7172709b58[0];
    } else if (event == 6 && component) {
        if (!fn_7101178e6c(component, 12))
            return;
        if (movement->field_0x6f8 == 1)
            *out = g_unk_7172709b70;
        else
            *out = g_unk_7172709b68;
    }
}

// ---- fn_71006d4fdc: the sideways reach of an animation's motion ----

// As src/game/CTakumakuAIComponent/CTakumakuAIComponent_Send_b56.cpp declares it (the key is a hash).
struct UnkStruct_7101176ea8;
void* fn_7101176d34(UnkStruct_7101176ea8* self, void* arg);

// As src/game/UnkSpawnVolume.cpp names it.
struct UnkMat4f {
    f32 m[4][4];
};
UnkVec3f fn_7100078170(const UnkMat4f* m, const UnkVec3f* v);
UnkMat4f fn_7100079c60(const UnkVec3f* from, const UnkVec3f* to);
extern __attribute__((visibility("hidden"))) const UnkVec3f g_unk_710164a7f0;

// As src/game/CSamusFloorSlideMovementState/CParams/CParams_Slide_ovb0004.cpp defines them.
struct UnkFlags_71007142e8 {
    u32 bits;
    UnkFlags_71007142e8() : bits(0) {}
    UnkFlags_71007142e8(u32 b) : bits(b) {}
    UnkFlags_71007142e8(const UnkFlags_71007142e8& o) : bits(o.bits) {}
};

inline const UnkFlags_71007142e8& UnkFlagsNone_71007142e8() {
    static UnkFlags_71007142e8 s_none;
    return s_none;
}

inline const UnkFlags_71007142e8& UnkFlagsAll_71007142e8() {
    static UnkFlags_71007142e8 s_all(~UnkFlagsNone_71007142e8().bits & 7);
    return s_all;
}

struct UnkStruct_71007142e8 {
    u8 pad_0x0[0x2c];
    f32 field_0x2c;
    u8 pad_0x30[0x40];
};

struct UnkMotionTracks;
struct UnkTimeline {
    u8 pad_0x0[0x24];
    s32 frame_count;
    u8 pad_0x28[0x58];
    UnkMotionTracks* tracks;  // 0x80
};

void fn_71007142e8(UnkStruct_71007142e8* self);

struct UnkTakumakuMotion_b0012 {
    UnkStruct_71007142e8 motion;

    // The translation column of the motion's 3x4 matrix.
    UnkVec3f Translation() const {
        const f32* m = reinterpret_cast<const f32*>(this);
        return UnkVec3f(m[3], m[7], m[11]);
    }
};

struct UnkTakumakuOwner4_b0012 {
    u8 pad_0x0[0x220];
    UnkStruct_7101176ea8* animation;  // 0x220
};

struct UnkTakumakuAI4_b0012 {
    u8 pad_0x0[0x30];
    UnkTakumakuOwner4_b0012* owner;  // 0x30
};

// The rotation from g_unk_710164a7f0 to straight up.
static inline UnkMat4f UpRotation() {
    UnkVec3f up(0.0f, 1.0f, 0.0f);
    return fn_7100079c60(&g_unk_710164a7f0, &up);
}

// The motion over the whole timeline, every channel.
static inline void Sample(UnkTimeline* timeline, UnkTakumakuMotion_b0012* motion) {
    UnkMotionTracks* tracks = timeline->tracks;
    UnkFlags_71007142e8 flags = UnkFlagsAll_71007142e8();
    fn_7100b8b3bc(timeline, motion, tracks, &flags.bits, 0.0f, timeline->frame_count);
}

void fn_71006d4fdc(UnkTakumakuAI4_b0012* self, const u64* key, f32* inout) {
    UnkTakumakuOwner4_b0012* owner = self->owner;
    if (!owner)
        return;
    UnkStruct_7101176ea8* animation = owner->animation;
    if (!animation)
        return;
    UnkMat4f rotation = UpRotation();
    fn_7100077bbc(&rotation);
    if (!(*inout < -0.001f))
        return;
    UnkTimeline* timeline = static_cast<UnkTimeline*>(fn_7101176d34(animation, reinterpret_cast<void*>(*key)));
    if (!timeline)
        return;
    UnkTakumakuMotion_b0012 motion;
    fn_71007142e8(&motion.motion);
    Sample(timeline, &motion);
    UnkVec3f offset = motion.Translation();
    UnkVec3f moved = fn_7100078170(&rotation, &offset);
    *inout = fabsf(moved.x);
}
