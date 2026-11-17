// GUI::CChozoArchivesComposition's unit (tools/tu_map.py unit 1655): stores the composition's state in the
// profile: an empty list of unlocked archives (they are consumed) and the archives' locked flags, then saves the
// profile. Names and layouts are placeholders.
#include "../../../game_5821e4/UnkStaticInit.h"
#include "base/UnkAtExit.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/CCollectionType.h"
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
using unk_static_init::UnkEnumType;

extern UnkEnumType g_unk_7101d60e78;      // the flags' type
extern UnkPropertyEx<> g_unk_717275adb0;  // "UnlockedArchives"

struct UnkProfileSaveArg {
    void* field_0x0;
    void* field_0x8;
};

struct UnkChozoArchives_laf7ba8 {
    u8 pad_0x0[0x2b8];
    CRntVector<bool> hidden;  // 0x2b8: one flag per archive
};

// A list of archive ids; freed on destruction.
struct UnkArchiveList_laf7ba8 : CRntVector<CStrId> {
    UnkArchiveList_laf7ba8() : CRntVector<CStrId>(0xf) {}
    ~UnkArchiveList_laf7ba8() { Free(); }
};

// The property's qualified id, built on first use.
static inline const UnkQualifiedId& Qid(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

// The id of the profile section that holds the archives.
static inline const CStrId& ArchivesSectionId() {
    return GameManagerIdTable().field_0x1be8;
}

// Stores `value` of type `type` under `name` in the section `section` names (unless the blackboard is locked).
static inline void SetValue(CBlackboard* blackboard, const CStrId& section, const UnkQualifiedId& name,
                            const base::reflection::CType* type, void* value) {
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = type;
            v.value = value;
            fn_7100332c80(s, reinterpret_cast<UnkPropertyName*>(const_cast<UnkQualifiedId*>(&name)), &v);
        }
    }
}

void fn_7100af7ba8(UnkChozoArchives_laf7ba8* self) {
    CBlackboard* profile = &g_unk_7101d47310->field_0x820;
    {
        const CStrId& section = ArchivesSectionId();
        const UnkQualifiedId& name = Qid(g_unk_717275adb0);
        UnkArchiveList_laf7ba8 none;
        SetValue(profile, section, name, &g_unk_7101d5c998, &none);
    }
    SetValue(profile, ArchivesSectionId(), Qid(g_unk_717275adf0),
             reinterpret_cast<const base::reflection::CType*>(&g_unk_7101d60e78), &self->hidden);
    UnkProfileSaveArg arg = {nullptr, nullptr};
    fn_7100df99f8(g_unk_7101d47310, 0, &arg, 0);
}
