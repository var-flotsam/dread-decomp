// CObsydomithonAttack's unit (tools/tu_map.py): how long the attack still has to run, or -1 when the owner
// has no character class, track or current track name. Outside a CCharClassAttackTrack it adds up two
// action lengths and the tunable delays; in the two named states it uses the attack's own time (+0x58) or
// what is left of the delay on the track. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CAnimationComponent.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU1134.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CStrId;

extern UnkTunable g_unk_71726f8988;
extern f32 g_unk_7101a57428;
__attribute__((visibility("hidden"))) extern CStrId g_unk_71726f8e00;
__attribute__((visibility("hidden"))) extern CStrId g_unk_71726f8e08;

UnkTrack* fn_7101177b68(UnkAnimationComponent* self, u64 key);

// The Obsydomithon tunable's value this file reads.
struct UnkObsydomithonTunable_l629368 {
    u8 pad_0x0[0x2c];
    f32 field_0x2c;
};

// What fn_7101177d2c finds: the track's name at +0x38.
struct UnkTrackEntry_l629368 {
    u8 pad_0x0[0x38];
    CStrId name;
};

// What fn_7101176d34 finds: an action's length at +0x28.
struct UnkActionEntry_l629368 {
    u8 pad_0x0[0x28];
    f32 length;
};

struct UnkObsydomithonAttack_l629368 {
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
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual const CStrId& vf_0xb0();
    UnkActor* owner;  // 0x8
    u8 pad_0x10[0x18];
    void* field_0x28;
    u8 pad_0x30[0x28];
    f32 time;  // 0x58
};

static const u64 kTrackKind = 0xf4061ff25a2163d6;
static const f32 kEpsilon = 0.001f;

static inline const CStrId& CurrentTrackName(UnkAnimationComponent* animation) {
    auto* entry = static_cast<UnkTrackEntry_l629368*>(fn_7101177d2c(animation, kTrackKind));
    return entry ? entry->name : g_unk_7101d470e0;
}

static inline UnkActionEntry_l629368* FindAction(UnkAnimationComponent* animation, const CStrId& id) {
    return static_cast<UnkActionEntry_l629368*>(fn_7101176d34(reinterpret_cast<UnkStruct_7101176ea8*>(animation),
                                                              reinterpret_cast<void*>(id.data->Key())));
}

f32 fn_7100629368(UnkObsydomithonAttack_l629368* self) {
    UnkAnimationComponent* animation = nullptr;
    const CStrId* current = &g_unk_7101d470e0;
    bool found = false;
    if (self->owner) {
        animation = static_cast<UnkAnimationComponent*>(self->owner->field_0x220);
        if (animation) {
            current = &CurrentTrackName(animation);
            found = true;
        }
    }
    CStrId name = *current;

    if (!found)
        return -1.0f;
    UnkTrack* track = fn_7101177b68(animation, kTrackKind);
    bool unnamed = name.data == &g_unk_7101d47108->empty;
    if (!track || unnamed || !self->field_0x28)
        return -1.0f;

    if (!fn_710117b348(animation, fn_7100ceda9c())) {
        f32 result = 0.0f;
        const CStrId& id = self->vf_0xb0();
        if (id.data != &g_unk_7101d47108->empty) {
            if (UnkActionEntry_l629368* action = FindAction(animation, id))
                result += action->length;
        }
        if (UnkActionEntry_l629368* action = FindAction(animation, g_unk_71726f8e00))
            result += action->length;
        f32 delay = reinterpret_cast<UnkObsydomithonTunable_l629368&>(g_unk_71726f8988).field_0x2c;
        if (delay > kEpsilon)
            result += delay;
        f32 extra = g_unk_7101a57428;
        if (extra > kEpsilon)
            result = extra + result;
        return result;
    }
    if (fn_710117ae38(animation, 0x7640910a55220b1b, false)) {
        f32 result = 0.0f;
        f32 time = self->time;
        if (time > kEpsilon)
            result += time;
        f32 extra = g_unk_7101a57428;
        if (extra > kEpsilon)
            result = extra + result;
        return result;
    }
    if (fn_710117ae38(animation, g_unk_71726f8e08.data->Key(), false)) {
        f32 extra = g_unk_7101a57428;
        if (extra > kEpsilon)
            return extra - fn_710070d890(track);
        return 0.0f;
    }
    return -1.0f;
}
