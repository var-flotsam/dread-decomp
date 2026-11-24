// CDemolitionBlockComponent unit (tools/tu_map.py unit 1211): fn_71007b0e04 is the array of callback sets'
// GrowAt: the set at an index, growing the array up to it (moving the sets with fn_71007b1928) and
// default-constructing the new ones when it may grow and the index is past the end. Names and layouts are
// placeholders.
#include "base/UnkErasedVector.h"
#include "base/UnkRefHandle.h"
#include "base/global/CRntVector.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/CDemolitionBlockComponent.h"
#include "game/UnkTU341.h"
#include "types.h"

struct UnkDemolitionSetBase_l7b0e04 {
    char* vtable;
    UnkOwnedErasedVector callbacks;
    u32 field_0x30;

    UnkDemolitionSetBase_l7b0e04() : vtable(reinterpret_cast<char*>(g_unk_7101a20998) + 0x10), field_0x30(0) {}
};

struct UnkDemolitionSet_l7b0e04 : UnkDemolitionSetBase_l7b0e04 {
    base::UnkRefHandle owner;

    UnkDemolitionSet_l7b0e04() {
        vtable = reinterpret_cast<char*>(g_unk_7101a33fa0) + 0x10;
        owner.ptr = nullptr;
    }

    // The virtual destructor (slot 0).
    void Destroy() { reinterpret_cast<void (*)(UnkDemolitionSet_l7b0e04*)>(*reinterpret_cast<void**>(vtable))(this); }
};
static_assert(sizeof(UnkDemolitionSet_l7b0e04) == 0x40, "set size");

struct UnkDemolitionSets_l7b0e04 {
    UnkDemolitionSet_l7b0e04* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::IAllocator* allocator;

    void Reallocate(u32 n) {
        auto* p = static_cast<UnkDemolitionSet_l7b0e04*>(allocator->vf_0x40((u64)n * sizeof(UnkDemolitionSet_l7b0e04), 8, flags));
        if (data) {
            UnkDemolitionSet_l7b0e04* src = data;
            UnkDemolitionSet_l7b0e04* dst = p;
            for (u32 i = 0; i < count; ++i) {
                fn_71007b1928(dst, src);
                (src++)->Destroy();
                ++dst;
            }
            allocator->vf_0x48(data);
        }
        data = p;
        capacity = n;
    }
};

UnkDemolitionSet_l7b0e04* fn_71007b0e04(UnkDemolitionSets_l7b0e04* self, u32 index) {
    if (self->field_0x14 && self->count <= index) {
        // First to the next multiple of the step, then to exactly index + 1 sets.
        u32 cap = self->capacity;
        if (cap <= index && self->step != 0) {
            u32 grown = (index / self->step + 1) * self->step;
            if (cap < grown) {
                self->Reallocate(grown);
                cap = grown;
            }
        }
        u32 n = index + 1;
        if (cap < n)
            self->Reallocate(n);
        for (u32 i = self->count; i < n; ++i)
            new (&self->data[i]) UnkDemolitionSet_l7b0e04();
        self->count = n;
    }
    return &self->data[index];
}
