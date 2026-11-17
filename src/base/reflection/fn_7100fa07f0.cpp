// build-flags: no-strict-aliasing
// base::reflection unit (tools/tu_map.py unit 2609): the "move-assign at index" slot of the reflected
// CRntVector<std::unique_ptr<CSubareaCharclassGroup>>: grows the array up to the index when it may, then
// moves the owned entry in, destroying the one it replaces. Names and layouts are placeholders.
#include <memory>

#include "base/global/CRntVector.h"
#include "game/UnkTU2577.h"
#include "types.h"

struct UnkNamedIds;

// Deletes a group: its destructor (fn_7100ef1e1c), then the storage.
struct UnkGroupDelete_fa07f0 {
    void operator()(UnkNamedIds* p) const {
        fn_7100ef1e1c(p);
        operator delete(p);
    }
};

using UnkOwnedGroup_fa07f0 = std::unique_ptr<UnkNamedIds, UnkGroupDelete_fa07f0>;

void fn_7100fa07f0(base::global::CRntVector<UnkOwnedGroup_fa07f0>* self, const u32* index,
                   UnkOwnedGroup_fa07f0* value) {
    u32 i = *index;
    if (self->field_0x14 && self->count <= i) {
        // First to the next multiple of the step, then to exactly i + 1 entries.
        u32 cap = self->capacity;
        if (cap <= i && self->step != 0) {
            u32 grown = (i / self->step + 1) * self->step;
            if (cap < grown) {
                self->Reserve(grown);
                cap = grown;
            }
        }
        u32 n = i + 1;
        if (cap < n)
            self->Reserve(n);
        for (u32 j = self->count; j < n; ++j)
            new (&self->data[j]) UnkOwnedGroup_fa07f0();
        self->count = n;
    }
    self->data[i] = static_cast<UnkOwnedGroup_fa07f0&&>(*value);
}
