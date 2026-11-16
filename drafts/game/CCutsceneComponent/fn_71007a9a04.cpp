// CCutsceneComponent's unit: copies a byte array into the entry named `id`: the component's own
// entry when `id` is the id table's key at 0x1f00, otherwise the first entry in the list at 0xd0 with
// that name. Names and layouts are placeholders.
#include "../../game_s7/UnkPropS7.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CRntVector;
using base::global::CStrId;
using unk_prop::Key;

struct UnkCutsceneEntry_l7a9a04 {
    void** vtable;
    CRntVector<u8> bytes;  // 0x8
    CStrId name;  // 0x30
    u8 pad_0x38[0x50];
};
static_assert(sizeof(UnkCutsceneEntry_l7a9a04) == 0x88, "layout");

struct UnkCutsceneComponent_l7a9a04 {
    u8 pad_0x0[0xd0];
    CRntVector<UnkCutsceneEntry_l7a9a04> entries;  // 0xd0
    u8 pad_0xf8[0x60];
    UnkCutsceneEntry_l7a9a04 own;  // 0x158
};
static_assert(__builtin_offsetof(UnkCutsceneComponent_l7a9a04, own) == 0x158, "layout");

void fn_71007a9a04(UnkCutsceneComponent_l7a9a04* self, const CStrId& id, const CRntVector<u8>& bytes) {
    if (id == Key(0x1f00)) {
        for (u32 i = 0; i < bytes.count; ++i)
            self->own.bytes.GrowAt(i) = bytes.data[i];
        return;
    }
    UnkCutsceneEntry_l7a9a04* entry = self->entries.data;
    UnkCutsceneEntry_l7a9a04* end = entry + self->entries.count;
    for (; entry != end; ++entry) {
        if (entry->name == id) {
            for (u32 i = 0; i < bytes.count; ++i)
                entry->bytes.GrowAt(i) = bytes.data[i];
            return;
        }
    }
}
