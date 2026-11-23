// CGoliathAIComponent's unit (tools/tu_map.py unit 844): the kind of a hit. After the base check
// (fn_71011061b4), the id the owner's collision container (+0x2a0) gives the hit's object decides:
// 2 when its text contains the text of the unit's id at 0x71726e6740, 6 or 0 (bit 27 of the hit's
// flags) when it is the unit's id at 0x71726e6748, else 0. Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/CCollisionComponent.h"
#include "game/CGoliathAIComponent.h"
#include "game/UnkGoliathIdData_b55.h"
#include "types.h"

using base::global::CStrId;

struct UnkCollisionContainer;

struct UnkGoliathOwner_l5917a8 {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer* field_0x2a0;
};

struct UnkGoliathHit_l5917a8 {
    u8 pad_0x0[0x60];
    void* field_0x60;
    u8 pad_0x68[0x4c];
    u32 field_0xb4;
};

struct UnkGoliathAI_l5917a8 {
    void* vtable;
    u8 pad_0x8[0x28];
    UnkGoliathOwner_l5917a8* owner;  // 0x30
};

u32 fn_71011061b4(UnkGoliathAI_l5917a8* self, UnkGoliathHit_l5917a8* hit);

// The text of the unit's id, counting the read.
static inline const char* PartText() {
    UnkGoliathIdData_b55* part = g_unk_71726e6740;
    ++part->count;
    return part->text ? part->text : g_unk_710162e2ed;
}

u32 fn_71005917a8(UnkGoliathAI_l5917a8* self, UnkGoliathHit_l5917a8* hit) {
    u32 result = fn_71011061b4(self, hit);
    if (result)
        return result;
    UnkCollisionContainer* collision = self->owner->field_0x2a0;
    CStrId id(collision ? *fn_7100864018(collision, hit->field_0x60) : g_unk_7101d470e0);
    id.Text();  // the read is counted even for the empty id
    if (id.data != &g_unk_7101d47108->empty && fn_710007fb9c(&id.data->String(), PartText()))
        return 2;
    if (id.data == g_unk_71726e6748.data)
        return (hit->field_0xb4 & 0x8000000) ? 6 : 0;
    return 0;
}
