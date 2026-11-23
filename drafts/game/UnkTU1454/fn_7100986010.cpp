// Unit 1454: a state's parameter step. While the first machine is in state 3 and the second one is not in
// state 1, and the actor allows it, it turns the given object a quarter turn about y towards its matrix's
// z sign, pushes the actor sideways, and stores a movement-state request (with an animation id picked by
// the owner's flag) into the reflected value the caller passed; otherwise fn_710097f678 does the work.
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CAbilityGhostAura.h"
#include "game/CSamusGrappleMovementState.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU1450.h"
#include "game/UnkTU1730.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CType;
using base::reflection::UnkReflValue;

struct UnkDesc_SpitSend;
extern UnkDesc_SpitSend g_unk_710164b958;

// The animation track a request plays: its key, blend time and blend count.
struct UnkTurnTrack_l986010 {
    s64 key;
    f32 blend_time;
    u32 blend_count;
};

struct UnkTurnBlend_l986010 {
    u32 a, b, c;
};

struct UnkTurnTrackParams_l986010 {
    s64 id;
    UnkTurnTrack_l986010 track;
    UnkTurnBlend_l986010 blend;
    u32 flags;
    u32 mask;

    UnkTurnTrackParams_l986010(UnkTurnTrack_l986010 track_, UnkTurnBlend_l986010 blend_, u32 mask_)
        : id(-1), track(track_), blend(blend_), flags(0), mask(mask_) {}
    UnkTurnTrackParams_l986010()
        : UnkTurnTrackParams_l986010(reinterpret_cast<const UnkTurnTrack_l986010&>(g_unk_710164b958),
                                     reinterpret_cast<const UnkTurnBlend_l986010&>(g_unk_7172769740),
                                     base::reflection::refl_GetEnumNot8<0x7101d57cb8, 0x7101d57cc8>()) {}
};

typedef u64 UnkU64Align4_l986010 __attribute__((aligned(4)));

// CSamusMovementState::CParams (vtable g_unk_7101a22cb8).
struct UnkTurnParamsBase_l986010 {
    void* vtable;
    UnkTurnParamsBase_l986010() { vtable = g_unk_7101a22cb8 + 2; }

    // The request's class (vtable slot 0).
    const CType* GetClass() {
        typedef const CType* (*GetClassFn)(UnkTurnParamsBase_l986010*);
        return (*reinterpret_cast<GetClassFn*>(vtable))(this);
    }
};

// The request (vtable g_unk_7101a22ce8).
struct UnkTurnRequest_l986010 : UnkTurnParamsBase_l986010 {
    UnkTurnTrackParams_l986010 params;  // 0x8
    u32 field_0x38;
    UnkVec3f field_0x3c;

    UnkTurnRequest_l986010() : field_0x3c(0.0f, 0.0f, 0.0f) {
        field_0x38 = 3;
        vtable = g_unk_7101a22ce8 + 0x10;
        params.flags |= 1;
    }
};
static_assert(sizeof(UnkTurnRequest_l986010) == 0x48, "request");

// The grapple movement tunable (g_unk_717273ba88): the push's horizontal range and least height.
struct UnkTurnTunable_l986010 {
    u8 pad_0x0[0x90];
    f32 min_x;     // 0x90
    f32 max_x;     // 0x94
    f32 min_y;     // 0x98
};

static inline const UnkTurnTunable_l986010& TurnTunable() {
    return reinterpret_cast<const UnkTurnTunable_l986010&>(g_unk_717273ba88);
}

struct UnkTurnFacing_l986010 {
    u8 pad_0x0[0x9ec];
    f32 facing;  // 0x9ec
};

struct UnkTurnFlags_l986010 {
    u8 pad_0x0[0x360];
    bool rumble;  // 0x360
};

struct UnkTurnOwner_l986010 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x1d8 - 0x94];
    UnkTurnFacing_l986010* facing;  // 0x1d8
    u8 pad_0x1e0[0x2b0 - 0x1e0];
    UnkTurnFlags_l986010* flags;  // 0x2b0
};
static_assert(__builtin_offsetof(UnkTurnOwner_l986010, flags) == 0x2b0, "owner");

class UnkTurnActor_l986010 {
public:
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual void vf_0xe8();
    virtual void vf_0xf0();
    virtual void vf_0xf8();
    virtual void vf_0x100();
    virtual void vf_0x108();
    virtual void vf_0x110();
    virtual void vf_0x118();
    virtual void vf_0x120();
    virtual void vf_0x128();
    virtual void vf_0x130();
    virtual void vf_0x138();
    virtual void vf_0x140();
    virtual void vf_0x148();
    virtual void vf_0x150();
    virtual void vf_0x158();
    virtual void vf_0x160();
    virtual void vf_0x168();
    virtual void vf_0x170();
    virtual void vf_0x178();
    virtual void vf_0x180();
    virtual void vf_0x188();
    virtual void vf_0x190();
    virtual void vf_0x198();
    virtual void vf_0x1a0();
    virtual void vf_0x1a8();
    virtual void vf_0x1b0();
    virtual void vf_0x1b8();
    virtual void vf_0x1c0();
    virtual void vf_0x1c8();
    virtual void vf_0x1d0();
    virtual void vf_0x1d8();
    virtual void vf_0x1e0();
    virtual void vf_0x1e8();
    virtual void vf_0x1f0();
    virtual void vf_0x1f8();
    virtual void vf_0x200();
    virtual void vf_0x208(const UnkVec3f* push);

    u8 pad_0x8[0x28];
    UnkTurnOwner_l986010* owner;  // 0x30
    u8 pad_0x38[0x1d8 - 0x38];
    UnkVec3f target;  // 0x1d8
};
static_assert(__builtin_offsetof(UnkTurnActor_l986010, target) == 0x1d8, "actor");

struct UnkTurnState_l986010 {
    void* vtable;
    u64 field_0x8;
    u32 id;  // 0x10
};

struct UnkTurnSelf_l986010 {
    u8 pad_0x0[0x40];
    UnkTurnState_l986010* state;  // 0x40: the first machine's current state
    u8 pad_0x48[0xf0 - 0x48];
    UnkTurnState_l986010* other_state;  // 0xf0: the second machine's current state
};

// The object turned: a matrix reset to identity and a transform.
struct UnkTurnObject_l986010 {
    u8 pad_0x0[0x78];
    f32 matrix[16];  // 0x78
    UnkXform xform;  // 0xb8
};
static_assert(__builtin_offsetof(UnkTurnObject_l986010, xform) == 0xb8, "object");

bool fn_71009e9fe0(UnkTurnActor_l986010* actor);
void fn_71009ecab0(UnkTurnActor_l986010* actor, f32 a, f32 b, f32 c, f32 d);
void fn_710097f678(UnkTurnSelf_l986010* self, UnkTurnObject_l986010* object, UnkReflValue* value);

static inline UnkTurnActor_l986010* TurnActor(UnkTurnSelf_l986010* self) {
    return reinterpret_cast<UnkTurnActor_l986010*>(fn_710097f198(reinterpret_cast<UnkSmBase_b60*>(self)));
}

static inline f32 SignOf(f32 v) {
    return (v > 0.0f) - (v < 0.0f);
}

static inline f32 Clamp(f32 v, const f32& lo, const f32& hi) {
    return lo >= v ? lo : (hi <= v ? hi : v);
}

// The id table's slot at 0x21a0 (no member yet).
static inline const CStrId& TurnIdSlot_21a0() {
    return *reinterpret_cast<const CStrId*>(&GameManagerIdTable().pad_0x20f8[0x21a0 - 0x20f8]);
}

void fn_7100986010(UnkTurnSelf_l986010* self, UnkTurnObject_l986010* object, UnkReflValue* value) {
    UnkTurnActor_l986010* actor = TurnActor(self);
    if (!self->state || self->state->id != 3) {
        fn_710097f678(self, object, value);
        return;
    }
    UnkTurnOwner_l986010* owner = actor->owner;
    if ((self->other_state && self->other_state->id == 1) || !fn_71009e9fe0(actor)) {
        fn_710097f678(self, object, value);
        return;
    }

    UnkTurnRequest_l986010 req;
    UnkTurnFlags_l986010* flags = owner->flags;
    for (s32 i = 0; i < 16; i++)
        object->matrix[i] = 0.0f;
    object->matrix[15] = 1.0f;
    object->matrix[10] = 1.0f;
    object->matrix[5] = 1.0f;
    object->matrix[0] = 1.0f;
    const UnkMatrix44& m = object->xform.GetMatrix();
    f32 angle = SignOf(m.m[2]) * 1.5707964f;
    object->xform.SetAngle(UnkVec3f(0.0f, angle, 0.0f));
    if (flags->rumble) {
        req.params.id = GameManagerIdTable().field_0xfd0.data->Key();
        req.params.track.key = -1;
        req.params.track.blend_time = 0.06f;
        req.params.track.blend_count = 1;
    } else {
        req.params.id = TurnIdSlot_21a0().data->Key();
    }

    UnkTurnFacing_l986010* facing = TurnActor(self)->owner->facing;
    f32 dir = SignOf(facing ? facing->facing : 0.0f);
    if (dir + 0.1f >= 0.0f && dir - 0.1f <= 0.0f)
        dir = owner->xform.GetAngle().y >= 0.0f ? 1.0f : -1.0f;
    UnkVec3f push(dir, 0.0f, 0.0f);
    req.field_0x38 = 1;
    req.field_0x3c = actor->target;
    const UnkTurnTunable_l986010& tunable = TurnTunable();
    if (dir > 0.0f)
        req.field_0x3c.x = Clamp(req.field_0x3c.x, tunable.min_x, tunable.max_x);
    else
        req.field_0x3c.x = Clamp(req.field_0x3c.x, -tunable.max_x, -tunable.min_x);
    req.field_0x3c.y = tunable.min_y < req.field_0x3c.y ? req.field_0x3c.y : tunable.min_y;
    fn_71009ecab0(actor, 1.0f, 0.5f, -1.0f, 2.5f);
    actor->vf_0x208(&push);

    const CType* t = req.GetClass();
    const CType* cur = reinterpret_cast<const CType*>(value->type);
    if (cur && value->field_0x14 == 0 &&
        (cur == t ? *reinterpret_cast<void* const*>(reinterpret_cast<const char*>(t) + 0x48) != nullptr
                  : fn_710009d52c(t, cur))) {
        fn_71000a1cc8(value, t, &req);
    } else {
        fn_71000a1c60(value);
        fn_71000a18d4(value, t, &req, false);
    }
}
