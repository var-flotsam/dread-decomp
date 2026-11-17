// CCharClassStartPointComponent's unit (tools/tu_map.py unit 2717): while the start point is enabled
// (+0x7b) and the game manager allows it (fn_7100dfbe3c), stores the checkpoint (the owner's name, an
// offset and an angle) under "LastCheckpointOffset" in the current scenario's section of the game
// manager's blackboard, then hands the game manager the owner's name twice with the flag (fn_7100dfaf88).
// Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/CCharClassStartPointComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU2553.h"
#include "game/UnkTUProperty.h"
#include "game/UnkTypedValue.h"
#include "game/UnkXformNode.h"
#include "../../game_aca57c/UnkBufferString.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

typedef UnkBufferString<0x100, g_unk_7101a194b8> UnkStartPointName_l16c128;

// A name string built from another string.
struct UnkStartPointNameCopy_l16c128 : UnkStartPointName_l16c128 {
    explicit UnkStartPointNameCopy_l16c128(const CRntString& text) { fn_710007eaa0(Str(), &text); }
};

struct UnkEnumType;

// "LastCheckpointOffset" and the reflected type of its value (the unit's static initialiser builds both).
extern UnkEnumType g_unk_7172a0a4a8;

struct UnkStartPointRecord_l16c128 {
    UnkStartPointName_l16c128 name;
    UnkVec3f offset;
    UnkVec3f angle;

    UnkStartPointRecord_l16c128(const UnkStartPointName_l16c128& name_, const UnkVec3f& offset_,
                                const UnkVec3f& angle_)
        : name(name_), offset(offset_), angle(angle_) {}
};

struct UnkStartPointOwner_l16c128 {
    u8 pad_0x0[0x30];
    CStrId name;  // 0x30
};

struct UnkGeneratorTarget_ovb0120 {
    u8 pad_0x0[0x18];
    UnkStartPointOwner_l16c128* owner;  // 0x18
    u8 pad_0x20[0x5b];
    bool enabled;  // 0x7b
};

// The id's string, counting the read.
static inline const CRntString& NameString(const CStrId& id) {
    ++id.data->reads;
    return id.data->String();
}

// The property's qualified id, built on first use.
static inline UnkQualifiedId& Qid(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

// Stores `value` under `name` in the blackboard's section `section` names (unless the blackboard is
// locked).
static inline void SetSectionValue(CBlackboard* blackboard, const CStrId& section, UnkQualifiedId& name, void* value) {
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = reinterpret_cast<const base::reflection::CType*>(&g_unk_7172a0a4a8);
            v.value = value;
            fn_7100332c80(s, reinterpret_cast<UnkPropertyName*>(&name), &v);
        }
    }
}

// Returns bool: include/game/CCharClassStartPointComponent.h declares it void (to change).
bool fn_710116c128(UnkGeneratorTarget_ovb0120* self, const UnkVec3f* offset, const UnkVec3f* angle, bool flag) {
    if (!self->enabled || !fn_7100dfbe3c(g_unk_7101d47310))
        return false;
    UnkStartPointNameCopy_l16c128 first(NameString(self->owner->name));
    UnkStartPointNameCopy_l16c128 second(NameString(self->owner->name));
    UnkGameManager* manager = g_unk_7101d47310;
    CBlackboard* blackboard = &manager->field_0x968;
    UnkStartPointRecord_l16c128 record(first, *offset, *angle);
    {
        CStrId section_id = fn_7100de9c30(g_unk_7101d47310);
        SetSectionValue(blackboard, section_id, Qid(g_unk_7172a0a5e8), &record);
    }
    return fn_7100dfaf88(g_unk_7101d47310, first.Str(), *second.Str(), flag);
}
