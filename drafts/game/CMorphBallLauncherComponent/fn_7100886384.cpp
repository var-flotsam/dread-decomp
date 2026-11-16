// CMorphBallLauncherComponent's unit: the component's destructor. Its two vtables, the listeners at +0x368
// and +0x348, the references at +0x318 and +0x310, the name, the callback set, the state machine and its four
// states, then the component base's destructor. Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkDestructors.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAbilityOpticCamouflage.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/CGunComponent.h"
#include "game/UnkComponent.h"
#include "game/UnkTU341.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

extern void* g_unk_7101a7bdf0[];
extern void* g_unk_7101a7c000[];
extern void* g_unk_7101a7c058[];

struct UnkMblCallbacks_l886384;
template <>
struct base::UnkCallbackTraits<UnkMblCallbacks_l886384> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a33fa0); }
    static char* base_vtable() { return reinterpret_cast<char*>(g_unk_7101a20998); }
};
using UnkMblCallbackList_l886384 = base::UnkCallbackListShort<UnkMblCallbacks_l886384>;

// A counted link to the object a listener is registered with.
struct UnkMblLink_l886384 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Release();  // 0x10
    u32 ref_count;           // 0x8
    void* owner;             // 0x10
};

// The listener at +0x368: it unregisters from its link's owner (fn_710083b0bc) on destruction.
struct UnkMblGunListener_l886384 {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkMblLink_l886384* link;  // 0x18

    ~UnkMblGunListener_l886384() {
        vtable = &g_unk_7101a23280[2];
        if (UnkMblLink_l886384* l = link) {
            bool held = true;
            if (void* owner = l->owner) {
                fn_710083b0bc(owner, reinterpret_cast<UnkListener_b40*>(this));
                l = link;
                held = l != nullptr;
            }
            if (held && --l->ref_count == 0)
                l->Release();
        }
    }
};

// The listener at +0x348: it unregisters from its link's owner (fn_710117b3f0) on destruction.
struct UnkMblWeakListener_l886384 {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkMblLink_l886384* link;  // 0x18

    ~UnkMblWeakListener_l886384() {
        vtable = &g_unk_7101a22d48[2];
        if (UnkMblLink_l886384* l = link) {
            bool held = true;
            if (void* owner = l->owner) {
                fn_710117b3f0(static_cast<UnkWeakTarget*>(owner), this);
                l = link;
                held = l != nullptr;
            }
            if (held && --l->ref_count == 0)
                l->Release();
        }
    }
};

// A bound callback.
struct UnkMblDelegate_l886384 {
    void* object;
    void (*function)();
};

// A reflected value that releases what it holds on destruction.
struct UnkMblValue_l886384 {
    base::reflection::UnkReflValue value;

    ~UnkMblValue_l886384() { fn_71000a1c60(&value); }
};

// The states' base: it unbinds its callbacks, last bound first, on destruction.
struct UnkMblStateBase_l886384 {
    void** vtable;
    void* machine;
    u32 id;
    UnkMblValue_l886384 value;  // 0x18
    UnkMblDelegate_l886384 field_0x30;
    UnkMblDelegate_l886384 field_0x40;
    UnkMblDelegate_l886384 field_0x50;

    ~UnkMblStateBase_l886384() {
        vtable = &g_unk_7101a7c000[2];
        field_0x50 = {};
        field_0x40 = {};
        field_0x30 = {};
    }
};

struct UnkMblState_l886384 : UnkMblStateBase_l886384 {
    u64 field_0x60;
    u64 field_0x68;

    ~UnkMblState_l886384() {
        field_0x60 = 0;
        field_0x68 = 0;
    }
};
static_assert(sizeof(UnkMblState_l886384) == 0x70, "layout");

struct UnkMblMachine_l886384 {
    void** vtable;
    void* owner;
    UnkMblState_l886384* current;
    UnkMblState_l886384* previous;
    u32 count;
    u8 pad_0x24[0xb0 - 0x24];

    ~UnkMblMachine_l886384() {
        vtable = &g_unk_7101a7c058[2];
        current = nullptr;
        previous = nullptr;
        count = 0;
    }
};
static_assert(sizeof(UnkMblMachine_l886384) == 0xb0, "layout");

struct UnkMorphBallLauncherD_l886384 {
    void** vtable;
    u8 pad_0x8[0x38];
    void** vtable_0x40;
    UnkMblState_l886384 state_1;           // 0x48
    UnkMblState_l886384 state_2;           // 0xb8
    UnkMblState_l886384 state_3;           // 0x128
    UnkMblState_l886384 state_4;           // 0x198
    UnkMblMachine_l886384 machine;         // 0x208
    UnkMblCallbackList_l886384 callbacks;  // 0x2b8
    base::UnkStrIdMember name;             // 0x2f8
    bool field_0x300;
    void* field_0x308;
    base::UnkRefPtrMember field_0x310;
    base::UnkRefPtrMember field_0x318;
    u8 pad_0x320[0x28];
    UnkMblWeakListener_l886384 listener_0x348;
    UnkMblGunListener_l886384 listener_0x368;
};
static_assert(__builtin_offsetof(UnkMorphBallLauncherD_l886384, callbacks) == 0x2b8, "layout");
static_assert(__builtin_offsetof(UnkMorphBallLauncherD_l886384, listener_0x368) == 0x368, "layout");

void fn_7100886384(UnkMorphBallLauncherD_l886384* self) {
    self->vtable = g_unk_7101a7bdf0 + 2;
    self->vtable_0x40 = g_unk_7101a7bdf0 + 52;
    self->listener_0x368.~UnkMblGunListener_l886384();
    self->listener_0x348.~UnkMblWeakListener_l886384();
    using base::UnkRefPtrMember;
    self->field_0x318.~UnkRefPtrMember();
    self->field_0x310.~UnkRefPtrMember();
    using base::UnkStrIdMember;
    self->name.~UnkStrIdMember();
    self->callbacks.~UnkMblCallbackList_l886384();
    self->machine.~UnkMblMachine_l886384();
    self->state_4.~UnkMblState_l886384();
    self->state_3.~UnkMblState_l886384();
    self->state_2.~UnkMblState_l886384();
    self->state_1.~UnkMblState_l886384();
    fn_71010c4194(reinterpret_cast<UnkBaseComponent*>(self));
}
