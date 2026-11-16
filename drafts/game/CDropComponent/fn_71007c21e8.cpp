// The CDropComponent unit (tools/tu_map.py unit 1223): drops a part's items, one try per item, each
// named from the optional list in turn, until the list runs out. Names and layouts are placeholders.
#include <stdlib.h>
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CDropComponent.h"
#include "game/CHydrogigaAIComponent.h"
#include "game/UnkActor.h"
#include "game/UnkTU2562.h"
#include "types.h"

struct UnkDropTunable_7c21e8 {
    void** vtable;
    u8 pad_0x8[0x21];
    bool disabled;
    u8 pad_0x2a[0x6];
    f32 field_0x30;
};

struct UnkDropAI_7c21e8 {
    u8 pad_0x0[0x14698];
    bool field_0x14698;
};

struct UnkDropTarget_7c21e8 {
    u8 pad_0x0[0x54];
    f32 field_0x54;
};

struct UnkDropComp_7c21e8 {
    u8 pad_0x0[0x20];
    bool enabled;
    u8 pad_0x21[0xf];
    UnkActor* owner;
};

struct UnkDropPart_7c21e8 {
    s32 total;
    s32 count;
    u8 pad_0x8[0x8];
    f32 chance;
};

struct UnkDropCounts_7c21e8 {
    u32 field_0x0[6];
    s32 dropped[6];
};

struct UnkDropArea_7c21e8 {
    u8 field_0x0[0x20];
};

struct UnkDropManager_7c21e8 {
    u8 pad_0x0[0x7848];
    u8 field_0x7848;
};

void fn_71007c2928(UnkActor* owner, UnkDropPart_7c21e8* part, UnkDropCounts_7c21e8* counts);
void fn_71007c2ac8(UnkActor* owner, UnkDropPart_7c21e8* part, UnkDropArea_7c21e8* area);
void* fn_71007c34c8(UnkActor* owner, u8* manager, UnkDropPart_7c21e8* part, base::global::CStrId* name,
                    UnkDropArea_7c21e8* area, UnkDropCounts_7c21e8* counts);

static inline UnkDropManager_7c21e8* DropManager() {
    void* instance = g_unk_71726eb3e8;
    if (!instance) {
        instance = fn_71000839a0(0xa0f8, 0x3f);
        fn_7100e211cc(instance);
        g_unk_71726eb3e8 = instance;
    }
    return static_cast<UnkDropManager_7c21e8*>(instance);
}

static inline f32 RandomRange(f32 min, f32 max) {
    return (f32)((rand() & 0x7fff) * (1.0 / 32767.0)) * (max - min) + min;
}

// Drops one item, named from the list's next entry if there is a list; false once the list is used up.
static inline bool DropOne(UnkDropComp_7c21e8* self, UnkDropPart_7c21e8* part,
                           base::global::CRntVector<base::global::CStrId>* names, u32& next,
                           UnkDropArea_7c21e8* area, UnkDropCounts_7c21e8* counts) {
    base::global::CStrId name(g_unk_7101d470e0);
    if (names && names->count)
        name = names->data[next];
    bool dropped = fn_71007c34c8(self->owner, &DropManager()->field_0x7848, part, &name, area, counts);
    if (names && dropped && names->count) {
        if (++next >= names->count)
            return false;
    }
    return true;
}

void fn_71007c21e8(char* self_, char* part_, void* extra_) {
    UnkDropComp_7c21e8* self = reinterpret_cast<UnkDropComp_7c21e8*>(self_);
    UnkDropPart_7c21e8* part = reinterpret_cast<UnkDropPart_7c21e8*>(part_);
    auto* names = static_cast<base::global::CRntVector<base::global::CStrId>*>(extra_);
    const UnkDropTunable_7c21e8& tunable = reinterpret_cast<const UnkDropTunable_7c21e8&>(g_unk_717271b668);

    if (tunable.disabled)
        return;
    UnkDropAI_7c21e8* ai = static_cast<UnkDropAI_7c21e8*>(self->owner->field_0x240);
    if (ai && ai->field_0x14698)
        return;
    if (!self->enabled)
        return;

    UnkDropCounts_7c21e8 counts = {};
    fn_71007c2928(self->owner, part, &counts);
    UnkDropArea_7c21e8 area;
    fn_71007c2ac8(self->owner, part, &area);

    u32 next = 0;
    for (s32 i = 0; i < part->count; i++) {
        UnkActor* target = fn_7100d7c990(self->owner);
        UnkDropTarget_7c21e8* node = target ? static_cast<UnkDropTarget_7c21e8*>(target->field_0x1b8) : nullptr;
        if (!(node && node->field_0x54 <= tunable.field_0x30)) {
            s32 left = part->total - part->count;
            for (s32 k = 0; k < 6; k++)
                left -= counts.dropped[k];
            if (i + left < 0 && part->chance >= RandomRange(0.0f, 1.0f))
                continue;
        }

        if (!DropOne(self, part, names, next, &area, &counts))
            return;
    }
}
