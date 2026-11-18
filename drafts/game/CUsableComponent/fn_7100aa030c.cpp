// CUsableComponent unit (tools/tu_map.py, unit 1607): starts the use. Hands the player's movement an access
// transition, runs the component's start hook, plays the use timeline on the owner and moves the listener
// onto its timeline, then picks the use animation by the player's facing and starts it through
// fn_7100aa0b04. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "game/CAbilityGhostAura.h"
#include "game/CTimelineComponent.h"
#include "game/CUsableComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU2553.h"
#include "game/UnkTimelineComponent.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

extern void* g_unk_7101a874c0[];
extern const u64 g_unk_710164b658[];

struct UnkAccessTarget_la70e48;
void fn_710031433c(UnkAccessTarget_la70e48* target, void* params);
void fn_71009e8664(UnkAccessTarget_la70e48* movement);

struct CAbilityComponent;
void fn_7100302180(CAbilityComponent* self);

struct UnkMovement2_ovb0048;
UnkVec3f fn_71010d2b08(UnkMovement2_ovb0048* self);

void fn_7100a2d4c8(UnkWeakTarget* target, void* listener);
void fn_710082efe8(UnkWeakTarget* self, UnkWeakHandle** out);

struct UnkUsable_laa0810;
void fn_7100aa0810(UnkUsable_laa0810* self, const CStrId* prefix, CStrId* side, CStrId* animation_id);

struct __attribute__((packed, aligned(4))) UnkTrackBlend_laa030c {
    u64 kind;
    f32 value;
};
static_assert(sizeof(UnkTrackBlend_laa030c) == 0xc, "layout");

struct UnkDesc16_laa030c {
    u64 a, b;
};

// A movement transition: an id (-1 for none), the two descriptors of UnkSpitSend.h and a mask.
struct UnkMoveTransition_laa030c {
    s64 id;
    UnkDesc16_laa030c desc;
    UnkTrackBlend_laa030c blend;
    u32 field_0x24;
    u32 mask;

    UnkMoveTransition_laa030c(UnkDesc16_laa030c d, UnkTrackBlend_laa030c b, u32 m)
        : id(-1), desc(d), blend(b), field_0x24(0), mask(m) {}
    UnkMoveTransition_laa030c()
        : UnkMoveTransition_laa030c(*reinterpret_cast<const UnkDesc16_laa030c*>(&g_unk_710164b958),
                                    *reinterpret_cast<const UnkTrackBlend_laa030c*>(&g_unk_7172769740),
                                    UnkAll_SpitSend().value) {}
};
static_assert(sizeof(UnkMoveTransition_laa030c) == 0x30, "layout");

typedef u64 UnkU64Align4_laa030c __attribute__((aligned(4)));

// CSamusMovementState::CParams (vtable g_unk_7101a22cb8) with the access vtable g_unk_7101a874c0.
struct UnkUseParams_laa030c {
    void* vtable;
    UnkMoveTransition_laa030c transition;  // 0x8
    u32 field_0x38;
    UnkU64Align4_laa030c field_0x3c;
    u32 field_0x44;

    UnkUseParams_laa030c() : vtable(reinterpret_cast<char*>(g_unk_7101a22cb8) + 0x10), transition() {
        field_0x38 = 3;
        field_0x3c = 0;
        field_0x44 = 0;
        vtable = reinterpret_cast<char*>(g_unk_7101a874c0) + 0x10;
    }
};
static_assert(sizeof(UnkUseParams_laa030c) == 0x48, "layout");

struct UnkPlayerMode_laa030c {
    u8 pad_0x0[0x10];
    u32 id;  // 0x10
};

struct UnkPlayerModes_laa030c {
    u8 pad_0x0[0x50];
    UnkPlayerMode_laa030c* current;  // 0x50
};

struct UnkPlayer_laa030c {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x1f0 - 0x94];
    UnkMovement2_ovb0048* motion;  // 0x1f0
    u8 pad_0x1f8[0x208 - 0x1f8];
    UnkAccessTarget_la70e48* movement;  // 0x208
    u8 pad_0x210[0x2b0 - 0x210];
    UnkPlayerModes_laa030c* modes;  // 0x2b0
    u8 pad_0x2b8[0x2e0 - 0x2b8];
    CAbilityComponent* ability;  // 0x2e0
};
static_assert(__builtin_offsetof(UnkPlayer_laa030c, ability) == 0x2e0, "layout");

struct UnkUsableOwner_laa030c {
    u8 pad_0x0[0x2e8];
    UnkWeakTarget* timeline;  // 0x2e8
};

struct UnkUsableAnims_laa030c {
    u8 pad_0x0[0x168];
    CStrId animation;  // 0x168
};

struct UnkUsable_laa030c;
struct UnkUsableVtable_laa030c {
    void* pad_0x0[0x1e0 / 8];
    const CStrId* (*vf_0x1e0)(UnkUsable_laa030c* self);  // the use prefix when facing right
    const CStrId* (*vf_0x1e8)(UnkUsable_laa030c* self);  // the use prefix otherwise
    void* pad_0x1f0[(0x3a0 - 0x1f0) / 8];
    void (*vf_0x3a0)(UnkUsable_laa030c* self);
};

// A listener: the object and function it calls, and the counted link to what it listens to.
struct UnkUsableListener_laa030c {
    void** vtable;
    UnkUsable_laa030c* context;            // 0x8
    void (*callback)(UnkUsable_laa030c*);  // 0x10
    UnkWeakHandle* link;                   // 0x18
};

struct UnkUsable_laa030c {
    UnkUsableVtable_laa030c* vtable;
    u8 pad_0x8[0x28];
    UnkUsableOwner_laa030c* owner;  // 0x30
    u8 pad_0x38[0xc8 - 0x38];
    bool started;  // 0xc8
    u8 pad_0xc9[0x4f8 - 0xc9];
    bool in_use;  // 0x4f8
    u8 pad_0x4f9[0x500 - 0x4f9];
    UnkUsableAnims_laa030c* anims;  // 0x500
    bool facing_right;              // 0x508
    u8 pad_0x509[0x518 - 0x509];
    UnkUsableListener_laa030c listener;  // 0x518
};
static_assert(__builtin_offsetof(UnkUsable_laa030c, listener) == 0x518, "layout");

void fn_7100aa4780(UnkUsable_laa030c* self);

// `id`, or `fallback` when `id` is empty.
static inline const CStrId* OrFallback(const CStrId* id, const CStrId* fallback) {
    return id->data == &g_unk_7101d47108->empty ? fallback : id;
}

// Hands the player's movement the access transition.
static inline void StartAccess_laa030c(UnkPlayer_laa030c* player) {
    if (UnkAccessTarget_la70e48* movement = player->movement) {
        fn_71009e8664(movement);
        UnkUseParams_laa030c params;
        fn_710031433c(movement, &params);
    }
}

// Picks the use animation by the player's facing and starts it.
static inline void StartAnimation_laa030c(UnkUsable_laa030c* self, UnkPlayer_laa030c* player) {
    bool aiming = player->modes && player->modes->current && player->modes->current->id == 2;
    CStrId prefix = CStrId::Empty();
    f32 facing = aiming ? player->xform.GetAngle().y : fn_71010d2b08(player->motion).x;
    if (facing > 0.0f) {
        prefix = *self->vtable->vf_0x1e0(self);
        self->facing_right = true;
    } else {
        prefix = *self->vtable->vf_0x1e8(self);
        self->facing_right = false;
    }

    CStrId animation = CStrId::Empty();
    CStrId side = CStrId::Empty();
    fn_7100aa0810(reinterpret_cast<UnkUsable_laa0810*>(self), &prefix, &side, &animation);
    const CStrId* use = OrFallback(&animation, &prefix);
    const CStrId* fallback = OrFallback(&self->anims->animation, &prefix);
    u64 key = side.data->Key();
    fn_7100aa0b04(reinterpret_cast<UnkUsableVirt_A6_b62*>(self), reinterpret_cast<u64>(use),
                  reinterpret_cast<u64>(fallback), reinterpret_cast<const u8*>(&key));
}

void fn_7100aa030c(UnkUsable_laa030c* self) {
    UnkPlayer_laa030c* player = reinterpret_cast<UnkPlayer_laa030c*>(fn_7100dec788(g_unk_7101d47310));
    StartAccess_laa030c(player);
    self->in_use = true;
    self->vtable->vf_0x3a0(self);
    if (player->ability)
        fn_7100302180(player->ability);

    UnkUsableOwner_laa030c* owner = self->owner;
    if (UnkWeakTarget* timeline = owner->timeline) {
        fn_7100d7c828(reinterpret_cast<UnkActor*>(owner), g_unk_710164b658, false);
        UnkUsableListener_laa030c* listener = &self->listener;
        listener->context = self;
        listener->callback = fn_7100aa4780;
        if (UnkWeakHandle* link = self->listener.link) {
            if (UnkWeakTarget* target = link->target)
                fn_7100a2d4d0(target, listener);
            if (UnkWeakHandle* p = self->listener.link) {
                if (--p->ref_count == 0)
                    p->vf_0x10();
                self->listener.link = nullptr;
            }
        }
        fn_710082efe8(timeline, &self->listener.link);
        fn_7100a2d4c8(self->listener.link ? self->listener.link->target : nullptr, listener);
    }

    StartAnimation_laa030c(self, player);
    self->started = true;
}
