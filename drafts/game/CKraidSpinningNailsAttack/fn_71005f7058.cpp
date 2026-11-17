// CKraidSpinningNailsAttack's unit (tools/tu_map.py unit 918): on an event of kind 1 for the id table's
// slot 0x10c0 whose target is not already in the relax track, takes the next nail entry (fn_71005f67cc)
// and sends the owner's character class a track: the relax track when no entry is left or Kraid (phase 1)
// stands more than 200 units above its target, else the entry's track by its kind and side. Names and
// layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CClass;

struct UnkKraidNailsAttack_A4_b71;
void fn_71005f67cc(UnkKraidNailsAttack_A4_b71* self);

// The id table's slot this file reads (not yet a member of UnkGameManagerIdTable).
struct UnkIdTableSlot_l5f7058 {
    u8 pad_0x0[0x10c0];
    CStrId field_0x10c0;
};

struct UnkKraidAI_l5f7058 {
    void** vtable;
    u8 pad_0x8[0x1140 - 0x8];
    u32 phase;  // 0x1140

    const CClass* vf_0x0() { return reinterpret_cast<const CClass* (*)(UnkKraidAI_l5f7058*)>(vtable[0])(this); }
};

UnkVec3f fn_71005fd8c0(UnkKraidAI_l5f7058* kraid);

// The nail entry taken last: its track kind (1 to 4) and side.
struct UnkKraidNailEntry_l5f7058 {
    u8 pad_0x0[0x8];
    u32 kind;  // 0x8
    u32 side;  // 0xc
};

struct UnkKraidNailsAttack_l5f7058 {
    u8 pad_0x0[0x8];
    UnkActor* owner;  // 0x8
    u8 pad_0x10[0x48];
    UnkKraidNailEntry_l5f7058* entry;  // 0x58
};

struct UnkEventTarget_l5f7058 {
    u8 pad_0x0[0x38];
    CStrId track;  // 0x38
};

struct UnkEventSource_l5f7058 {
    u8 pad_0x0[0x10];
    UnkEventTarget_l5f7058* target;  // 0x10
};

struct UnkEvent_l5f7058 {
    u32 kind;
    UnkEventSource_l5f7058* source;  // 0x8
    CStrId id;                       // 0x10
};

// The tracks of each entry kind, by side (0, 2, other).
extern const u64 g_unk_710164a180;
extern const u64 g_unk_710164a188;
extern const u64 g_unk_710164a190;
extern const u64 g_unk_710164a198;
extern const u64 g_unk_710164a1a0;
extern const u64 g_unk_710164a1a8;
extern const u64 g_unk_710164a1b0;
extern const u64 g_unk_710164a1b8;
extern const u64 g_unk_710164a1c0;
extern const u64 g_unk_710164a1c8;
extern const u64 g_unk_710164a1d0;
extern const u64 g_unk_710164a1d8;

static const u64 kRelaxTrack = 0x8d46827d12e936a6ull;

static inline u64 BySide(u32 side, const u64& side0, const u64& side2, const u64& other) {
    return (side == 0 ? side0 : side == 2 ? side2 : other);
}

// Sends a track with mode 0 (as UnkSpitSend) through the caller's descriptors.
static inline void Send(void* animation, u64 hash, base::reflection::UnkReflTypeIdArg* mode,
                        base::reflection::UnkReflTypeIdArg* mask) {
    mode->id = 0;
    mask->id = UnkAll_SpitSend().value;
    fn_7101176690(animation, hash, 0xf4061ff25a2163d6ull, mode, &g_unk_710164b958, &g_unk_7172769740, mask);
}

bool fn_71005f7058(UnkKraidNailsAttack_l5f7058* self, UnkEvent_l5f7058* event) {
    base::reflection::UnkReflTypeIdArg relax_mode;
    base::reflection::UnkReflTypeIdArg relax_mask;
    base::reflection::UnkReflTypeIdArg mode;
    base::reflection::UnkReflTypeIdArg mask;

    if (event->kind != 1)
        return true;
    UnkEventTarget_l5f7058* target = event->source->target;
    const UnkIdTableSlot_l5f7058& table = reinterpret_cast<const UnkIdTableSlot_l5f7058&>(GameManagerIdTable());
    if (event->id.data->Key() != table.field_0x10c0.data->Key())
        return true;
    if (target->track.data->Key() == kRelaxTrack)
        return true;

    fn_71005f67cc(reinterpret_cast<UnkKraidNailsAttack_A4_b71*>(self));
    void* animation = self->owner->field_0x220;
    bool relax = self->entry == nullptr;
    if (!relax) {
        auto* ai = static_cast<UnkKraidAI_l5f7058*>(self->owner->field_0x240);
        UnkKraidAI_l5f7058* kraid = fn_710009d50c(ai->vf_0x0(), fn_71005fac74()) ? ai : nullptr;
        if (kraid->phase == 1)
            relax = fn_71005fd8c0(kraid).y > fn_7100d7c990(self->owner)->field_0x54 + 200.0f;
    }
    if (relax) {
        Send(animation, kRelaxTrack, &relax_mode, &relax_mask);
        return true;
    }

    UnkKraidNailEntry_l5f7058* entry = self->entry;
    u64 hash;
    switch (entry->kind) {
    case 1:
        hash = BySide(entry->side, g_unk_710164a180, g_unk_710164a188, g_unk_710164a190);
        break;
    case 3:
        hash = BySide(entry->side, g_unk_710164a1b0, g_unk_710164a1b8, g_unk_710164a1c0);
        break;
    case 4:
        hash = BySide(entry->side, g_unk_710164a1c8, g_unk_710164a1d0, g_unk_710164a1d8);
        break;
    case 2:
        hash = BySide(entry->side, g_unk_710164a198, g_unk_710164a1a0, g_unk_710164a1a8);
        break;
    default:
        hash = static_cast<u64>(-1);
        break;
    }
    Send(animation, hash, &mode, &mask);
    return true;
}
