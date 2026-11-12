// More callers that request a state from a component's small state machine (the template of
// src/game/UnkStateMachineSet_b60.cpp, copied here with _b61 names): the request stores the pending id,
// gives up past the last state or while busy, and otherwise switches state. Larger caller shapes:
// requests after a base call and before a virtual tail call, behind timers, behind message tests.
// Names are placeholders.
#include "base/UnkDeferredSet.h"
#include "base/UnkRefHandle.h"
#include "base/UnkVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAutsharpAIComponent.h"
#include "game/CCharClassEventPropComponent.h"
#include "game/CCharClassShootActivatorHidrogigaComponent.h"
#include "game/CCharacterMovement.h"
#include "game/CLifeComponent.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1376.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2547.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU367.h"
#include "game/UnkTimelineComponent.h"
#include "game/navmesh/SNavMeshPath.h"
#include "types.h"

struct UnkSmStateA5_b61 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    u64 field_0x8;
    u32 id;
};

typedef UnkSmStateA5_b61* __attribute__((aligned(4))) UnkSmStatePtrA5_b61;

struct UnkSmMachineA5_b61 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkSmStateA5_b61* previous, UnkSmStateA5_b61* next);
    virtual void vf_0x28(UnkSmStateA5_b61* current, UnkSmStateA5_b61* next);

    u64 field_0x8;
    UnkSmStateA5_b61* current;
    UnkSmStateA5_b61* previous;
    u32 count;
    UnkSmStatePtrA5_b61 states[16];
    u32 pending;
    bool busy;
};

template <u32 Offset>
struct UnkSmOwnerA5_b61 {
    u8 pad_0x0[Offset];
    UnkSmMachineA5_b61 machine;
};

__attribute__((always_inline)) static inline bool FindStateA5_b61(UnkSmMachineA5_b61* self, u32 id,
                                                               UnkSmStateA5_b61*& out) {
    u64 n = (u64)self->count * sizeof(UnkSmStatePtrA5_b61);
    asm("" : "+r"(n));
    for (UnkSmStatePtrA5_b61* p = self->states; n != 0; ++p, n -= sizeof(UnkSmStatePtrA5_b61)) {
        if ((*p)->id == id) {
            out = *p;
            return out != nullptr;
        }
    }
    out = nullptr;
    return false;
}

// Switches to state `id` (the b59 "apply pending" body) and clears the pending id.
template <u32 None>
__attribute__((always_inline)) static inline void SwitchStateA5_b61(UnkSmMachineA5_b61* self, u32 id,
                                                                 UnkSmStateA5_b61* current) {
    if (!current || current->id != id) {
        UnkSmStateA5_b61* found;
        UnkSmStateA5_b61* next;
        u32 enter;
        if (FindStateA5_b61(self, id, found)) {
            next = found;
            enter = true;
        } else {
            if (id != 0)
                goto done;
            enter = false;
            asm("" : "+r"(enter));
            next = nullptr;
            asm("" : "+r"(next));
        }
        if (current) {
            current->vf_0x18(id);
            self->vf_0x28(self->current, next);
        }
        self->previous = self->current;
        self->current = found;
        if (enter) {
            typedef void (*EnteredFn)(UnkSmStateA5_b61*, s32);
            s32 previous_id = self->previous ? self->previous->id : None;
            EnteredFn entered = (*reinterpret_cast<EnteredFn**>(found))[2];
            entered(next, previous_id);
            self->vf_0x20(self->previous, next);
        }
    }
done:
    self->pending = None;
}

template <u32 Last, u32 None>
__attribute__((always_inline)) static inline void RequestStateA5_b61(UnkSmMachineA5_b61* self, u32 id) {
    // Stored through a plain u32 pointer: the original rereads a state id the caller tested after this.
    u32* pending = &self->pending;
    *pending = id;
    if (id > Last)
        return;
    if (self->busy)
        return;
    SwitchStateA5_b61<None>(self, id, self->current);
}

struct UnkEmmyWakeUp_b56;

static inline void CallSlot_A5_b61(void* self, u32 slot) {
    typedef void (*Fn)(void*);
    (*reinterpret_cast<Fn**>(self))[slot / 8](self);
}

void fn_71007deedc(UnkSmOwnerA5_b61<0xa0>* self) {
    fn_71007dc328(reinterpret_cast<UnkEmmyWakeUp_b56*>(self));
    RequestStateA5_b61<2, 0x7fffffff>(&self->machine, 1);
    CallSlot_A5_b61(self, 0x178);
}

void fn_71007fa610(UnkSmOwnerA5_b61<0x228>* self, f32 dt) {
    f32* timer = reinterpret_cast<f32*>(reinterpret_cast<char*>(self) + 0x44);
    if (*timer > 0.001f) {
        *timer -= dt;
        return;
    }
    RequestStateA5_b61<4, 5>(&self->machine, 4);
}

static inline u32 StateIdA5_b61(UnkSmStateA5_b61* state) {
    return *reinterpret_cast<u32*>(reinterpret_cast<char*>(state) + 0x10);
}

struct UnkFrozenSmA5_b61 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x228 - 0x38];
    UnkSmMachineA5_b61 machine;
    u8 pad_0x2d8[0x398 - 0x2d8];
    bool field_0x398;
};

bool fn_71007fb020(UnkFrozenSmA5_b61* self, const s32* message) {
    UnkSmStateA5_b61* current = self->machine.current;
    if (current && StateIdA5_b61(current) == 4) {
        switch (*message) {
        case 1:
            if (self->field_0x398)
                RequestStateA5_b61<4, 5>(&self->machine, 1);
            break;
        case 2:
            RequestStateA5_b61<4, 5>(&self->machine, 1);
            break;
        }
    }
    return true;
}

struct UnkLauncherLinkA5_b61 {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkLauncherSmA5_b61 {
    u8 pad_0x0[0x208];
    UnkSmMachineA5_b61 machine;
    u8 pad_0x2b8[0x310 - 0x2b8];
    UnkLauncherLinkA5_b61* field_0x310;
};

struct UnkMsgNode3A5_b61 {
    u8 pad_0x0[0x10];
    base::reflection::UnkReflObject* field_0x10;
};
struct UnkMsgNode2A5_b61 {
    u8 pad_0x0[0x10];
    UnkMsgNode3A5_b61* field_0x10;
};
struct UnkMsgA5_b61 {
    u8 pad_0x0[0x10];
    UnkMsgNode2A5_b61* field_0x10;
};

bool fn_7100888aa8(UnkLauncherSmA5_b61* self, const UnkMsgA5_b61* message) {
    base::reflection::UnkReflObject* obj = message->field_0x10->field_0x10->field_0x10;
    if (!obj || !fn_710009d50c(obj->vf_0x0(), fn_710085fc44()))
        return false;
    void* owner = *reinterpret_cast<void**>(reinterpret_cast<char*>(obj) + 0x30);
    if (owner != (self->field_0x310 ? self->field_0x310->field_0x10 : nullptr))
        return false;
    UnkSmStateA5_b61* current = self->machine.current;
    if (current && (StateIdA5_b61(current) == 3 || StateIdA5_b61(current) == 4))
        return false;
    RequestStateA5_b61<4, 5>(&self->machine, 1);
    return false;
}

// CCharClassMushroomPlatformComponent's unit: base call, request state 1, then a bound callback
// (self, function) at +0x4f0.
struct UnkMushroomSmA5_b61;
void fn_710088cfd0(UnkMushroomSmA5_b61*);

struct UnkMushroomSmA5_b61 {
    u8 pad_0x0[0x260];
    UnkSmMachineA5_b61 machine;
    u8 pad_0x310[0x4f0 - 0x310];
    UnkMushroomSmA5_b61* callback_self;
    void (*callback)(UnkMushroomSmA5_b61*);
};

void fn_710088a92c(UnkMushroomSmA5_b61* self) {
    fn_710115fc5c(reinterpret_cast<char*>(self));
    RequestStateA5_b61<3, 0x7fffffff>(&self->machine, 1);
    self->callback_self = self;
    self->callback = fn_710088cfd0;
}

bool fn_710088cea8(UnkMushroomSmA5_b61* self, const u8* message) {
    if (message[8])
        return false;
    RequestStateA5_b61<3, 0x7fffffff>(&self->machine, 1);
    return true;
}

// Message handlers: a message whose type hash and payload's hash match asks for state 3.
struct UnkMsgHashed_b61 {
    u8 pad_0x0[0x30];
    u64 hash;
};
struct UnkMsgPayload2_b61 {
    u8 pad_0x0[0x38];
    UnkMsgHashed_b61* field_0x38;
};
struct UnkMsgPayload1_b61 {
    u8 pad_0x0[0x10];
    UnkMsgPayload2_b61* field_0x10;
};
struct UnkMsgHashedMessage_b61 {
    s32 kind;
    UnkMsgPayload1_b61* payload;
    UnkMsgHashed_b61* type;
};

bool fn_710088c2d8(UnkMushroomSmA5_b61* self, const UnkMsgHashedMessage_b61* message) {
    if (message->type->hash == 0xf4061ff25a2163d6 && message->kind == 1 && message->payload &&
        message->payload->field_0x10) {
        if (message->payload->field_0x10->field_0x38->hash == 0x5c812431a6579adc)
            RequestStateA5_b61<3, 0x7fffffff>(&self->machine, 3);
        return false;
    }
    return true;
}

// CCharClassShootActivatorHidrogigaComponent's unit: the part's machine sits at +0x40. Turning a part
// on or off requests state 4, then plays (fn_7100d7c94c) and gives the actor two timeline keys.
extern u64 g_unk_710164af50;
extern u64 g_unk_710164af58;

struct UnkHidrogigaPartA5_b61 {
    u8 pad_0x0[0x30];
    UnkActor* actor;
    u8 pad_0x38[0x40 - 0x38];
    UnkSmMachineA5_b61 machine;
};

// Signatures copied from CCharClassShootActivatorHidrogigaComponent_b44.cpp.
struct UnkHidrogigaPart_b44a2;

void fn_71008a1598(UnkHidrogigaPart_b44a2* p, bool on) {
    auto* part = reinterpret_cast<UnkHidrogigaPartA5_b61*>(p);
    RequestStateA5_b61<4, 5>(&part->machine, 4);
    if (on) {
        fn_7100d7c94c(part->actor, &g_unk_710164af50);
        fn_7100d7c828(part->actor, &g_unk_710164af58, false);
    }
    fn_7100d7c828(part->actor, &g_unk_710164af50, false);
    fn_7100d7c828(part->actor, &g_unk_710164af58, false);
}

// Resolves the lazily resolved handle at +0x5f8 (the shape of
// CCharClassShootActivatorHidrogigaComponent_b47.cpp), then asks the named object's part for state 1.
struct UnkCountedA5_b61 : base::UnkRefCounted {
    union {
        void* field_0x10;
        u64 raw_0x10;
    };
};

struct UnkRef {
    UnkCountedA5_b61* ptr;
    ~UnkRef() {
        if (ptr && --ptr->ref_count == 0)
            delete ptr;
    }
};

UnkRef fn_7100353be8(void* from);

struct UnkHidrogigaActivatorA5_b61 {
    u8 pad_0x0[0x5c8];
    u8 source[0x28];
    u32 state;
    u32 pad_0x5f4;
    base::UnkRefHandle handle;
};

void fn_7100897f48(UnkHidrogigaActivatorA5_b61* self) {
    if (self->state == 1) {
        {
            UnkRef fresh = fn_7100353be8(self->source);
            fn_710031d208(&self->handle, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
        }
        self->state = 2;
    }
    auto* c = static_cast<UnkCountedA5_b61*>(self->handle.ptr);
    if (!c || !c->raw_0x10)
        return;
    asm("" ::: "memory");
    if (auto* part = reinterpret_cast<UnkHidrogigaPartA5_b61*>(fn_7100897b0c(c->field_0x10)))
        RequestStateA5_b61<4, 5>(&part->machine, 1);
}

// CDoorLifeComponent's unit: once the base step reports 6, enters state 1 (registering with a lazily
// created 4-byte singleton) and starts a blend; reports 7.
struct UnkDoorRegistry_b61;
struct UnkDoorLifeA5_b61;
void fn_7100ddb6f8(UnkDoorRegistry_b61* self, UnkDoorLifeA5_b61* door);
s32 fn_71008e22c8(UnkDoorLifeA5_b61* self);

struct UnkDoorLifeA5_b61 {
    u8 pad_0x0[0x420];
    UnkSmMachineA5_b61 machine;
    u8 pad_0x4d0[0x638 - 0x4d0];
    f32 field_0x638;
};

s32 fn_71008d6bc4(UnkDoorLifeA5_b61* self) {
    s32 r = fn_71008e22c8(self);
    if (r != 6)
        return r;
    UnkSmStateA5_b61* current = self->machine.current;
    if (!current || StateIdA5_b61(current) != 1) {
        RequestStateA5_b61<4, 0x7fffffff>(&self->machine, 1);
        UnkDoorRegistry_b61* reg = g_unk_71726bee10;
        if (!reg) {
            reg = static_cast<UnkDoorRegistry_b61*>(fn_71000839a0(4, 0x3f));
            fn_7100ddb6dc(reg);
            g_unk_71726bee10 = reg;
        }
        fn_7100ddb6f8(reg, self);
    }
    fn_71008d0c24(self, 1.0f, self->field_0x638);
    return 7;
}

// CCentralUnitComponent's unit: enters state 5, refreshes (fn_71007738c8), passes the target's object
// on (fn_7100773b74) and drops the two registrations at +0x6e0 and +0x7c8.
struct UnkLinkList;
void fn_71007738c8();
struct UnkCentralUnitA5_b61;
u32 fn_7100773b74(UnkCentralUnitA5_b61* self, void* object);

struct UnkCentralUnitTarget_b61 {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkCentralUnitA5_b61 {
    u8 pad_0x0[0x30];
    UnkSmMachineA5_b61 machine;
    u8 pad_0xe0[0x500 - 0xe0];
    u8 source[0x28];
    u32 state;
    u32 pad_0x52c;
    base::UnkRefHandle handle;
    u8 pad_0x538[0x6d8 - 0x538];
    UnkCentralUnitTarget_b61* target;
    UnkLinkList* link;
    u8 pad_0x6e8[0x7c8 - 0x6e8];
    base::UnkDeferredSet<void>* set;
};

void fn_7100773768(UnkCentralUnitA5_b61* self) {
    RequestStateA5_b61<5, 8>(&self->machine, 5);
    fn_71007738c8();
    fn_7100773b74(self, self->target ? self->target->field_0x10 : nullptr);
    if (self->link) {
        fn_71002aeb9c(self->link, &self->link);
        self->link = nullptr;
    }
    if (self->set) {
        fn_71004f3ea4(self->set, &self->set);
        self->set = nullptr;
    }
}

// The door the resolved handle names (fn_71002af860), when it is `entity` and its machine is in
// state 1, makes this unit enter state 6.
struct UnkWeakTarget;
struct UnkDoorLock;

struct UnkCentralUnitDoorA5_b61 {
    u8 pad_0x0[0x430];
    UnkSmStateA5_b61* state;
};

bool fn_710077418c(UnkCentralUnitA5_b61* self, void* entity) {
    if (self->state == 1) {
        {
            UnkRef fresh = fn_7100353be8(self->source);
            fn_710031d208(&self->handle, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
        }
        self->state = 2;
    }
    auto* c = static_cast<UnkCountedA5_b61*>(self->handle.ptr);
    if (!c || !c->raw_0x10)
        return true;
    // the original reloads the object for the test and again for the call
    void* volatile* object = &c->field_0x10;
    if (*object != entity)
        return true;
    auto* door = reinterpret_cast<UnkCentralUnitDoorA5_b61*>(fn_71002af860(static_cast<UnkWeakTarget*>(*object)));
    if (door && door->state && StateIdA5_b61(door->state) == 1)
        RequestStateA5_b61<6, 8>(&self->machine, 6);
    return true;
}

// CSensorDoorComponent's unit: while the cooldown flag (+0x1e9) is set, count up to the tunable and
// clear it; otherwise test the player's shape against the door's box (fn_710012b330) and request a
// state: 2 when the player is not inside, 1 when it is.
struct UnkPlayer;
struct UnkFlagged;
struct UnkShape;
struct UnkBox2D;
struct UnkContactPointA5_b61 {
    u8 data[0x2c];
};
template <u32 N>
struct UnkContactBuffer {
    u32 count;
    UnkContactPointA5_b61 points[N];
};
bool fn_710012b330(const UnkBox2D* a, const void* b, UnkContactBuffer<4>* out, bool flag);

struct UnkSensorShape_b61 {
    virtual void vf_0x0();
    u8 pad_0x8[0x40 - 0x8];
    u8 geometry[0x20];
    bool dirty;
    void Update() {
        typedef void (*Fn)(UnkSensorShape_b61*);
        (*reinterpret_cast<Fn**>(this))[0x98 / 8](this);
    }
};

struct UnkSensorPlayer_b61 {
    u8 pad_0x0[0x200];
    UnkFlagged* field_0x200;
};

struct UnkSensorTunables_b61 {
    u8 pad_0x0[0x58];
    f32 field_0x58;
    f32 field_0x5c;
};

struct UnkSensorDoorA5_b61 {
    u8 pad_0x0[0x40];
    UnkSensorTunables_b61* tunables;
    u8 box[0x10];
    UnkSmMachineA5_b61 machine;
    u8 pad_0x108[0x1e9 - 0x108];
    bool cooldown;
    u8 pad_0x1ea[2];
    f32 timer;
};

static inline bool PlayerInside_A5_b61(UnkSensorDoorA5_b61* self, bool& inside) {
    auto* player = reinterpret_cast<UnkSensorPlayer_b61*>(fn_7100dec788(g_unk_7101d47310));
    if (!player || !player->field_0x200)
        return false;
    auto* shape = reinterpret_cast<UnkSensorShape_b61*>(fn_7100976588(player->field_0x200));
    if (!shape)
        return false;
    UnkContactBuffer<4> contacts;
    contacts.count = 0;
    const UnkBox2D* box = reinterpret_cast<const UnkBox2D*>(self->box);
    asm("" : "+r"(box));
    if (shape->dirty)
        shape->Update();
    inside = fn_710012b330(box, shape->geometry, &contacts, true);
    return true;
}

void fn_71007b8954(UnkSensorDoorA5_b61* self, f32 dt) {
    if (self->cooldown) {
        if (self->timer >= self->tunables->field_0x58)
            self->cooldown = false;
        self->timer += dt;
        return;
    }
    bool inside = false;
    if (PlayerInside_A5_b61(self, inside) && inside)
        return;
    RequestStateA5_b61<2, 0x7fffffff>(&self->machine, 2);
}

void fn_71007b8cc0(UnkSensorDoorA5_b61* self, f32 dt) {
    if (self->cooldown) {
        if (self->timer >= self->tunables->field_0x5c)
            self->cooldown = false;
        self->timer += dt;
        return;
    }
    bool inside = false;
    if (PlayerInside_A5_b61(self, inside) && inside)
        RequestStateA5_b61<2, 0x7fffffff>(&self->machine, 1);
}
