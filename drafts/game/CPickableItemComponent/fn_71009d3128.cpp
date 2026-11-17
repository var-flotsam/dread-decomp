// Unit 1493 (CPickableItemComponent): when the tanks of the item's kind picked up in the current scenario
// reach the scenario's maximum and its archive is locked, flags the new-item pop-up in the profile and
// adds the scenario to the profile's list of scenarios. Names and layouts are placeholders.
#include <new>

#include "../../game_5821e4/UnkStaticInit.h"
#include "base/UnkCloneHelpers.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/CCollectionType.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU1645.h"
#include "game/UnkTU2553.h"
#include "game/UnkTypedValue.h"
#include "types.h"

using base::global::CRntVector;
using base::global::CStrId;

bool fn_71009d2dc0(const CStrId& scenario);

struct UnkStrIdHandle {
    CStrId id;
    void* field_0x8;
};

struct UnkPickableItem_l9d3128 {
    u8 pad_0x0[0x1a0];
    UnkStrIdHandle field_0x1a0;  // the key of the picked-up count
};

// The scenarios read from the profile are converted to a list of string ids.
struct UnkScenarioListTag_l9d3128;
template <>
struct base::reflection::UnkFieldTraits<UnkScenarioListTag_l9d3128> {
    static const CType* type() { return &g_unk_7101d5c998; }
};

// A list of scenario ids; freed on destruction.
struct UnkScenarioList_l9d3128 : CRntVector<CStrId> {
    UnkScenarioList_l9d3128() : CRntVector<CStrId>(0xf) {}
    ~UnkScenarioList_l9d3128() { Free(); }
};

// Appends an id to the list; growing moves the ids over.
static void Append(UnkScenarioList_l9d3128* list, const CStrId& id) {
    u32 n = list->count;
    u32 old_capacity = list->capacity;
    u32 new_capacity = old_capacity;
    if (old_capacity <= n && list->step != 0)
        new_capacity = (n / list->step + 1) * list->step;
    if ((s32)n >= (s32)new_capacity)
        for (;;) {
        }
    if (old_capacity < new_capacity) {
        CStrId* p = static_cast<CStrId*>(list->allocator->vf_0x40((u64)new_capacity * sizeof(CStrId), 8, list->flags));
        if (list->data) {
            CStrId* src = list->data;
            for (u32 i = 0; i < list->count; ++i, ++src) {
                CStrId::MoveConstructReloading(&p[i], *src);
                src->~CStrId();
            }
            list->allocator->vf_0x48(list->data);
        }
        list->data = p;
        list->capacity = new_capacity;
    }
    new (&list->data[list->count]) CStrId(id);
    ++list->count;
}

// The property's qualified id, built on first use.
static inline const UnkQualifiedId& Qid(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

// The id of the profile section that holds the archive state.
static inline const CStrId& ArchivesSectionId() {
    return GameManagerIdTable().field_0x1be8;
}

static inline UnkPropertyName* PropertyName(const UnkQualifiedId& name) {
    return reinterpret_cast<UnkPropertyName*>(const_cast<UnkQualifiedId*>(&name));
}

// Stores the flag under `name` in the section `section` names (unless the blackboard is locked).
static inline void SetFlag(CBlackboard* blackboard, const CStrId& section, const UnkQualifiedId& name, bool value) {
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = fn_710000d228();
            v.value = &value;
            fn_7100332c80(s, PropertyName(name), &v);
        }
    }
}

// Stores `value` of type `type` under `name` in the section `section` names (unless the blackboard is locked).
static inline void SetValue(CBlackboard* blackboard, const CStrId& section, const UnkQualifiedId& name,
                            const base::reflection::CType* type, void* value) {
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = type;
            v.value = value;
            fn_7100332c80(s, PropertyName(name), &v);
        }
    }
}

bool fn_71009d3128(UnkPickableItem_l9d3128* self) {
    UnkGameManager* manager = g_unk_7101d47310;
    s32 picked = 0;
    CBlackboard* blackboard = &manager->field_0x968;
    CStrId scenario = fn_7100dfbda4(manager, *fn_7100de9bf8(manager));
    if (self->field_0x1a0.id.data != &g_unk_7101d47108->empty) {
        if (CBlackboard::CSection* section = fn_7101047270(blackboard, scenario))
            fn_7100395a30(section, &self->field_0x1a0, &picked);
    }
    s32 max = -1;
    {
        const UnkQualifiedId& name = Qid(g_unk_7172759138);
        if (CBlackboard::CSection* section = fn_7101047270(blackboard, scenario))
            fn_7100395a30(section, &name, &max);
    }
    if (max > 0 && picked == max && fn_71009d2dc0(scenario)) {
        CBlackboard* profile = &g_unk_7101d47310->field_0x820;
        SetFlag(profile, ArchivesSectionId(), Qid(g_unk_717275ad68), true);
        UnkScenarioList_l9d3128 scenarios;
        {
            const CStrId& section_id = ArchivesSectionId();
            const UnkQualifiedId& name = Qid(g_unk_717275adb0);
            if (CBlackboard::CSection* section = fn_7101047270(profile, section_id))
                base::refl_GetKeyedValue<UnkScenarioListTag_l9d3128>(
                    reinterpret_cast<char*>(section), reinterpret_cast<const base::UnkStrIdMember*>(&name.id),
                    &scenarios);
        }
        Append(&scenarios, scenario);
        SetValue(profile, ArchivesSectionId(), Qid(g_unk_717275adb0), &g_unk_7101d5c998, &scenarios);
    }
    return false;
}
