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
#include "game/CAbilityLayout.h"
#include "game/CAbilityShinespark.h"
#include "game/CAbilitySpeedBooster.h"
#include "game/CFXComponent.h"
#include "game/CGrabComponent.h"
#include "game/CModelUpdaterComponent.h"
#include "game/CSpecialEnergyComponent.h"
#include "game/UnkComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkSwitchObject.h"
#include "game/UnkTU1428.h"
#include "game/UnkTU2553.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CClass;
using base::reflection::UnkReflValue;

struct UnkSelf;
struct UnkObject;
struct UnkComponent;
struct UnkPlayer;
struct UnkWorld;
struct UnkLifeComponent;
struct UnkSamusGunComponent;
struct UnkScriptCaller;
struct UnkSmLockon_A1_b64;
struct UnkSmAbility3_A2_b63;
struct UnkAbilityC_A2_b69;
struct UnkStateObject;
struct UnkEnergyWaveCompA_b49;
struct UnkEnergyWaveCompB_b49;
struct UnkEnergyWaveObject_b49;
struct UnkEnergyWaveFxComp_b49;

extern char g_unk_7101648d28[];
extern const u64 g_unk_7101648d18;

// ---- The ability set ----

struct UnkAbilityOwner_ovb22 {
    u8 pad_0x0[0x210];
    UnkEnergyWaveCompB_b49* field_0x210;
    u8 pad_0x218[0x48];
    UnkEnergyWaveCompA_b49* field_0x260;
};

static inline CAbility** FindSlot(CAbilityComponent* self, const CStrId* id) {
    return CAbilityComponent::FindByName(self->begin(), self->end(), *id);
}

static inline CAbility* Find(CAbilityComponent* self, const CStrId* id) {
    CAbility** it = CAbilityComponent::FindByName(self->begin(), self->end(), *id);
    if (it == self->end())
        return nullptr;
    return *it;
}

// Whether the ability is active (state 6).
static inline bool IsActive(const CAbility* ability) {
    return ability->State() == 6;
}

// Hands the selected ability to the owner's two components.
static inline void Attach(CAbilityComponent* self, CAbility* ability) {
    if (self->Owner<UnkAbilityOwner_ovb22>()->field_0x260)
        fn_710113f270(self->Owner<UnkAbilityOwner_ovb22>()->field_0x260, ability);
    if (self->Owner<UnkAbilityOwner_ovb22>()->field_0x210)
        fn_710094f8f0(self->Owner<UnkAbilityOwner_ovb22>()->field_0x210,
                      reinterpret_cast<UnkEnergyWaveObject_b49*>(self->current));
}

// Selects the ability named `id` when it is selectable, its state 2 or more; the components are refreshed
// when it is already selected. `notify` plays the timeline key g_unk_7101648d18 on the owner.
bool fn_71003010e0(CAbilityComponent* self, const CStrId* id, bool notify, bool) {
    CAbility* ability = Find(self, id);
    if (ability && ability->selectable && (u32)ability->State() >= 2) {
        CAbility* current = self->current;
        if (current && current->def->name.data == id->data) {
            Attach(self, current);
            return true;
        }
        if ((u32)ability->State() >= 2) {
            if (current != ability) {
                if (current)
                    current->Virtual(0x90);
                self->current = ability;
                ability->Virtual(0x88);
            }
            Attach(self, self->current);
        }
        if (notify)
            fn_7100d7c828(self->Owner<UnkActor>(), &g_unk_7101648d18, false);
        return true;
    }
    return false;
}

// Drops the selected ability when its state is below 2, then selects the one the id table names at +0xf18.
bool fn_71002fdbc0(CAbilityComponent* self, bool) {
    if (CAbility* current = self->current) {
        if ((u32)current->State() > 1)
            return false;
        self->current = nullptr;
    }
    CAbility* ability = Find(self, &GameManagerIdTable().field_0xf18);
    if (ability && ability->selectable) {
        if ((u32)ability->State() >= 2) {
            if (self->current != ability) {
                if (self->current)
                    self->current->Virtual(0x90);
                self->current = ability;
                ability->Virtual(0x88);
            }
            Attach(self, self->current);
        }
        return true;
    }
    return false;
}

struct UnkSelf;

// Toggles the ability named `id` (fn_71002f9460).
void fn_71003018f4(CAbilityComponent* self, const CStrId* id, bool on) {
    CAbility** it = FindSlot(self, id);
    if (it != self->end() && *it)
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
    CAbilityComponent* self = *reinterpret_cast<CAbilityComponent**>(obj + 8);
    const CStrId* id = ArgPtr<CStrId>(&a[0]);
    bool on = *base::reflection::refl_ArgValue<u8>(&a[1]) != 0;
    CAbility** it = FindSlot(self, id);
    if (it != self->end() && *it)
        fn_71002f9694(reinterpret_cast<UnkAbilityC_A2_b69*>(*it), on);
}

// Whether the ability named by the argument is active (state 6).
UnkReflValue fn_7100302928(void*, char* obj, UnkReflValue* const* args) {
    CAbilityComponent* self = *reinterpret_cast<CAbilityComponent**>(obj + 8);
    const CStrId* id = base::reflection::refl_ArgValue<CStrId>(args[0]);
    CAbility* ability = Find(self, id);
    return BoolValue(ability && IsActive(ability));
}

// ---- The flags of two abilities ----

CAbility* fn_7100301cf0(CAbilityComponent* self, const CStrId* id);
CAbility* fn_7100301e20(CAbilityComponent* self, const CStrId* id);

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

UnkEnergyWaveFlags_ovb22 fn_710033e498(CAbility* entry, void* a, void* b);
UnkEnergyWaveFlags_ovb22 fn_710032b328(CAbility* entry, void* a, void* b);

// The flags of the two abilities fn_710033b6a4 and fn_7100327d14 name, combined.
UnkEnergyWaveFlags_ovb22 fn_7100301c0c(CAbilityComponent* self, void* a, void* b) {
    UnkEnergyWaveFlags_ovb22 flags = NoFlags();
    if (CAbility* entry = fn_7100301cf0(self, reinterpret_cast<const CStrId*>(fn_710033b6a4()))) {
        UnkEnergyWaveFlags_ovb22 more = fn_710033e498(entry, a, b);
        flags.bits = more.bits | flags.bits;
    }
    if (CAbility* entry = fn_7100301e20(self, reinterpret_cast<const CStrId*>(fn_7100327d14()))) {
        UnkEnergyWaveFlags_ovb22 more = fn_710032b328(entry, a, b);
        flags.bits = flags.bits | more.bits;
    }
    return flags;
}

// ---- The wave controller ----

struct UnkEnergyWaveLifeValue_ovb22 {
    u8 pad_0x0[0x40];
    f32 value;  // 0x40
};

// The life component behind the component's handle (+0xe8).
static inline UnkLifeComponent* Life(UnkWeakHandle* handle) {
    return reinterpret_cast<UnkLifeComponent*>(handle->target);
}

void fn_71002fe5f8(CAbilityComponent* self, f32 dt);

// Updates the wave without time passing, then sets the energy to its current value (+0x40).
void fn_71002fe5a0(CAbilityComponent* self) {
    fn_71002fe5f8(self, 0.0f);
    UnkLifeComponent* life = nullptr;
    f32 value = 0.0f;
    if (self->life) {
        life = Life(self->life);
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
bool fn_71002f8da8(CAbility* ability);
void fn_710113f3fc(UnkEnergyWaveCompA_b49* comp);
void fn_710113f378(UnkEnergyWaveCompA_b49* comp);
void fn_710094fa7c(UnkEnergyWaveCompB_b49* comp);
void fn_710094f9f8(UnkEnergyWaveCompB_b49* comp);

// Fires the selected ability while +0xcc is below the tunable's +0x34: a ready one (fn_71002f8da8) through
// its slot 0xa0, else, with energy (fn_7100a1ff18) for its cost (slot 0xc0, at least 1e-5), through
// fn_71002fa070. A failed attempt sets the hold mode to 1.
void fn_7100300ec0(CAbilityComponent* self) {
    if (!(self->field_0xcc < g_unk_71726aeb90.field_0x34))
        return;
    if (!self->current)
        return;
    if (fn_71002f8da8(self->current)) {
        if (self->current->Virtual<bool>(0xb0)) {
            self->current->Virtual(0xa0, true);
            if (self->Owner<UnkAbilityOwner_ovb22>()->field_0x260)
                fn_710113f3fc(self->Owner<UnkAbilityOwner_ovb22>()->field_0x260);
            if (self->Owner<UnkAbilityOwner_ovb22>()->field_0x210)
                fn_710094fa7c(self->Owner<UnkAbilityOwner_ovb22>()->field_0x210);
            return;
        }
    } else {
        f32 value = fn_7100a1ff18(self->life ? Life(self->life) : nullptr);
        f32 cost = self->current->Virtual<f32>(0xc0);
        f32 need = cost < 1e-5f ? 1e-5f : cost;
        if (!(value >= need)) {
            GameManagerIdTable();
            return;
        }
        if (self->current->Virtual<bool>(0xa8)) {
            fn_71002fa070(reinterpret_cast<UnkSmAbility3_A2_b63*>(self->current));
            if (self->Owner<UnkAbilityOwner_ovb22>()->field_0x260)
                fn_710113f378(self->Owner<UnkAbilityOwner_ovb22>()->field_0x260);
            if (self->Owner<UnkAbilityOwner_ovb22>()->field_0x210)
                fn_710094f9f8(self->Owner<UnkAbilityOwner_ovb22>()->field_0x210);
            return;
        }
    }
    GameManagerIdTable();
    if (self->hold.mode != 1)
        self->hold = {1, 0.0f};
}
