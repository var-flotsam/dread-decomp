// Unit 1501 (CSamusMovement): fn_71009e7870 picks the animation id of the actor's
// CCharClassAllowFallendRepositionTrack record for a nearby edge: the first one when the edge on the
// one side is nearer than 42 units, the second one for the other side, the empty id otherwise (and when
// both edges are nearer than 84). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU363.h"
#include "types.h"

using base::global::CStrId;

struct UnkMove_b152;
struct UnkIdHolder_b152;

struct UnkMsg_b152 {
    s16 id;
    u8 field_0x2;
    u8 pad_0x3[9];
    f32 field_0xc;
};

// The movement as this function sees it.
struct UnkMovement_l9e7870 {
    void** vtable;  // 0x0
    u8 pad_0x8[0x28];
    UnkActor* actor;  // 0x30
};

// The CCharClassAllowFallendRepositionTrack record: two animation ids.
struct UnkFallenTrack_l9e7870 {
    u8 pad_0x0[0x18];
    CStrId first;  // 0x18
    CStrId second;  // 0x20
};

// Calls virtual slot off of self.
template <class R, class... A>
static inline R CallVf(void* self, u32 off, A... args) {
    return reinterpret_cast<R (*)(void*, A...)>((*reinterpret_cast<void***>(self))[off / 8])(self, args...);
}

static inline bool IsSet(const UnkMsg_b152& msg) {
    return msg.id != -1 && msg.field_0x2 != 0xff;
}

static inline bool IsEmpty(const CStrId& id) {
    return id.data == &g_unk_7101d47108->empty;
}

UnkIdHolder_b152* fn_71009e7870(UnkMove_b152* move) {
    UnkMovement_l9e7870* self = reinterpret_cast<UnkMovement_l9e7870*>(move);
    bool frontBlocked;
    bool backBlocked;
    if (!self->actor->field_0x220)
        return reinterpret_cast<UnkIdHolder_b152*>(&g_unk_7101d470e0);
    UnkTrackQuery query = {fn_7100bf1f28(), nullptr, -1.0f};
    fn_710117b08c(static_cast<UnkAnimationComponent*>(self->actor->field_0x220), &query);
    if (!query.result)
        return reinterpret_cast<UnkIdHolder_b152*>(&g_unk_7101d470e0);

    UnkMsg_b152 front;
    fn_71002878dc(&front);
    UnkMsg_b152 back;
    fn_71002878dc(&back);
    frontBlocked = false;
    backBlocked = false;
    CallVf<void>(self, 0x2e8, &front, &frontBlocked, &back, &backBlocked);

    // Which way to turn: positive for the back edge, negative for the front one, zero for neither.
    f32 side = 0.0f;
    if (IsSet(back) && IsSet(front) && back.field_0xc < 84.0f && front.field_0xc < 84.0f) {
        // Both edges are near: no turn.
    } else if (IsSet(back) && back.field_0xc < 42.0f) {
        if (!backBlocked)
            side = 42.0f - back.field_0xc;
    } else if (IsSet(front) && front.field_0xc < 42.0f) {
        if (!frontBlocked)
            side = -(42.0f - front.field_0xc);
    }

    UnkFallenTrack_l9e7870* track = reinterpret_cast<UnkFallenTrack_l9e7870*>(query.result);
    const CStrId* result = &g_unk_7101d470e0;
    if (side > 0.0f && !IsEmpty(track->first))
        result = &track->first;
    else if (side < 0.0f && !IsEmpty(track->second))
        result = &track->second;
    fn_7100287954(&back);
    fn_7100287954(&front);
    return reinterpret_cast<UnkIdHolder_b152*>(const_cast<CStrId*>(result));
}
