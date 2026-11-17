// CCharClassStartPointComponent's unit (tools/tu_map.py unit 2717): stores the last checkpoint (the owner's
// name, a position and an angle) under "LastCheckpointOffset" in the current scenario's section of the game
// manager's blackboard at +0xd40, then hands the game manager the owner's name twice with `a` and the
// flag (fn_7100dfc344). Names and layouts are placeholders.
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

typedef UnkBufferString<0x100, g_unk_7101a194b8> UnkStartPointName_l16c388;

// A name string built from another string.
struct UnkStartPointNameCopy_l16c388 : UnkStartPointName_l16c388 {
    explicit UnkStartPointNameCopy_l16c388(const CRntString& text) { fn_710007eaa0(Str(), &text); }
};

struct UnkEnumType;

// "LastCheckpointOffset" and the reflected type of its value (the unit's static initialiser builds both).
extern UnkEnumType g_unk_7172a0a4a8;

struct UnkStartPointRecord_l16c388 {
    UnkStartPointName_l16c388 name;
    UnkVec3f position;
    UnkVec3f angle;

    UnkStartPointRecord_l16c388(const UnkStartPointName_l16c388& name_, const UnkVec3f& position_,
                                const UnkVec3f& angle_)
        : name(name_), position(position_), angle(angle_) {}
};

struct UnkStartPointOwner_l16c388 {
    u8 pad_0x0[0x30];
    CStrId name;  // 0x30
};

struct UnkStartPointSaver_l16c388 {
    u8 pad_0x0[0x18];
    UnkStartPointOwner_l16c388* owner;  // 0x18
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

bool fn_710116c388(UnkStartPointSaver_l16c388* self, const char* const& a, const UnkVec3f* position,
                   const UnkVec3f* angle, bool flag) {
    UnkStartPointNameCopy_l16c388 first(NameString(self->owner->name));
    UnkStartPointNameCopy_l16c388 second(NameString(self->owner->name));
    // Needs `CBlackboard field_0xd40;` in UnkGameManager (include/game/UnkGameManager.h).
    CBlackboard* blackboard = &g_unk_7101d47310->field_0xd40;
    UnkStartPointRecord_l16c388 record(first, *position, *angle);
    SetSectionValue(blackboard, fn_7100de9c30(g_unk_7101d47310), Qid(g_unk_7172a0a5e8), &record);
    return fn_7100dfc344(g_unk_7101d47310, a, reinterpret_cast<const char* const&>(*first.Str()), *second.Str(),
                         flag);
}
