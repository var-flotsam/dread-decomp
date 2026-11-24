// CSmartObjectComponent's unit (tools/tu_map.py unit 1512): whether `target` belongs to this smart
// object's owner and counts as one: its id is the id table's entry at +0x20f0, or the player is there
// (its +0x210 set) and the owner's collision container lists it under "smartobject_fake".
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU2553.h"
#include "types.h"

using base::global::CStrId;

// As in src/game/CCollisionComponent/fn_71008651c8.cpp.
struct UnkStruct_71008651c8_Entry;
struct UnkStruct_71008651c8;
using UnkOut_71008651c8 = base::global::CRntVector<UnkStruct_71008651c8_Entry*>;
void CCollisionComponent_CollectRecordsBySecondaryId(UnkStruct_71008651c8* self, void* const* key, UnkOut_71008651c8* out);

struct UnkSmartObjectTarget_la04514;

// The found colliders, freed when the list goes out of scope.
struct UnkColliderList_la04514 : UnkOut_71008651c8 {
    explicit UnkColliderList_la04514(u32 flags) : UnkOut_71008651c8(flags) {}
    ~UnkColliderList_la04514() { Free(); }
};

// A collider the container finds: the object it stands for at +0x28.
struct UnkSmartObjectCollider_la04514 {
    u8 pad_0x0[0x28];
    UnkSmartObjectTarget_la04514* object;
};

struct UnkSmartObjectTarget_la04514 {
    u8 pad_0x0[0x10];
    UnkStruct_71008651c8* container;  // 0x10
    u8 pad_0x18[0x8];
    CStrId id;  // 0x20
};

struct UnkSmartObjectOwner_la04514 {
    u8 pad_0x0[0x2a0];
    UnkStruct_71008651c8* collision;  // 0x2a0
};

struct UnkSmartObject_la04514 {
    u8 pad_0x0[0x30];
    UnkSmartObjectOwner_la04514* owner;  // 0x30
};

struct UnkSmartObjectPlayer_la04514 {
    u8 pad_0x0[0x210];
    void* field_0x210;
};

// Whether the container lists `target` among its "smartobject_fake" colliders.
static inline bool IsListedFake(UnkStruct_71008651c8* collision, const UnkSmartObjectTarget_la04514* target) {
    UnkColliderList_la04514 found(0xf);
    static CStrId s_fake("smartobject_fake", true);
    CCollisionComponent_CollectRecordsBySecondaryId(collision, reinterpret_cast<void* const*>(&s_fake), &found);
    for (UnkStruct_71008651c8_Entry **it = found.data, **end = found.data + found.count; it != end; ++it) {
        if (reinterpret_cast<UnkSmartObjectCollider_la04514*>(*it)->object == target)
            return true;
    }
    return false;
}

bool fn_7100a04514(UnkSmartObject_la04514* self, UnkSmartObjectTarget_la04514* target) {
    if (target && target->container == self->owner->collision) {
        const CStrId& player_id = GameManagerIdTable().field_0x20f0;
        if (target->id.data == player_id.data)
            return true;
        UnkSmartObjectPlayer_la04514* player =
            reinterpret_cast<UnkSmartObjectPlayer_la04514*>(fn_7100dec788(g_unk_7101d47310));
        if (player && player->field_0x210 && IsListedFake(self->owner->collision, target))
            return true;
    }
    return false;
}
