// CChainReactionActionSwitcherComponent's unit (tools/tu_map.py): gives the owner's (+0x30) weak
// target (fn_71002b3b84), when there is one, the hash of the second string id (fn_71009c0f1c),
// then sends the owner's sub-object at +0x220, when there is one, the hash of the first with mode
// 0 (as UnkSpitSend, src/game_s2/UnkSpitSend.h). Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "game/CNavMeshItemComponent.h"
#include "game/UnkTU372.h"
#include "types.h"

struct UnkWeakTarget;

struct UnkChainReactionOwner_b54 {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkChainReactionSwitcher_b54 {
    u8 pad_0x0[0x30];
    union {
        UnkWeakTarget* owner;
        UnkChainReactionOwner_b54* owner_actor;
    };
};

static inline u64 StrIdHash_b54(const base::global::CStrId& id) {
    return *reinterpret_cast<u64*>(reinterpret_cast<char*>(id.data) + 0x30);
}

void fn_7100876a38(UnkChainReactionSwitcher_b54* self, const base::global::CStrId& first,
                   const base::global::CStrId& second) {
    if (void* target = fn_71002b3b84(self->owner))
        fn_71009c0f1c(target, StrIdHash_b54(second));
    if (void* target = self->owner_actor->field_0x220)
        UnkSpitSend(target, StrIdHash_b54(first), 0xf4061ff25a2163d6);
}
