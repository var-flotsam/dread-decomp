// More callers that request a state from a component's small state machine (the template of
// src/game/UnkStateMachineSet_b61.cpp, copied here: the machine of src/game/UnkStateMachineUpdate_b59.cpp
// embedded at some offset, followed by a "busy" flag at +0xa8). One template per (last state, "none"
// value), instantiated per component and machine offset. Names are placeholders.
#include "base/global/CRntString.h"
#include "../game_aca57c/UnkBufferString.h"
#include "game/CComponent.h"
#include "game/CElectricReactionComponent.h"
#include "game/CUsableComponent.h"
#include "game/GUI/CDisplayObjectContainer.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

struct UnkSmState_A6_b62 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    virtual void vf_0x20(f32 dt);        // updated
    u64 field_0x8;
    u32 id;
};

typedef UnkSmState_A6_b62* __attribute__((aligned(4))) UnkSmStatePtr_A6_b62;

struct UnkSmMachine_A6_b62 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkSmState_A6_b62* previous, UnkSmState_A6_b62* next);
    virtual void vf_0x28(UnkSmState_A6_b62* current, UnkSmState_A6_b62* next);

    u64 field_0x8;
    UnkSmState_A6_b62* current;
    UnkSmState_A6_b62* previous;
    u32 count;
    UnkSmStatePtr_A6_b62 states[16];
    u32 pending;
    bool busy;
};

template <u32 Offset>
struct UnkSmOwner_A6_b62 {
    u8 pad_0x0[Offset];
    UnkSmMachine_A6_b62 machine;
};

// Pin: the states array's address is not derived from another one the caller computed.
template <bool Pin>
__attribute__((always_inline)) static inline bool FindState_A6_b62(UnkSmMachine_A6_b62* self, u32 id,
                                                               UnkSmState_A6_b62*& out) {
    u64 n = (u64)self->count * sizeof(UnkSmStatePtr_A6_b62);
    asm("" : "+r"(n));
    UnkSmStatePtr_A6_b62* p = self->states;
    if (Pin)
        asm("" : "+r"(p));
    for (; n != 0; ++p, n -= sizeof(UnkSmStatePtr_A6_b62)) {
        if ((*p)->id == id) {
            out = *p;
            return out != nullptr;
        }
    }
    out = nullptr;
    return false;
}

// Switches to state `id` (the b59 "apply pending" body) and clears the pending id.
template <u32 None, bool Pin = true>
__attribute__((always_inline)) static inline void SwitchState_A6_b62(UnkSmMachine_A6_b62* self, u32 id,
                                                                 UnkSmState_A6_b62* current) {
    if (!current || current->id != id) {
        UnkSmState_A6_b62* found;
        UnkSmState_A6_b62* next;
        u32 enter;
        if (FindState_A6_b62<Pin>(self, id, found)) {
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
            typedef void (*EnteredFn)(UnkSmState_A6_b62*, s32);
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
__attribute__((always_inline)) static inline void RequestState_A6_b62(UnkSmMachine_A6_b62* self, u32 id) {
    // Stored through a plain u32 pointer: the original rereads a state id the caller tested after this.
    u32* pending = &self->pending;
    *pending = id;
    if (id > Last)
        return;
    if (self->busy)
        return;
    SwitchState_A6_b62<None>(self, id, self->current);
}

// A timer that holds the request while it runs.
struct UnkSmTimer_A6_b62 {
    u8 pad_0x0[0x148];
    f32 timer;
    u32 field_0x14c;
    u8 pad_0x150[0x180 - 0x150];
    UnkSmMachine_A6_b62 machine;
};

void fn_7100a0d4f4(UnkSmTimer_A6_b62* self) {
    if (!(self->timer <= 0.0f)) {
        self->field_0x14c = 0;
        return;
    }
    RequestState_A6_b62<5, 6>(&self->machine, 2);
}

// A callback, then a reset and the request for state 0.
struct UnkSmCallback_A6_b62;
typedef void (*UnkSmCallbackFn_A6_b62)();
struct UnkSmCallback_A6_b62 {
    u8 pad_0x0[0x48];
    UnkSmCallbackFn_A6_b62 callback;
    UnkSmMachine_A6_b62 machine;
};

void fn_7100b91b20(UnkSmCallback_A6_b62* self) {
    if (UnkSmCallbackFn_A6_b62 callback = self->callback)
        callback();
    RequestState_A6_b62<3, 4>(&self->machine, 0);
}

// After a base call: the request for state 0, then a flag cleared here and one set in the argument.
struct UnkSmArg_A6_b62 {
    u8 pad_0x0[0x1a];
    bool field_0x1a;
};
struct UnkSmBase_A6_b62;

struct UnkSmFlagArg_A6_b62 {
    u8 pad_0x0[0x238];
    UnkSmMachine_A6_b62 machine;
    u8 pad_0x2e8[0x7a4 - 0x2e8];
    bool field_0x7a4;
};

void fn_7100b27cec(UnkSmFlagArg_A6_b62* self, UnkSmArg_A6_b62* arg) {
    fn_71011d9a44(reinterpret_cast<UnkSmBase_A6_b62*>(self), arg);
    RequestState_A6_b62<3, 0x7fffffff>(&self->machine, 0);
    self->field_0x7a4 = false;
    arg->field_0x1a = true;
}

// CWeightActivatedPlatformSmartObjectComponent's unit (as fn_7100a0a08c): finds its linked object first
// when it has none, then requests state 1 when the object's smart-object user says so.
struct UnkSmartObjectUserA;
struct UnkSmartObjectActor;

struct UnkPlatformUser_A6_b62 {
    virtual void vf_0x0();
    bool vf_0x180() { return reinterpret_cast<bool (*)(UnkPlatformUser_A6_b62*)>((*reinterpret_cast<void***>(this))[0x180 / 8])(this); }
};
struct UnkPlatformLink_A6_b62 {
    u8 pad_0x0[0x10];
    UnkSmartObjectActor* object;
};
struct UnkPlatformSm_A6_b62 {
    u8 pad_0x0[0xb0];
    UnkPlatformLink_A6_b62* link;
    u8 pad_0xb8[0x180 - 0xb8];
    UnkSmMachine_A6_b62 machine;
};
void fn_7100a042e0(UnkPlatformSm_A6_b62*, void* arg);

static inline UnkSmartObjectActor* LinkedObject_A6_b62(UnkPlatformSm_A6_b62* self) {
    UnkPlatformLink_A6_b62* link = self->link;
    return link ? link->object : nullptr;
}

// `arg` (x1) reaches fn_7100a042e0 untouched in the original (tools/forwarded_args.py).
void fn_7100a0a9e0(UnkPlatformSm_A6_b62* self, void* arg) {
    if (!LinkedObject_A6_b62(self))
        fn_7100a042e0(self, arg);
    if (UnkSmartObjectActor* object = LinkedObject_A6_b62(self)) {
        UnkPlatformUser_A6_b62* user = reinterpret_cast<UnkPlatformUser_A6_b62*>(fn_71007cfd8c(object));
        if (user && user->vf_0x180())
            RequestState_A6_b62<7, 6>(&self->machine, 1);
    }
}

// CUsableComponent's unit (machine at +0x100): before use, logs to the scenario and copies three values.

struct UnkUsableArg_A6_b62 {
    u8 pad_0x0[0x44];
    u32 field_0x44;
    u8 pad_0x48[0x54 - 0x48];
    u32 field_0x54;
    u8 pad_0x58[0x64 - 0x58];
    u32 field_0x64;
};
struct UnkUsablePrepare_A6_b62 {
    u8 pad_0x0[0x30];
    void* actor;
    u8 pad_0x38[0xc8 - 0x38];
    bool field_0xc8;
    u8 pad_0xc9[0xd0 - 0xc9];
    u32 field_0xd0;
    u32 field_0xd4;
    u32 field_0xd8;
    u32 field_0xdc;
    u8 pad_0xe0[0x100 - 0xe0];
    UnkSmMachine_A6_b62 machine;
};

void fn_7100aa1fbc(UnkUsablePrepare_A6_b62* self, UnkUsableArg_A6_b62* arg) {
    if (self->machine.current->id != 2)
        self->field_0xc8 = true;
    fn_7100000790("CurrentScenario.OnUsablePrepareUse", "o", self->actor);
    self->field_0xd0 = 0;
    u32 a = arg->field_0x44;
    u32 b = arg->field_0x54;
    u32 c = arg->field_0x64;
    self->field_0xd4 = a;
    self->field_0xd8 = b;
    self->field_0xdc = c;
    RequestState_A6_b62<7, 8>(&self->machine, 3);
}

// CUsableComponent's unit (machine at +0x100): hands two of its virtual getters' values and a constant
// to fn_7100aa0b04, then requests a state.
extern const u8 g_unk_710164b668[] __attribute__((visibility("hidden")));

struct UnkUsableVirt_A6_b62 {
    void* const* vtable;
    template <u32 Slot>
    u64 Get() {
        return reinterpret_cast<u64 (*)(UnkUsableVirt_A6_b62*)>(vtable[Slot / 8])(this);
    }
    template <u32 Slot>
    void Call() {
        reinterpret_cast<void (*)(UnkUsableVirt_A6_b62*)>(vtable[Slot / 8])(this);
    }
};

struct UnkUsableComponent_A6_b62 : UnkUsableVirt_A6_b62 {
    u8 pad_0x8[0x30 - 0x8];
    void* actor;
    u8 pad_0x38[0xf0 - 0x38];
    bool field_0xf0;
    u8 pad_0xf1[0xf4 - 0xf1];
    f32 field_0xf4;
    u8 pad_0xf8[0x100 - 0xf8];
    UnkSmMachine_A6_b62 machine;
};

template <u32 GetA, u32 GetB>
static inline bool Use_A6_b62(UnkUsableComponent_A6_b62* self) {
    u64 a = self->Get<GetA>();
    return fn_7100aa0b04(self, a, self->Get<GetB>(), g_unk_710164b668);
}

void fn_7100aa210c(UnkUsableComponent_A6_b62* self) {
    if (Use_A6_b62<0x198, 0x1f8>(self))
        RequestState_A6_b62<7, 8>(&self->machine, 4);
    else
        self->Call<0x2b0>();
}

void fn_7100aa26fc(UnkUsableComponent_A6_b62* self) {
    UnkSmState_A6_b62* current = self->machine.current;
    if (current && current->id == 1)
        return;
    Use_A6_b62<0x1b8, 0x218>(self);
    RequestState_A6_b62<7, 8>(&self->machine, 1);
}

void fn_7100aa30ec(UnkUsableComponent_A6_b62* self) {
    if (fn_7100aa2948(self)) {
        Use_A6_b62<0x1b8, 0x218>(self);
        RequestState_A6_b62<7, 8>(&self->machine, 1);
    }
}

void fn_7100aa2268(UnkUsableComponent_A6_b62* self) {
    fn_7100000790("CurrentScenario.OnUsableCancelUse", "o", self->actor);
    UnkSmState_A6_b62* current = self->machine.current;
    if (current && current->id == 1)
        return;
    Use_A6_b62<0x1b8, 0x218>(self);
    RequestState_A6_b62<7, 8>(&self->machine, 1);
}

struct UnkUsableData_A6_b62 {
    u8 pad_0x0[0x19c];
    f32 field_0x19c;
};
struct UnkUsableComponent2_A6_b62 : UnkUsableComponent_A6_b62 {
    u8 pad_0x1b0[0x500 - 0x1b0];
    UnkUsableData_A6_b62* data;
};

void fn_7100aa23d4(UnkUsableComponent2_A6_b62* self) {
    Use_A6_b62<0x1a0, 0x208>(self);
    f32 v = self->data->field_0x19c;
    self->field_0xf4 = v;
    self->field_0xf0 = !(v + -0.001f <= 0.0f) | !(v + 0.001f >= 0.0f);
    fn_7100000790("CurrentScenario.OnUsableUse", "o", self->actor);
    RequestState_A6_b62<7, 8>(&self->machine, 5);
}

// After the request, optionally shows the use-success dialog with the component's text.
struct UnkUsableDialog_A6_b62 : UnkUsableComponent_A6_b62 {
    base::global::CRntString Text() {
        return reinterpret_cast<base::global::CRntString (*)(UnkUsableDialog_A6_b62*)>(vtable[0x230 / 8])(this);
    }
    void Show(const char* name, const char* text) {
        reinterpret_cast<void (*)(UnkUsableDialog_A6_b62*, const char*, const char*)>(vtable[0x260 / 8])(
            this, name, text);
    }
};

void fn_7100aa2570(UnkUsableDialog_A6_b62* self, bool show) {
    Use_A6_b62<0x1b0, 0x210>(self);
    RequestState_A6_b62<7, 8>(&self->machine, 7);
    if (show) {
        base::global::CRntString text = self->Text();
        self->Show("ShowDialogUseSuccess", text.data ? text.data : "");
    }
}

// Usable components that save the game when used: the game manager gets the owner's name and the
// user's name as strings with 0x40 bytes of storage, and a callback (this, a function of the unit).
using base::global::CRntString;

struct UnkSaveGamePair_b54 {
    u64 first;
    u64 second;
};
void fn_7100dfd174(UnkGameManager* self, CRntString* name, CRntString* tag, bool flag, u32 kind,
                   const UnkSaveGamePair_b54& extra);
extern void* g_unk_7101a1b678[];
using UnkString40_A6_b62 = UnkBufferString<0x40, g_unk_7101a1b678>;

typedef u32 __attribute__((may_alias)) UnkAliasU32_A6_b62;

// A string-id entry: the string at +0x10, a use count at +0x44 (the original reloads the id after the
// increment).
struct UnkSaveStrEntry_A6_b62 {
    u8 pad_0x0[0x10];
    CRntString str;
    u8 pad_0x40[0x44 - 0x40];
    u32 uses;
};
struct UnkSaveActor_A6_b62 {
    u8 pad_0x0[0x30];
    UnkSaveStrEntry_A6_b62* name;
};
struct UnkSaveUserLink_A6_b62 {
    u8 pad_0x0[0x10];
    UnkSaveActor_A6_b62* actor;
};

static inline const CRntString* Name_A6_b62(UnkSaveActor_A6_b62* actor) {
    (*reinterpret_cast<UnkAliasU32_A6_b62*>(&actor->name->uses))++;
    return &actor->name->str;
}

struct UnkSaveUsable_A6_b62 {
    void* const* vtable;
    u8 pad_0x8[0x30 - 0x8];
    UnkSaveActor_A6_b62* actor;
    u8 pad_0x38[0xe0 - 0x38];
    UnkSaveUserLink_A6_b62* user;

    void Call(u32 slot) { reinterpret_cast<void (*)(UnkSaveUsable_A6_b62*)>(vtable[slot / 8])(this); }
};

template <class After>
static inline void Save_A6_b62(UnkSaveUsable_A6_b62* self, void (*callback)(), After after) {
    const CRntString* owner = Name_A6_b62(self->actor);
    UnkString40_A6_b62 name;
    fn_710007eaa0(name.Str(), owner);
    const CRntString* user = Name_A6_b62(self->user->actor);
    UnkString40_A6_b62 tag;
    fn_710007eaa0(tag.Str(), user);
    fn_7100dfd174(g_unk_7101d47310, name.Str(), tag.Str(), true, 1,
                  UnkSaveGamePair_b54{reinterpret_cast<u64>(self), reinterpret_cast<u64>(callback)});
    after();
}

void fn_7100a74674();

struct UnkSaveAccessPoint_A6_b62 : UnkSaveUsable_A6_b62 {
    u8 pad_0xe8[0x4f8 - 0xe8];
    bool field_0x4f8;
    u8 pad_0x4f9[0x908 - 0x4f9];
    u32 field_0x908;
    u8 pad_0x90c[0x918 - 0x90c];
    u32 field_0x918;
};

void fn_7100a70a34(UnkSaveAccessPoint_A6_b62* self) {
    self->field_0x4f8 = true;
    self->Call(0x3a0);
    self->Call(0x298);
    self->field_0x908 = 0;
    Save_A6_b62(self, &fn_7100a74674, [self] { self->field_0x918 = 0; });
}

void fn_7100a88634();

struct UnkSaveUsable2_A6_b62 : UnkSaveUsable_A6_b62 {
    u8 pad_0xe8[0x4f8 - 0xe8];
    bool field_0x4f8;
    u8 pad_0x4f9[0x550 - 0x4f9];
    bool field_0x550;
    u8 pad_0x551[0x568 - 0x551];
    bool field_0x568;
    u8 pad_0x569[0x800 - 0x569];
    u32 field_0x800;
    u8 pad_0x804[0x814 - 0x804];
    u32 field_0x814;
};

void fn_7100a86e5c(UnkSaveUsable2_A6_b62* self) {
    self->field_0x4f8 = true;
    self->Call(0x3a0);
    self->Call(0x298);
    self->field_0x800 = 0;
    Save_A6_b62(self, &fn_7100a88634, [self] { self->field_0x814 = 0; });
}

void fn_7100a8ad44();
void fn_7100a9fa28(UnkSaveUsable2_A6_b62*);

void fn_7100a89ab0(UnkSaveUsable2_A6_b62* self) {
    fn_7100a9fa28(self);
    if (!self->field_0x568) {
        self->field_0x4f8 = true;
        self->Call(0x3a0);
    }
    self->field_0x568 = false;
    Save_A6_b62(self, &fn_7100a8ad44, [self] { self->field_0x550 = false; });
}

// The same save, then the linked object's timeline key, a state request and a use of the component's
// data (fn_7100aa0b04 with a constant).
struct UnkSaveLink_A6_b62 {
    u8 pad_0x0[0x10];
    UnkActor* actor;
};
static inline void SetKey_A6_b62(UnkSaveLink_A6_b62* link, const u64* key) {
    if (link) {
        if (UnkActor* actor = link->actor)
            fn_7100d7c828(actor, key, false);
    }
}

extern const u64 g_unk_710164b548;
extern const u8 g_unk_710164b540[] __attribute__((visibility("hidden")));

struct UnkSaveAccessPoint2_A6_b62 : UnkSaveUsable_A6_b62 {
    UnkSaveLink_A6_b62* link;
    u8 pad_0xf0[0x5e8 - 0xf0];
    UnkSmMachine_A6_b62 machine;
    u8 pad_0x698[0x698 - 0x698];
    u8* data;
    u8 pad_0x6a0[0x908 - 0x6a0];
    u32 field_0x908;
    u8 pad_0x90c[0x918 - 0x90c];
    u32 field_0x918;
};

void fn_7100a70380(UnkSaveAccessPoint2_A6_b62* self) {
    Save_A6_b62(self, &fn_7100a74674, [self] {
        self->field_0x918 = 0;
        SetKey_A6_b62(self->link, &g_unk_710164b548);
        RequestState_A6_b62<4, 5>(&self->machine, 2);
        self->field_0x908 = 0;
        u64 a = reinterpret_cast<UnkUsableVirt_A6_b62*>(self)->Get<0x1a0>();
        fn_7100aa0b04(reinterpret_cast<UnkUsableVirt_A6_b62*>(self), a,
                      reinterpret_cast<u64>(self->data + 0x1e0), g_unk_710164b540);
    });
}

extern const u64 g_unk_710164b5a8;
extern const u8 g_unk_710164b590[] __attribute__((visibility("hidden")));

struct UnkSaveUsable3_A6_b62 : UnkSaveUsable_A6_b62 {
    UnkSaveLink_A6_b62* link;
    u8 pad_0xf0[0x548 - 0xf0];
    UnkSmMachine_A6_b62 machine;
    u8 pad_0x5f8[0x7f8 - 0x5f8];
    u8* data;
    u32 field_0x800;
    u8 pad_0x804[0x814 - 0x804];
    u32 field_0x814;
};

void fn_7100a86490(UnkSaveUsable3_A6_b62* self) {
    Save_A6_b62(self, &fn_7100a88634, [self] {
        self->field_0x814 = 0;
        SetKey_A6_b62(self->link, &g_unk_710164b5a8);
        RequestState_A6_b62<5, 6>(&self->machine, 3);
        u8* data = self->data;
        self->field_0x800 = 0;
        fn_7100aa0b04(reinterpret_cast<UnkUsableVirt_A6_b62*>(self), reinterpret_cast<u64>(data + 0x1d8),
                      reinterpret_cast<u64>(data + 0x1f0), g_unk_710164b590);
    });
}

// Using an access point or a map station: while its own machine is in one of two states, or the usable
// machine is in state 6, both machines are asked for a state and the use goes on with the component's
// data; otherwise it is the plain use (fn_7100aa2570).

template <u32 MachineOffset, u32 DataOffset>
struct UnkUseStation_A6_b62 : UnkUsableDialog_A6_b62 {
    u8 pad_0x1b0[MachineOffset - 0x1b0];
    UnkSmMachine_A6_b62 station;
    u8 pad_end[DataOffset - MachineOffset - sizeof(UnkSmMachine_A6_b62)];
    u8* data;
};

void fn_7100a71040(UnkUseStation_A6_b62<0x5e8, 0x698>* self, bool show) {
    UnkSmState_A6_b62* current = self->station.current;
    if ((current && (current->id == 2 || current->id == 3)) ||
        (self->machine.current && self->machine.current->id == 6)) {
        RequestState_A6_b62<4, 5>(&self->station, 1);
        RequestState_A6_b62<7, 8>(&self->machine, 7);
        u64 a = self->Get<0x1b0>();
        fn_7100aa0b04(self, a, reinterpret_cast<u64>(self->data + 0x1e8), g_unk_710164b540);
        return;
    }
    fn_7100aa2570(self, show);
}

void fn_7100a869d4(UnkUseStation_A6_b62<0x548, 0x7f8>* self, bool show) {
    UnkSmState_A6_b62* current = self->station.current;
    if ((current && (current->id == 3 || current->id == 4)) ||
        (self->machine.current && self->machine.current->id == 6)) {
        RequestState_A6_b62<5, 6>(&self->station, 1);
        RequestState_A6_b62<7, 8>(&self->machine, 7);
        u64 a = self->Get<0x1b0>();
        fn_7100aa0b04(self, a, reinterpret_cast<u64>(self->data + 0x1f8), g_unk_710164b590);
        return;
    }
    fn_7100aa2570(self, show);
}

// CAccessPointComponent's unit: with the flag at +0x9e0 set, both machines are asked for state 1;
// otherwise the unit's own call.
struct UnkAccessPointPair_A6_b62 : UnkUseStation_A6_b62<0x5e8, 0x698> {
    u8 pad_0x6a0[0x9e0 - 0x6a0];
    bool field_0x9e0;
};
void fn_7100a6ef4c(UnkAccessPointPair_A6_b62*);
void fn_7100a6faf0(UnkAccessPointPair_A6_b62*);

void fn_7100a6a74c(UnkAccessPointPair_A6_b62* self) {
    if (!self->field_0x9e0) {
        fn_7100a6ef4c(self);
        return;
    }
    RequestState_A6_b62<4, 5>(&self->station, 1);
    RequestState_A6_b62<7, 8>(&self->machine, 1);
}

void fn_7100a6a948(UnkAccessPointPair_A6_b62* self) {
    if (!self->field_0x9e0) {
        fn_7100a6faf0(self);
        return;
    }
    RequestState_A6_b62<4, 5>(&self->station, 1);
    RequestState_A6_b62<7, 8>(&self->machine, 1);
}

// The machine's update after the component's base update (Update_b61 of
// src/game/UnkStateMachineSet_b61.cpp): applies the pending state, then updates the current state and
// applies what it asked for, at most ten times, while marked busy.
template <u32 Last, u32 None>
__attribute__((always_inline)) static inline void Apply_A6_b62(UnkSmMachine_A6_b62* self) {
    u32 id = self->pending;
    if (id > Last)
        return;
    SwitchState_A6_b62<None, false>(self, id, self->current);
}

template <u32 Last, u32 None>
__attribute__((always_inline)) static inline void Update_A6_b62(UnkSmMachine_A6_b62* self, f32 dt) {
    self->busy = true;
    if (self->pending != None)
        Apply_A6_b62<Last, None>(self);
    u32 i = 0;
    while (UnkSmState_A6_b62* current = self->current) {
        current->vf_0x20(dt);
        if (self->pending == None)
            break;
        Apply_A6_b62<Last, None>(self);
        if (i++ > 8)
            break;
    }
    self->busy = false;
}

// CUsableComponent's update (declared as in src/game/CThermalDeviceComponent/CThermalDeviceComponent_b50.cpp).
struct UnkThermalDevice_b50a5;
void fn_7100a6dec4(UnkUseStation_A6_b62<0x5e8, 0x698>*);

void fn_7100a6da5c(UnkUseStation_A6_b62<0x5e8, 0x698>* self, f32 dt) {
    fn_7100a9fad8(reinterpret_cast<UnkThermalDevice_b50a5*>(self), dt);
    Update_A6_b62<4, 5>(&self->station, dt);
    fn_7100a6dec4(self);
}

void fn_7100a85c08(UnkUseStation_A6_b62<0x548, 0x7f8>* self, f32 dt) {
    fn_7100a9fad8(reinterpret_cast<UnkThermalDevice_b50a5*>(self), dt);
    Update_A6_b62<5, 6>(&self->station, dt);
}

struct UnkParent;

void fn_7100ac98e4(UnkSmOwner_A6_b62<0x130>* self, f32 dt) {
    fn_71010c424c(reinterpret_cast<UnkParent*>(self));
    Update_A6_b62<2, 3>(&self->machine, dt);
}
