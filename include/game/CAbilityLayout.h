#pragma once

// The objects of CAbilityEnergyWave's unit (tools/tu_map.py unit 429), as far as its hand-written sources
// know them: one layout each, grown by each source that reads a member the layout does not have yet.
//
// - CAbilityComponent (reflection: 0x108 bytes, base CComponent of 0x40): the actor's list of abilities at
//   +0x40 and the one selected at +0x68. Most of the unit's functions are its methods; they call
//   CComponent's (fn_71010c4228) on themselves, which is how the base is known.
// - CAbility (reflection: 0x390 bytes, base CGameObject): an entry of that list, seen through the members
//   every ability has (its state, its component, its definition) and its vtable.
// - CAbilityEnergyWave (reflection: 0x590 bytes, base CAbility): the energy wave's own members from 0x390.
//
// The class names and sizes come from reflection (src/reflection/); the member names and everything the
// members point to are placeholders. The owning actor (the component's +0x30) is left as the base
// component's untyped slot: each source still reads it through its own view, until the actor has one layout.
#include "base/UnkVector.h"
#include "base/UnkNamedRegistry.h"
#include "base/global/CStrId.h"
#include "game/UnkComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

namespace base::reflection {
class CClass;
}

struct CAbility;
struct CAbilityComponent;

// What an ability's +0x20 points to while its own state is 7: something whose state (+0x10) stands in for
// the ability's.
struct UnkAbilityStateHolder {
    u8 pad_0x0[0x10];
    s32 state;  // 0x10
};

// An ability's definition (+0xc8): the name the component looks abilities up by, and a second string id
// that fn_7100301860 switches abilities on by.
struct UnkAbilityDef {
    base::global::CStrId name;  // 0x0
    u8 pad_0x8[0x18];
    base::global::CStrId id;  // 0x20
};

struct CAbility {
    void** vtable;
    u8 pad_0x8[0x18];
    UnkAbilityStateHolder* state_holder;  // 0x20
    u8 pad_0x28[0x8c];
    s32 state;  // 0xb4; 7 defers to the state holder's
    u8 pad_0xb8[0x8];
    CAbilityComponent* component;  // 0xc0
    UnkAbilityDef* def;            // 0xc8
    u8 pad_0xd0[0xe];
    bool selectable;  // 0xde
    u8 pad_0xdf[0x390 - 0xdf];

    // The state, 7 deferring to the state holder's when there is one (7 stays when there is none).
    s32 State() const {
        s32 s = state;
        if (s == 7 && state_holder)
            s = state_holder->state;
        return s;
    }

    // Slot 0: the ability's reflected class.
    const base::reflection::CClass* GetClass() {
        return reinterpret_cast<const base::reflection::CClass* (*)(CAbility*)>(vtable[0])(this);
    }

    // The virtual slots the unit calls, by byte offset in the vtable.
    template <class R = void, class... A>
    R Virtual(u32 offset, A... args) {
        return reinterpret_cast<R (*)(CAbility*, A...)>(vtable[offset / 8])(this, args...);
    }
};
static_assert(sizeof(CAbility) == 0x390, "CAbility layout");

// One of the two effects of the wave (+0x390, +0x398): turned on and off by the hash at +0x30.
struct UnkEnergyWaveFx {
    u8 pad_0x0[0x30];
    u64 hash;  // 0x30
};

// The primary gun the wave holds (+0x3d0): its class is read through vtable slot 0.
struct UnkEnergyWaveGun {
    void** vtable;
    const base::reflection::CClass* GetClass() {
        return reinterpret_cast<const base::reflection::CClass* (*)(UnkEnergyWaveGun*)>(vtable[0])(this);
    }
};

// A handle (+0x400) to what fn_7100072328 fades out: current while its serial is the target's.
struct UnkEnergyWaveHandleTarget {
    void* object;
    u64 serial;  // 0x8
};

struct UnkEnergyWaveHandle {
    UnkEnergyWaveHandleTarget* target;
    u64 serial;  // 0x8; -1 for none
};

struct CAbilityEnergyWave : CAbility {
    UnkEnergyWaveFx* fx_a;  // 0x390
    UnkEnergyWaveFx* fx_b;  // 0x398
    u8 pad_0x3a0[0x8];
    base::global::CStrId gun_id;  // 0x3a8
    u8 pad_0x3b0[0x20];
    UnkEnergyWaveGun* gun;  // 0x3d0
    u8 pad_0x3d8[0x28];
    UnkEnergyWaveHandle handle;  // 0x400
    u8 pad_0x410[0x590 - 0x410];
};
static_assert(__builtin_offsetof(CAbilityEnergyWave, gun) == 0x3d0, "CAbilityEnergyWave layout");
static_assert(__builtin_offsetof(CAbilityEnergyWave, handle) == 0x400, "CAbilityEnergyWave layout");
static_assert(sizeof(CAbilityEnergyWave) == 0x590, "CAbilityEnergyWave layout");

// The component's input listener (+0x78), named "ABILITY_INPUT": an object with a vtable of its own, its
// name and an array of 20 entries of 0x1c bytes after their count.
struct UnkAbilityInputListener {
    void** vtable;                        // 0x78
    base::UnkNamedEntry name;             // 0x80; a CRntString built without its constructor
    u64 capacity;                         // 0xb0
    void* entries;                        // 0xb8
};

// What the component holds the fire button with (+0x70).
struct UnkAbilityHold {
    s32 mode;
    f32 time;
};

// A component's owning actor (its +0x30), through the view the caller has of it.
template <class T>
inline T* OwnerOf(const UnkBaseComponent* component) {
    return static_cast<T*>(component->field_0x30);
}

struct CAbilityComponent : UnkBaseComponent {
    base::UnkVector<CAbility*> abilities;  // 0x40
    CAbility* current;                     // 0x68; the selected ability
    UnkAbilityHold hold;                   // 0x70
    UnkAbilityInputListener input;         // 0x78
    UnkBaseComponent* target;              // 0xc0; the component whose actor's input component has the listener
    bool field_0xc8;
    f32 field_0xcc;  // compared with the tunable's +0x34 before the selected ability fires
    bool field_0xd0;
    bool field_0xd1;
    bool field_0xd2;  // a refresh of the abilities is pending
    bool field_0xd3;
    bool field_0xd4;
    f32 energy;  // 0xd8; -1 when there is none
    u8 pad_0xdc[0x4];
    f32 timer;  // 0xe0
    u8 pad_0xe4[0x4];
    UnkWeakHandle* life;  // 0xe8; its target is the actor's life component
    u32 field_0xf0;
    base::global::CStrId field_0xf8;
    void* field_0x100;

    // The component's own virtual slots, by byte offset in the vtable.
    template <class R = void, class... A>
    R Virtual(u32 offset, A... args) {
        return reinterpret_cast<R (*)(CAbilityComponent*, A...)>(vtable[offset / 8])(this, args...);
    }

    CAbility** begin() const { return abilities.data; }
    CAbility** end() const { return abilities.data + abilities.count; }

    // The slot in [it, end) of the ability whose definition is named `key`, or `end`. The first ability is
    // tested before the loop, as the original does.
    static CAbility** FindByName(CAbility** it, CAbility** end, base::global::CStrId key) {
        if (it != end) {
            if ((*it)->def->name.data == key.data)
                return it;
            for (++it; it != end; ++it)
                if ((*it)->def->name.data == key.data)
                    return it;
        }
        return end;
    }

    // The owning actor (the base component's +0x30), through the view the caller has of it.
    template <class T>
    T* Owner() const {
        return OwnerOf<T>(this);
    }
};
static_assert(__builtin_offsetof(CAbilityComponent, field_0x100) == 0x100, "CAbilityComponent layout");
static_assert(sizeof(CAbilityComponent) == 0x108, "CAbilityComponent layout");
