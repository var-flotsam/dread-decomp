// CUsableComponent's unit (tools/tu_map.py unit 1607): the use by an actor. When the component cannot be used
// it shows "ShowDialogCanNotUse"; otherwise it picks one of its two animation prefixes, stops what the user is
// doing, starts the side animation and the use, and lists itself. Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game/CGunComponent.h"
#include "game/CUsableComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU1730.h"
#include "game/UnkXformNode.h"
#include "../../game_s2/UnkSpitSend.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

struct CAbilityComponent;
struct UnkMovement2_ovb0048;
struct UnkUsable_laa0810;
struct UnkPickList_ovb0094;
struct UnkLinkGroups;
extern UnkLinkGroups g_unk_7172753260;
extern void* g_unk_7101a874c0[];

void fn_7100302180(CAbilityComponent* self);
UnkVec3f fn_71010d2b08(UnkMovement2_ovb0048* self);
void fn_7100aa0810(UnkUsable_laa0810* self, const CStrId* prefix, CStrId* side, CStrId* animation_id);
void fn_710082d0ac(UnkPickList_ovb0094* list, void* const& item);

// The two id slots of the game's id table this use tests the user's animation against.
struct UnkUsableIds_laa122c {
    u8 pad_0x0[0x15f0];
    CStrId field_0x15f0;
    u8 pad_0x15f8[0x1df0 - 0x15f8];
    CStrId field_0x1df0;
};

struct __attribute__((packed, aligned(4))) UnkTrackBlend_laa122c {
    u64 kind;
    f32 value;
};

struct UnkDesc16_laa122c {
    u64 a, b;
};

// A movement transition (as in CSamusFloorSlideMovementState's leave): an id, two descriptors and a mask.
struct UnkMoveTransition_laa122c {
    s64 id;
    UnkDesc16_laa122c desc;
    UnkTrackBlend_laa122c blend;
    u32 field_0x24;
    u32 mask;

    UnkMoveTransition_laa122c(UnkDesc16_laa122c d, UnkTrackBlend_laa122c b, u32 m)
        : id(-1), desc(d), blend(b), field_0x24(0), mask(m) {}
    UnkMoveTransition_laa122c()
        : UnkMoveTransition_laa122c(*reinterpret_cast<const UnkDesc16_laa122c*>(&g_unk_710164b958),
                                    *reinterpret_cast<const UnkTrackBlend_laa122c*>(&g_unk_7172769740),
                                    UnkAll_SpitSend().value) {}
};

typedef u64 UnkU64Align4_laa122c __attribute__((aligned(4)));

// CSamusMovementState::CParams (vtable g_unk_7101a22cb8) with the vtable of the state it asks for.
struct UnkUseParams_laa122c {
    void* vtable;
    UnkMoveTransition_laa122c transition;  // 0x8
    u32 field_0x38;
    UnkU64Align4_laa122c field_0x3c;
    u32 field_0x44;

    explicit UnkUseParams_laa122c(void* state_vtable)
        : vtable(reinterpret_cast<char*>(g_unk_7101a22cb8) + 0x10), transition() {
        field_0x38 = 3;
        field_0x3c = 0;
        field_0x44 = 0;
        vtable = state_vtable;
    }
};
static_assert(sizeof(UnkUseParams_laa122c) == 0x48, "layout");

struct UnkUsableMovement_laa122c;
void fn_71009e8664(UnkUsableMovement_laa122c* movement);
void fn_710031433c(UnkUsableMovement_laa122c* movement, UnkUseParams_laa122c* params);

struct UnkUsableSubState_laa122c {
    u8 pad_0x0[0x10];
    u32 kind;  // 0x10
};

// What the game manager's fn_7100df59cc returns: a state of a kind (7: the kind of its sub-state).
struct UnkUsableHud_laa122c {
    void** vtable;
    u8 pad_0x8[0x18];
    UnkUsableSubState_laa122c* sub;  // 0x20
    u8 pad_0x28[0x8c];
    u32 kind;  // 0xb4

    void vf_0xa0(bool on) { reinterpret_cast<void (*)(UnkUsableHud_laa122c*, bool)>(vtable[0xa0 / 8])(this, on); }
};
UnkUsableHud_laa122c* fn_7100df59cc(UnkGameManager* manager);

struct UnkUsableGrab_laa122c {
    u8 pad_0x0[0x50];
    UnkUsableSubState_laa122c* state;  // 0x50
};

struct UnkUsableUser_laa122c {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x1f0 - 0x94];
    UnkMovement2_ovb0048* movement_0x1f0;
    u8 pad_0x1f8[0x10];
    UnkUsableMovement_laa122c* movement;  // 0x208
    u8 pad_0x210[0x10];
    UnkStruct_710117af10* animation;  // 0x220
    u8 pad_0x228[0x80];
    char* gun;                    // 0x2a8
    UnkUsableGrab_laa122c* grab;  // 0x2b0
    u8 pad_0x2b8[0x28];
    CAbilityComponent* ability;  // 0x2e0
};
static_assert(__builtin_offsetof(UnkUsableUser_laa122c, ability) == 0x2e0, "layout");

struct UnkUsableState_laa122c {
    u8 pad_0x0[0x10];
    u32 state;  // 0x10
};

// Only the slots this use calls are typed; the others are placeholders.
struct UnkUsable_laa122c {
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
    virtual void vf_0x170(UnkUsableUser_laa122c* user);
    virtual void vf_0x178();
    virtual void vf_0x180();
    virtual bool vf_0x188(UnkUsableUser_laa122c* user);
    virtual void vf_0x190();
    virtual void vf_0x198();
    virtual void vf_0x1a0();
    virtual void vf_0x1a8();
    virtual void vf_0x1b0();
    virtual void vf_0x1b8();
    virtual const CStrId* vf_0x1c0();
    virtual const CStrId* vf_0x1c8();
    virtual void vf_0x1d0();
    virtual void vf_0x1d8();
    virtual void vf_0x1e0();
    virtual void vf_0x1e8();
    virtual const CStrId* vf_0x1f0();
    virtual void vf_0x1f8();
    virtual void vf_0x200();
    virtual void vf_0x208();
    virtual void vf_0x210();
    virtual void vf_0x218();
    virtual void vf_0x220();
    virtual CRntString vf_0x228();
    virtual void vf_0x230();
    virtual void vf_0x238();
    virtual void vf_0x240();
    virtual void vf_0x248();
    virtual void vf_0x250();
    virtual void vf_0x258();
    virtual void vf_0x260(const char* name, const char* text);
    virtual void vf_0x268();
    virtual void vf_0x270();
    virtual void vf_0x278();
    virtual void vf_0x280();
    virtual void vf_0x288();
    virtual void vf_0x290(UnkUsableUser_laa122c* user);
    virtual void vf_0x298();
    virtual void vf_0x2a0();
    virtual void vf_0x2a8();
    virtual void vf_0x2b0();
    virtual void vf_0x2b8();
    virtual void vf_0x2c0();
    virtual void vf_0x2c8();
    virtual void vf_0x2d0();
    virtual void vf_0x2d8();
    virtual void vf_0x2e0();
    virtual void vf_0x2e8();
    virtual void vf_0x2f0();
    virtual void vf_0x2f8();
    virtual void vf_0x300();
    virtual void vf_0x308();
    virtual void vf_0x310();
    virtual void vf_0x318();
    virtual void vf_0x320();
    virtual void vf_0x328();
    virtual void vf_0x330();
    virtual void vf_0x338();
    virtual void vf_0x340();
    virtual void vf_0x348();
    virtual void vf_0x350();
    virtual void vf_0x358();
    virtual void vf_0x360();
    virtual void vf_0x368();
    virtual void vf_0x370();
    virtual void vf_0x378();
    virtual void vf_0x380();
    virtual void vf_0x388();
    virtual void vf_0x390();
    virtual void vf_0x398();
    virtual void vf_0x3a0();

    u8 pad_0x8[0x108];
    UnkUsableState_laa122c* machine;  // 0x110
    u8 pad_0x118[0x508 - 0x118];
    u8 field_0x508;
};
static_assert(__builtin_offsetof(UnkUsable_laa122c, field_0x508) == 0x508, "layout");

static inline UnkUsableIds_laa122c& Ids() {
    return reinterpret_cast<UnkUsableIds_laa122c&>(GameManagerIdTable());
}

static inline bool IsEmpty(const CStrId& id) {
    return id.data == &g_unk_7101d47108->empty;
}

// The kind of the state the game manager shows, looking through a kind 7 to its sub-state.
static inline bool IsKind6(UnkUsableHud_laa122c* hud) {
    u32 kind = hud->kind;
    if (kind == 7) {
        if (!hud->sub)
            return false;
        kind = hud->sub->kind;
    }
    return kind == 6;
}

bool fn_7100aa122c(UnkUsable_laa122c* self, UnkUsableUser_laa122c* user) {
    if (!self->vf_0x188(user)) {
        CRntString text = self->vf_0x228();
        self->vf_0x260("ShowDialogCanNotUse", text.data ? text.data : g_unk_710162e2ed);
    } else {
        bool grabbed = user->grab && user->grab->state && user->grab->state->kind == 2;
        CStrId prefix = g_unk_7101d470e0;
        // The prefix the use animations start with: vf_0x1c0's or vf_0x1c8's.
        const CStrId* id;
        if (self->machine->state == 2) {
            u8 was = self->field_0x508;
            if (UnkStruct_710117af10* animation = user->animation) {
                if (fn_710117af10(animation, reinterpret_cast<UnkCls_710117af10**>(&Ids().field_0x15f0), false)) {
                    self->field_0x508 = false;
                    id = self->vf_0x1c8();
                } else {
                    bool use = (was != 0) | fn_710117af10(user->animation,
                                                          reinterpret_cast<UnkCls_710117af10**>(&Ids().field_0x1df0),
                                                          false);
                    self->field_0x508 = use;
                    id = use ? self->vf_0x1c0() : self->vf_0x1c8();
                }
            } else {
                id = was ? self->vf_0x1c0() : self->vf_0x1c8();
            }
        } else {
            if (grabbed)
                id = user->xform.GetAngle().y > 0.0f ? self->vf_0x1c0() : self->vf_0x1c8();
            else
                id = fn_71010d2b08(user->movement_0x1f0).x > 0.0f ? self->vf_0x1c0() : self->vf_0x1c8();
        }
        prefix = *id;

        if (!IsEmpty(prefix)) {
            self->vf_0x2d0();
            if (user->gun)
                fn_7100839b00(user->gun, false);
            if (CAbilityComponent* ability = user->ability) {
                fn_7100302180(ability);
                if (UnkUsableHud_laa122c* hud = fn_7100df59cc(g_unk_7101d47310))
                    if (IsKind6(hud))
                        hud->vf_0xa0(true);
            }
            if (UnkUsableMovement_laa122c* movement = user->movement) {
                fn_71009e8664(movement);
                self->vf_0x3a0();
                UnkUseParams_laa122c params(reinterpret_cast<char*>(g_unk_7101a874c0) + 0x10);
                fn_710031433c(movement, &params);
            }
            CStrId animation = g_unk_7101d470e0;
            CStrId side = g_unk_7101d470e0;
            fn_7100aa0810(reinterpret_cast<UnkUsable_laa0810*>(self), &prefix, &side, &animation);
            const CStrId* fallback = self->vf_0x1f0();
            const CStrId* a = IsEmpty(animation) ? &prefix : &animation;
            const CStrId* b = IsEmpty(*fallback) ? &prefix : fallback;
            u64 key = side.data->Key();
            if (fn_7100aa0b04(reinterpret_cast<UnkUsableVirt_A6_b62*>(self), reinterpret_cast<u64>(a),
                              reinterpret_cast<u64>(b), reinterpret_cast<const u8*>(&key)))
                self->vf_0x290(user);
        } else {
            self->vf_0x170(user);
        }
    }
    UnkUsable_laa122c* item = self;
    fn_710082d0ac(reinterpret_cast<UnkPickList_ovb0094*>(reinterpret_cast<u8*>(&g_unk_7172753260) + 0x100), item);
    return true;
}
