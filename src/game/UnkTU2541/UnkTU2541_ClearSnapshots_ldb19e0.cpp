// Unit 2541: stores an empty custom-data block under the eight "...LastTimeSnapShot" properties in the game
// manager's archive blackboard (each unless it is locked), then saves the profile. Names and layouts are
// placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/animation/CBlendSpaceResource.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU1645.h"
#include "game/UnkTU2553.h"
#include "game/UnkTypedValue.h"
#include "types.h"

using base::global::CStrId;

// A block that is either owned (copied on assignment) or borrowed.
struct UnkBlob_u71 {
    u32 size;
    void* data;  // 0x8
    bool owns;   // 0x10
};

void fn_71000947d4(UnkBlob_u71* self);

struct UnkProfileSaveArg {
    void* field_0x0;
    void* field_0x8;
};

// The snapshot properties (src/game_s7/fn_7100ae39e0.cpp).

// An empty block, released when it goes out of scope.
struct UnkEmptyBlob_db19e0 {
    UnkBlob_u71 blob;

    UnkEmptyBlob_db19e0() { fn_71000947d4(&blob); }
    ~UnkEmptyBlob_db19e0() {
        if (blob.owns && blob.data) {
            fn_7100083c30(blob.data);
            blob.data = nullptr;
        }
    }
};

static inline const CStrId& Section() {
    return base::reflection::refl_GetStrIdTable<0x7100031894>().ids[0xb4];
}

static inline const UnkQualifiedId& DefId(UnkPropertyEx<>& def) {
    UnkQualifiedId& qid = ItemQualifiedId(def);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(def), g_unk_7101d470e0);
    return qid;
}

// Stores the block `value` under `name` in the section `section`, unless the blackboard is locked.
static inline void SetBlob(CBlackboard* blackboard, const CStrId& section, const UnkQualifiedId& name,
                           UnkBlob_u71* value) {
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = static_cast<const base::reflection::CType*>(fn_710000ffd0());
            v.value = value;
            fn_7100332c80(s, reinterpret_cast<UnkPropertyName*>(const_cast<UnkQualifiedId*>(&name)), &v);
        }
    }
}

void fn_7100db19e0() {
    CBlackboard* archive = &g_unk_7101d47310->field_0x820;
    UnkEmptyBlob_db19e0 empty;
    SetBlob(archive, Section(), DefId(g_unk_717275b188), &empty.blob);
    SetBlob(archive, Section(), DefId(g_unk_717275b1c8), &empty.blob);
    SetBlob(archive, Section(), DefId(g_unk_717275b208), &empty.blob);
    SetBlob(archive, Section(), DefId(g_unk_717275b248), &empty.blob);
    SetBlob(archive, Section(), DefId(g_unk_717275b288), &empty.blob);
    SetBlob(archive, Section(), DefId(g_unk_717275b2c8), &empty.blob);
    SetBlob(archive, Section(), DefId(g_unk_717275b308), &empty.blob);
    SetBlob(archive, Section(), DefId(g_unk_717275b348), &empty.blob);
    UnkProfileSaveArg arg = {nullptr, nullptr};
    fn_7100df99f8(g_unk_7101d47310, 0, &arg, false);
}
