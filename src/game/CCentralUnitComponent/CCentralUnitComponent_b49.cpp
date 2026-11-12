// CCentralUnitComponent unit (tools/tu_map.py): fn_71007717c0 saves the central unit to the current
// scenario's blackboard section (only with a scenario and an unlocked blackboard): its state's id (+0x10
// of +0x40, 8 without a state) under g_unk_71727141f8, then, when its linked entity (+0x10 of +0x6d8) is
// done (no +0x1b4, or +0x1b0 with nothing at +0x1a8) or the section already has it, that flag under
// g_unk_7172714220 (keys at +0x7a8, count +0x7b0, values at +0x7a0, looked up as in
// CEventScenarioComponent_b46.cpp; fn_71010474f4 is src/game/CBlackboard.cpp's, here given the section
// itself). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CBlackboard.h"
#include "game/CScenario.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1176.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CStrId;

bool fn_71010474f4(CBlackboard* self, const CStrId& section, const CStrId& name);

// A typed value as the section setter takes it (as in src/game/CDoorLifeComponent.cpp).
struct UnkTypedValue {
    const base::reflection::CType* type;
    void* value;
};
void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);

struct UnkCentralUnitState_b49 {
    u8 pad_0x0[0x10];
    u32 id;
};

struct UnkCentralUnitEntity_b49 {
    u8 pad_0x0[0x1a8];
    void* field_0x1a8;
    bool field_0x1b0;
    u8 pad_0x1b1[3];
    bool field_0x1b4;
};

struct UnkCentralUnitLink_b49 {
    u8 pad_0x0[0x10];
    UnkCentralUnitEntity_b49* entity;
};

struct UnkCentralUnitValue_b49 {
    union {
        CStrId id;
        UnkPropertyName name;
    };
    u64 field_0x8;
};

struct UnkCentralUnit_b49 {
    u8 pad_0x0[0x40];
    UnkCentralUnitState_b49* state;
    u8 pad_0x48[0x6d8 - 0x48];
    UnkCentralUnitLink_b49* link;
    u8 pad_0x6e0[0x7a0 - 0x6e0];
    UnkCentralUnitValue_b49* values;
    u64* keys;
    u32 count;

    __attribute__((always_inline)) UnkCentralUnitValue_b49* Find(const UnkProperty& property) {
        u64 index = 0;
        u32 n = count;
        if (n) {
            u64* k = keys;
            u64 key = (u64)property.name.data;
            for (; index != n; ++index) {
                if (k[index] == key)
                    break;
            }
        }
        return &values[index];
    }
};

static inline bool IsDone(UnkCentralUnitLink_b49* link) {
    if (!link || !link->entity)
        return false;
    UnkCentralUnitEntity_b49* entity = link->entity;
    if (!entity->field_0x1b4)
        return true;
    if (!entity->field_0x1b0)
        return false;
    return !entity->field_0x1a8;
}

void fn_71007717c0(UnkCentralUnit_b49* self) {
    UnkGameManager* manager = g_unk_7101d47310;
    if (!manager->field_0x608 || manager->field_0x968.field_0xe8)
        return;
    CBlackboard* blackboard = &manager->field_0x968;
    CStrId section_id = fn_71011a2fbc(manager->field_0x608);
    if (CBlackboard::CSection* section = fn_71010472e0(blackboard, section_id)) {
        UnkTypedValue v;
        {
            u32 state = self->state ? self->state->id : 8;
            UnkCentralUnitValue_b49* name = self->Find(g_unk_71727141f8);
            u32 value = state;
            if (!blackboard->field_0xe8) {
                v.type = fn_7100014768();
                v.value = &value;
                fn_7100332c80(section, &name->name, &v);
            }
        }
        bool done = IsDone(self->link);
        bool known = fn_71010474f4(blackboard, *reinterpret_cast<const CStrId*>(section),
                                   self->Find(g_unk_7172714220)->id);
        if (known || done) {
            UnkCentralUnitValue_b49* name = self->Find(g_unk_7172714220);
            if (!blackboard->field_0xe8) {
                v.type = fn_710000d228();
                v.value = &done;
                fn_7100332c80(section, &name->name, &v);
            }
        }
    }
}
