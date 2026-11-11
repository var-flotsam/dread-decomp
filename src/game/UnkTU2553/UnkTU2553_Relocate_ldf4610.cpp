// CGameManager unit (tools/tu_map.py). fn_7100df4610: moves the second tracked actor to where the first one
// stands (plus the first one's movement offset), puts it on the ground below and hands it the first one's
// attached children. Names and layouts are placeholders.

#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CAbilityShinespark.h"
#include "game/CCharacterMovement.h"
#include "game/CMovementComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1501.h"
#include "game/UnkTU2553.h"
#include "types.h"

struct UnkFlagged;
struct UnkShape;
struct UnkAbility;
struct UnkAbilitySet;
struct UnkSamusMovement_sm42;
struct UnkMovement_71010d08d4;
struct UnkMovementVec_71010d08d4;
struct UnkCharacterMovement;
struct UnkCollisionHit;
struct CVector3D;

struct UnkVec3_sm42 {
    f32 x, y, z;
    UnkVec3_sm42(f32 x, f32 y, f32 z) : x(x), y(y), z(z) {}
    UnkVec3_sm42(const UnkVec3_sm42& o) : x(o.x), y(o.y), z(o.z) {}
};

struct UnkRelocateVec {
    f32 x, y, z;
    UnkRelocateVec(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
};

// An object that counts its references and goes away (vf_0x10) with the last one.
struct UnkRelocateCounted {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;
};

// A counted reference that lets go of its object when it dies.
struct UnkRelocateRef {
    UnkRelocateCounted* object;

    UnkRelocateRef() : object(nullptr) {}
    ~UnkRelocateRef() {
        if (object && --object->ref_count == 0)
            object->vf_0x10();
    }
};

// A collision hit as fn_7100977e50 fills it (0x48 bytes).
struct UnkRelocateHit {
    UnkRelocateRef object;
    UnkRelocateRef field_0x8;
    s32 id;
    UnkRelocateVec point;
    UnkRelocateVec field_0x20;
    UnkRelocateVec normal;
    u32 field_0x38;
    u32 flags;
    u32 field_0x40;

    UnkRelocateHit()
        : id(-1), point(0.0f, 0.0f, 0.0f), field_0x20(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), field_0x38(0),
          flags(0), field_0x40(0) {}
};
static_assert(sizeof(UnkRelocateHit) == 0x48, "layout");

// A state object: what it is doing is the id at +0x10.
struct UnkRelocateState {
    u8 pad_0x0[0x10];
    s32 id;
};

// An ability: its own state, or (state 7) the one of the state object it runs.
struct UnkRelocateAbility {
    virtual const base::reflection::CType* vf_0x0();
    u8 pad_0x8[0x18];
    UnkRelocateState* field_0x20;
    u8 pad_0x28[0x8c];
    s32 field_0xb4;
};
static_assert(__builtin_offsetof(UnkRelocateAbility, field_0xb4) == 0xb4, "layout");

struct UnkRelocateAbilitySet;
struct UnkRelocateAbilitySetVtbl {
    void* pad_0x0[0x2e];
    void (*vf_0x170)(UnkRelocateAbilitySet* self);
};
struct UnkRelocateAbilitySet {
    UnkRelocateAbilitySetVtbl* vtable;
};

// The first actor's movement component.
struct UnkRelocateSourceMovement {
    u8 pad_0x0[0x58];
    u8 field_0x58[0x180];  // a transform
    u8 field_0x1d8[0x51c];  // a vector
    u8 field_0x6f4;
    u8 pad_0x6f5[0x2613];
    UnkRelocateState* field_0x2d08;
};
static_assert(__builtin_offsetof(UnkRelocateSourceMovement, field_0x6f4) == 0x6f4, "layout");
static_assert(__builtin_offsetof(UnkRelocateSourceMovement, field_0x2d08) == 0x2d08, "layout");

// The second actor's movement component.
struct UnkRelocateMovement;
struct UnkRelocateMovementVtbl {
    void* pad_0x0[0x41];
    void (*vf_0x208)(UnkRelocateMovement* self, const void* transform);
    void* pad_0x210;
    void (*vf_0x218)(UnkRelocateMovement* self, const UnkRelocateHit* ground);
    void* pad_0x220[8];
    void (*vf_0x260)(UnkRelocateMovement* self);
};
struct UnkRelocateMovement {
    UnkRelocateMovementVtbl* vtable;
    u8 pad_0x8[0xf1];
    u8 field_0xf9;  // set while it is in the air
    u8 pad_0xfa[0x2d6];
    u32 field_0x3d0;  // the collision mask
    u8 pad_0x3d4[0x215c];
    u32 field_0x2530;
};
static_assert(__builtin_offsetof(UnkRelocateMovement, field_0x3d0) == 0x3d0, "layout");
static_assert(__builtin_offsetof(UnkRelocateMovement, field_0x2530) == 0x2530, "layout");

struct UnkRelocateChild {
    u8 pad_0x0[0x1e8];
    void* field_0x1e8;
};

struct UnkRelocateActor {
    u8 pad_0x0[0x44];
    f32 field_0x44;  // translation x
    u8 pad_0x48[0xc];
    f32 field_0x54;  // translation y
    u8 pad_0x58[0xc];
    f32 field_0x64;  // translation z
    u8 pad_0x68[0x158];
    void* field_0x1c0;
    u8 pad_0x1c8[0x10];
    void* field_0x1d8;
    u8 pad_0x1e0[0x28];
    UnkRelocateSourceMovement* field_0x208;
    UnkRelocateMovement* field_0x210;
    u8 pad_0x218[0xc8];
    UnkRelocateAbilitySet* field_0x2e0;
    u8 pad_0x2e8[0x38];
    u32 field_0x320;  // how many children are attached
    UnkRelocateChild** field_0x328;
};
static_assert(__builtin_offsetof(UnkRelocateActor, field_0x1c0) == 0x1c0, "layout");
static_assert(__builtin_offsetof(UnkRelocateActor, field_0x208) == 0x208, "layout");
static_assert(__builtin_offsetof(UnkRelocateActor, field_0x2e0) == 0x2e0, "layout");
static_assert(__builtin_offsetof(UnkRelocateActor, field_0x328) == 0x328, "layout");

struct UnkRelocateHandle {
    u8 pad_0x0[0x10];
    UnkRelocateActor* target;
};

// The game manager's tracked actors.
struct UnkRelocateManager {
    u8 pad_0x0[0x2998];
    UnkRelocateHandle* field_0x2998;
    UnkRelocateHandle* field_0x29a0;
    u8 pad_0x29a8[0x10];
    UnkRelocateHandle* field_0x29b8;  // the one in use
    u8 field_0x29c0;
};
static_assert(__builtin_offsetof(UnkRelocateManager, field_0x29c0) == 0x29c0, "layout");

bool fn_71009e12d0(void*);
UnkVec3_sm42 fn_71009e83fc(UnkSamusMovement_sm42* self);
void fn_7101167da0(char* p0);
char* fn_71008721dc(char* p0);
bool fn_7100977e50(UnkCharacterMovement* self, f32 max_dot, f32 extra, f32 step, f32 back, u32 mask,
                   const CVector3D* pos, CVector3D* out_point, UnkCollisionHit* out_hit, const CVector3D* dir);
void fn_7100a601cc(void* child, UnkRelocateActor* to, UnkRelocateActor* from);
void fn_7100a5fccc(UnkRelocateActor* actor);

static inline UnkRelocateActor* TargetOf(UnkRelocateHandle* handle) {
    return handle ? handle->target : nullptr;
}

// Whether the second actor lands exactly on the first one's position, without the movement's offset.
static inline bool LandsOnSpot(UnkRelocateManager* self, UnkRelocateSourceMovement* movement) {
    if (fn_71009e75e8(reinterpret_cast<UnkFlagged*>(movement)))
        return true;
    if (fn_71009e12d0(movement)) {
        UnkRelocateState* state = movement->field_0x2d08;
        if (!state || state->id != 6)
            return true;
    }
    UnkRelocateHandle* handle = self->field_0x29b8;
    if (handle && handle->target) {
        if (UnkRelocateAbilitySet* abilities = handle->target->field_0x2e0) {
            UnkRelocateAbility* ability = reinterpret_cast<UnkRelocateAbility*>(
                fn_71003013ec(reinterpret_cast<UnkAbilitySet*>(abilities), fn_7100327d14()));
            if (ability && fn_710009d50c(ability->vf_0x0(), fn_71003255a4())) {
                s32 state = ability->field_0xb4;
                if (state == 7) {
                    if (!ability->field_0x20)
                        return false;
                    state = ability->field_0x20->id;
                }
                if (state == 6)
                    return true;
            }
        }
    }
    return false;
}

// Looks for ground from `from` along `dir`, no further than `reach`.
static inline bool FindGround(UnkShape* shape, f32 max_dot, f32 reach, u32 mask, const UnkRelocateVec& from,
                              UnkRelocateHit* hit, const UnkRelocateVec& dir) {
    return fn_7100977e50(reinterpret_cast<UnkCharacterMovement*>(shape), max_dot, reach, 3.4028235e38f, 0.0f, mask,
                         reinterpret_cast<const CVector3D*>(&from), nullptr,
                         reinterpret_cast<UnkCollisionHit*>(hit), reinterpret_cast<const CVector3D*>(&dir));
}

// NON_MATCHING(minor): the block that zeroes the offset has the same five instructions in another order (the
// original copies the zero into the two other registers before the two stores; here they alternate).
void fn_7100df4610(UnkGameManager* manager) {
    UnkRelocateManager* self = reinterpret_cast<UnkRelocateManager*>(manager);
    self->field_0x29c0 = 1;
    UnkRelocateHandle* handle = self->field_0x29a0;
    UnkRelocateActor* actor;
    if (handle && (actor = handle->target)) {
        UnkRelocateMovement* movement = actor->field_0x210;
        movement->field_0x2530 = 0;
        if (TargetOf(self->field_0x29b8) != actor) {
            UnkRelocateActor* source = TargetOf(self->field_0x2998);
            if (UnkRelocateAbilitySet* abilities = source->field_0x2e0)
                abilities->vtable->vf_0x170(abilities);
            UnkRelocateSourceMovement* from = source->field_0x208;

            UnkVec3_sm42 offset = LandsOnSpot(self, from)
                                      ? UnkVec3_sm42(0.0f, 0.0f, 0.0f)
                                      : fn_71009e83fc(reinterpret_cast<UnkSamusMovement_sm42*>(from));
            UnkRelocateVec pos(source->field_0x44 + offset.x, source->field_0x54 + offset.y,
                               source->field_0x64 + offset.z);
            fn_7100df492c(g_unk_7101d47310, actor, &pos, nullptr);
            fn_71010d08d4(reinterpret_cast<UnkMovement_71010d08d4*>(movement),
                          reinterpret_cast<const UnkMovementVec_71010d08d4*>(from->field_0x1d8), true);
            movement->vtable->vf_0x208(movement, from->field_0x58);
            if (actor->field_0x1c0)
                fn_7101167da0(static_cast<char*>(actor->field_0x1c0));
            if (actor->field_0x1d8)
                fn_71008721dc(static_cast<char*>(actor->field_0x1d8));

            // Look for ground up to 20 units below the new position.
            UnkRelocateHit ground;
            if (FindGround(fn_7100976588(reinterpret_cast<UnkFlagged*>(movement)), -0.3f, 20.0f,
                           movement->field_0x3d0, pos, &ground, UnkRelocateVec(0.0f, -1.0f, 0.0f))) {
                movement->vtable->vf_0x218(movement, &ground);
            } else if (movement->field_0xf9 != 1) {
                movement->field_0xf9 = 1;
                movement->vtable->vf_0x260(movement);
            }
            from->field_0x6f4 = 0;

            while (source->field_0x320 != 0)
                fn_7100a601cc(source->field_0x328[0]->field_0x1e8, actor, source);
            fn_7100a5fccc(actor);
        }
    }
    self->field_0x29c0 = 0;
}
