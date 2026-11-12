// CEnemyMovement's unit (unit 2686 of tools/tu_map.py): its constructor and destructor, update, message and
// contact handlers, direction and probe helpers, and the deferred pointer list next to it. The state machines
// at +0x700 and +0x838 and the listener at +0x1be8 are spelled out only as far as these functions touch them.
// Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "UnkClone.h"
#include "base/UnkMatrix44.h"
#include "base/UnkVector.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game/CEnemyMovement.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU1442.h"
#include "game/UnkTU1444.h"
#include "game/UnkXformNode.h"
#include "game/behaviortree/CSwitchBetweenAIModesAction.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

// fn_71010c7920: the constructor: after the base (fn_7100975d8c), the vtables, the two state machines
// (+0x700, none 0x7fffffff; +0x838, none 5), the three arrays at +0x7b8, the sub-objects and the listener
// at +0x1be8.
extern void* g_unk_7101cb0218[];
extern void* g_unk_7101cb05f0[];
extern void* g_unk_7101cb0720[];
extern void* g_unk_7101cb0688[];

struct UnkUnit1441;
struct UnkUnit1443;
struct UnkUnit1436;
void fn_7100975d8c(void* self);
void fn_7100969ff4(void* self, u32 kind);
void fn_710096939c(UnkUnit1441* self, u32 arg);
void fn_710097061c(UnkUnit1443* self, u32 arg);
void fn_71009714bc(void* self, u32 kind);
void fn_71009550dc(void* self, u32 kind);
void fn_71009558bc(void* self, u32 kind);
void fn_710095495c(void* self, u32 kind);
void fn_710095520c(UnkUnit1436* self, void* arg);

template <u32 None>
struct UnkStateMachineShell_t {
    void** vtable;
    u64 field_0x8;
    void* current;   // 0x10
    void* previous;  // 0x18
    u32 count;       // 0x20
    u8 states[0x80];
    u32 requested;  // 0xa4
    bool updating;  // 0xa8
    bool field_0xa9;

    void Init(void** vt) {
        field_0x8 = 0;
        current = nullptr;
        previous = nullptr;
        count = 0;
        requested = None;
        updating = false;
        field_0xa9 = false;
        vtable = vt;
    }
};
static_assert(__builtin_offsetof(UnkStateMachineShell_t<0>, requested) == 0xa4, "layout");

struct UnkVectorSet_t {
    void** vtable;
    base::UnkVector<void*> lists[3];  // 0x8, 0x30, 0x58
};

struct UnkVec3_t {
    f32 x, y, z;
};

struct UnkEnemyMovement_t {
    void** vtable;
    u8 pad_0x8[0x3c8 - 0x8];
    u32 field_0x3c8;
    u32 field_0x3cc;
    u32 field_0x3d0;
    u8 pad_0x3d4[0x6e8 - 0x3d4];
    void** vtable_0x6e8;  // 0x6e8
    bool field_0x6f0;
    bool field_0x6f1;
    u8 pad_0x6f2[0x2];
    u32 field_0x6f4;
    u32 field_0x6f8;
    bool field_0x6fc;
    UnkStateMachineShell_t<0x7fffffff> actions;  // 0x700
    UnkVectorSet_t vectors;                      // 0x7b0
    u32 field_0x830;
    UnkStateMachineShell_t<5> machine;  // 0x838
    u8 field_0x8e8[0x15b0 - 0x8e8];
    u8 field_0x15b0[0x16d0 - 0x15b0];
    u8 field_0x16d0[0x17a0 - 0x16d0];
    u8 field_0x17a0[0x19d0 - 0x17a0];
    void* field_0x19d0;
    void* field_0x19d8;
    void* field_0x19e0;
    void* field_0x19e8;
    void* field_0x19f0;
    bool field_0x19f8;
    bool field_0x19f9;
    UnkVec3_t field_0x19fc;
    u8 field_0x1a08[0x8];
    u8 field_0x1a10[0x48];
    u8 field_0x1a58[0x48];
    u8 field_0x1aa0[0x48];
    u8 field_0x1ae8[0x48];
    u8 field_0x1b30[0x48];
    u8 field_0x1b78[0x1bd0 - 0x1b78];
    void* field_0x1bd0;
    void* field_0x1bd8;
    bool field_0x1be0;
    bool field_0x1be1;
    void** listener_vtable;  // 0x1be8
    void* listener_context;  // 0x1bf0
    void* listener_callback;  // 0x1bf8
    void* listener_handle;    // 0x1c00
    u8 pad_0x1c08;
    bool field_0x1c09;
    u32 field_0x1c0c;
    bool field_0x1c10;
    bool field_0x1c11;
    bool field_0x1c12;
    void* field_0x1c18;
};
static_assert(__builtin_offsetof(UnkEnemyMovement_t, machine) == 0x838, "layout");
static_assert(__builtin_offsetof(UnkEnemyMovement_t, field_0x19fc) == 0x19fc, "layout");
static_assert(__builtin_offsetof(UnkEnemyMovement_t, field_0x1c18) == 0x1c18, "layout");

void fn_71010c7920(UnkEnemyMovement_t* self) {
    fn_7100975d8c(self);
    self->vtable = &g_unk_7101cb0218[2];
    self->vtable_0x6e8 = &g_unk_7101cb0218[0x3d0 / 8];
    self->field_0x6f0 = true;
    self->field_0x6f1 = true;
    self->field_0x6f4 = 0;
    self->field_0x6f8 = 0;
    self->field_0x6fc = true;
    self->actions.Init(&g_unk_7101cb05f0[2]);
    self->vectors.vtable = &g_unk_7101cb0720[2];
    new (&self->vectors.lists[0]) base::UnkVector<void*>(0xf);
    new (&self->vectors.lists[1]) base::UnkVector<void*>(0xf);
    new (&self->vectors.lists[2]) base::UnkVector<void*>(0xf);
    self->field_0x830 = 0;
    self->machine.Init(&g_unk_7101cb0688[2]);
    fn_7100969ff4(self->field_0x8e8, 1);
    fn_710096939c(reinterpret_cast<UnkUnit1441*>(self->field_0x15b0), 2);
    fn_710097061c(reinterpret_cast<UnkUnit1443*>(self->field_0x16d0), 3);
    fn_71009714bc(self->field_0x17a0, 4);
    self->field_0x19d0 = nullptr;
    self->field_0x19d8 = nullptr;
    self->field_0x19e0 = nullptr;
    self->field_0x19e8 = nullptr;
    self->field_0x19f0 = nullptr;
    self->field_0x19f8 = true;
    self->field_0x19f9 = true;
    self->field_0x19fc = {0.0f, 0.0f, 0.0f};
    fn_71009550dc(self->field_0x1a10, 1);
    fn_71009558bc(self->field_0x1a58, 2);
    fn_71009558bc(self->field_0x1aa0, 3);
    fn_71009558bc(self->field_0x1ae8, 4);
    fn_710095495c(self->field_0x1b30, 5);
    fn_710095520c(reinterpret_cast<UnkUnit1436*>(self->field_0x1b78), reinterpret_cast<void*>(6));
    self->field_0x1bd0 = nullptr;
    self->field_0x1bd8 = nullptr;
    self->field_0x1be0 = false;
    self->field_0x1be1 = false;
    self->listener_vtable = &g_unk_7101a22d48[2];
    self->listener_context = nullptr;
    self->listener_callback = nullptr;
    self->listener_handle = nullptr;
    self->field_0x1c09 = true;
    self->field_0x1c0c = 5;
    self->field_0x1c10 = true;
    self->field_0x1c11 = false;
    self->field_0x1c12 = false;
    self->field_0x1c18 = nullptr;
    self->field_0x3c8 = 3;
    self->field_0x3cc = 0x13;
    self->field_0x3d0 = 3;
}

// fn_71010c7b70: the destructor, the constructor's work undone in reverse before the base's (fn_7100975f10).
extern void* g_unk_7101cb0218[];
extern void* g_unk_7101cb06e0[];
extern void* g_unk_7101cb0720[];
extern void* g_unk_7101cb0648[];

struct UnkUnit1444;
void fn_7100971574(UnkUnit1444* self);
void fn_7100955368(void* self);
void fn_7100954990(void* self);
void fn_71009558f0(void* self);
void fn_7100955110(void* self);
void fn_7100970680(void* self);
void fn_7100969424(void* self);
void fn_710096a110(void* self);
void fn_7100975f10(void* self);

struct UnkStateMachineShell_s {
    void** vtable;
    u8 pad_0x8[0x8];
    void* current;   // 0x10
    void* previous;  // 0x18
    u32 count;       // 0x20
    u8 states[0x88];

    void Destroy(void** vt) {
        vtable = vt;
        current = nullptr;
        previous = nullptr;
        count = 0;
    }
};

struct UnkVectorSet_s {
    void** vtable;
    base::UnkVector<void*> lists[3];  // 0x8, 0x30, 0x58
};

struct UnkTrackListener_s {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkWeakHandle* handle;  // 0x18

    // Detaches from the tracked object and drops the handle's reference.
    void Destroy() {
        vtable = &g_unk_7101a22d48[2];
        UnkWeakHandle* h = handle;
        if (!h)
            return;
        if (UnkWeakTarget* target = h->target) {
            fn_710117b3f0(target, this);
            h = handle;
            if (!h)
                return;
        }
        if (--h->ref_count == 0)
            h->vf_0x10();
    }
};

struct UnkEnemyMovement_s {
    void** vtable;
    u8 pad_0x8[0x6e8 - 0x8];
    void** vtable_0x6e8;  // 0x6e8
    u8 pad_0x6f0[0x700 - 0x6f0];
    UnkStateMachineShell_s actions;  // 0x700
    UnkVectorSet_s vectors;  // 0x7b0
    u8 pad_0x830[0x8];
    UnkStateMachineShell_s machine;  // 0x838
    u8 field_0x8e8[0x15b0 - 0x8e8];
    u8 field_0x15b0[0x16d0 - 0x15b0];
    u8 field_0x16d0[0x17a0 - 0x16d0];
    u8 field_0x17a0[0x1a10 - 0x17a0];
    u8 field_0x1a10[0x48];
    u8 field_0x1a58[0x48];
    u8 field_0x1aa0[0x48];
    u8 field_0x1ae8[0x48];
    u8 field_0x1b30[0x48];
    u8 field_0x1b78[0x1be8 - 0x1b78];
    UnkTrackListener_s listener;  // 0x1be8
};
static_assert(__builtin_offsetof(UnkEnemyMovement_s, machine) == 0x838, "layout");
static_assert(__builtin_offsetof(UnkEnemyMovement_s, listener) == 0x1be8, "layout");

void fn_71010c7b70(UnkEnemyMovement_s* self) {
    self->vtable = &g_unk_7101cb0218[2];
    self->vtable_0x6e8 = &g_unk_7101cb0218[0x3d0 / 8];
    self->listener.Destroy();
    fn_7100955368(self->field_0x1b78);
    fn_7100954990(self->field_0x1b30);
    fn_71009558f0(self->field_0x1ae8);
    fn_71009558f0(self->field_0x1aa0);
    fn_71009558f0(self->field_0x1a58);
    fn_7100955110(self->field_0x1a10);
    fn_7100971574(reinterpret_cast<UnkUnit1444*>(self->field_0x17a0));
    fn_7100970680(self->field_0x16d0);
    fn_7100969424(self->field_0x15b0);
    fn_710096a110(self->field_0x8e8);
    self->machine.Destroy(&g_unk_7101cb06e0[2]);
    self->vectors.vtable = &g_unk_7101cb0720[2];
    self->vectors.lists[2].Free();
    self->vectors.lists[1].Free();
    self->vectors.lists[0].Free();
    self->actions.Destroy(&g_unk_7101cb0648[2]);
    fn_7100975f10(self);
}

// fn_71010c7de4: after fn_7100979854, sends itself (fn_71010c7e6c) a parameter block with the direction
// from virtual slot 0x370 set.
struct UnkVec3_7de4 {
    f32 x;
    f32 y;
    f32 z;
};

template <class T>
struct UnkOpt_7de4 {
    T value;
    bool has;
};

struct UnkSinkParams_7de4 {
    void** vtable;
    UnkOpt_7de4<u32> field_0x8;
    UnkOpt_7de4<f32> field_0x10;
    UnkOpt_7de4<UnkVec3_7de4> field_0x18;
    UnkOpt_7de4<UnkVec3_7de4> field_0x28;
    u32 kind;  // 0x38
};

struct UnkEffectSink;

struct UnkEnemyMovement_7de4 {
    void** vtable;

    UnkVec3f vf_0x370() { return reinterpret_cast<UnkVec3f (*)(UnkEnemyMovement_7de4*)>(vtable[0x370 / 8])(this); }
};

void fn_7100979854(void* self, void* xform, const u32* changed);

void fn_71010c7de4(UnkEnemyMovement_7de4* self, void* xform, const u32* changed) {
    fn_7100979854(self, xform, changed);
    UnkSinkParams_7de4 params;
    params.vtable = g_unk_7101a24b80 + 2;
    params.field_0x8.has = false;
    params.field_0x10.has = false;
    params.kind = 0;
    __builtin_memset(&params.field_0x28, 0, 13);
    UnkVec3f dir = self->vf_0x370();
    __builtin_memcpy(&params.field_0x18.value, &dir, sizeof(UnkVec3_7de4));
    params.field_0x18.has = true;
    fn_71010c7e6c(reinterpret_cast<UnkEffectSink*>(self), &params, 0);
}

// fn_71010c8f98: marks the movement active (+0x1c11); a pending block (+0x1c12) is sent from the state's
// +0x48 and, in state 1, the owner turns towards virtual slot 0x360's direction.
struct UnkEffectSink;
void fn_7100976110(void* self);

struct UnkOwner_e {
    void** vtable;

    void vf_0xd8(const UnkVec3f* v, bool now) {
        reinterpret_cast<void (*)(UnkOwner_e*, const UnkVec3f*, bool)>(vtable[0xd8 / 8])(this, v, now);
    }
};

struct UnkState_e {
    u8 pad_0x0[0x10];
    s32 kind;  // 0x10
    u8 pad_0x14[0x48 - 0x14];
    u8 params[0x40];  // 0x48
};

struct UnkEnemyMovement_e {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkOwner_e* owner;  // 0x30
    u8 pad_0x38[0x848 - 0x38];
    UnkState_e* state;  // 0x848
    u8 pad_0x850[0x1c11 - 0x850];
    bool active;  // 0x1c11
    bool pending;  // 0x1c12

    UnkVec3f vf_0x360() { return reinterpret_cast<UnkVec3f (*)(UnkEnemyMovement_e*)>(vtable[0x360 / 8])(this); }
};

void fn_71010c8f98(UnkEnemyMovement_e* self) {
    fn_7100976110(self);
    self->active = true;
    if (!self->pending)
        return;
    fn_71010c7e6c(reinterpret_cast<UnkEffectSink*>(self), self->state->params, 1);
    if (self->state && self->state->kind == 1) {
        self->active = false;
        UnkVec3f v = self->vf_0x360();
        self->owner->vf_0xd8(&v, true);
        self->active = true;
    }
    self->pending = false;
}

// fn_71010c9a50: in state 4 the event at +0x17a0 is raised first; the three flags go to the state's slot 0x98.
struct UnkUnit1444_Event;

struct UnkMovementState_9a50 {
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
    virtual void vf_0x98(bool a, bool b, bool c);
    u8 pad_0x8[0x8];
    s32 kind;  // 0x10
};

struct UnkEnemyMovement_9a50 {
    u8 pad_0x0[0x848];
    UnkMovementState_9a50* state;  // 0x848
    u8 pad_0x850[0x17a0 - 0x850];
    u8 event[0x10];  // 0x17a0
};

void fn_71010c9a50(UnkEnemyMovement_9a50* self, void* unused, bool a, bool b, bool c) {
    if (!self->state)
        return;
    if (self->state->kind == 4) {
        fn_7100974798(reinterpret_cast<UnkUnit1444_Event*>(self->event), 1);
        if (!self->state)
            return;
    }
    self->state->vf_0x98(a, b, c);
}

// fn_71010c9b44: the update: the time (+0x6f4), virtual slot 0x3a0 and the state machine (fn_71010c9c34),
// with the bound callback at +0x19e0 told of a state change.
struct UnkMovementState_i {
    u8 pad_0x0[0x10];
    u32 kind;  // 0x10
};

struct UnkMovementStateMachine_i {
    void** vtable;
    u8 pad_0x8[0x8];
    UnkMovementState_i* current;  // 0x10

    u32 CurrentKind() const { return current ? current->kind : 5; }
};
void fn_71010c9c34(UnkMovementStateMachine_i* self, f32 dt);
void fn_7100976408(void* self, f32 dt);

struct UnkActor_i {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

// A bound callback: called with its context first.
struct UnkStateChanged_i {
    void* context;
    void (*fn)(void* context, u32 from, u32 to);

    void operator()(u32 from, u32 to) const { fn(context, from, to); }
};

struct UnkEnemyMovement_i {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkActor_i* owner;  // 0x30
    u8 pad_0x38[0x6f4 - 0x38];
    f32 time;  // 0x6f4
    u8 pad_0x6f8[0x838 - 0x6f8];
    UnkMovementStateMachine_i machine;  // 0x838
    u8 pad_0x850[0x19e0 - 0x850];
    UnkStateChanged_i state_changed;  // 0x19e0
    u8 pad_0x19f0[0x1c0c - 0x19f0];
    u32 last_kind;  // 0x1c0c

    void vf_0x3a0() { reinterpret_cast<void (*)(UnkEnemyMovement_i*)>(vtable[0x3a0 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkEnemyMovement_i, last_kind) == 0x1c0c, "layout");

void fn_71010c9b44(UnkEnemyMovement_i* self, f32 dt) {
    fn_7100976408(self, dt);
    if (!self->owner || !self->owner->field_0x220)
        return;
    self->last_kind = self->machine.CurrentKind();
    self->time += dt;
    self->vf_0x3a0();
    u32 kind = self->machine.CurrentKind();
    fn_71010c9c34(&self->machine, dt);
    if (self->state_changed.fn && self->machine.CurrentKind() != kind)
        self->state_changed(kind, self->machine.CurrentKind());
}

// fn_71010c9928: the facing angles (pitch +-pi/2 by the side the owner faces, yaw from the 2D direction of
// the source at +0x8e8) turned into a vector by fn_710007a220.
extern "C" f32 atan2f(f32 y, f32 x);
UnkVec3f fn_710007a220(const UnkVec3f* v);

struct UnkVec2f_A5 {
    f32 x, y;
    UnkVec2f_A5(f32 x_, f32 y_) : x(x_), y(y_) {}
    UnkVec2f_A5(const UnkVec2f_A5& o) : x(o.x), y(o.y) {}
};

struct UnkEnemyOwner_k {
    u8 pad_0x0[0x1b1];
    bool flag;  // 0x1b1
};

struct UnkEnemyState_k {
    u8 pad_0x0[0x10];
    s32 kind;  // 0x10
};

struct UnkActor_k {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
};

struct UnkEnemyMovement_k {
    u8 pad_0x0[0x30];
    UnkActor_k* actor;  // 0x30
    u8 pad_0x38[0x848 - 0x38];
    UnkEnemyState_k* state;  // 0x848
    u8 pad_0x850[0x8e8 - 0x850];
    u8 source[0x971 - 0x8e8];  // 0x8e8
    bool facing;  // 0x971
    u8 pad_0x972[0x1c18 - 0x972];
    UnkEnemyOwner_k* owner;  // 0x1c18

    bool InState1() const { return state && state->kind == 1; }
};
static_assert(__builtin_offsetof(UnkEnemyMovement_k, owner) == 0x1c18, "layout");

static inline UnkVec2f_A5 Direction(UnkEnemyMovement_k* self) {
    if (!self->owner->flag && self->InState1())
        return fn_710096ee98(self->source);
    return UnkVec2f_A5(0.0f, 1.0f);
}

UnkVec3f fn_71010c9928(UnkEnemyMovement_k* self) {
    UnkVec2f_A5 dir = Direction(self);
    f32 yaw = atan2f(dir.y, dir.x) + -1.5707964f;
    bool facing = fn_710002614c(&self->actor->transform).m[2] > 0.0f;
    if (self->InState1())
        facing = self->facing;
    UnkVec3f angles(0.0f, facing ? 1.5707964f : -1.5707964f, yaw);
    return fn_710007a220(&angles);
}

// fn_71010cd278: the ground probe (fn_7100977e50) of the character movement from virtual slot 0x2a8, from
// the owner's position or `from`.
using base::math::CVector3D;

struct UnkCharacterMovement;
struct UnkCollisionHit;
bool fn_7100977e50(UnkCharacterMovement* self, f32 max_dot, f32 extra, f32 step, f32 back, u32 mask,
                   const CVector3D* pos, CVector3D* out_point, UnkCollisionHit* out_hit, const CVector3D* dir);

struct UnkActor_g {
    u8 pad_0x0[0x38];
    f32 transform[3][4];  // 0x38
};

struct UnkEnemyMovement_g {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkActor_g* owner;  // 0x30
    u8 pad_0x38[0x3d0 - 0x38];
    u32 collision_mask;  // 0x3d0

    UnkCharacterMovement* vf_0x2a8() { return reinterpret_cast<UnkCharacterMovement* (*)(UnkEnemyMovement_g*)>(vtable[0x2a8 / 8])(this); }
};

bool fn_71010cd278(UnkEnemyMovement_g* self, const CVector3D* dir, UnkCollisionHit* hit, f32 extra,
                   const CVector3D* from) {
    UnkCharacterMovement* character = self->vf_0x2a8();
    if (!character)
        return false;
    UnkVec3f pos(self->owner->transform[0][3], self->owner->transform[1][3], self->owner->transform[2][3]);
    if (from)
        pos = *reinterpret_cast<const UnkVec3f*>(from);
    return fn_7100977e50(character, 1.0f, extra, 3.4028235e38f, 0.0f,
                         self->collision_mask, reinterpret_cast<const CVector3D*>(&pos), nullptr, hit, dir);
}

// fn_71010cb488: the up vector: the stored one (+0x19fc when +0x1a08), the state's (slot 0x68), or the
// owner's matrix column 1.
struct UnkEnemyMovementState_b488 {
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
    virtual UnkVec3f vf_0x68();
};

struct UnkEnemyOwner_b488 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
};

struct UnkEnemyMovement_b488 {
    u8 pad_0x0[0x30];
    UnkEnemyOwner_b488* owner;  // 0x30
    u8 pad_0x38[0x848 - 0x38];
    UnkEnemyMovementState_b488* state;  // 0x848
    u8 pad_0x850[0x19fc - 0x850];
    UnkVec3f up;  // 0x19fc
    bool has_up;  // 0x1a08
};

UnkVec3f fn_71010cb488(UnkEnemyMovement_b488* self) {
    if (self->has_up)
        return self->up;
    if (self->state)
        return self->state->vf_0x68();
    const UnkMatrix44& m = fn_710002614c(&self->owner->transform);
    return UnkVec3f(m.m[1], m.m[5], m.m[9]);
}

// fn_71010caa64: whether the movement turns: never while the 0xf406... track plays; always with a
// CCharClassIgnoreTurnTrack or fn_7101179870; otherwise as the AI's slot 0x1b8 says.
struct UnkStateSet_1441;

struct UnkMovementSlot_f {
    void** vtable;
    bool vf_0x1b8() { return reinterpret_cast<bool (*)(UnkMovementSlot_f*)>(vtable[0x1b8 / 8])(this); }
};

struct UnkActor_f {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x18];
    UnkMovementSlot_f* field_0x240;  // 0x240
};

struct UnkEnemyMovement_f {
    u8 pad_0x0[0x30];
    UnkActor_f* owner;  // 0x30
};

bool fn_71010caa64(UnkEnemyMovement_f* self) {
    UnkMovementSlot_f* slot = self->owner->field_0x240;
    UnkAnimationComponent* animation = self->owner->animation;
    if (!slot || !animation)
        return false;
    if (fn_71011782ec(animation, 0xf4061ff25a2163d6))
        return false;
    if (fn_710117b348(animation, fn_7100d14318()) ||
        fn_7101179870(reinterpret_cast<UnkStateSet_1441*>(animation)))
        return true;
    return slot->vf_0x1b8();
}

// fn_71010cb304: a contact: ignored when the shape is flagged and the contact points down; otherwise the
// base handler (fn_71009778e8) and, for a shallow contact, the AI's slots 0x2b0 and 0x2a8.
struct UnkContactShape_p {
    void** vtable;
    u32 vf_0x58() { return reinterpret_cast<u32 (*)(UnkContactShape_p*)>(vtable[0x58 / 8])(this); }
};

struct UnkContactBody_p {
    u8 pad_0x0[0x10];
    UnkContactShape_p* shape;  // 0x10
};

struct UnkContact_p {
    UnkContactBody_p* body;  // 0x0
    u8 pad_0x8[0x1c];
    f32 normal_y;  // 0x24
    u8 pad_0x28[0x8];
    f32 field_0x30;  // 0x30
    u8 pad_0x34[0x8];
    u8 flags;  // 0x3c
};

struct UnkAI_p {
    void** vtable;
    void vf_0x1a0(bool* flag) { reinterpret_cast<void (*)(UnkAI_p*, bool*)>(vtable[0x1a0 / 8])(this, flag); }
    void vf_0x2a8(UnkContact_p* c) { reinterpret_cast<void (*)(UnkAI_p*, UnkContact_p*)>(vtable[0x2a8 / 8])(this, c); }
    void vf_0x2b0(UnkContact_p* c) { reinterpret_cast<void (*)(UnkAI_p*, UnkContact_p*)>(vtable[0x2b0 / 8])(this, c); }
};

struct UnkActor_p {
    u8 pad_0x0[0x240];
    UnkAI_p* ai;  // 0x240
};

struct UnkEnemyOwner_p {
    u8 pad_0x0[0x68];
    bool field_0x68;
    u8 pad_0x69[0x1b0 - 0x69];
    bool field_0x1b0;
};

struct UnkEnemyMovement_p {
    u8 pad_0x0[0x30];
    UnkActor_p* actor;  // 0x30
    u8 pad_0x38[0xf9 - 0x38];
    bool flying;  // 0xf9
    u8 pad_0xfa[0x6f8 - 0xfa];
    s32 mode;  // 0x6f8
    u8 pad_0x6fc[0x1c18 - 0x6fc];
    UnkEnemyOwner_p* owner;  // 0x1c18
};
static_assert(__builtin_offsetof(UnkEnemyMovement_p, owner) == 0x1c18, "layout");

u32 fn_71009778e8(UnkEnemyMovement_p* self, UnkContact_p* contact, void* a, void* b, void* c, void* d, void* e);

static inline bool IgnoresContact(const UnkContact_p* contact) {
    if (contact->body) {
        if (UnkContactShape_p* shape = contact->body->shape) {
            if ((shape->vf_0x58() & 0x10) && (contact->flags & 0x20) && contact->normal_y < -0.6f)
                return true;
        }
    }
    return false;
}

u32 fn_71010cb304(UnkEnemyMovement_p* self, UnkContact_p* contact, void* a, void* b, void* c, void* d, void* e) {
    u32 result = 0;
    if (!IgnoresContact(contact)) {
        result = fn_71009778e8(self, contact, a, b, c, d, e);
        if (result) {
            if (UnkAI_p* ai = self->actor->ai) {
                f32 amount = __builtin_fabsf(contact->field_0x30);
                if (self->flying || amount < 0.4f)
                    ai->vf_0x2b0(contact);
                if (amount < 0.4f) {
                    bool blocked = self->owner->field_0x1b0;
                    if (UnkAI_p* other = self->actor->ai)
                        other->vf_0x1a0(&blocked);
                    if (!blocked) {
                        UnkEnemyOwner_p* owner = self->owner;
                        if (!owner || owner->field_0x68)
                            self->mode = 6;
                        ai->vf_0x2a8(contact);
                    }
                }
            }
        }
    }
    return result;
}

// fn_71010cb1c4: after fn_71009765d8, in mode 6, sends the animation the action's hash (refl_EmitU32).
struct UnkHashed_j {
    u8 pad_0x0[0x30];
    u64 hash;  // 0x30
};

struct UnkActionSet_j {
    void** vtable;
    UnkHashed_j* const* vf_0x68(s32 kind) {
        return reinterpret_cast<UnkHashed_j* const* (*)(UnkActionSet_j*, s32)>(vtable[0x68 / 8])(this, kind);
    }
};

struct UnkActor_j {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkEnemyMovement_j {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkActor_j* owner;  // 0x30
    u8 pad_0x38[0x6f8 - 0x38];
    s32 mode;  // 0x6f8
    u8 pad_0x6fc[0x710 - 0x6fc];
    UnkActionSet_j* actions;  // 0x710
};

void fn_71009765d8(void* self);

void fn_71010cb1c4(UnkEnemyMovement_j* self) {
    fn_71009765d8(self);
    if (self->mode != 6 || !self->actions)
        return;
    u64 hash = (*self->actions->vf_0x68(6))->hash;
    if (hash == (u64)-1)
        return;
    base::reflection::refl_EmitU32(self->owner->field_0x220, hash, 0xf4061ff25a2163d6, 0, &g_unk_710164b958,
                                   &g_unk_7172769740);
}

// fn_71010c9338: a message: forwarded to the state; for kind 1 with a turn track playing, the action set
// picks a mode (2 front, 3 back) and the tweak track for it starts an animation request.
struct UnkStruct_7101176ea8;
struct UnkRequest_7101176ea8;

struct UnkAnimTime_q {
    u64 a, b;
};
struct UnkAnimBlend_q {
    u32 a, b, c;
};

// An animation request record (as fn_7101176ea8 takes it): no id, the default time and blend.
struct UnkAnimRequest_q {
    s64 id;
    UnkAnimTime_q time;    // 0x8
    UnkAnimBlend_q blend;  // 0x18
    u32 flags;             // 0x24
    u32 mask;              // 0x28

    UnkAnimRequest_q(s64 id_, UnkAnimTime_q time_, UnkAnimBlend_q blend_, u32 flags_, u32 mask_)
        : id(id_), time(time_), blend(blend_), flags(flags_), mask(mask_) {}
};

struct UnkMessage_q {
    s32 kind;  // 0x0
    u8 pad_0x4[0x4];
    void* field_0x8;
    struct Info {
        u8 pad_0x0[0x30];
        u64 hash;  // 0x30
    }* info;  // 0x10
};

struct UnkMovementState_q {
    void** vtable;
    void vf_0xb8(UnkMessage_q* m) { reinterpret_cast<void (*)(UnkMovementState_q*, UnkMessage_q*)>(vtable[0xb8 / 8])(this, m); }
};

struct UnkActionSet_q {
    void** vtable;
    void vf_0x60(s32* mode, UnkAnimRequest_q* req, bool flag) {
        reinterpret_cast<void (*)(UnkActionSet_q*, s32*, UnkAnimRequest_q*, bool)>(vtable[0x60 / 8])(this, mode, req,
                                                                                                 flag);
    }
};

struct UnkAI_q {
    u8 pad_0x0[0x1610];
    void* field_0x1610;
};

struct UnkActor_q {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x18];
    UnkAI_q* ai;  // 0x240
};

struct UnkEnemyMovement_q {
    u8 pad_0x0[0x30];
    UnkActor_q* actor;  // 0x30
    u8 pad_0x38[0x6f8 - 0x38];
    s32 mode;  // 0x6f8
    u8 pad_0x6fc[0x710 - 0x6fc];
    UnkActionSet_q* actions;  // 0x710
    u8 pad_0x718[0x848 - 0x718];
    UnkMovementState_q* state;  // 0x848
};

bool fn_71010c9338(UnkEnemyMovement_q* self, UnkMessage_q* message) {
    if (!message->field_0x8 || message->info->hash != 0xf4061ff25a2163d6)
        return true;
    if (self->state)
        self->state->vf_0xb8(message);
    if (message->kind != 1)
        return true;
    UnkActor_q* actor = self->actor;
    if (!actor->ai || !actor->ai->field_0x1610)
        return true;
    UnkAnimationComponent* animation = actor->animation;
    if (!animation || !fn_710117b348(animation, fn_7100d2b3a8()))
        return true;
    UnkActionSet_q* actions = self->actions;
    if (!actions)
        return true;
    s32 mode = self->mode;
    UnkAnimRequest_q req(-1, reinterpret_cast<const UnkAnimTime_q&>(g_unk_710164b958),
                         reinterpret_cast<const UnkAnimBlend_q&>(g_unk_7172769740), 0, UnkAll_SpitSend().value);
    actions->vf_0x60(&mode, &req, false);
    req.flags |= 0x100;
    if (mode == 2) {
        if (!fn_710117b348(animation, fn_7100c59098()))
            return true;
    } else if (mode == 3) {
        if (!fn_710117b348(animation, fn_7100c56928()))
            return true;
    } else {
        return true;
    }
    self->mode = mode;
    fn_7101176ea8(reinterpret_cast<UnkStruct_7101176ea8*>(animation),
                  reinterpret_cast<const UnkRequest_7101176ea8*>(&req), 0xf4061ff25a2163d6);
    return true;
}

// fn_71010ce798: removes an item from a list whose changes are deferred while it is locked (+0x78).
// A list of pointers whose changes are deferred while it is locked (the counter at +0x78).
struct UnkLockedList {
    base::UnkVector<void*> pending_add;     // 0x0
    base::UnkVector<void*> pending_remove;  // 0x28
    base::UnkVector<void*> items;           // 0x50
    s32 lock;                               // 0x78
};
static_assert(__builtin_offsetof(UnkLockedList, lock) == 0x78, "layout");

void fn_71010ce798(UnkLockedList* self, void* item) {
    if (self->lock <= 0) {
        self->items.Remove(item);
    } else {
        self->pending_remove.PushBack(item);
        self->pending_add.Remove(item);
    }
}

// fn_71010ce92c: unlocks such a list (through its lock holder) and applies the pending removals and additions.
struct UnkClone_fn_71010ceaf0;
using UnkPendingElem = UnkCloneElem<UnkClone_fn_71010ceaf0, 8>;
extern template UnkPendingElem& base::UnkVector<UnkPendingElem>::GrowAt(u32);

struct UnkLockedListPending {
    base::UnkVector<UnkPendingElem> pending_add;     // 0x0
    base::UnkVector<UnkPendingElem> pending_remove;  // 0x28
    base::UnkVector<void*> items;                    // 0x50
    union {
        s32 lock;  // 0x78
    };
};

struct UnkLockedListLock {
    union {
        UnkLockedListPending* list;
    };
};

void fn_71010ce92c(UnkLockedListLock* self) {
    self->list->lock--;
    UnkLockedListPending* list = self->list;
    if (list->lock > 0)
        return;
    for (u32 i = 0; i < list->pending_remove.count; i++)
        list->items.Remove(*reinterpret_cast<void* const*>(&list->pending_remove.GrowAt(i)));
    list->pending_remove.count = 0;
    list = self->list;
    for (u32 i = 0; i < list->pending_add.count; i++)
        list->items.PushBack(*reinterpret_cast<void* const*>(&list->pending_add.GrowAt(i)));
    list->pending_add.count = 0;
}
