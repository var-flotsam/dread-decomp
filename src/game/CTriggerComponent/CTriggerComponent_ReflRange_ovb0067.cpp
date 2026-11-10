// CTriggerComponent unit (tools/tu_map.py): a reflected setter of a string id. Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflValue;

extern UnkTunable g_unk_717274ddd8;

struct UnkTriggerRange_ovb0067 {
    s32 first;
    s32 second;
};

struct UnkTriggerTunable_ovb0067 {
    u8 pad_0x0[0x24];
    UnkTriggerRange_ovb0067 field_0x24;
    UnkTriggerRange_ovb0067 field_0x2c;
};

struct UnkTriggerRangeTarget_ovb0067 {
    u8 pad_0x0[0x40];
    UnkTriggerRange_ovb0067 range;  // 0x40
    u8 pad_0x48;
    bool has_range;  // 0x49
    u8 pad_0x4a[0x3d6];
    CStrId field_0x420;
};

struct UnkTriggerIdHash_ovb0067 {
    u8 pad_0x0[0x30];
    u64 hash;
};

static inline const UnkTriggerTunable_ovb0067* Tunable() {
    return reinterpret_cast<const UnkTriggerTunable_ovb0067*>(&g_unk_717274ddd8);
}

static inline void SetRange(UnkTriggerRangeTarget_ovb0067* target, const UnkTriggerRange_ovb0067& range) {
    s32 first = range.first;
    s32 second = range.second;
    target->range.first = first;
    target->range.second = second;
    target->has_range = first != -1 || second != -1;
}

// A reflected function of the unit: sets the string id at +0x420 of the object at 8 of the reflected
// object from the argument and, for two known ids (by their CRC64), takes one of two ranges of the
// unit's tunable (g_unk_717274ddd8) into +0x40, with +0x49 telling whether it is set. It returns an
// empty reflected value.
UnkReflValue fn_7100a647d4(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* arg = args[0];
    UnkTriggerRangeTarget_ovb0067* target = *reinterpret_cast<UnkTriggerRangeTarget_ovb0067**>(obj + 8);
    const void* in = arg;
    if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        in = arg->field_0x0;
    target->field_0x420 = *static_cast<const CStrId*>(in);
    base::global::UnkStringData* id = target->field_0x420.data;
    if (id != &g_unk_7101d47108->empty) {
        u64 hash = reinterpret_cast<UnkTriggerIdHash_ovb0067*>(id)->hash;
        if (hash == 0x56c03b185c54c35eull)
            SetRange(target, Tunable()->field_0x24);
        else if (hash == 0x7a70c1b8ca7d7c7aull)
            SetRange(target, Tunable()->field_0x2c);
    }
    UnkReflValue r;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    r.kind = 2;
    r.field_0x14 = 1;
    return r;
}
