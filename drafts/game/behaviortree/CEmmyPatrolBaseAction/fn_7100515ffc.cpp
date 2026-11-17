// behaviortree::CEmmyPatrolBaseAction unit (tools/tu_map.py unit 803): the patrol point at an index of
// an array, growing the array up to it with zeroed points (vtable 0x7101a47d80): CRntVector::GrowAt written out.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "types.h"

extern void* g_unk_7101a47d80[];

// A point (two words) in a polymorphic wrapper whose vtable lives in the original.
struct UnkPatrolPoint_l515ffc {
    void** vtable;
    u32 x;
    u32 y;

    UnkPatrolPoint_l515ffc() : vtable(g_unk_7101a47d80 + 2), x(0), y(0) {}
    UnkPatrolPoint_l515ffc(UnkPatrolPoint_l515ffc&& other)
        : vtable(g_unk_7101a47d80 + 2), x(other.x), y(other.y) {}
    ~UnkPatrolPoint_l515ffc() { reinterpret_cast<void (*)(UnkPatrolPoint_l515ffc*)>(vtable[0])(this); }
};

UnkPatrolPoint_l515ffc* fn_7100515ffc(base::global::CRntVector<UnkPatrolPoint_l515ffc>* self, u32 index) {
    if (self->field_0x14 && self->count <= index) {
        // First to the next multiple of the step, then to exactly index + 1 points.
        u32 cap = self->capacity;
        if (cap <= index && self->step != 0) {
            u32 grown = self->GrownCapacity(index);
            if (cap < grown) {
                self->Reserve(grown);
                cap = grown;
            }
        }
        u32 n = index + 1;
        if (cap < n)
            self->Reserve(n);
        for (u32 i = self->count; i < n; ++i)
            new (&self->data[i]) UnkPatrolPoint_l515ffc();
        self->count = n;
    }
    return &self->data[index];
}
