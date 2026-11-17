// CSamusStandMovementState::CParams' unit (tools/tu_map.py): for kind 1 or 2, hands the movement at +0x38 a
// request (fn_71009e8218) for the id table's slot +0xaf8 or +0xb00, with the default descriptor and blend.
// Names and layouts are placeholders.
#include "../../../game_s2/UnkSpitSend.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU1730.h"
#include "types.h"

struct UnkMove_b152;
struct UnkRequest_7101176ea8;
bool fn_71009e8218(UnkMove_b152* self, const UnkRequest_7101176ea8* request, bool flag);

struct UnkStandDesc_l9b4688 {
    u64 words[2];
};

struct UnkStandBlend_l9b4688 {
    u64 kind;
    u32 value;
};

struct UnkStandRequest_l9b4688 {
    u64 id;
    u64 desc[2];     // 0x8
    u64 blend_kind;  // 0x18
    u32 blend_value; // 0x20
    u32 flags;       // 0x24
    u32 mask;        // 0x28
};

struct UnkStandParams_l9b4688 {
    u8 pad_0x0[0x38];
    UnkMove_b152* movement;  // 0x38
};

// The id table's slots this unit reads that the shared layout does not name yet.
inline const base::global::CStrId& IdSlot_l9b4688(u32 offset) {
    return *reinterpret_cast<const base::global::CStrId*>(reinterpret_cast<const u8*>(&GameManagerIdTable()) +
                                                          offset);
}

inline void Request_l9b4688(UnkMove_b152* movement, const base::global::CStrId& id,
                            UnkStandRequest_l9b4688& request) {
    const base::global::CStrId* slot = &id;
    const UnkStandDesc_l9b4688* desc = reinterpret_cast<const UnkStandDesc_l9b4688*>(&g_unk_710164b958);
    u64 desc0 = desc->words[0];
    u64 desc1 = desc->words[1];
    const UnkStandBlend_l9b4688* blend = reinterpret_cast<const UnkStandBlend_l9b4688*>(&g_unk_7172769740);
    u64 blend_kind = blend->kind;
    u32 blend_value = blend->value;
    u64 key = slot->data->Key();
    u32 mask = UnkAll_SpitSend().value;
    request.id = key;
    request.desc[0] = desc0;
    request.desc[1] = desc1;
    request.blend_kind = blend_kind;
    request.blend_value = blend_value;
    request.flags = 0;
    request.mask = mask;
    fn_71009e8218(movement, reinterpret_cast<const UnkRequest_7101176ea8*>(&request), false);
}

void fn_71009b4688(UnkStandParams_l9b4688* self, s32 kind) {
    UnkStandRequest_l9b4688 request;
    switch (kind) {
    case 1:
        Request_l9b4688(self->movement, IdSlot_l9b4688(0xaf8), request);
        break;
    case 2:
        Request_l9b4688(self->movement, IdSlot_l9b4688(0xb00), request);
        break;
    }
}
