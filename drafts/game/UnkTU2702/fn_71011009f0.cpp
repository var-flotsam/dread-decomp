// CAIComponent unit (tools/tu_map.py unit 2702): decides whether a hit shows its bump effect, from the
// component's default and the owner's playing tracks (attack, modify-damage, ignore- and force-bump-FX
// tracks, each with a list of ids), and when it does picks the effect's path. Names and layouts are
// placeholders.
#include <algorithm>

#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CCollisionComponent.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

__attribute__((visibility("hidden"))) extern CFilePathStrId g_unk_7172a06350;
__attribute__((visibility("hidden"))) extern CFilePathStrId g_unk_7172a06358;

// The record of a track that lists ids (the modify-damage and bump-FX tracks).
struct UnkTrackIds_l1009f0 {
    u8 pad_0x0[0x20];
    base::global::CRntVector<CStrId> ids;  // 0x20
};
static_assert(__builtin_offsetof(UnkTrackIds_l1009f0, ids) == 0x20, "layout");

struct UnkHitSource_l1009f0 {
    u8 pad_0x0[0x20];
    CStrId name;  // 0x20
};

struct UnkHit_l1009f0 {
    u8 pad_0x0[0x98];
    UnkHitSource_l1009f0* source;  // 0x98
    u8 pad_0xa0[0xb7 - 0xa0];
    u8 flags;  // 0xb7
};
static_assert(__builtin_offsetof(UnkHit_l1009f0, flags) == 0xb7, "layout");

struct UnkBumpOwner_l1009f0 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x2a0 - 0x228];
    UnkCollisionContainer* collision;  // 0x2a0
};
static_assert(__builtin_offsetof(UnkBumpOwner_l1009f0, collision) == 0x2a0, "layout");

struct UnkBumpData_l1009f0 {
    u8 pad_0x0[0x4d6];
    u8 bump;  // 0x4d6
};

struct UnkBumpAI_l1009f0 {
    u8 pad_0x0[0x30];
    UnkBumpOwner_l1009f0* owner;  // 0x30
    u8 pad_0x38[0x1610 - 0x38];
    UnkBumpData_l1009f0* data;  // 0x1610
};
static_assert(__builtin_offsetof(UnkBumpAI_l1009f0, data) == 0x1610, "layout");

// Whether the track's list holds `id` (never the empty id).
static inline bool Lists_l1009f0(const UnkTrackIds_l1009f0* track, const CStrId& id) {
    if (&g_unk_7101d47108->empty == id.data)
        return false;
    for (u32 i = 0; i < track->ids.count; ++i)
        if (track->ids.data[i] == id)
            return true;
    return false;
}

// The same test written over the list's elements (the modify-damage track's).
static inline bool Holds_l1009f0(const UnkTrackIds_l1009f0* track, const CStrId& id) {
    if (id.data == &g_unk_7101d47108->empty || track->ids.count == 0)
        return false;
    const CStrId* begin = track->ids.data;
    const CStrId* end = begin + track->ids.count;
    return std::find(begin, end, id) != end;
}

// The record the query found, as a track that lists ids.
static inline const UnkTrackIds_l1009f0* Ids_l1009f0(const UnkTrackQuery& query) {
    return reinterpret_cast<const UnkTrackIds_l1009f0*>(query.result);
}

bool fn_71011009f0(UnkBumpAI_l1009f0* self, UnkHit_l1009f0* hit, CFilePathStrId* path) {
    UnkAnimationComponent* animation = self->owner->animation;
    UnkCollisionContainer* collision = self->owner->collision;
    UnkHitSource_l1009f0* source = hit->source;
    const CStrId& name = source ? source->name : g_unk_7101d470e0;
    bool bump = self->data->bump != 0;
    if (animation && collision && fn_710117b348(animation, fn_7100ceda9c())) {
        UnkTrackQuery query = {fn_7100d217d8(), nullptr, -1.0f};
        fn_710117b08c(animation, &query);
        const UnkTrackIds_l1009f0* modify = Ids_l1009f0(query);
        if (modify && (modify->ids.count == 0 || Holds_l1009f0(modify, *fn_7100864018(collision, source))))
            bump = false;
    }
    if (animation) {
        UnkTrackQuery ignore = {fn_7100c26e68(), nullptr, -1.0f};
        if (fn_710117b08c(animation, &ignore)) {
            if (const UnkTrackIds_l1009f0* track = Ids_l1009f0(ignore))
                bump &= track->ids.count != 0;
        }
        UnkTrackQuery force = {fn_7100c1d4b8(), nullptr, -1.0f};
        bool force_found = fn_710117b08c(animation, &force);
        const UnkTrackIds_l1009f0* forced = Ids_l1009f0(force);
        if (force_found && forced)
            bump |= forced->ids.count == 0;
        if (ignore.result && Lists_l1009f0(Ids_l1009f0(ignore), name))
            bump = false;
        if (forced && Lists_l1009f0(forced, name))
            bump = true;
    }
    if (!bump)
        return false;
    *path = g_unk_7172a06350;
    if (hit->flags & 4)
        *path = g_unk_7172a06358;
    return true;
}
