// CShotManager::CTunableShotManager's unit (tools/tu_map.py): resizing a base::UnkVector of 0x290-byte
// objects: a larger capacity moves each object over (fn_7100eddc68) and destroys the old one (its
// vf_0x0), then new objects are constructed (fn_7100ecbddc) up to the new count. The shape of
// fn_7100c77bcc (src/game/UnkUnit2177/UnkObj188Vector_b43.cpp). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

struct UnkObj290_b43a3 {
    virtual void vf_0x0();  // destroys
    u8 pad_0x8[0x288];
};
static_assert(sizeof(UnkObj290_b43a3) == 0x290, "UnkObj290_b43a3 layout");

void fn_7100eddc68(UnkObj290_b43a3* self, UnkObj290_b43a3* other);  // move constructor
void fn_7100ecbddc(UnkObj290_b43a3* self);  // constructor

void fn_7100ede9e4(base::UnkVector<UnkObj290_b43a3>* self, u32 n) {
    if (self->capacity < n) {
        UnkObj290_b43a3* p =
            static_cast<UnkObj290_b43a3*>(self->allocator->vf_0x40((u64)n * sizeof(UnkObj290_b43a3), 8, self->flags));
        if (UnkObj290_b43a3* old = self->data) {
            UnkObj290_b43a3* dst = p;
            for (u32 i = 0; i < self->count; ++i) {
                fn_7100eddc68(dst, old);
                old->vf_0x0();
                ++dst;
                ++old;
            }
            self->allocator->vf_0x48(self->data);
        }
        self->data = p;
        self->capacity = n;
    }
    for (u32 i = self->count; i < n; ++i)
        fn_7100ecbddc(&self->data[i]);
    self->count = n;
}
