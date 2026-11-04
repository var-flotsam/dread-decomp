// CEmmyAIComponent's unit (tools/tu_map.py): a message handler that never consumes the message. For the
// message id at +0x18c0 of the singleton (fn_7100031894), when the record at +0x29ef0 is not in state 1
// nor locked (+0x4) and the owner's (+0x30) animation does not play the CCharClassBumpedTrack class: an
// owner whose +0x218 fn_7100979ef0 accepts clears the record's flag +0x800; otherwise, unless that part
// is in mode 1 with a part at +0x260 that fn_710113d83c says no to (which sets the flag), asks
// fn_710054efb0 for 2 with the tunable's +0xec. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game_s2/UnkChargeComponent.h"
#include "types.h"

bool fn_7100979ef0(void*);
u32 fn_71010cc3a4(void*);
bool fn_710113d83c(void*);

struct UnkEmmyMsgRecord_A3_b64 {
    u32 state;
    bool locked;
    u8 pad_0x5[0x800 - 0x5];
    bool field_0x800;
};

struct UnkEmmyMsgAI_A3_b64 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x29ef0 - 0x38];
    UnkEmmyMsgRecord_A3_b64 record;
};

void fn_710054efb0(UnkEmmyMsgAI_A3_b64* self, u32 kind, f32 value);

// The owner's part at +0x260 (not spelled out in UnkActor).
static inline void* Part260_A3_b64(UnkActor* owner) {
    return *reinterpret_cast<void**>(reinterpret_cast<char*>(owner) + 0x260);
}

bool fn_71005519cc(UnkEmmyMsgAI_A3_b64* self, const u64* message) {
    const u64* id =
        reinterpret_cast<const u64*>(reinterpret_cast<char*>(&base::reflection::refl_GetSingleton()) + 0x18c0);
    if (*message != *id)
        return false;
    UnkEmmyMsgRecord_A3_b64* record = &self->record;
    if (record->state == 1 || record->locked)
        return false;
    UnkAnimationComponent* animation = static_cast<UnkAnimationComponent*>(self->owner->field_0x220);
    if (animation && fn_710117b348(animation, fn_7100cee4b8()))
        return false;
    void* part = self->owner->field_0x218;
    if (fn_7100979ef0(part)) {
        record->field_0x800 = false;
        return false;
    }
    void* gate = nullptr;
    if (fn_71010cc3a4(part) != 1 || !(gate = Part260_A3_b64(self->owner)) || fn_710113d83c(gate)) {
        record->field_0x800 = true;
        return false;
    }
    fn_710054efb0(self, 2, *reinterpret_cast<const f32*>(reinterpret_cast<const char*>(&g_unk_71726e0aa8) + 0xec));
    return false;
}
