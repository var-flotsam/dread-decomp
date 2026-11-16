// CChozoCommanderAIComponent unit (tools/tu_map.py): the animation event handler. A kind-3 event of the
// usual track kind raises the send flag at +0x15f00 unless the animation is on that track and its name
// contains g_unk_71726c9540's text; a kind-1 event raises it for its own track kind, otherwise runs slot
// 0x208 for two ids and, for a third, sends the animation a fixed request with mode 1 (descriptors as
// UnkSpitSend, src/game_s2/UnkSpitSend.h). Always returns true. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/CAnimationComponent.h"
#include "game/CChozoCommanderAIComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU1730.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

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
    return fn_710007fb9c(&text, part.Text());
}

struct UnkEventId_l455200 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkEventIdHolder_l455200 {
    u8 pad_0x0[0x38];
    UnkEventId_l455200* id;
};

struct UnkEventRef_l455200 {
    u8 pad_0x0[0x10];
    UnkEventIdHolder_l455200* holder;
};

struct UnkAnimEvent_l455200 {
    s32 kind;
    u8 pad_0x4[4];
    UnkEventRef_l455200* ref;    // 0x8
    UnkEventId_l455200* track;   // 0x10
};

struct UnkCommanderOwner_l455200 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkCommanderAI_l455200 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkCommanderOwner_l455200* owner;  // 0x30
    u8 pad_0x38[0x15ec8];
    bool send_pending;  // 0x15f00

    void vf_0x208(bool a, bool b) {
        reinterpret_cast<void (*)(UnkCommanderAI_l455200*, bool, bool)>(vtable[0x208 / 8])(this, a, b);
    }
};
static_assert(__builtin_offsetof(UnkCommanderAI_l455200, send_pending) == 0x15f00, "layout");

bool fn_7100455200(UnkCommanderAI_l455200* self, UnkAnimEvent_l455200* event) {
    UnkAnimationComponent* animation = self->owner->animation;
    if (!animation)
        return true;
    const u64 track = 0xf4061ff25a2163d6;
    const u64 kind = 0x4f924882b434779c;
    if (event->kind == 3) {
        if (event->track->hash != track)
            return true;
        CStrId name = *fn_7101177ae8(animation, kind);
        if (fn_71011782ec(animation, kind) || !Contains(name, g_unk_71726c9540))
            self->send_pending = true;
    } else if (event->kind == 1) {
        if (event->track->hash == kind) {
            self->send_pending = true;
            return true;
        }
        UnkEventIdHolder_l455200* holder = event->ref->holder;
        if (holder->id->hash == 0x344e6b8e8781639b || holder->id->hash == 0xb1908e0ac25fa340)
            self->vf_0x208(true, true);
        if (holder->id->hash == 0xcf6b801b800650c3) {
            base::reflection::UnkReflTypeIdArg mode;
            mode.id = 1;
            base::reflection::UnkReflTypeIdArg mask;
            mask.id = UnkAll_SpitSend().value;
            fn_7101176690(animation, 0x1926dfc8432a8bb9, track, &mode, &g_unk_710164b958, &g_unk_7172769740,
                          &mask);
        }
    }
    return true;
}
