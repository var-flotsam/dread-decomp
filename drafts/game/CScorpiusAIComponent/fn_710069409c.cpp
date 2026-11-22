// CScorpiusAIComponent's unit (tools/tu_map.py): sends the owner's (+0x30) animation (+0x220) the id
// `id` with the values `a` and `b` (b -1 for the default descriptor), then the same under the id's
// name followed by "_taillayer" when the animation has that one, else again under the id. Names and
// layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/global/timeline/CEvent.h"
#include "game/CAnimationComponent.h"
#include "game/UnkActor.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CRntString;

struct UnkScorpiusAI_b59a4;
struct UnkScorpiusAIVtbl_b59a4 {
    void* slots[0x518 / 8];
    void (*vf_0x518)(UnkScorpiusAI_b59a4* self);
};

struct UnkScorpiusAI_b59a4 {
    UnkScorpiusAIVtbl_b59a4* vtable;
    u8 pad_0x8[0x28];
    UnkActor* owner;
    u8 pad_0x38[0x14cc0 - 0x38];
    bool field_0x14cc0;
    u8 pad_0x14cc1[0x14cd4 - 0x14cc1];
    u32 field_0x14cd4;
};

// A string id: its pooled data.
struct UnkScorpiusKey_b59a4 {
    base::global::UnkStringData* data;

    u64 Hash() const { return data->Key(); }
    const CRntString& Str() const {
        ++data->reads;
        return data->String();
    }
};

// The value descriptor of a send (the default one is g_unk_7172769740).
struct UnkSendValue_l69409c {
    u32 field_0x0;
    u32 field_0x4;
    f32 value;
};

// A typed value: a 4-byte payload and its kind (1: float).
struct UnkSendFloat_l69409c {
    f32 value;
    u32 kind;
};

static inline UnkSendFloat_l69409c MakeSendFloat_l69409c(f32 value) {
    UnkSendFloat_l69409c v;
    v.value = value;
    v.kind = 1;
    return v;
}

// The range a send applies over.
struct UnkSendRange_l69409c {
    s64 field_0x0;
    UnkSendFloat_l69409c value;

    explicit UnkSendRange_l69409c(UnkSendFloat_l69409c v) : field_0x0(-1), value(v) {}
};

bool fn_7101176e78(UnkStruct_7101176ea8* self, void* key, void** out);

// Sends `hash` under `kind` with the mode, the range of `a` and the value descriptor given; the mode and the mask
// are filled in the caller's.
static inline bool Send(void* target, u64 hash, u64 kind, base::reflection::UnkReflTypeIdArg* mode,
                        u32 modeId, UnkSendFloat_l69409c a, const UnkSendValue_l69409c* desc,
                        base::reflection::UnkReflTypeIdArg* mask) {
    mode->id = modeId;
    UnkSendRange_l69409c range(a);
    mask->id = UnkAll_SpitSend().value;
    return fn_7101176690(target, hash, kind, mode, &range, desc, mask);
}

bool fn_710069409c(UnkScorpiusAI_b59a4* self, const UnkScorpiusKey_b59a4* id, bool flag, f32 a, f32 b) {
    void* target = self->owner->field_0x220;
    const u64 track = 0xf4061ff25a2163d6;
    const u64 layer = 0x722b01c64fe0ffe0;
    // Every send's mode and mask at function scope: none of their stack slots is shared, as in the
    // original.
    base::reflection::UnkReflTypeIdArg mode;
    UnkSendValue_l69409c desc;
    if (b == -1.0f) {
        desc = reinterpret_cast<const UnkSendValue_l69409c&>(g_unk_7172769740);
    } else {
        desc.field_0x0 = 0;
        desc.field_0x4 = 0;
        desc.value = b;
    }
    base::reflection::UnkReflTypeIdArg mask;
    u64 hash;
    u32 modeId = flag ? 2 : 0;
    UnkSendFloat_l69409c value = MakeSendFloat_l69409c(a);
    bool sent = Send(target, id->Hash(), track, &mode, modeId, value, &desc, &mask);
    {
        CRntString name = CRntString_ConcatCString(&id->Str(), "_taillayer");
        fn_7100066630(&hash, &name);
    }
    // Declared past a goto so that they get no lifetime markers: each send keeps its own slots.
    goto send;
    base::reflection::UnkReflTypeIdArg mode2, mask2, mode3, mask3;
send:
    if (fn_7101176e78(static_cast<UnkStruct_7101176ea8*>(target), reinterpret_cast<void*>(hash), nullptr))
        return sent & Send(target, hash, layer, &mode2, modeId, value, &desc, &mask2);
    return sent & Send(target, id->Hash(), layer, &mode3, modeId, value, &desc, &mask3);
}
