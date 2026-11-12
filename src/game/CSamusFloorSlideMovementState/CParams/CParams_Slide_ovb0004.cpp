// CSamusFloorSlideMovementState's unit (tools/tu_map.py unit 1459): the floor slide state's constructor
// (fn_7100991b30) and destructor body (fn_7100991d0c), its enter (fn_7100992044), exit (fn_71009921a8)
// and event (fn_7100994c08) handlers, and the helpers around the slide's collision body: the body of
// kind 8 at +0x158 of the collision object named g_unk_717273b7c0 is resized to the slide's height
// (+0x190) and offset (+0x18c), and grows back (fn_7100992724) when there is room. Without room the
// state falls back to the morph ball when the inventory holds ITEM_MORPH_BALL. Also the transition
// the state reports (fn_71009954ec, by the animation tracks it has) and the slide timing helpers.
// Names and layouts are placeholders.
#include "../../../game_s2/UnkSpitSend.h"
#include "base/UnkStrIdPathPair.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CAbilityGhostAura.h"
#include "game/CActorComponent.h"
#include "game/CCharacterMovement.h"
#include "game/CInventoryComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU1501.h"
#include "game/UnkTU1728.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU25.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU39.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

// ---- shared declarations ----

// The Samus settings object of src/game/CSamusMovement.cpp (reached through the GOT).
struct UnkSamusSettings {
    u8 pad_0x0[0x24];
    f32 field_0x24;
    f32 field_0x28;
    u8 pad_0x2c[0x8];
    u32 field_0x34;
    u32 field_0x38;
    u8 pad_0x3c[0x3c];
    bool field_0x78;
};

extern const base::global::CStrId g_unk_717273b7c0 __attribute__((visibility("hidden")));  // the slide collider's name
extern const CStrId g_unk_717273b7b8 __attribute__((visibility("hidden")));
extern const CStrId g_unk_717273b7c8 __attribute__((visibility("hidden")));
extern const CStrId g_unk_717273b7d0 __attribute__((visibility("hidden")));

struct UnkFlagged;
struct UnkShape;
struct CSamusMovementState;
bool fn_71009b2d0c(CSamusMovementState* self);
struct UnkGroundPlayer_b52;
bool fn_71010d1598(UnkGroundPlayer_b52*);
struct UnkInventory;
struct UnkStandState_A6_b70;
void fn_710099d6f0(UnkStandState_A6_b70* self);
struct UnkActor;
struct UnkAirTarget_B71a6;
struct UnkAirRequest_B71a6;
void fn_710098ae04(UnkAirTarget_B71a6* target, UnkAirRequest_B71a6* req);

static inline const UnkQualifiedId& GetQualifiedId_ovb0004(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

// The slide's collision body (the object at +0x158 of the collision object named g_unk_717273b7c0),
// of kind 8: its half height at +0xb0.
struct UnkSlideBody_ovb0004 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual s32 vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual UnkVec3f vf_0x68();
    virtual void vf_0x70(const UnkVec3f* v);
    virtual void vf_0x78();
    virtual void vf_0x80();
    u8 pad_0x8[0xa8];
    f32 field_0xb0;
    u32 field_0xb4;
};

struct UnkSlideCollisionObject_ovb0004 {
    u8 pad_0x0[0x158];
    UnkSlideBody_ovb0004* body;
};

// What fn_7100976588 gives for the movement: its kind (+0x18), its name (+0x20) and its body.
struct UnkSlideShape_ovb0004 {
    u8 pad_0x0[0x18];
    void* kind;
    UnkStringData* name;
    u8 pad_0x28[0x130];
    UnkSlideBody_ovb0004* body;
};

// What the id g_unk_717273b7c0 names: the kind a shape is compared with at +0x30.
struct UnkSlideKind_ovb0004 {
    u8 pad_0x0[0x30];
    void* kind;
};

struct UnkSlideMorph_ovb0004 {
    u8 pad_0x0[0x349];
    bool active;
};

struct UnkSlideOwner_ovb0004 {
    void** vtable;
    u8 pad_0x8[0x1b8];
    UnkSlideMorph_ovb0004* morph;  // 0x1c0
    u8 pad_0x1c8[0x58];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x48];
    UnkInventory* inventory;  // 0x270
    u8 pad_0x278[0x28];
    UnkCollisionContainer* collision;  // 0x2a0

    void vf_0xd8(const UnkVec3f* rotation, bool flag) {
        reinterpret_cast<void (*)(UnkSlideOwner_ovb0004*, const UnkVec3f*, bool)>(vtable[0xd8 / 8])(this, rotation,
                                                                                                   flag);
    }
};

struct UnkSlideMovement_ovb0004 {
    void** vtable;
    u8 pad_0x8[0xf1];
    bool field_0xf9;
    u8 pad_0xfa[0x5a];
    f32 field_0x154;
    u8 pad_0x158[0x1ec];
    f32 x;  // 0x344
    u8 pad_0x348[0xc];
    f32 y;  // 0x354
    u8 pad_0x358[0xc];
    f32 z;  // 0x364
    u8 pad_0x368[0x40];
    bool field_0x3a8;
    u8 pad_0x3a9[0x51a4 - 0x3a9];
    bool field_0x51a4;
    bool field_0x51a5;

    UnkCollisionObject* vf_0x2a8() {
        return reinterpret_cast<UnkCollisionObject* (*)(UnkSlideMovement_ovb0004*)>(vtable[0x2a8 / 8])(this);
    }
    void vf_0x2b0(UnkCollisionObject* object) {
        reinterpret_cast<void (*)(UnkSlideMovement_ovb0004*, UnkCollisionObject*)>(vtable[0x2b0 / 8])(this, object);
    }
};
static_assert(__builtin_offsetof(UnkSlideMovement_ovb0004, field_0x3a8) == 0x3a8, "layout");
static_assert(__builtin_offsetof(UnkSlideMovement_ovb0004, field_0x51a4) == 0x51a4, "layout");

struct UnkSlidePair_ovb0004 {
    u32 a;
    u32 b;
};

// The floor slide state as its handlers see it.
struct UnkSlideState_ovb0004 {
    virtual void vf_0();
    virtual void vf_1();
    virtual void vf_2();
    virtual void vf_3();
    virtual void vf_4();
    virtual void vf_5();
    virtual void vf_6();
    virtual void vf_7();
    virtual void vf_8();
    virtual void vf_9();
    virtual void vf_10();
    virtual void vf_11();
    virtual void vf_12();
    virtual void vf_13();
    virtual void vf_14();
    virtual void vf_15();
    virtual void vf_16();
    virtual void vf_17();
    virtual void vf_18();
    virtual void vf_19();
    virtual void vf_20();
    virtual void vf_21();
    virtual void vf_22();
    virtual void vf_23();
    virtual void vf_24();
    virtual void vf_25();
    virtual void vf_26();
    virtual void vf_27();
    virtual void vf_28();
    virtual void vf_29();
    virtual void vf_30();
    virtual void vf_31();
    virtual void vf_32();
    virtual void vf_33();
    virtual void vf_34();
    virtual void vf_35();
    virtual void vf_36();
    virtual void vf_37();
    virtual bool vf_38(bool a, bool b, void* c);
    UnkAirTarget_B71a6* target;  // 0x8
    u8 pad_0x10[0x20];
    UnkSlideOwner_ovb0004* owner;  // 0x30
    UnkSlideMovement_ovb0004* movement;  // 0x38
    u8 pad_0x40[0x14c];
    f32 field_0x18c;
    f32 field_0x190;  // the slide's height
    u32 field_0x194;
    s32 state;  // 0x198
    UnkSlidePair_ovb0004 field_0x19c;
    u8 pad_0x1a4[0x10];
    f32 field_0x1b4;
    u8 pad_0x1b8[0xc];
    bool field_0x1c4;
    u8 pad_0x1c5[0x7];
    f32 field_0x1cc;
    f32 field_0x1d0;
    f32 field_0x1d4;
    UnkStrIdPathPair ids;  // 0x1d8
    u8 pad_0x1e8[0x8];
    u32 field_0x1f0;
    bool field_0x1f4;
    u8 pad_0x1f5[0x3];
    u8 timer[0xb0];  // 0x1f8 (fn_710002fa90 restarts it)
    u32 field_0x2a8;
};
static_assert(__builtin_offsetof(UnkSlideState_ovb0004, field_0x1cc) == 0x1cc, "layout");
static_assert(__builtin_offsetof(UnkSlideState_ovb0004, ids) == 0x1d8, "layout");
static_assert(__builtin_offsetof(UnkSlideState_ovb0004, field_0x2a8) == 0x2a8, "layout");

void fn_710099230c(UnkSlideState_ovb0004* self);
void fn_7100992724(UnkSlideState_ovb0004* self);

// The slide body (kind 8) of the collision object named g_unk_717273b7c0, or null.
static inline UnkSlideBody_ovb0004* FindBody_ovb0004(UnkSlideState_ovb0004* self,
                                                    UnkSlideCollisionObject_ovb0004** out) {
    auto* object = reinterpret_cast<UnkSlideCollisionObject_ovb0004*>(
        fn_71008640a4(self->owner->collision, &g_unk_717273b7c0));
    *out = object;
    if (!object)
        return nullptr;
    UnkSlideBody_ovb0004* body = object->body;
    if (!body || body->vf_0x18() != 8)
        return nullptr;
    return body;
}

// Writes the slide's half height (+0x190) and its offset (+0x18c) to the body.
static inline void ApplyBody_ovb0004(UnkSlideState_ovb0004* self, UnkSlideBody_ovb0004* body) {
    body->field_0xb0 = self->field_0x190 * 0.5f;
    body->vf_0x80();
    UnkVec3f v = body->vf_0x68();
    v.y = self->field_0x18c;
    body->vf_0x70(&v);
}

// Morph ball: the movement may roll (fn_710024fbc8), then state 4.
static inline void TryMorph_ovb0004(UnkSlideState_ovb0004* self) {
    if (fn_71011428bc(self->owner->inventory, GetQualifiedId_ovb0004(g_unk_717275a8d8))) {
        UnkSlideMovement_ovb0004* movement = self->movement;
        if (fn_710024fbc8(movement)) {
            movement->field_0x51a4 = true;
            movement->field_0x51a5 = false;
        }
        self->state = 4;
    }
}

// ---- fn_7100996dcc: how far the slide still is from the settings' limit ----

f32 fn_7100996dcc(UnkSlideState_ovb0004* self, bool alt) {
    if (alt)
        return g_unk_7172740de0.field_0x28 - self->field_0x1d4;
    return g_unk_7172740de0.field_0x24 - self->field_0x1cc;
}

// ---- fn_7100996e00: whether the body no longer has the slide's height ----

bool fn_7100996e00(UnkSlideState_ovb0004* self) {
    if (self->movement->field_0xf9 && !fn_710117b348(self->owner->animation, fn_7100cf919c())) {
        auto* shape = reinterpret_cast<UnkSlideShape_ovb0004*>(
            fn_7100976588(reinterpret_cast<UnkFlagged*>(self->movement)));
        if (shape->name != g_unk_717273b7c0.data)
            return false;
        UnkSlideBody_ovb0004* body = shape->body;
        UnkSlideBody_ovb0004* box = body->vf_0x18() == 8 ? body : nullptr;
        f32 size = box->field_0xb0 * 2.0f;
        f32 value = self->field_0x190;
        return !(size - 0.001f <= value && size + 0.001f >= value);
    }
    return fn_71009b2d0c(reinterpret_cast<CSamusMovementState*>(self));
}

// ---- fn_710099674c: the id of the slide animation ----

const CStrId& fn_710099674c(UnkSlideState_ovb0004* self) {
    if (self->movement->field_0xf9)
        return GameManagerIdTable().field_0x17a0;
    UnkAnimationComponent* animation = self->owner->animation;
    if (animation && fn_710117b348(animation, fn_7100cf8268()))
        return GameManagerIdTable().field_0x1770;
    return g_unk_717273b7d0;
}

// ---- fn_7100994a30: the morph ball check of the update ----

void fn_71009b2170(UnkSlideState_ovb0004* self, const u32* state);

void fn_7100994a30(UnkSlideState_ovb0004* self) {
    if (fn_71010d1598(reinterpret_cast<UnkGroundPlayer_b52*>(self->movement)))
        self->field_0x2a8 = g_unk_7172740de0.field_0x34;
    auto* shape = reinterpret_cast<UnkSlideShape_ovb0004*>(
        fn_7100976588(reinterpret_cast<UnkFlagged*>(self->movement)));
    void* shape_kind = shape->kind;
    auto* kind = reinterpret_cast<const UnkSlideKind_ovb0004*>(g_unk_717273b7c0.data);
    if (kind->kind == shape_kind && !self->vf_38(false, false, nullptr)) {
        UnkInventory* inventory = self->owner->inventory;
        if (fn_71011428bc(inventory, GetQualifiedId_ovb0004(g_unk_717275a8d8))) {
            UnkSlideMovement_ovb0004* movement = self->movement;
            if (fn_710024fbc8(movement)) {
                movement->field_0x51a4 = true;
                movement->field_0x51a5 = false;
            }
            self->state = 4;
        }
    }
    if (!self->movement->field_0x51a4) {
        alignas(8) u32 next = 1;  // an 8-byte aligned slot, as in the original
        fn_71009b2170(self, &next);
    }
}

// ---- fn_710099230c: fits the body to the slide, or morphs ----

void fn_710099230c(UnkSlideState_ovb0004* self) {
    UnkSlideCollisionObject_ovb0004* object;
    UnkSlideBody_ovb0004* body = FindBody_ovb0004(self, &object);
    if (!body)
        return;
    if (self->vf_38(false, false, nullptr))
        ApplyBody_ovb0004(self, body);
    else
        TryMorph_ovb0004(self);
}

// ---- fn_7100992044: enter ----

void fn_7100992044(UnkSlideState_ovb0004* self, s32 mode) {
    fn_710099d6f0(reinterpret_cast<UnkStandState_A6_b70*>(self));
    self->state = 1;
    UnkSlideCollisionObject_ovb0004* object;
    if (UnkSlideBody_ovb0004* body = FindBody_ovb0004(self, &object)) {
        self->movement->vf_0x2b0(reinterpret_cast<UnkCollisionObject*>(object));
        UnkVec3f v = body->vf_0x68();
        self->field_0x18c = v.y;
        self->field_0x190 = body->field_0xb0 + body->field_0xb0;
        self->field_0x194 = body->field_0xb4;
    }
    self->field_0x19c = UnkSlidePair_ovb0004();
    self->field_0x1b4 = -1.0f;
    self->field_0x1c4 = false;
    self->field_0x1f0 = 0;
    self->field_0x1f4 = true;
    if (fn_71010d1598(reinterpret_cast<UnkGroundPlayer_b52*>(self->movement))) {
        if (mode == 1)
            self->field_0x2a8 = g_unk_7172740de0.field_0x34;
        else if (mode == 2)
            self->field_0x2a8 = g_unk_7172740de0.field_0x38;
        f32 facing = self->movement->field_0x154;
        UnkVec3f rotation(0.0f, (s32)((facing > 0.0f) - (facing < 0.0f)) * 1.5707964f, 0.0f);
        self->owner->vf_0xd8(&rotation, true);
    }
}

// ---- fn_71009921a8: exit ----

void fn_710099d734(UnkSlideState_ovb0004* self);

void fn_71009921a8(UnkSlideState_ovb0004* self) {
    fn_710099d734(self);
    fn_710099230c(self);
    UnkSlideCollisionObject_ovb0004* found;
    if (UnkSlideBody_ovb0004* body = FindBody_ovb0004(self, &found))
        ApplyBody_ovb0004(self, body);
    UnkSlideMovement_ovb0004* movement = self->movement;
    if (movement->field_0x3a8) {
        if (UnkCollisionObject* object = movement->vf_0x2a8())
            self->movement->vf_0x2b0(object);
    } else {
        if (UnkCollisionObject* object = fn_71008640a4(self->owner->collision, &g_unk_717273b7b8))
            self->movement->vf_0x2b0(object);
    }
    self->state = 0;
    if (self->ids.id.data != &g_unk_7101d47108->empty && self->ids.path.data != &g_unk_7101d47200->empty)
        fn_7100d7c548(reinterpret_cast<UnkActor*>(self->owner), &self->ids, 0.2f, 0.0f);
    fn_710002fa90(self->timer);
}

// ---- fn_71009965d4: a request while sliding ----

struct UnkSlideRequest_ovb0004 {
    u8 pad_0x0[0xa0];
    CStrId id;
};

bool fn_71009965d4(UnkSlideState_ovb0004* self, UnkSlideRequest_ovb0004* request) {
    UnkSlideMovement_ovb0004* movement = self->movement;
    UnkVec3f v(movement->x, movement->y, movement->z);
    if (self->vf_38(false, true, &v)) {
        if (request->id.data == &g_unk_7101d47108->empty)
            request->id = g_unk_717273b7c8;
        fn_710099230c(self);
        return false;
    }
    TryMorph_ovb0004(self);
    return true;
}

// ---- fn_7100992724: grows the body back ----

void fn_7100992724(UnkSlideState_ovb0004* self) {
    UnkSlideCollisionObject_ovb0004* found;
    UnkSlideBody_ovb0004* body = FindBody_ovb0004(self, &found);
    if (!body)
        return;
    f32 size = body->field_0xb0 + body->field_0xb0;
    f32 height = self->field_0x190;
    bool fits = size - 0.001f <= height && size + 0.001f >= height;
    UnkAnimationComponent* animation = self->owner->animation;
    bool shrinking = animation && fn_710117b348(animation, fn_7100cf8c88());
    if (fits) {
        if (shrinking) {
            body->field_0xb0 = 0.0f;
            body->vf_0x80();
            UnkVec3f offset(0.0f, self->field_0x18c + self->field_0x190 * -0.5f, 0.0f);
            body->vf_0x70(&offset);
        }
        return;
    }
    if (shrinking)
        return;
    animation = self->owner->animation;
    if (animation && fn_710117b348(animation, fn_7100cf8778()) && !self->vf_38(false, false, nullptr))
        return;
    fn_710099230c(self);
}

// ---- fn_71009954ec: the transition the state reports ----

struct __attribute__((packed, aligned(4))) UnkTrackBlend {
    u64 kind;
    f32 value;
};
static_assert(sizeof(UnkTrackBlend) == 0xc, "UnkTrackBlend layout");

struct UnkTrackOptional {
    u64 bits;
};

struct UnkTrackTiming {
    s64 id;
    UnkTrackOptional duration;
};

struct UnkTrackTransition {
    u64 state;
    UnkTrackTiming timing;
    UnkTrackBlend blend;
    u32 flags;
};

// A string id's data: its hash at +0x30.
struct UnkStrIdHash_ovb0004 {
    u8 pad_0x0[0x30];
    s64 hash;
};

static inline bool IsMorphing_ovb0004(UnkSlideState_ovb0004* self) {
    UnkSlideOwner_ovb0004* owner = self->owner;
    return owner && owner->morph && owner->morph->active;
}

static inline UnkTrackQuery Query_ovb0004(const base::reflection::CClass* cls) {
    UnkTrackQuery query;
    query.cls = cls;
    query.result = nullptr;
    query.value = -1.0f;
    return query;
}

bool fn_71009954ec(UnkSlideState_ovb0004* self, UnkTrackTransition* out) {
    UnkTrackQuery air = Query_ovb0004(fn_7100cf75b8());
    UnkTrackQuery converting = Query_ovb0004(fn_7100cf2ae8());
    UnkTrackQuery fall = Query_ovb0004(fn_7100c1b388());
    if (self->owner->animation) {
        fn_710117b08c(self->owner->animation, &air);
        fn_710117b08c(self->owner->animation, &converting);
        fn_710117b08c(self->owner->animation, &fall);
        if (converting.result) {
            if (!IsMorphing_ovb0004(self))
                return false;
            out->state = 0x48dddc8e555ed5fe;
            out->blend = g_unk_7172769758;
            return true;
        }
    }
    if (self->movement->field_0xf9 && air.result) {
        s64 hash = reinterpret_cast<UnkStrIdHash_ovb0004*>(air.result->sources.data)->hash;
        out->state = hash == -1 ? 0xc3bac003d160a7f4 : hash;
    } else if (IsMorphing_ovb0004(self)) {
        out->state = 0x48dddc8e555ed5fe;
    } else {
        auto* player = reinterpret_cast<UnkGroundPlayer_b52*>(self->movement);
        if (fall.result && !fn_71010d1598(player))
            out->state = 0x84b38bc15c2c4d9e;
        else
            out->state = fn_71010d1598(reinterpret_cast<UnkGroundPlayer_b52*>(self->movement)) ? 0x69e003f99cedcb05
                                                                                              : 0x0f14d5402f02345f;
    }
    return true;
}

// ---- fn_7100994c08: an event while sliding ----

struct UnkDesc16_ovb0004 {
    u64 a, b;
};

// A movement transition: an id (-1 for none), the two descriptors of UnkSpitSend.h and a mask.
struct UnkMoveTransition_ovb0004 {
    s64 id;
    UnkDesc16_ovb0004 desc;
    UnkTrackBlend blend;
    u32 field_0x24;
    u32 mask;

    UnkMoveTransition_ovb0004(UnkDesc16_ovb0004 d, UnkTrackBlend b, u32 m)
        : id(-1), desc(d), blend(b), field_0x24(0), mask(m) {}
    UnkMoveTransition_ovb0004()
        : UnkMoveTransition_ovb0004(*reinterpret_cast<const UnkDesc16_ovb0004*>(&g_unk_710164b958),
                                    *reinterpret_cast<const UnkTrackBlend*>(&g_unk_7172769740),
                                    UnkAll_SpitSend().value) {}
};
static_assert(sizeof(UnkMoveTransition_ovb0004) == 0x30, "layout");

// A 64-bit word at a 4-byte aligned offset.
typedef u64 UnkU64Align4_ovb0004 __attribute__((aligned(4)));

// CSamusMovementState::CParams (vtable g_unk_7101a22cb8) with the floor slide's vtable.
struct UnkSlideParams_ovb0004 {
    void* vtable;
    UnkMoveTransition_ovb0004 transition;  // 0x8
    u32 field_0x38;
    UnkU64Align4_ovb0004 field_0x3c;
    u32 field_0x44;

    UnkSlideParams_ovb0004() : vtable(reinterpret_cast<char*>(g_unk_7101a22cb8) + 0x10), transition() {
        field_0x38 = 3;
        field_0x3c = 0;
        field_0x44 = 0;
        vtable = reinterpret_cast<char*>(g_unk_7101a22d18) + 0x10;
    }
};
static_assert(__builtin_offsetof(UnkSlideParams_ovb0004, field_0x44) == 0x44, "layout");
static_assert(sizeof(UnkSlideParams_ovb0004) == 0x48, "layout");

void fn_71009b1ae8(UnkSlideState_ovb0004* self, const u64* event);
void fn_7100994db8(UnkSlideState_ovb0004* self, bool flag);

void fn_7100994c08(UnkSlideState_ovb0004* self, const u64* event) {
    fn_71009b1ae8(self, event);
    fn_710099230c(self);
    if (*event == 0xdcd5f5354a260c14) {
        UnkSlideParams_ovb0004 params;
        fn_710098ae04(self->target, reinterpret_cast<UnkAirRequest_B71a6*>(&params));
        return;
    }
    if (self->state == 5 || self->state == 6) {
        fn_7100994db8(self, true);
        return;
    }
    self->state = 3;
}

// ---- fn_7100991e30: the motion's end time over a timeline ----

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

// The motion fn_71007142e8 initialises: the time it ends on at +0x2c.
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

f32 fn_7100991e30(void* self, UnkTimeline* timeline) {
    UnkStruct_71007142e8 motion;
    fn_71007142e8(&motion);
    UnkMotionTracks* tracks = timeline->tracks;
    UnkFlags_71007142e8 flags = UnkFlagsAll_71007142e8();
    fn_7100b8b3bc(timeline, &motion, tracks, &flags.bits, 0.0f, timeline->frame_count);
    return motion.field_0x2c;
}

// ---- fn_7100991b30, fn_7100991d0c: constructor and destructor body ----

struct UnkPathIds;
struct UnkInlineString;

// The ids the builder fills in place (the members start out unset), destroyed in reverse order
// (as in src/base/UnkStrIdPathResource_Ctor_b45.cpp).
struct UnkBuiltPathIds_ovb0004 {
    union {
        CStrId id;
    };
    union {
        base::UnkStrIdMemberOf<g_unk_7101d47200> path;
    };

    explicit UnkBuiltPathIds_ovb0004(const UnkInlineString* source) {
        fn_710001f074(reinterpret_cast<UnkPathIds*>(this), source);
    }
    ~UnkBuiltPathIds_ovb0004() {
        path.~UnkStrIdMemberOf();
        id.~CStrId();
    }
};

// The unit's default path text, with its use count.
struct UnkPathSource_ovb0004 {
    u8 pad_0x0[0x10];
    u8 text[0x34];  // 0x10
    union {
        u32 use_count;  // 0x44
    };
};
extern UnkPathSource_ovb0004* g_unk_717273b7d8 __attribute__((visibility("hidden")));

extern void* g_unk_7101a86930[];

// The state's memory as its constructor and destructor write it.
struct UnkFloorSlideStateData_ovb0004 {
    void** vtable;
    u8 pad_0x8[0x8];
    s32 kind;  // 0x10
    u8 pad_0x14[0x178];
    f32 field_0x18c;
    f32 field_0x190;
    u32 field_0x194;
    s32 state;  // 0x198
    u32 field_0x19c;
    u32 field_0x1a0;
    f32 field_0x1a4;
    u32 field_0x1a8;
    u32 field_0x1ac;
    u32 field_0x1b0;
    f32 field_0x1b4;
    u32 field_0x1b8;
    u32 field_0x1bc;
    u32 field_0x1c0;
    bool field_0x1c4;
    u64 field_0x1c8;
    u64 field_0x1d0;
    UnkStrIdPathResource path;  // 0x1d8
    u32 field_0x1f0;
    bool field_0x1f4;
    u8 pad_0x1f5[0x3];
    u8 timer[0xb0];  // 0x1f8
    u32 field_0x2a8;
    bool field_0x2ac;
};
static_assert(__builtin_offsetof(UnkFloorSlideStateData_ovb0004, timer) == 0x1f8, "layout");
static_assert(__builtin_offsetof(UnkFloorSlideStateData_ovb0004, field_0x2ac) == 0x2ac, "layout");

void fn_710099d370(UnkFloorSlideStateData_ovb0004* self);
void fn_710099d3ac(UnkFloorSlideStateData_ovb0004* self);

void fn_7100991b30(UnkFloorSlideStateData_ovb0004* self) {
    fn_710099d370(self);
    self->vtable = g_unk_7101a86930 + 2;
    self->field_0x18c = 0.0f;
    self->field_0x190 = 0.0f;
    self->field_0x194 = 0;
    self->state = 0;
    self->field_0x19c = 0;
    self->field_0x1a0 = 0;
    self->field_0x1a4 = 1.0f;
    self->field_0x1a8 = 0;
    self->field_0x1ac = 0;
    self->field_0x1b0 = 0;
    self->field_0x1b4 = -1.0f;
    self->field_0x1b8 = 0;
    self->field_0x1bc = 0;
    self->field_0x1c0 = 0;
    self->field_0x1c4 = false;
    self->field_0x1c8 = 0;
    self->field_0x1d0 = 0;
    new (&self->path.ids.id) CStrId(g_unk_7101d470e0);
    new (&self->path.ids.path) base::global::CFilePathStrId(g_unk_7101d470e8);
    self->path.resource = nullptr;
    self->field_0x1f0 = 0;
    self->field_0x1f4 = false;
    fn_710002f4e4(self->timer);
    self->field_0x2a8 = 0;
    self->field_0x2ac = false;
    self->kind = 3;
    if (self->path.ids.id.data == &g_unk_7101d47108->empty || self->path.ids.path.data == &g_unk_7101d47200->empty) {
        UnkPathSource_ovb0004* source = g_unk_717273b7d8;
        ++source->use_count;
        fn_71000267f8(&self->path, reinterpret_cast<const UnkStrIdPathPair&>(static_cast<const UnkBuiltPathIds_ovb0004&>(
                                       UnkBuiltPathIds_ovb0004(reinterpret_cast<const UnkInlineString*>(source->text)))));
    }
}

// A string id's destructor on the given pool: decrement, then test the reloaded count.
static inline void ReleaseId_ovb0004(UnkStringData** id, UnkStringPool** global_pool) {
    if (UnkStringPool* pool = *global_pool) {
        if (*id && *id != &pool->empty) {
            --(*id)->ref_count;
            if ((*id)->ref_count == 0) {
                fn_71000803a4(pool, *id);
                pool = *global_pool;
            }
        }
        *id = &pool->empty;
    }
}

void fn_7100991d0c(void* p) {
    auto* self = static_cast<UnkFloorSlideStateData_ovb0004*>(p);
    self->vtable = &g_unk_7101a86930[2];
    fn_710002f67c(self->timer);
    fn_7100027044(&self->path);
    if (UnkCountedResource* resource = self->path.resource)
        if (--resource->ref_count == 0)
            fn_710001737c(resource);
    ReleaseId_ovb0004(&self->path.ids.path.data, &g_unk_7101d47200);
    ReleaseId_ovb0004(&self->path.ids.id.data, &g_unk_7101d47108);
    fn_710099d3ac(self);
}
