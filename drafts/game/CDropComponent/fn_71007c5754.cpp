// The CDropComponent unit (tools/tu_map.py unit 1223): picks one of six drop kinds at random, each
// weighted by its +0x484, skipping kinds already at their limit unless `ignore_limits` is set; the
// power bomb kind (fifth) and the one after it only when the player owns power bombs. Counts the pick
// and returns it, or null. Names and layouts are placeholders.
#include <stdlib.h>

#include "base/global/CStrId.h"
#include "game/CInventoryComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkItemProperties.h"
#include "types.h"

struct UnkDropKind_l7c5754 {
    u8 pad_0x0[0x484];
    f32 weight;
};

struct UnkDropKinds_l7c5754 {
    UnkDropKind_l7c5754* kinds[6];
};

struct UnkDropLimits_l7c5754 {
    u32 max[6];
    u32 count[6];
};

static inline UnkQualifiedId& Qid_l7c5754(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

static inline f32 RandomRange(f32 min, f32 max) {
    f32 t = (rand() & 0x7fff) * (1.0 / 32767.0);
    return min + (max - min) * t;
}

static inline bool IsNearZero(f32 v) { return v + 1e-05f >= 0.0f && v - 1e-05f <= 0.0f; }

// Takes the kind's share off `left`; picks it (and counts it) when that reaches zero and the kind is
// not at its limit.
static inline bool TryPick(UnkDropKind_l7c5754* kind, f32& left, u32* count, const u32* max, bool ignore_limits) {
    f32 weight = kind->weight;
    if (!(weight > 0.0f))
        return false;
    left -= weight;
    if (!ignore_limits && *count >= *max)
        return false;
    if (left < 0.0f || IsNearZero(left)) {
        (*count)++;
        return true;
    }
    return false;
}

UnkDropKind_l7c5754* fn_71007c5754(UnkDropKinds_l7c5754* self, UnkInventory* inventory, UnkDropLimits_l7c5754* limits,
                                   bool ignore_limits) {
    f32 total = self->kinds[0]->weight + self->kinds[1]->weight + self->kinds[2]->weight + self->kinds[3]->weight +
                self->kinds[4]->weight + self->kinds[5]->weight;
    f32 left = RandomRange(0.0f, total);
    UnkDropKind_l7c5754** kinds = self->kinds;
    u32* max = limits->max;
    u32* count = limits->count;
    if (TryPick(kinds[0], left, &count[0], &max[0], ignore_limits))
        return kinds[0];
    if (TryPick(kinds[1], left, &count[1], &max[1], ignore_limits))
        return kinds[1];
    if (TryPick(kinds[2], left, &count[2], &max[2], ignore_limits))
        return kinds[2];
    if (TryPick(kinds[3], left, &count[3], &max[3], ignore_limits))
        return kinds[3];
    // The last two only with power bombs.
    if (!fn_71011428bc(inventory, Qid_l7c5754(g_unk_717275a818)))
        return nullptr;
    if (TryPick(kinds[4], left, &count[4], &max[4], ignore_limits))
        return kinds[4];
    if (TryPick(kinds[5], left, &count[5], &max[5], ignore_limits))
        return kinds[5];
    return nullptr;
}
