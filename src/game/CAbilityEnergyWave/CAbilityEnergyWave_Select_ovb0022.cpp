// CAbilityEnergyWave's unit (tools/tu_map.py): the ability set (+0x40, count +0x48), looked up by the name at
// +0xc8 of each ability; the selected one is kept at +0x68 and handed to the owner's (+0x30) two
// components (+0x260, +0x210). Also reflected wrappers that toggle and query abilities by name, the flags of
// two abilities, the energy update and the firing of the selected ability. Names and layouts are
// placeholders.
#include <new>
#include "base/UnkNamedRegistry.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CAbility.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CAbilityShinespark.h"
#include "game/CAbilitySpeedBooster.h"
#include "game/CFXComponent.h"
#include "game/CGrabComponent.h"
#include "game/CModelUpdaterComponent.h"
#include "game/CSpecialEnergyComponent.h"
#include "game/UnkComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkSwitchObject.h"
#include "game/UnkTU1428.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CClass;
using base::reflection::UnkReflValue;

struct UnkSelf;
struct UnkOwner;
struct UnkObject;
struct UnkComponent;
struct UnkPlayer;
struct UnkWorld;
struct UnkEntry;
struct UnkLifeComponent;
struct UnkSamusGunComponent;
struct UnkScriptCaller;
struct UnkSmLockon_A1_b64;
struct UnkSmAbility3_A2_b63;
struct UnkAbilityC_A2_b69;
struct UnkStruct_71002fda50;
struct UnkWaveChild_A2_b72;
struct UnkStateObject;
struct UnkEnergyWaveCompA_b49;
struct UnkEnergyWaveCompB_b49;
struct UnkEnergyWaveObject_b49;
struct UnkEnergyWaveFxComp_b49;

UnkOwner* fn_7100defcd4(UnkGameManager*);
UnkPlayer* fn_7100dec788(UnkGameManager*);
extern char g_unk_7101648d28[];
extern const u64 g_unk_7101648d18;

// The id table (fn_7100031894 builds it; as src/game/CEmmyAIComponent/CEmmyAIComponent_IsStateId_b56.cpp).
struct UnkGameManagerIdTable {
    u8 pad_0x0[0xf18];
    CStrId field_0xf18;
    u8 pad_0xf20[0x2578 - 0xf20];
    UnkGameManagerIdTable() asm("fn_7100031894");
    ~UnkGameManagerIdTable() asm("fn_71000407f0");
};
inline UnkGameManagerIdTable& GameManagerIdTable() {
    static UnkGameManagerIdTable s_table;
    return s_table;
}

// ---- The ability set ----

struct UnkAbilityState_ovb22 {
    u8 pad_0x0[0x10];
    s32 state;  // 0x10
};

struct UnkAbilityName_ovb22 {
    CStrId id;
};

struct UnkAbility_ovb22 {
    void** vtable;
    u8 pad_0x8[0x18];
    UnkAbilityState_ovb22* parent;  // 0x20
    u8 pad_0x28[0x8c];
    s32 state;  // 0xb4
    u8 pad_0xb8[0x10];
    UnkAbilityName_ovb22* name;  // 0xc8
    u8 pad_0xd0[0xe];
    bool selectable;  // 0xde

    // The state, 7 deferring to the parent's.
    s32 State() const {
        s32 s = state;
        if (s == 7 && parent)
            s = parent->state;
        return s;
    }
    bool IsActive() const { return State() == 6; }
    void vf_0x88() { reinterpret_cast<void (*)(UnkAbility_ovb22*)>(vtable[0x88 / 8])(this); }
    void vf_0x90() { reinterpret_cast<void (*)(UnkAbility_ovb22*)>(vtable[0x90 / 8])(this); }
};

struct UnkAbilityOwner_ovb22 {
    u8 pad_0x0[0x210];
    UnkEnergyWaveCompB_b49* field_0x210;
    u8 pad_0x218[0x48];
    UnkEnergyWaveCompA_b49* field_0x260;
};

struct UnkAbilitySet_ovb22 {
    u8 pad_0x0[0x30];
    UnkAbilityOwner_ovb22* owner;  // 0x30
    u8 pad_0x38[0x8];
    UnkAbility_ovb22** abilities;  // 0x40
    u32 count;                     // 0x48
    u8 pad_0x4c[0x1c];
    UnkAbility_ovb22* current;  // 0x68
};

// The first ability is tested before the loop, as in fn_7100301cf0.
static inline UnkAbility_ovb22** FindByName(UnkAbility_ovb22** begin, UnkAbility_ovb22** end, CStrId key) {
    UnkAbility_ovb22** it = begin;
    if (it != end) {
        if ((*it)->name->id.data == key.data)
            return it;
        for (++it; it != end; ++it)
            if ((*it)->name->id.data == key.data)
                return it;
    }
    return end;
}

static inline UnkAbility_ovb22** FindSlot(UnkAbilitySet_ovb22* self, const CStrId* id) {
    return FindByName(self->abilities, self->abilities + self->count, *id);
}

static inline UnkAbility_ovb22* Find(UnkAbilitySet_ovb22* self, const CStrId* id) {
    UnkAbility_ovb22** it = FindByName(self->abilities, self->abilities + self->count, *id);
    if (it == self->abilities + self->count)
        return nullptr;
    return *it;
}

// Hands the selected ability to the owner's two components.
static inline void Attach(UnkAbilitySet_ovb22* self, UnkAbility_ovb22* ability) {
    if (self->owner->field_0x260)
        fn_710113f270(self->owner->field_0x260, ability);
    if (self->owner->field_0x210)
        fn_710094f8f0(self->owner->field_0x210, reinterpret_cast<UnkEnergyWaveObject_b49*>(self->current));
}

// Selects the ability named `id` when it is selectable, its state 2 or more; the components are refreshed
// when it is already selected. `notify` plays the timeline key g_unk_7101648d18 on the owner.
bool fn_71003010e0(UnkAbilitySet_ovb22* self, const CStrId* id, bool notify, bool) {
    UnkAbility_ovb22* ability = Find(self, id);
    if (ability && ability->selectable && (u32)ability->State() >= 2) {
        UnkAbility_ovb22* current = self->current;
        if (current && current->name->id.data == id->data) {
            Attach(self, current);
            return true;
        }
        if ((u32)ability->State() >= 2) {
            if (current != ability) {
                if (current)
                    current->vf_0x90();
                self->current = ability;
                ability->vf_0x88();
            }
            Attach(self, self->current);
        }
        if (notify)
            fn_7100d7c828(reinterpret_cast<UnkActor*>(self->owner), &g_unk_7101648d18, false);
        return true;
    }
    return false;
}

// Drops the selected ability when its state is below 2, then selects the one the id table names at +0xf18.
bool fn_71002fdbc0(UnkAbilitySet_ovb22* self, bool) {
    if (UnkAbility_ovb22* current = self->current) {
        if ((u32)current->State() > 1)
            return false;
        self->current = nullptr;
    }
    UnkAbility_ovb22* ability = Find(self, &GameManagerIdTable().field_0xf18);
    if (ability && ability->selectable) {
        if ((u32)ability->State() >= 2) {
            if (self->current != ability) {
                if (self->current)
                    self->current->vf_0x90();
                self->current = ability;
                ability->vf_0x88();
            }
            Attach(self, self->current);
        }
        return true;
    }
    return false;
}

struct UnkSelf;

// Toggles the ability named `id` (fn_71002f9460).
void fn_71003018f4(UnkAbilitySet_ovb22* self, const CStrId* id, bool on) {
    UnkAbility_ovb22** it = FindSlot(self, id);
    if (it != self->abilities + self->count && *it)
        fn_71002f9460(reinterpret_cast<UnkSelf*>(*it), on);
}

// The reflected wrappers (the object at +8 of `obj`; arguments in the array at args[0]).

// A pointer argument's value: null without a type.
template <class T>
static inline const T* ArgPtr(UnkReflValue* arg) {
    return arg->type ? base::reflection::refl_ArgValue<T>(arg) : nullptr;
}

static inline UnkReflValue BoolValue(bool value) {
    const base::reflection::CType* type = fn_710000d228();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &value, false);
    return r;
}

void fn_710030275c(char* obj, UnkReflValue* const* args) {
    UnkReflValue* a = args[0];
    UnkAbilitySet_ovb22* self = *reinterpret_cast<UnkAbilitySet_ovb22**>(obj + 8);
    const CStrId* id = ArgPtr<CStrId>(&a[0]);
    bool on = *base::reflection::refl_ArgValue<u8>(&a[1]) != 0;
    UnkAbility_ovb22** it = FindSlot(self, id);
    if (it != self->abilities + self->count && *it)
        fn_71002f9694(reinterpret_cast<UnkAbilityC_A2_b69*>(*it), on);
}

// Whether the ability named by the argument is active (state 6).
UnkReflValue fn_7100302928(void*, char* obj, UnkReflValue* const* args) {
    UnkAbilitySet_ovb22* self = *reinterpret_cast<UnkAbilitySet_ovb22**>(obj + 8);
    const CStrId* id = base::reflection::refl_ArgValue<CStrId>(args[0]);
    UnkAbility_ovb22* ability = Find(self, id);
    return BoolValue(ability && ability->IsActive());
}

// ---- The flags of two abilities ----

UnkEntry* fn_7100301cf0(UnkOwner* self, const CStrId* id);
UnkEntry* fn_7100301e20(UnkOwner* self, const CStrId* id);

// A set of flags (a u32) that starts empty; the empty set is a shared constant.
struct UnkEnergyWaveFlags_ovb22 {
    u32 bits;
    UnkEnergyWaveFlags_ovb22() : bits(0) {}
    UnkEnergyWaveFlags_ovb22(const UnkEnergyWaveFlags_ovb22& o) : bits(o.bits) {}
};

inline const UnkEnergyWaveFlags_ovb22& NoFlags() {
    static UnkEnergyWaveFlags_ovb22 s_none;
    return s_none;
}

UnkEnergyWaveFlags_ovb22 fn_710033e498(UnkEntry* entry, void* a, void* b);
UnkEnergyWaveFlags_ovb22 fn_710032b328(UnkEntry* entry, void* a, void* b);

// The flags of the two abilities fn_710033b6a4 and fn_7100327d14 name, combined.
UnkEnergyWaveFlags_ovb22 fn_7100301c0c(UnkOwner* self, void* a, void* b) {
    UnkEnergyWaveFlags_ovb22 flags = NoFlags();
    if (UnkEntry* entry = fn_7100301cf0(self, reinterpret_cast<const CStrId*>(fn_710033b6a4()))) {
        UnkEnergyWaveFlags_ovb22 more = fn_710033e498(entry, a, b);
        flags.bits = more.bits | flags.bits;
    }
    if (UnkEntry* entry = fn_7100301e20(self, reinterpret_cast<const CStrId*>(fn_7100327d14()))) {
        UnkEnergyWaveFlags_ovb22 more = fn_710032b328(entry, a, b);
        flags.bits = flags.bits | more.bits;
    }
    return flags;
}

// ---- The wave controller ----

struct UnkEnergyWaveLifeHolder_ovb22 {
    u8 pad_0x0[0x10];
    UnkLifeComponent* life;  // 0x10
};

struct UnkEnergyWaveLifeValue_ovb22 {
    u8 pad_0x0[0x40];
    f32 value;  // 0x40
};

struct UnkEnergyWave_ovb22 {
    u8 pad_0x0[0xe8];
    UnkEnergyWaveLifeHolder_ovb22* holder;  // 0xe8
};

void fn_71002fe5f8(UnkEnergyWave_ovb22* self, f32 dt);

// Updates the wave without time passing, then sets the energy to its current value (+0x40).
void fn_71002fe5a0(UnkEnergyWave_ovb22* self) {
    fn_71002fe5f8(self, 0.0f);
    UnkLifeComponent* life = nullptr;
    f32 value = 0.0f;
    if (self->holder) {
        life = self->holder->life;
        if (life)
            value = reinterpret_cast<UnkEnergyWaveLifeValue_ovb22*>(life)->value;
    }
    // The value is passed ahead of the flags here (same registers either way), which orders the moves.
    using SetValueFn = void (*)(UnkLifeComponent*, f32, bool, bool);
    reinterpret_cast<SetValueFn>(&fn_7100a1fa98)(life, value, false, false);
}

struct UnkEnergyWaveTunables_ovb22 {
    u8 pad_0x0[0x34];
    f32 field_0x34;
};
extern UnkEnergyWaveTunables_ovb22 g_unk_71726aeb90;
bool fn_71002f8da8(UnkAbility_ovb22* ability);
void fn_710113f3fc(UnkEnergyWaveCompA_b49* comp);
void fn_710113f378(UnkEnergyWaveCompA_b49* comp);
void fn_710094fa7c(UnkEnergyWaveCompB_b49* comp);
void fn_710094f9f8(UnkEnergyWaveCompB_b49* comp);

struct UnkAbility2_ovb22 {
    void** vtable;
    void vf_0xa0(bool on) { reinterpret_cast<void (*)(UnkAbility2_ovb22*, bool)>(vtable[0xa0 / 8])(this, on); }
    bool vf_0xa8() { return reinterpret_cast<bool (*)(UnkAbility2_ovb22*)>(vtable[0xa8 / 8])(this); }
    bool vf_0xb0() { return reinterpret_cast<bool (*)(UnkAbility2_ovb22*)>(vtable[0xb0 / 8])(this); }
    f32 vf_0xc0() { return reinterpret_cast<f32 (*)(UnkAbility2_ovb22*)>(vtable[0xc0 / 8])(this); }
};

struct UnkEnergyWaveHold2_ovb22 {
    s32 mode;
    f32 time;
};

struct UnkEnergyWaveFull_ovb22 {
    u8 pad_0x0[0x30];
    UnkAbilityOwner_ovb22* owner;  // 0x30
    u8 pad_0x38[0x30];
    UnkAbility2_ovb22* current;     // 0x68
    UnkEnergyWaveHold2_ovb22 hold;  // 0x70
    u8 pad_0x78[0x54];
    f32 field_0xcc;
    u8 pad_0xd0[0x18];
    UnkEnergyWaveLifeHolder_ovb22* life;  // 0xe8
};

// Fires the selected ability while +0xcc is below the tunable's +0x34: a ready one (fn_71002f8da8) through
// its slot 0xa0, else, with energy (fn_7100a1ff18) for its cost (slot 0xc0, at least 1e-5), through
// fn_71002fa070. A failed attempt sets the hold mode to 1.
void fn_7100300ec0(UnkEnergyWaveFull_ovb22* self) {
    if (!(self->field_0xcc < g_unk_71726aeb90.field_0x34))
        return;
    if (!self->current)
        return;
    if (fn_71002f8da8(reinterpret_cast<UnkAbility_ovb22*>(self->current))) {
        if (self->current->vf_0xb0()) {
            self->current->vf_0xa0(true);
            if (self->owner->field_0x260)
                fn_710113f3fc(self->owner->field_0x260);
            if (self->owner->field_0x210)
                fn_710094fa7c(self->owner->field_0x210);
            return;
        }
    } else {
        f32 value = fn_7100a1ff18(self->life ? self->life->life : nullptr);
        f32 cost = self->current->vf_0xc0();
        f32 need = cost < 1e-5f ? 1e-5f : cost;
        if (!(value >= need)) {
            GameManagerIdTable();
            return;
        }
        if (self->current->vf_0xa8()) {
            fn_71002fa070(reinterpret_cast<UnkSmAbility3_A2_b63*>(self->current));
            if (self->owner->field_0x260)
                fn_710113f378(self->owner->field_0x260);
            if (self->owner->field_0x210)
                fn_710094f9f8(self->owner->field_0x210);
            return;
        }
    }
    GameManagerIdTable();
    if (self->hold.mode != 1)
        self->hold = {1, 0.0f};
}
