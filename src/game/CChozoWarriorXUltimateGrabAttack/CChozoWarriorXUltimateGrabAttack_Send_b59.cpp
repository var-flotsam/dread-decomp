// CChozoWarriorXUltimateGrabAttack's unit (tools/tu_map.py): sends the owner's (+0x8) character class
// (+0x220) the hash for a kind: kind 12 with the object at +0x270 of the game manager's owner
// (fn_7100defcd4) reacting to ITEM_METROIDNIZATION's id gets a fixed hash; kinds 0, 1, 2, 5 and 10
// (otherwise) send nothing and return false; the rest take their hash from a table. The request carries
// the value (0 for -1) with first descriptor g_unk_710164b968. Returns whether it applied. Names are
// placeholders.
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkTU2553.h"
#include "types.h"

using base::global::CStrId;

struct UnkOwner;
bool fn_71011428bc(void* object, const CStrId* id);
extern UnkDesc_SpitSend g_unk_710164b968;  // reached through the GOT
extern const u64 g_unk_7101649920[] __attribute__((visibility("hidden")));

struct UnkUltGrabValue_b59 {
    u64 field_0x0;
    f32 value;
};

struct UnkUltGrabManagerOwner_b59 {
    u8 pad_0x0[0x270];
    void* field_0x270;
};

struct UnkUltGrabOwner_b59 {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkUltGrab_b59 {
    u8 pad_0x0[0x8];
    UnkUltGrabOwner_b59* owner;
};

bool fn_7100488f50(UnkUltGrab_b59* self, s32 kind, f32 value) {
    u64 hash;
    if (kind == 12) {
        auto* owner = reinterpret_cast<UnkUltGrabManagerOwner_b59*>(fn_7100defcd4(g_unk_7101d47310));
        if (void* object = owner->field_0x270) {
            UnkQualifiedId& id = ItemQualifiedId(g_unk_717275ab18);
            if (id.id.data == &g_unk_7101d47108->empty)
                fn_7100ae9a10(&id, ItemSource(g_unk_717275ab18), g_unk_7101d470e0);
            if (fn_71011428bc(object, &id.id)) {
                hash = 0x3490d5d956677171;
                goto send;
            }
        }
    }
    if ((0x427ull >> kind) & 1)
        return false;
    hash = g_unk_7101649920[kind];
send:
    void* target = self->owner->field_0x220;
    base::reflection::UnkReflTypeIdArg mode;
    UnkUltGrabValue_b59 param;
    param.field_0x0 = 0;
    mode.id = 0;
    param.value = value != -1.0f ? value : 0.0f;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    return fn_7101176690(target, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b968, &param,
                         &mask);
}
