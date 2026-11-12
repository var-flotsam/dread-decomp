// CFireComponent's unit (tools/tu_map.py): fn_7100831aa0 asks the message (vtable +0x50) whether it
// applies and, if so, lets it act (+0x58), clears +0x58 and, for kind 2, passes the message on to the
// game manager's object fn_7100df5460 returns (fn_7100305818); then looks the owner's (+0x30)
// sub-object at +0x220 up for CCharClassCantFireTrack's record (fn_710117b08c) and, when found, sends
// it the record's string id hash with mode 0 and second descriptor g_unk_7172769758 when the record's
// flag (+0x28) is set, else g_unk_7172769740 (first descriptor g_unk_710164b968). Returns whether the
// message applied. Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU2553.h"
#include "types.h"

// As in src/game/CCharClassToStateTrack/CCharClassToStateTrack_Fill.cpp.
struct __attribute__((packed, aligned(4))) UnkTrackBlend {
    u64 kind;
    f32 value;
};
extern UnkDesc_SpitSend g_unk_710164b968;  // reached through the GOT

struct UnkFireMessage_b57 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual bool vf_0x50(s32* kind);
    virtual void vf_0x58(s32* kind);
};

struct UnkFireOwner_b57 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* field_0x220;
};

struct UnkFireComponent_b57 {
    u8 pad_0x0[0x30];
    UnkFireOwner_b57* owner;
    u8 pad_0x38[0x58 - 0x38];
    u32 field_0x58;
};

struct UnkCantFireRecord_b57 {
    u8 pad_0x0[0x20];
    base::global::UnkStringData* id;
    bool field_0x28;
};

void fn_7100305818(void* object, UnkFireMessage_b57* message, s32* kind);

bool fn_7100831aa0(UnkFireComponent_b57* self, UnkFireMessage_b57* message, void* unused, s32* kind) {
    bool applies = message->vf_0x50(kind);
    if (applies) {
        message->vf_0x58(kind);
        self->field_0x58 = 0;
        if (*kind == 2) {
            if (void* object = fn_7100df5460(g_unk_7101d47310))
                fn_7100305818(object, message, kind);
        }
    }
    if (UnkAnimationComponent* target = self->owner->field_0x220) {
        UnkTrackQuery query = {fn_7100cf0d38(), nullptr, -1.0f};
        fn_710117b08c(self->owner->field_0x220, &query);
        if (query.result) {
            auto* record = reinterpret_cast<UnkCantFireRecord_b57*>(query.result);
            u64 hash = *reinterpret_cast<u64*>(reinterpret_cast<char*>(record->id) + 0x30);
            base::reflection::UnkReflTypeIdArg mode;
            mode.id = 0;
            UnkDesc_SpitSend* desc = record->field_0x28 ? reinterpret_cast<UnkDesc_SpitSend*>(&g_unk_7172769758) : &g_unk_7172769740;
            base::reflection::UnkReflTypeIdArg mask;
            mask.id = UnkAll_SpitSend().value;
            fn_7101176690(target, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b968, desc, &mask);
        }
    }
    return applies;
}
