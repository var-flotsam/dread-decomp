// CAreaFXComponent's unit: when enabled (+0x400) and the owner's animation does not already have the
// track named at +0x3f8, plays the animation named at +0x408 (fn_7101176ea8, key 0xf4061ff25a2163d6) with
// the blend time at +0x410, inside a scoped 0x100-byte object. Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "game/CAnimationComponent.h"
#include "game/UnkActor.h"
#include "game/UnkObject_0x100.h"
#include "game/UnkTU376.h"
#include "types.h"

using base::global::CStrId;

struct UnkTrackDesc_l802244 {
    u8 pad_0x0[0x90];
    CStrId name;
};

struct UnkAreaFX_l802244 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x3f8 - 0x38];
    UnkTrackDesc_l802244* track;  // 0x3f8
    bool enabled;                 // 0x400
    CStrId animation;             // 0x408
    f32 blend_time;               // 0x410
};
static_assert(__builtin_offsetof(UnkAreaFX_l802244, blend_time) == 0x410, "layout");

// The scoped object: constructed by fn_71002b57bc, destroyed by fn_71002b6094.
struct UnkScope_l802244 {
    u64 words[0x20];
    UnkScope_l802244() { fn_71002b57bc(reinterpret_cast<UnkObject_0x100*>(this)); }
    ~UnkScope_l802244() { fn_71002b6094(reinterpret_cast<UnkObject_0x100*>(this)); }
};

struct UnkDescWords_l802244 {
    u64 words[2];
};

struct UnkPlayRequest_l802244 {
    u64 key;
    u64 desc[2];
    u64 field_0x18;
    f32 blend_time;
    u32 mode;
    u32 mask;
};

static inline bool IsEmpty(const CStrId& id) {
    return id.data == &g_unk_7101d47108->empty;
}

void fn_7100802244(UnkAreaFX_l802244* self) {
    void* animation = self->owner ? self->owner->field_0x220 : nullptr;
    if (!self->enabled)
        return;
    if (animation && self->track && fn_71011771e0(animation, self->track->name.data->Key()))
        return;

    UnkScope_l802244 scope;
    if (animation && !IsEmpty(self->animation) && self->blend_time > -0.001f) {
        const UnkDescWords_l802244* desc = reinterpret_cast<const UnkDescWords_l802244*>(&g_unk_710164b958);
        u64 desc0 = desc->words[0];
        u64 desc1 = desc->words[1];
        u32 mask = UnkAll_SpitSend().value;
        UnkPlayRequest_l802244 request;
        request.key = self->animation.data->Key();
        request.desc[0] = desc0;
        request.desc[1] = desc1;
        request.field_0x18 = 0;
        request.blend_time = self->blend_time;
        request.mode = 2;
        request.mask = mask;
        fn_7101176ea8(static_cast<UnkStruct_7101176ea8*>(animation),
                      reinterpret_cast<const UnkRequest_7101176ea8*>(&request), 0xf4061ff25a2163d6);
    }
}
