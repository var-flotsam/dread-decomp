// In the CSamusMagnetGloveMovementState::CParams unit: fn_71009aaa98 updates the magnet glove state each frame.
// It falls when either attachment lost its body, cancels to a default move request, counts the two timers
// down and eases the camera offset towards the attachment point. Names and layouts are placeholders.
#include "../../../../src/game_s2/UnkSpitSend.h"
#include "game/CAbilityGhostAura.h"
#include "game/UnkTU1455.h"
#include "game/UnkTU1501.h"
#include "game/UnkTU1730.h"
#include "game/UnkTUProperty.h"
#include "types.h"

extern UnkTunable g_unk_717273c650;

struct UnkMagnetFallState_ovb0082;
struct UnkMagnetParams_l9aadf4;
struct UnkMagnetRumbleState_ovb0082;
void fn_71009a9434(UnkMagnetFallState_ovb0082* self);
void fn_71009aadf4(UnkMagnetParams_l9aadf4* self);
void fn_71009ab0cc(UnkMagnetRumbleState_ovb0082* self);
void fn_71009b0d24(void* object, void* arg);

struct __attribute__((packed, aligned(4))) UnkTrackBlend {
    u64 kind;
    f32 value;
};
static_assert(sizeof(UnkTrackBlend) == 0xc, "UnkTrackBlend layout");

struct UnkDesc16_l9aaa98 {
    u64 a, b;
};

// A movement transition: an id (-1 for none), the two descriptors of UnkSpitSend.h and a mask.
struct UnkMoveTransition_l9aaa98 {
    s64 id;
    UnkDesc16_l9aaa98 desc;
    UnkTrackBlend blend;
    u32 field_0x24;
    u32 mask;

    UnkMoveTransition_l9aaa98(UnkDesc16_l9aaa98 d, UnkTrackBlend b, u32 m)
        : id(-1), desc(d), blend(b), field_0x24(0), mask(m) {}
    UnkMoveTransition_l9aaa98()
        : UnkMoveTransition_l9aaa98(*reinterpret_cast<const UnkDesc16_l9aaa98*>(&g_unk_710164b958),
                                    *reinterpret_cast<const UnkTrackBlend*>(&g_unk_7172769740),
                                    UnkAll_SpitSend().value) {}
};
static_assert(sizeof(UnkMoveTransition_l9aaa98) == 0x30, "layout");

typedef u64 UnkU64Align4_l9aaa98 __attribute__((aligned(4)));

// CSamusMovementState::CParams (vtable g_unk_7101a22cb8) with a derived vtable.
struct UnkMagnetRequest_l9aaa98 {
    void* vtable;
    UnkMoveTransition_l9aaa98 transition;  // 0x8
    u32 field_0x38;
    UnkU64Align4_l9aaa98 field_0x3c;
    u32 field_0x44;

    UnkMagnetRequest_l9aaa98() : vtable(reinterpret_cast<char*>(g_unk_7101a22cb8) + 0x10), transition() {
        field_0x38 = 3;
        field_0x3c = 0;
        field_0x44 = 0;
        vtable = reinterpret_cast<char*>(g_unk_7101a22d18) + 0x10;
    }
};
static_assert(sizeof(UnkMagnetRequest_l9aaa98) == 0x48, "layout");

struct UnkVec3_l9aaa98 {
    f32 x, y, z;

    UnkVec3_l9aaa98() {}
    UnkVec3_l9aaa98(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkVec3_l9aaa98(const UnkVec3_l9aaa98& other) : x(other.x), y(other.y), z(other.z) {}
    UnkVec3_l9aaa98& operator=(const UnkVec3_l9aaa98& other) {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }
};

inline UnkVec3_l9aaa98 operator+(const UnkVec3_l9aaa98& a, const UnkVec3_l9aaa98& b) {
    return UnkVec3_l9aaa98(a.x + b.x, a.y + b.y, a.z + b.z);
}
inline UnkVec3_l9aaa98 operator-(const UnkVec3_l9aaa98& a, const UnkVec3_l9aaa98& b) {
    return UnkVec3_l9aaa98(a.x - b.x, a.y - b.y, a.z - b.z);
}
inline UnkVec3_l9aaa98 operator*(f32 s, const UnkVec3_l9aaa98& v) {
    return UnkVec3_l9aaa98(s * v.x, s * v.y, s * v.z);
}

struct UnkMtx34_l9aaa98 {
    f32 m[3][4];

    UnkVec3_l9aaa98 Translation() const { return UnkVec3_l9aaa98(m[0][3], m[1][3], m[2][3]); }
};

struct UnkMagnetBody_l9aaa98 {
    u8 pad_0x0[0x38];
    UnkMtx34_l9aaa98 transform;  // 0x38
    u8 pad_0x68[0x2];
    bool attached;  // 0x6a
};

struct UnkMagnetLink_l9aaa98 {
    u8 pad_0x0[0x10];
    UnkMagnetBody_l9aaa98* body;  // 0x10
};

inline UnkMagnetBody_l9aaa98* Body(UnkMagnetLink_l9aaa98* link) {
    return link ? link->body : nullptr;
}

struct UnkMagnetOwner_l9aaa98;
struct UnkMagnetOwnerVtable_l9aaa98 {
    u8 pad_0x0[0x3b8];
    bool (*vf_0x3b8)(UnkMagnetOwner_l9aaa98* self);
};
struct UnkMagnetOwner_l9aaa98 {
    UnkMagnetOwnerVtable_l9aaa98* vtable;
};

struct UnkMagnetConfig_l9aaa98 {
    u8 pad_0x0[0x4c];
    UnkVec3_l9aaa98 camera_offset;  // 0x4c
    u8 pad_0x58[0x38];
    UnkVec3_l9aaa98 anchor;  // 0x90
};

struct UnkMagnetTunable_l9aaa98 {
    u8 pad_0x0[0x54];
    bool frozen;  // 0x54
};

struct UnkMagnetUpdate_l9aaa98 {
    f32 dt;
    UnkMtx34_l9aaa98 transform;  // 0x4
    u8 pad_0x34[0x2c];
    UnkVec3_l9aaa98 bounds_min;  // 0x60
    UnkVec3_l9aaa98 bounds_max;  // 0x6c
};

struct UnkMagnetState_l9aaa98 {
    u8 pad_0x0[0x8];
    UnkAirTarget_B71a6* target;  // 0x8
    u8 pad_0x10[0x28];
    UnkMagnetOwner_l9aaa98* owner;  // 0x38
    u8 pad_0x40[0x148];
    UnkMagnetConfig_l9aaa98* config;  // 0x188
    f32 blend_time;                   // 0x190
    f32 cooldown;                     // 0x194
    u8 pad_0x198[0x50];
    UnkMagnetLink_l9aaa98* link_a;  // 0x1e8
    UnkMagnetLink_l9aaa98* link_b;  // 0x1f0
};
static_assert(__builtin_offsetof(UnkMagnetState_l9aaa98, link_b) == 0x1f0, "layout");

void fn_71009aaa98(UnkMagnetState_l9aaa98* self, UnkMagnetUpdate_l9aaa98* update) {
    fn_71009b0d24(self, update);
    UnkMagnetBody_l9aaa98* body_b;
    if (!Body(self->link_a) || !(body_b = Body(self->link_b)) || !body_b->attached) {
        fn_71009a9434(reinterpret_cast<UnkMagnetFallState_ovb0082*>(self));
        return;
    }
    if (self->owner->vtable->vf_0x3b8(self->owner) && !fn_71009e12d0(self->owner)) {
        UnkMagnetRequest_l9aaa98 request;
        fn_710098ae04(self->target, reinterpret_cast<UnkAirRequest_B71a6*>(&request));
        return;
    }
    const UnkMagnetTunable_l9aaa98& tunable = reinterpret_cast<const UnkMagnetTunable_l9aaa98&>(g_unk_717273c650);
    if (self->cooldown > 0.0f && !tunable.frozen) {
        self->cooldown -= update->dt;
        if (self->cooldown <= 0.0f)
            self->cooldown = 0.0f;
    }
    if (self->blend_time > 0.0f) {
        f32 time = self->blend_time - update->dt;
        time = time <= 0.0f ? 0.0f : time;
        self->blend_time = time;
        UnkMagnetConfig_l9aaa98* config = self->config;
        UnkVec3_l9aaa98 offset = config->camera_offset;
        if (UnkMagnetBody_l9aaa98* body = Body(self->link_a))
            offset = (body->transform.Translation() - config->anchor) + offset;
        UnkVec3_l9aaa98 origin = update->transform.Translation();
        f32 weight = time / -0.1f + 1.0f;
        if (!(weight >= 1.0f))
            offset = weight * offset + (1.0f - weight) * origin;
        UnkVec3_l9aaa98 delta = offset - origin;
        update->bounds_min = delta + update->bounds_min;
        update->bounds_max = delta + update->bounds_max;
    }
    fn_71009aadf4(reinterpret_cast<UnkMagnetParams_l9aadf4*>(self));
    fn_71009ab0cc(reinterpret_cast<UnkMagnetRumbleState_ovb0082*>(self));
}
