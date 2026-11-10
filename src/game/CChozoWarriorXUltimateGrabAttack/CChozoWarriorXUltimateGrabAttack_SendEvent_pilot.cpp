// Sends the player's character class a value (`value`, 0 when it is -1) under the hash of an event
// kind: kinds 6 and 11 have their own hashes while the flag at +0x131 is set, kind 12 has one while the
// player holds ITEM_METROIDNIZATION, kinds 0-2 send nothing, and the others (and the fallbacks) take
// theirs from the table g_unk_71016498b8. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "types.h"

struct UnkInventory;
bool fn_71011428bc(UnkInventory* inventory, const UnkQualifiedId& item);

struct UnkOwner {
    u8 pad_0x0[0x270];
    UnkInventory* field_0x270;
};

struct UnkPlayer {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

UnkOwner* fn_7100defcd4(UnkGameManager* manager);
UnkPlayer* fn_7100dec788(UnkGameManager* manager);

struct UnkDesc_SpitSend;
extern UnkDesc_SpitSend g_unk_710164b968;
__attribute__((visibility("hidden"))) extern const u64 g_unk_71016498b8[];  // a hash per kind

struct UnkEventValue {
    u64 field_0x0;
    f32 field_0x8;
};

struct UnkEventSender {
    u8 pad_0x0[0x131];
    bool field_0x131;
};

static inline const UnkQualifiedId& GetQualifiedId(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

bool fn_7100488d30(UnkEventSender* self, s32 kind, f32 value) {
    u64 hash;
    if (kind == 12) {
        UnkInventory* inventory = fn_7100defcd4(g_unk_7101d47310)->field_0x270;
        if (!inventory || !fn_71011428bc(inventory, GetQualifiedId(g_unk_717275ab18)))
            goto from_table;
        hash = 0xacfdc97860b564f3;
    } else if (kind == 11) {
        if (!self->field_0x131)
            goto from_table;
        hash = 0x002efb08d3eb1224;
    } else if (kind == 6) {
        if (!self->field_0x131)
            goto from_table;
        hash = 0x76481b88c94277ac;
    } else {
        if ((u32)kind < 3)
            return false;
    from_table:
        hash = g_unk_71016498b8[kind];
    }

    void* target = fn_7100dec788(g_unk_7101d47310)->field_0x220;
    base::reflection::UnkReflTypeIdArg mode;
    UnkEventValue v = {0, value != -1.0f ? value : 0.0f};
    mode.id = 0;
    base::reflection::UnkReflTypeIdArg arg;
    arg.id = base::reflection::UnkReflTypeId<base::reflection::UnkReflTypeIdTag>::id();
    return fn_7101176690(target, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b968, &v, &arg);
}
