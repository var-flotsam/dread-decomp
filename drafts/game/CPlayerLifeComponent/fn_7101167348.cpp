// CPlayerLifeComponent's unit (tools/tu_map.py unit 2716): reacts to a damage taken. It tells the owner's
// timeline, grab, gun and other parts about it, then either lets the AI part (+0x200) handle the damage or
// sends the movement (+0x208) the damage transition, with the hit effect and the id the damage names.
// Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game/CAutclastChargeAttack.h"
#include "game/CCentralUnitCannonBeamMovementComponent.h"
#include "game/CGrabComponent.h"
#include "game/CGunComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkModelManager.h"
#include "game/UnkTU134.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU2739.h"
#include "game/UnkTU785.h"
#include "game/UnkTimelineComponent.h"
#include "game/UnkXformNode.h"
#include "base/reflection/UnkClassGetters.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkGrabComponent_ovb0057;
struct UnkPlayerPart_b62a3;
struct UnkGun_ovb0091;
struct UnkMorphBallMovement;
struct UnkLifePart_l167348;
struct UnkLifeAim_l167348;
struct UnkLifeMovement_l167348;
struct UnkPlayerLife_l167348;
struct UnkLifeDamage_l167348;
void fn_710113b8cc(UnkGrabComponent_ovb0057* self);
bool fn_710083eb28(UnkPlayerPart_b62a3*);
void fn_710083f24c(UnkPlayerPart_b62a3* self);
void fn_7100839f68(UnkGun_ovb0091* self, const CStrId& id);
void fn_7100300ebc(UnkLifePart_l167348* self, UnkLifeDamage_l167348* damage);
void fn_7100347864(UnkLifeAim_l167348* self);
void fn_7100df3418(UnkGameManager* manager, bool a, bool b, void* x3);
void fn_7100947298(UnkMorphBallMovement* self, bool enable, f32 value);
void fn_710031433c(UnkLifeMovement_l167348* target, void* params);
UnkVec3f fn_7101158fa0(UnkPlayerLife_l167348* self, UnkLifeDamage_l167348* damage);
void fn_7101167aec(UnkPlayerLife_l167348* self, UnkLifeDamage_l167348* damage);

extern const u64 g_unk_710164d078;
extern const u64 g_unk_710164d080;
extern void* g_unk_7101a87320[];
extern __attribute__((visibility("hidden"))) UnkCls_710117af10* g_unk_7172a0a1a0;

struct __attribute__((packed, aligned(4))) UnkTrackBlend_l167348 {
    u64 kind;
    f32 value;
};
static_assert(sizeof(UnkTrackBlend_l167348) == 0xc, "layout");

struct UnkDesc16_l167348 {
    u64 a, b;
};

// A movement transition: an id (-1 for none), the two descriptors of UnkSpitSend.h and a mask.
struct UnkMoveTransition_l167348 {
    s64 id;
    UnkDesc16_l167348 desc;
    UnkTrackBlend_l167348 blend;
    u32 field_0x24;
    u32 mask;

    UnkMoveTransition_l167348(UnkDesc16_l167348 d, UnkTrackBlend_l167348 b, u32 m)
        : id(-1), desc(d), blend(b), field_0x24(0), mask(m) {}
    UnkMoveTransition_l167348()
        : UnkMoveTransition_l167348(*reinterpret_cast<const UnkDesc16_l167348*>(&g_unk_710164b958),
                                    *reinterpret_cast<const UnkTrackBlend_l167348*>(&g_unk_7172769740),
                                    UnkAll_SpitSend().value) {}
};
static_assert(sizeof(UnkMoveTransition_l167348) == 0x30, "layout");

typedef u64 UnkU64Align4_l167348 __attribute__((aligned(4)));

// CSamusMovementState::CParams (vtable g_unk_7101a22cb8) with a derived class's vtable.
struct UnkLifeParams_l167348 {
    void* vtable;
    UnkMoveTransition_l167348 transition;  // 0x8
    u32 field_0x38;
    UnkU64Align4_l167348 field_0x3c;
    u32 field_0x44;

    explicit UnkLifeParams_l167348(char* derived_vtable)
        : vtable(reinterpret_cast<char*>(g_unk_7101a22cb8) + 0x10), transition() {
        field_0x38 = 3;
        field_0x3c = 0;
        field_0x44 = 0;
        vtable = derived_vtable + 0x10;
    }
};
static_assert(sizeof(UnkLifeParams_l167348) == 0x48, "layout");

struct UnkLifeDamage_l167348 {
    u8 pad_0x0[0x26];
    bool field_0x26;
    bool field_0x27;
    u8 pad_0x28[0x44];
    UnkVec3f position;  // 0x6c
    u8 pad_0x78[0x28];
    CStrId id;  // 0xa0
    u8 pad_0xa8[0x10];
    void* field_0xb8;
    u8 pad_0xc0[0x8];
    s32 kind;  // 0xc8
    u8 pad_0xcc[0x4];
    f32 field_0xd0;

    bool IsKind3To5() const { return (u32)(kind - 3) < 3; }
};
static_assert(__builtin_offsetof(UnkLifeDamage_l167348, kind) == 0xc8, "layout");

struct UnkLifeAI_l167348 {
    void** vtable;
    u8 pad_0x8[0x7c0];
    f32 field_0x7c8;
    u8 pad_0x7cc[0x18d0 - 0x7cc];
    bool field_0x18d0;

    bool vf_0x3c8(UnkLifeDamage_l167348* damage) {
        return reinterpret_cast<bool (*)(UnkLifeAI_l167348*, UnkLifeDamage_l167348*)>(vtable[0x3c8 / 8])(this,
                                                                                                         damage);
    }
};
static_assert(__builtin_offsetof(UnkLifeAI_l167348, field_0x18d0) == 0x18d0, "layout");

struct UnkLifeMovementState_l167348 {
    u8 pad_0x0[0x10];
    s32 id;
};

struct UnkLifeMovement_l167348 {
    void** vtable;
    u8 pad_0x8[0xf1];
    bool field_0xf9;
    u8 pad_0xfa[0x2d08 - 0xfa];
    UnkLifeMovementState_l167348* state;  // 0x2d08
    u8 pad_0x2d10[0x51a4 - 0x2d10];
    bool field_0x51a4;

    bool vf_0x3b8() { return reinterpret_cast<bool (*)(UnkLifeMovement_l167348*)>(vtable[0x3b8 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkLifeMovement_l167348, state) == 0x2d08, "layout");
static_assert(__builtin_offsetof(UnkLifeMovement_l167348, field_0x51a4) == 0x51a4, "layout");

struct UnkLifeGun_l167348 {
    u8 pad_0x0[0x178];
    bool field_0x178;
};

struct UnkLifeOwner_l167348 {
    void** vtable;
    u8 pad_0x8[0x30];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x200 - 0x94];
    UnkLifeAI_l167348* ai;                  // 0x200
    UnkLifeMovement_l167348* movement;      // 0x208
    UnkMorphBallMovement* morph;            // 0x210
    u8 pad_0x218[0x8];
    UnkAnimationComponent* animation;       // 0x220
    u8 pad_0x228[0x38];
    UnkGrabComponent_ovb0057* grab;         // 0x260
    u8 pad_0x268[0x40];
    UnkLifeGun_l167348* gun;                // 0x2a8
    UnkLifeAim_l167348* aim;                // 0x2b0
    u8 pad_0x2b8[0x28];
    UnkLifePart_l167348* field_0x2e0;
};
static_assert(__builtin_offsetof(UnkLifeOwner_l167348, ai) == 0x200, "layout");
static_assert(__builtin_offsetof(UnkLifeOwner_l167348, field_0x2e0) == 0x2e0, "layout");

struct UnkLifeOwner2_l167348 {
    u8 pad_0x0[0x60];
    CStrId id;  // 0x60
};

// The hit effect's object: its position, angle and scale, then shown once.
struct UnkLifeFx_l167348 {
    void** vtable;
    void vf_0x10(const UnkVec3f* v) {
        reinterpret_cast<void (*)(UnkLifeFx_l167348*, const UnkVec3f*)>(vtable[0x10 / 8])(this, v);
    }
    void vf_0x18(const UnkVec3f* v) {
        reinterpret_cast<void (*)(UnkLifeFx_l167348*, const UnkVec3f*)>(vtable[0x18 / 8])(this, v);
    }
    void vf_0x20(const UnkVec3f* v) {
        reinterpret_cast<void (*)(UnkLifeFx_l167348*, const UnkVec3f*)>(vtable[0x20 / 8])(this, v);
    }
    void vf_0x90(bool v) { reinterpret_cast<void (*)(UnkLifeFx_l167348*, bool)>(vtable[0x90 / 8])(this, v); }
    void vf_0x98(f32 v) { reinterpret_cast<void (*)(UnkLifeFx_l167348*, f32)>(vtable[0x98 / 8])(this, v); }
};

struct UnkPlayerLife_l167348 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkLifeOwner_l167348* owner;  // 0x30
    u8 pad_0x38[0x300 - 0x38];
    CStrId id_0x300;
    u8 pad_0x308[0x20];
    CFilePathStrId path_0x328;
    u8 pad_0x330[0x19];
    bool field_0x349;
    u8 pad_0x34a[0x6];
    u8 damage_0x350[0x100];
    u8 pad_0x450[0x8];
    UnkLifeOwner2_l167348* owner2;  // 0x458
};
static_assert(__builtin_offsetof(UnkPlayerLife_l167348, path_0x328) == 0x328, "layout");
static_assert(__builtin_offsetof(UnkPlayerLife_l167348, owner2) == 0x458, "layout");

static inline bool IsEmpty(const CStrId& id) { return id.data == &g_unk_7101d47108->empty; }

static inline UnkModelManager* ModelManager() {
    UnkModelManager* manager = g_unk_71726b0cd8;
    if (!manager) {
        manager = static_cast<UnkModelManager*>(fn_71000839a0(0x1e8, 0x3f));
        fn_71011a870c(manager);
        g_unk_71726b0cd8 = manager;
    }
    return manager;
}

void fn_7101167348(UnkPlayerLife_l167348* self, UnkLifeDamage_l167348* damage) {
    const u64* timeline = damage->IsKind3To5() ? &g_unk_710164d078 : &g_unk_710164d080;
    fn_7100d7c828(reinterpret_cast<UnkActor*>(self->owner), timeline, false);
    if (self->owner->grab && fn_710113d83c(self->owner->grab))
        fn_710113b8cc(self->owner->grab);
    if (UnkLifeGun_l167348* gun = self->owner->gun) {
        if (gun->field_0x178)
            fn_7100839b00(reinterpret_cast<char*>(gun), false);
        if (fn_710083eb28(reinterpret_cast<UnkPlayerPart_b62a3*>(self->owner->gun)))
            fn_710083f24c(reinterpret_cast<UnkPlayerPart_b62a3*>(self->owner->gun));
    }
    if (UnkLifePart_l167348* part = self->owner->field_0x2e0)
        fn_7100300ebc(part, damage);
    if (UnkLifeAim_l167348* aim = self->owner->aim)
        fn_7100347864(aim);
    if (UnkAnimationComponent* animation = self->owner->animation) {
        if (fn_710117b348(animation, fn_7100cf2ff8()))
            fn_7100df3418(g_unk_7101d47310, true, false, nullptr);
    }

    if (self->owner->ai && self->owner->ai->vf_0x3c8(damage)) {
        if (self->owner->ai && damage->field_0xb8 == g_unk_71726dbf68)
            self->owner->ai->field_0x18d0 = true;
        UnkLifeOwner_l167348* owner = self->owner;
        if (!owner->movement || !owner->movement->field_0x51a4 || !damage->field_0x27 || !owner->ai)
            return;
        // The angles are brought up to date; the original does not use them here.
        owner->xform.GetAngle();
        if (!self->owner->movement || !self->owner->movement->vf_0x3b8())
            fn_7101158fa0(self, damage);
        self->field_0x349 = true;
        fn_7100400364(reinterpret_cast<UnkWeaponDamage_ovb0054*>(self->damage_0x350),
                      reinterpret_cast<UnkWeaponDamage_ovb0054*>(damage));
        return;
    }

    if (!self->owner->animation)
        return;
    UnkLifeParams_l167348 params(reinterpret_cast<char*>(g_unk_7101a87320));
    if (!IsEmpty(damage->id)) {
        params.transition.id = damage->id.data->Key();
        if (UnkLifeGun_l167348* gun = self->owner->gun)
            fn_7100839f68(reinterpret_cast<UnkGun_ovb0091*>(gun), GameManagerIdTable().field_0x1240);
    } else {
        const CStrId* id = &self->owner2->id;
        if (damage->IsKind3To5()) {
            if (!IsEmpty(self->id_0x300))
                id = &self->id_0x300;
            if (self->path_0x328.data != &g_unk_7101d47200->empty) {
                UnkModelManager* manager = ModelManager();
                UnkVec3f position = damage->position;
                UnkVec3f angle(0.0f, 0.0f, 0.0f);
                UnkVec3f scale(1.0f, 1.0f, 1.0f);
                if (UnkFxObject_ovb0041* object = fn_71003b7db0(manager, &self->path_0x328, false, true)) {
                    UnkLifeFx_l167348* fx = reinterpret_cast<UnkLifeFx_l167348*>(object);
                    fx->vf_0x10(&position);
                    fx->vf_0x18(&angle);
                    fx->vf_0x20(&scale);
                    fn_7100146dac(reinterpret_cast<UnkSwitchObject*>(object), true);
                    fx->vf_0x90(false);
                    fx->vf_0x98(1.0f);
                    fn_71011a9bb4(manager, object);
                }
            }
        }
        if (!IsEmpty(*id)) {
            params.transition.field_0x24 = damage->field_0x26 ? 10 : 2;
            params.transition.id = id->data->Key();
            bool special = fn_710117af10(reinterpret_cast<UnkStruct_710117af10*>(self->owner->animation),
                                         &g_unk_7172a0a1a0, false);
            UnkDesc16_l167348 desc = *reinterpret_cast<const UnkDesc16_l167348*>(&g_unk_710164b958);
            if (special) {
                desc.a = ~0ull;
                desc.b = 0x1bdf5c28full;
            }
            params.transition.desc = desc;
        }
    }
    if (UnkMorphBallMovement* morph = self->owner->morph)
        fn_7100947298(morph, false, -1.0f);
    if (UnkLifeMovement_l167348* movement = self->owner->movement) {
        if (movement->state && movement->state->id == 9) {
            if (movement->field_0xf9) {
                UnkLifeParams_l167348 state_params(g_unk_7101a22ce8);
                fn_710031433c(self->owner->movement, &state_params);
            } else {
                UnkLifeParams_l167348 state_params(reinterpret_cast<char*>(g_unk_7101a22d18));
                fn_710031433c(self->owner->movement, &state_params);
            }
        }
        f32 value = damage->field_0xd0;
        if (value > 0.0f && self->owner->ai)
            self->owner->ai->field_0x7c8 = value;
        fn_710031433c(self->owner->movement, &params);
    }
    if (damage->field_0x27)
        fn_7101167aec(self, damage);
}
