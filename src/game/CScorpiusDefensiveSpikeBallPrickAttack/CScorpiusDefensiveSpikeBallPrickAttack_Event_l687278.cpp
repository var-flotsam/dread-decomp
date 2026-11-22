// CScorpiusDefensiveSpikeBallPrickAttack's unit (tools/tu_map.py): the animation event handler. A kind-3
// event whose name does not contain the first id's text, from a track whose name contains the second's,
// drops the weak handle at +0x50, sends the owner's track (+0x220) a request with mode 2 under the two
// ids' hashes, and clears the value at +0x2318 of the owner's target. Always true. Names and layouts are
// placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/UnkActor.h"
#include "game/UnkTU1730.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

// The unit's two ids, side by side: the event name to skip and the track name to answer.
struct UnkPrickNames_l687278 {
    CStrId event;  // 0x7172703018
    CStrId track;  // 0x7172703020
};
extern UnkPrickNames_l687278 g_unk_7172703018 __attribute__((visibility("hidden")));

// The pooled string of an id (a CRntString at +0x10), counting the read as UnkStrIdBase::Text does.
static inline const CRntString& Str(const CStrId& id) {
    ++id.data->reads;
    return id.data->String();
}

// Whether `name` is set and its text contains that of `part`.
static inline bool Contains(const CStrId& name, const CStrId& part) {
    const CRntString& text = Str(name);
    if (name.data == &g_unk_7101d47108->empty)
        return false;
    return CRntString_Contains(&text, part.Text());
}

struct UnkPrickEventTrack_l687278 {
    u8 pad_0x0[0x38];
    CStrId name;  // 0x38
};

struct UnkPrickEventRef_l687278 {
    u8 pad_0x0[0x10];
    UnkPrickEventTrack_l687278* track;
};

struct UnkPrickEvent_l687278 {
    s32 kind;
    u8 pad_0x4[4];
    UnkPrickEventRef_l687278* ref;  // 0x8
    CStrId name;                    // 0x10
};

struct UnkPrickTarget_l687278 {
    u8 pad_0x0[0x2318];
    f32 value;  // 0x2318
    bool set;   // 0x231c
};

struct UnkPrickAttack_l687278 {
    u8 pad_0x0[0x8];
    UnkActor* owner;  // 0x8
    u8 pad_0x10[0x38 - 0x10];
    u8 link[0x18];          // 0x38
    UnkWeakHandle* handle;  // 0x50
};

bool fn_7100687278(UnkPrickAttack_l687278* self, UnkPrickEvent_l687278* event) {
    if (event->kind != 3)
        return true;
    if (Contains(event->name, g_unk_7172703018.event))
        return true;
    if (!Contains(event->ref->track->name, g_unk_7172703018.track))
        return true;
    void* track = self->owner->field_0x220;
    if (!track)
        return true;
    if (UnkWeakHandle* handle = self->handle) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, self->link);
        ResetRef(&self->handle);
    }
    base::reflection::refl_EmitU32(track, g_unk_7172703018.track.data->Key(),
                                   g_unk_7172703018.event.data->Key(), 2,
                                   &g_unk_710164b958, &g_unk_7172769740);
    if (UnkPrickTarget_l687278* target = static_cast<UnkPrickTarget_l687278*>(fn_7100d7c990(self->owner)->field_0x208)) {
        target->value = 0.0f;
        target->set = false;
    }
    return true;
}
