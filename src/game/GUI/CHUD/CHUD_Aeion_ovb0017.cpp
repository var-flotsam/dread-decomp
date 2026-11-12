// GUI::CHUD unit (tools/tu_map.py): the HUD's Aeion gauge and map-border events, the bound callback
// that forwards a blackboard change to the Aeion handler, the item-acquired pop-up for Aeion, an
// id/value array built from an initializer list and the insertion into its array of owned tasks.
// The event names are the strings the HUD sends (vtable slot 0x1e8 of the item at +0x10 of a
// handle). Names and layouts are placeholders.
#include <initializer_list>

#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/CSpecialEnergyComponent.h"
#include "game/GUI/CPopUpComposition.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU2553.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkStringData;

struct UnkPlayer;
struct UnkLifeComponent;

// A HUD item: its event slot (vtable 0x1e8) plays the named animation event.
struct UnkHudItem_ovb0017 {
    void** vtable;
    void SendEvent(const char* event, bool flag) {
        reinterpret_cast<void (*)(UnkHudItem_ovb0017*, const char*, bool)>(vtable[0x1e8 / 8])(this, event,
                                                                                              flag);
    }
};

struct UnkHudHandle_ovb0017 {
    u8 unk_0x0[0x10];
    UnkHudItem_ovb0017* target;
};

struct UnkHud_ovb0017 {
    u8 unk_0x0[0x258];
    bool field_0x258;
    u8 unk_0x259[0x6f];
    UnkHudHandle_ovb0017* field_0x2c8;  // the minimap
    u8 unk_0x2d0[0x8];
    UnkHudHandle_ovb0017* aeion;  // 0x2d8
};
static_assert(__builtin_offsetof(UnkHud_ovb0017, aeion) == 0x2d8, "UnkHud_ovb0017 layout");

// A blackboard change as the HUD's bound callbacks get it: the property's id, a value and a flag.
struct UnkHudEvent_ovb0017 {
    CStrId id;
    u32 field_0x8;
    bool field_0xc;
};

// The player's special energy (Aeion) component, as src/game/CSpecialEnergyComponent sees it.
struct UnkSpecialEnergy_ovb0017 {
    u8 unk_0x0[0x40];
    f32 max;
    f32 value;
    u8 unk_0x48;
    bool recovering;
    u8 unk_0x4a[0x6];
    f32 fast_time;
    f32 slow_time;
};
struct UnkSpecialEnergyHolder_ovb0017 {
    u8 unk_0x0[0x10];
    UnkSpecialEnergy_ovb0017* energy;
};
struct UnkPlayerComponents_ovb0017 {
    u8 unk_0x0[0xe8];
    UnkSpecialEnergyHolder_ovb0017* special_energy;
};
struct UnkPlayerView_ovb0017 {
    u8 unk_0x0[0x2e0];
    UnkPlayerComponents_ovb0017* components;
};

static inline UnkPlayerComponents_ovb0017* PlayerComponents() {
    return reinterpret_cast<UnkPlayerView_ovb0017*>(fn_7100dec788(g_unk_7101d47310))->components;
}

static inline UnkSpecialEnergy_ovb0017* PlayerSpecialEnergy() {
    UnkPlayerComponents_ovb0017* components = PlayerComponents();
    if (!components || !components->special_energy)
        return nullptr;
    return components->special_energy->energy;
}

// The item's qualified id, built on first use.
static inline UnkQualifiedId& GetQualifiedId(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

// The map border alert: chasing or searching.
bool fn_71011e7e8c(UnkHud_ovb0017* self, u32 chase) {
    if (UnkHudHandle_ovb0017* handle = self->field_0x2c8) {
        if (UnkHudItem_ovb0017* item = handle->target) {
            if (chase)
                item->SendEvent("OnMapBorderAlertChase", false);
            else
                item->SendEvent("OnMapBorderAlertSearch", false);
        }
    }
    return false;
}

// The Aeion gauge recharges while the energy is not full.
void fn_71011e4a28(UnkHud_ovb0017* self) {
    if (!self->aeion || !self->aeion->target)
        return;
    UnkPlayerComponents_ovb0017* components = PlayerComponents();
    if (!components || !components->special_energy || !components->special_energy->energy)
        return;
    UnkSpecialEnergy_ovb0017* energy = components->special_energy->energy;
    if (__builtin_fabsf(energy->value / energy->max - 1.0f) >= 1.1920929e-7f)
        self->aeion->target->SendEvent("OnAeionRecharge", false);
}

// The ids the Aeion gauge reacts to, built by fn_71011df1b0 on first use.
struct UnkAeionIdEntry_ovb0017 {
    UnkStringData* id;
    u64 field_0x8;
};
struct UnkAeionIdList_ovb0017 {
    UnkAeionIdEntry_ovb0017* data;
    u32 count;
};
extern __attribute__((visibility("hidden"))) UnkAeionIdList_ovb0017 g_unk_7172a10040;
void fn_71011df1b0();

// A change to one of the Aeion ids resets the gauge; a change to ITEM_OPTIC_CAMOUFLAGE also stops
// the energy's recovery. Returns whether it was that.
u32 fn_71011e463c(UnkHud_ovb0017* self, UnkHudEvent_ovb0017 event) {
    if (!event.field_0xc)
        return false;
    fn_71011df1b0();
    u8 found = 0;
    for (UnkAeionIdEntry_ovb0017* e = g_unk_7172a10040.data; e != g_unk_7172a10040.data + g_unk_7172a10040.count;
         ++e)
        found |= event.id.data == e->id;
    if (!found)
        return false;
    if (!self->aeion || !self->aeion->target)
        return false;
    self->aeion->target->SendEvent("OnAeionDefault", false);
    fn_71011cdce8(reinterpret_cast<UnkGuiDisplayObject*>(self->aeion ? self->aeion->target : nullptr), true);
    const UnkQualifiedId& camouflage = GetQualifiedId(g_unk_717275aa58);
    if (event.id.data != camouflage.id.data)
        return false;
    UnkSpecialEnergy_ovb0017* energy = PlayerSpecialEnergy();
    if (!energy)
        return false;
    fn_7100a1fa98(reinterpret_cast<UnkLifeComponent*>(energy), false, true, 0.0f);
    energy->recovering = false;
    return true;
}

// The bound callback for fn_71011e463c (the event is copied for it).
u32 fn_71011e7d60(UnkHud_ovb0017* self, const UnkHudEvent_ovb0017* event) { return fn_71011e463c(self, *event); }

struct UnkPopUpComposition_ovb0017 {
    u8 unk_0x0[0x2e4];
    bool field_0x2e4;
};
struct UnkPopUpEntry_ovb0017 {
    u8 unk_0x0[0x10];
    UnkPopUpComposition_ovb0017* composition;
};

// The game manager's pop-up entry (+0x2258), read untyped: the original reloads it after the flag
// store.
static inline UnkPopUpEntry_ovb0017* PopUpEntry(UnkGameManager* manager) {
    UnkPopUpEntry_ovb0017* entry;
    __builtin_memcpy(&entry, &manager->field_0x2258, sizeof(entry));
    return entry;
}

// Aeion acquired: the pop-up, a full gauge recovering at the default speeds and the
// "AfterNewAbilityAcquired" event.
bool fn_71011e4b4c(UnkHud_ovb0017* self) {
    CStrId text("#GUI_ITEM_ACQUIRED_AEION", true);
    PopUpEntry(g_unk_7101d47310)->composition->field_0x2e4 = true;
    UnkPopUpEntry_ovb0017* entry = PopUpEntry(g_unk_7101d47310);
    fn_7100b5fd6c(entry ? entry->composition : nullptr, &text.data, &g_unk_7101d470e0, false, false);
    if (UnkSpecialEnergy_ovb0017* energy = PlayerSpecialEnergy()) {
        self->field_0x258 = true;
        fn_7100a1fa98(reinterpret_cast<UnkLifeComponent*>(energy), false, true, 0.0f);
        energy->fast_time = 2.0f;
        energy->slow_time = 3.0f;
        energy->recovering = true;
    }
    UnkGameManager* manager = g_unk_7101d47310;
    CRntString name("AfterNewAbilityAcquired");
    fn_7100dfaf88(manager, &name, CRntString("[MANTAIN]"), true);
    return true;
}

// A string id with a value (base::UnkVector's element in fn_71011df420).
struct UnkHudIdValue_ovb0017 {
    CStrId id;
    u64 value;

    // The value is copied bitwise: the original reloads the array's count and the string pool
    // after storing it.
    UnkHudIdValue_ovb0017(const UnkHudIdValue_ovb0017& other) : id(other.id) {
        __builtin_memcpy(&value, &other.value, sizeof(value));
    }
};

using UnkHudIdValueVector_ovb0017 = base::UnkVector<UnkHudIdValue_ovb0017>;

// Grows the array to at least `capacity` elements.
void fn_71011e7ac8(UnkHudIdValueVector_ovb0017* self, u32 capacity);

// The array built from a list: reserved for the list, then each element appended (a copy of a copy).
void fn_71011df420(UnkHudIdValueVector_ovb0017* self, std::initializer_list<UnkHudIdValue_ovb0017> items,
                   u32 step, base::UnkAllocator* allocator, u32 flags) {
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = step;
    self->field_0x14 = step != 0;
    self->flags = flags;
    if (!allocator)
        allocator = fn_7100083990();
    self->allocator = allocator;
    fn_71011e7ac8(self, items.size());
    for (UnkHudIdValue_ovb0017 item : items) {
        u32 n = self->count;
        u32 new_capacity = self->capacity;
        if (new_capacity <= n && self->step != 0)
            new_capacity = (n / self->step + 1) * self->step;
        if ((s32)n >= (s32)new_capacity)
            for (;;) {
            }
        fn_71011e7ac8(self, new_capacity);
        new (&self->data[self->count]) UnkHudIdValue_ovb0017(item);
        ++self->count;
    }
}

struct UnkHudOwned_ovb0017 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
};

// An owning pointer, as src/game/GUI/CHUD/CHUD_PushOwned.cpp spells it.
struct UnkHudOwnedRef_ovb0017 {
    union {
        UnkHudOwned_ovb0017* ptr;
    };
    UnkHudOwnedRef_ovb0017(UnkHudOwnedRef_ovb0017&& other) {
        UnkHudOwned_ovb0017* p = other.ptr;
        other.ptr = nullptr;
        ptr = p;
    }
    ~UnkHudOwnedRef_ovb0017() {
        UnkHudOwned_ovb0017* old = ptr;
        ptr = nullptr;
        if (old)
            old->vf_0x10();
    }
};

struct UnkHudOwnedPtr;
void fn_71011e7700(base::UnkVector<UnkHudOwnedPtr>* self, UnkHudOwnedPtr* value);

using UnkHudOwnedVector_ovb0017 = base::UnkVector<UnkHudOwnedRef_ovb0017>;

// base::UnkVector's Insert on the array of owned tasks, appending through the out-of-line
// EmplaceBack (fn_71011e7700).
void fn_71011e755c(UnkHudOwnedVector_ovb0017* self, UnkHudOwnedRef_ovb0017* value, u32 index) {
    u32 n = self->count;
    if (n == index) {
        fn_71011e7700(reinterpret_cast<base::UnkVector<UnkHudOwnedPtr>*>(self),
                      reinterpret_cast<UnkHudOwnedPtr*>(value));
        return;
    }
    u32 cap = self->capacity;
    if (cap <= n && self->step != 0) {
        u32 grown = (n / self->step + 1) * self->step;
        if (cap < grown) {
            auto* p = static_cast<UnkHudOwnedRef_ovb0017*>(
                self->allocator->vf_0x40((u64)grown * sizeof(UnkHudOwnedRef_ovb0017), 8, self->flags));
            if (self->data) {
                UnkHudOwnedRef_ovb0017* src = self->data;
                UnkHudOwnedRef_ovb0017* dst = p;
                for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
                    new (dst) UnkHudOwnedRef_ovb0017(static_cast<UnkHudOwnedRef_ovb0017&&>(*src));
                    src->~UnkHudOwnedRef_ovb0017();
                }
                self->allocator->vf_0x48(self->data);
            }
            n = self->count;
            self->data = p;
            self->capacity = grown;
        }
    }
    for (u32 i = n - 1; i >= index; --i) {
        UnkHudOwnedRef_ovb0017* q = &self->data[i];
        new (q + 1) UnkHudOwnedRef_ovb0017(static_cast<UnkHudOwnedRef_ovb0017&&>(*q));
        self->data[i].~UnkHudOwnedRef_ovb0017();
        if (i == 0)
            break;
    }
    new (&self->data[index]) UnkHudOwnedRef_ovb0017(static_cast<UnkHudOwnedRef_ovb0017&&>(*value));
    ++self->count;
}
