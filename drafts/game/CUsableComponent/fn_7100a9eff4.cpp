// CUsableComponent's unit (tools/tu_map.py): unless the usable is already marked used (+0xf8), reads
// the flag stored under the unit's property g_unk_7172753198 in the blackboard section its vf_0x2d8
// names; when found, copies it to +0xf8 and +0x538, and while it is clear sends the owner's (+0x30)
// sub-object at +0x220 the hash at +0x30 of the record +0x150 of +0x500 (UnkSpitSend). Names and
// layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CStrId;

extern u8 g_unk_7172753198[];  // a UnkProperty

struct UnkUsableValue_la9eff4 {
    CStrId name;
    u64 field_0x8;
};

struct UnkUsableOwner_la9eff4 {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkUsableIdRecord_la9eff4 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkUsableIdHolder_la9eff4 {
    u8 pad_0x0[0x150];
    UnkUsableIdRecord_la9eff4* record;
};

struct UnkUsable_la9eff4 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkUsableOwner_la9eff4* owner;  // 0x30
    u8 pad_0x38[0xc0];
    bool field_0xf8;
    u8 pad_0xf9[0x3d7];
    UnkUsableValue_la9eff4* values;  // 0x4d0
    u64* keys;                       // 0x4d8
    u32 count;                       // 0x4e0
    u8 pad_0x4e4[0x1c];
    UnkUsableIdHolder_la9eff4* field_0x500;
    u8 pad_0x508[0x30];
    bool field_0x538;

    // The index of the value stored under `property` (count when there is none).
    u64 IndexOf(u8* property) {
        u64 index = 0;
        u32 n = count;
        if (n) {
            u64* k = keys;
            u64 key = (u64)reinterpret_cast<UnkProperty*>(property)->name.data;
            for (; index != n; ++index) {
                if (k[index] == key)
                    break;
            }
        }
        return index;
    }

    // The blackboard section the usable's flags are stored in.
    CStrId SectionId() { return reinterpret_cast<CStrId (*)(UnkUsable_la9eff4*)>(vtable[0x2d8 / 8])(this); }
};
// Reads the bool stored under `name` in the blackboard section `section` names into `out`; false when
// there is no such section or value.
static inline bool GetSectionBool(CBlackboard* blackboard, const CStrId& section, const CStrId* name, bool* out) {
    if (CBlackboard::CSection* s = fn_7101047270(blackboard, section))
        return fn_710006d11c(s, name, out);
    return false;
}

// Reads the flag stored under `property` in the usable's section `section_id` into `out`; false when
// it has none.
static inline bool ReadFlag(UnkUsable_la9eff4* self, CBlackboard* blackboard, const CStrId& section_id, u8* property,
                            bool* out) {
    u64 index = self->IndexOf(property);
    UnkUsableValue_la9eff4* values = self->values;
    return GetSectionBool(blackboard, section_id, &values[index].name, out);
}

static_assert(__builtin_offsetof(UnkUsable_la9eff4, values) == 0x4d0, "UnkUsable_la9eff4 layout");
static_assert(__builtin_offsetof(UnkUsable_la9eff4, field_0x500) == 0x500, "UnkUsable_la9eff4 layout");
static_assert(__builtin_offsetof(UnkUsable_la9eff4, field_0x538) == 0x538, "UnkUsable_la9eff4 layout");

void fn_7100a9eff4(UnkUsable_la9eff4* self, CBlackboard* blackboard) {
    if (self->field_0xf8)
        return;
    bool flag = false;
    if (ReadFlag(self, blackboard, self->SectionId(), g_unk_7172753198, &flag)) {
        self->field_0xf8 = flag;
        self->field_0x538 = flag;
    }
    if (!flag)
        UnkSpitSend(self->owner->field_0x220, self->field_0x500->record->hash, 0xf4061ff25a2163d6);
}
