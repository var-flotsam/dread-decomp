// CCharClassLiquidPoolComponent (tools/tu_map.py unit 1394): fn_7100900cf4 keeps the pool's bounds in the
// current scenario's blackboard section: reads the dictionary of bounds by owner name stored under the
// component's property, sets (or, when inactive, removes) the owner's entry and stores it back. Nothing
// happens while the blackboard is locked. Names and layouts are placeholders.
#include "base/UnkCloneCalls.h"
#include "base/UnkCloneHelpers.h"
#include "base/global/CRntSmallDictionary.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/CCharClassLiquidPoolComponent.h"
#include "game/CScenario.h"
#include "game/UnkGameManager.h"
#include "game/UnkTypedValue.h"
#include "game/UnkValue_0x10.h"
#include "types.h"

#include <new>

using base::global::CRntSmallDictionary;
using base::global::CStrId;


struct UnkKeyedValues_71009046bc;
using UnkValues10ByKey = base::UnkKeyedArray<UnkKeyedValues_71009046bc, u64, UnkValue_0x10>;

// A 2D box (base::spatial::CAABox2D, by the dictionary's reflected name), copied member by member.
struct UnkBounds_l900cf4 {
    f32 min_x;
    f32 min_y;
    f32 max_x;
    f32 max_y;

    UnkBounds_l900cf4& operator=(const UnkBounds_l900cf4& other) {
        min_x = other.min_x;
        min_y = other.min_y;
        max_x = other.max_x;
        max_y = other.max_y;
        return *this;
    }
};

// The bounds by owner name, freed on destruction.
struct UnkBoundsByName_l900cf4 : CRntSmallDictionary<CStrId, UnkBounds_l900cf4> {
    UnkBoundsByName_l900cf4() : CRntSmallDictionary(0xe) {}
    ~UnkBoundsByName_l900cf4() {
        Clear();
        if (keys)
            allocator->vf_0x48(keys);
        if (values)
            allocator->vf_0x48(values);
    }

    void Add(const CStrId& key, const UnkBounds_l900cf4& value) {
        if (count == capacity)
            fn_7100904c48(reinterpret_cast<UnkPropertyArrays*>(this), step + count);
        new (&keys[count]) CStrId(key);
        values[count] = value;
        ++count;
    }
};

struct UnkBoundsByNameTag;
template <>
struct base::reflection::UnkFieldTraits<UnkBoundsByNameTag> {
    static const CType* type() { return &g_unk_7172733a70; }
};

struct UnkBoundsSource_l900cf4;
struct UnkBoundsSourceVtbl_l900cf4 {
    void* slots[0x98 / 8];
    void (*vf_0x98)(UnkBoundsSource_l900cf4* self);  // recomputes the bounds
};

// What the pool takes its bounds from.
struct UnkBoundsSource_l900cf4 {
    UnkBoundsSourceVtbl_l900cf4* vtable;
    u8 pad_0x8[0x38];
    UnkBounds_l900cf4 bounds;  // 0x40
    u8 pad_0x50[0x10];
    bool dirty;  // 0x60

    const UnkBounds_l900cf4& Bounds() {
        if (dirty)
            vtable->vf_0x98(this);
        return bounds;
    }
};

struct UnkLiquidOwner_l900cf4 {
    u8 pad_0x0[0x30];
    CStrId name;  // 0x30
    u8 pad_0x38[0x1b4 - 0x38];
    bool field_0x1b4;
};

struct UnkLiquidPool_l900cf4;
struct UnkLiquidPoolVtbl_l900cf4 {
    void* slots[0x240 / 8];
    UnkPropertyName* (*vf_0x240)(UnkLiquidPool_l900cf4* self);  // the property the bounds go under
};

struct UnkLiquidPool_l900cf4 {
    UnkLiquidPoolVtbl_l900cf4* vtable;
    u8 pad_0x8[0x10];
    UnkLiquidOwner_l900cf4* owner;  // 0x18
    bool field_0x20;
    u8 pad_0x21[0x88 - 0x21];
    UnkBoundsSource_l900cf4* source;  // 0x88
};
static_assert(__builtin_offsetof(UnkLiquidPool_l900cf4, source) == 0x88, "layout");

static inline void GetSectionValue(CBlackboard* blackboard, const CStrId& section, UnkPropertyName* name,
                                   UnkBoundsByName_l900cf4* out) {
    if (CBlackboard::CSection* s = fn_7101047270(blackboard, section))
        base::refl_GetKeyedValue<UnkBoundsByNameTag>(reinterpret_cast<char*>(s),
                                                     reinterpret_cast<const base::UnkStrIdMember*>(name), out);
}

static inline void SetSectionValue(CBlackboard* blackboard, const CStrId& section, UnkPropertyName* name,
                                   UnkBoundsByName_l900cf4* value) {
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = base::reflection::UnkFieldTraits<UnkBoundsByNameTag>::type();
            v.value = value;
            fn_7100332c80(s, name, &v);
        }
    }
}

void fn_7100900cf4(UnkLiquidPool_b44a2* object) {
    UnkLiquidPool_l900cf4* self = reinterpret_cast<UnkLiquidPool_l900cf4*>(object);
    UnkBoundsByName_l900cf4 all;
    UnkGameManager* manager = g_unk_7101d47310;
    if (manager->field_0x968.field_0xe8)
        return;
    CBlackboard* blackboard = &manager->field_0x968;
    CStrId scenario = fn_71011a2fbc(manager->field_0x608);
    GetSectionValue(blackboard, scenario, self->vtable->vf_0x240(self), &all);

    bool active = self->field_0x20 && self->owner->field_0x1b4 && self->source != nullptr;
    UnkBounds_l900cf4* it = all.Find(self->owner->name);
    if (active) {
        if (it == all.End())
            all.Add(self->owner->name, self->source->Bounds());
        else
            *it = self->source->Bounds();
    } else {
        if (it == all.End())
            return;
        CCharClassLiquidPoolComponent_SwapEraseDictionaryEntry(reinterpret_cast<UnkValues10ByKey*>(&all), reinterpret_cast<UnkValue_0x10*>(it));
    }
    SetSectionValue(blackboard, scenario, self->vtable->vf_0x240(self), &all);
}
