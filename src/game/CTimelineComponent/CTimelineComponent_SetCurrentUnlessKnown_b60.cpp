// CTimelineComponent (unit from tools/tu_map.py): in states 1 and 2 (+0x48), unless one of the
// table's entries (their objects' class ids: the word at +0x30 of what fn_710070d7f8 returns) is
// known (fn_7100a2be94), sets the id at +0x50 to the all-flags mask. Names and layouts are
// placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkTU1134.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

struct UnkObj_710117af10;
struct UnkCls_710117af10;

struct UnkTimelineClass_b60 {
    u8 unk_0x0[0x30];
    u64 id;
};

struct UnkTimelineState_b60 {
    u8 unk_0x0[0x48];
    s32 state;
    u8 unk_0x4c[0x4];
    u64 current;
};

// Sets `id` to the all-flags mask now.
static inline __attribute__((always_inline)) void SetAll_b60(UnkTimelineComponent* self, const u64& id) {
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    fn_7100a2cb7c(self, &id, &mask.id, false, -1.0f);
}

void fn_7100a2bd5c(UnkTimelineComponent* self) {
    UnkTimelineState_b60* state = reinterpret_cast<UnkTimelineState_b60*>(self);
    if (static_cast<u32>(state->state - 1) > 1)
        return;
    bool found = false;
    u64 key;
    for (UnkTableEntry* it = self->table.values, *end = it + self->table.count; it != end; ++it) {
        auto** cls = reinterpret_cast<UnkTimelineClass_b60**>(
            fn_710070d7f8(reinterpret_cast<UnkObj_710117af10*>(it->field_0x0)));
        key = (*cls)->id;
        if (fn_7100a2be94(self, &key)) {
            found = true;
            break;
        }
    }
    if (found)
        return;
    SetAll_b60(self, state->current);
}
