// Dropped-item manager (unit 2562, hint CLogicDroppedItemManager::CTunableDroppableBillboard): points the
// manager's path resources at the tunable's names, each under its prefix, skipping empty names.
// Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkPathIdsFromText.h"
#include "base/UnkStrIdPathPair.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/CLogicDroppedItemManager/CTunableDroppableBillboard.h"
#include "game/UnkTU2562.h"
#include "types.h"

using base::global::CRntString;

struct UnkDroppableTunable_e21b8c {
    u8 pad_0x0[0x98];
    CRntString names[10];  // 0x98
};
static_assert(__builtin_offsetof(UnkDroppableTunable_e21b8c, names) + 8 * sizeof(CRntString) == 0x218, "names");

static inline UnkDroppableTunable_e21b8c& Tunable() {
    return reinterpret_cast<UnkDroppableTunable_e21b8c&>(g_unk_71727c10c8);
}

struct UnkDropManager_e21b8c {
    u8 pad_0x0[0x7830];
    UnkStrIdPathResource path_0x7830;
    u8 pad_0x7848[0xa020 - 0x7848];
    UnkStrIdPathResource paths[7];  // 0xa020
};
static_assert(__builtin_offsetof(UnkDropManager_e21b8c, paths) == 0xa020, "paths");

static inline CRntString operator+(const CRntString& a, const CRntString& b) { return fn_710007eee0(&a, &b); }

static inline bool IsEmpty(const CRntString& text) { return !text.data || !text.data[0]; }

// Points a path resource at the ids parsed from a path.
static inline void SetPath(UnkStrIdPathResource* resource, const CRntString& path) {
    fn_71000267f8(resource, UnkPathIdsFromText(path).ids);
}

void fn_7100e21b8c(UnkDropManager_e21b8c* self) {
    UnkDroppableTunable_e21b8c& tunable = Tunable();
    if (!IsEmpty(tunable.names[0]))
        SetPath(&self->paths[0], g_unk_71727c13c8[0] + tunable.names[0]);
    if (!IsEmpty(tunable.names[1]))
        SetPath(&self->paths[1], g_unk_71727c13c8[0] + tunable.names[1]);
    if (!IsEmpty(tunable.names[2]))
        SetPath(&self->paths[2], g_unk_71727c13c8[0] + tunable.names[2]);
    if (!IsEmpty(tunable.names[4]))
        SetPath(&self->paths[3], g_unk_71727c13c8[1] + tunable.names[4]);
    if (!IsEmpty(tunable.names[5]))
        SetPath(&self->paths[4], g_unk_71727c13c8[1] + tunable.names[5]);
    if (!IsEmpty(tunable.names[6]))
        SetPath(&self->paths[5], g_unk_71727c13c8[1] + tunable.names[6]);
    if (!IsEmpty(tunable.names[7]))
        SetPath(&self->paths[6], g_unk_71727c13c8[1] + tunable.names[7]);
    if (!IsEmpty(tunable.names[8]))
        SetPath(&self->path_0x7830, g_unk_71727c13c8[1] + tunable.names[8]);
}
