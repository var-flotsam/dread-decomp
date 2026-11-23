// Unit 1646: brings blackboard data up to the current version, running each version's upgrade step in
// turn from the stored version, then stores the version reached. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/ReflectedAttributes.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU1645.h"
#include "game/UnkTypedValue.h"
#include "types.h"

using base::global::CStrId;

extern UnkPropertyEx<> g_unk_7172758b70;
extern s32 g_unk_710164b8b4;

// One version's upgrade step: called with its object and the blackboard, says whether it succeeded.
struct UnkUpgradeStep_lae8bfc {
    void* object;
    bool (*run)(void* object, CBlackboard* blackboard);

    bool operator()(CBlackboard* blackboard) const { return run(object, blackboard); }
};

// The upgrade steps by the version they start from: parallel arrays of steps and versions.
struct UnkUpgradeSteps_lae8bfc {
    UnkUpgradeStep_lae8bfc* values;
    s32* keys;
    u32 count;

    UnkUpgradeStep_lae8bfc* End() { return &values[count]; }
    UnkUpgradeStep_lae8bfc* Find(s32 key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &values[i];
        return End();
    }
    UnkUpgradeStep_lae8bfc& Get(s32 key) {
        u64 i = 0;
        for (; i < count; ++i)
            if (keys[i] == key)
                break;
        return values[i];
    }
};

struct UnkUpgrader_lae8bfc {
    void* vtable;
    UnkUpgradeSteps_lae8bfc steps;  // 0x8
};

static inline const UnkQualifiedId& DefId_lae8bfc(UnkPropertyEx<>& def) {
    UnkQualifiedId& qid = ItemQualifiedId(def);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(def), g_unk_7101d470e0);
    return qid;
}

// Reads the int `def` names in the blackboard section `section` into `out`, when the section exists.
static inline void ReadInt_lae8bfc(CBlackboard* blackboard, const CStrId& section, UnkPropertyEx<>& def, s32* out) {
    const UnkQualifiedId& name = DefId_lae8bfc(def);
    if (CBlackboard::CSection* s = fn_7101047270(blackboard, section))
        fn_7100395a30(s, &name.id, out);
}

// Stores the int `value` points to under `name` in the section `section`, unless the blackboard is locked.
static inline void SetInt_lae8bfc(CBlackboard* blackboard, const CStrId& section, const UnkQualifiedId& name,
                                  s32* value) {
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = fn_7100014768();
            v.value = value;
            fn_7100332c80(s, reinterpret_cast<UnkPropertyName*>(const_cast<UnkQualifiedId*>(&name)), &v);
        }
    }
}

bool fn_7100ae8bfc(UnkUpgrader_lae8bfc* self, CBlackboard* blackboard) {
    s32 version = 0;
    ReadInt_lae8bfc(blackboard, base::reflection::refl_GetStrIdTable<0x7100031894>().ids[0x229], g_unk_7172758b70,
                    &version);

    const s32 latest = g_unk_710164b8b4;
    bool ok = true;
    s32 step = version;
    while (ok && step < latest) {
        ok = self->steps.Find(step) != self->steps.End() && self->steps.Get(step)(blackboard);
        ++step;
    }

    s32 reached = ok ? latest : step - 1;
    SetInt_lae8bfc(blackboard, base::reflection::refl_GetStrIdTable<0x7100031894>().ids[0x229],
                   DefId_lae8bfc(g_unk_7172758b70), &reached);
    return ok;
}
