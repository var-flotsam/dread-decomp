// CGoliathAIComponent's unit (tools/tu_map.py unit 844): whether the owner's collision container
// (+0x2a0) maps `object` to a non-empty id whose text contains the text of the id at
// 0x71726e6740. Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/CCollisionComponent.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

struct UnkCollisionContainer;

extern CStrId g_unk_71726e6740 __attribute__((visibility("hidden")));

struct UnkGoliathOwner_l591c50 {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer* collision;
};

struct UnkGoliathAI_l591c50 {
    u8 pad_0x0[0x30];
    UnkGoliathOwner_l591c50* owner;
};

// The id's string, counting the read.
static inline const CRntString& Name(const CStrId& id) {
    ++id.data->reads;
    return id.data->String();
}

u32 fn_7100591c50(UnkGoliathAI_l591c50* self, void* object) {
    bool result = false;
    if (object) {
        if (UnkCollisionContainer* collision = self->owner->collision) {
            CStrId id(*fn_7100864018(collision, object));
            const CRntString& name = Name(id);
            if (id.data != &g_unk_7101d47108->empty)
                result = CRntString_Contains(&name, g_unk_71726e6740.Text());
        }
    }
    return result;
}
