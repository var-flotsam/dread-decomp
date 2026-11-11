// Unit 2177 (tools/tu_map.py, no name hint): resizing a base::UnkVector of 0x188-byte objects (the
// ones src/game/CCharClassEventScenarioComponent/fn_71007e44b8.cpp creates, constructed by
// fn_7100d36624): a larger capacity moves each object over (fn_7100d36688) and destroys the old one
// (its vf_0x8), then new objects are constructed up to the new count. Names and layouts are
// placeholders.
#include "base/UnkVector.h"
#include "game/logic/collision/CCollider.h"
#include "types.h"

// As src/game/CCharClassEventScenarioComponent/fn_71007e44b8.cpp declares it.
struct UnkObj_71007e44b8;

struct UnkObj188_b43a3 {
    virtual void vf_0x0();
    virtual void vf_0x8();  // destroys
    u8 pad_0x8[0x180];
};
static_assert(sizeof(UnkObj188_b43a3) == 0x188, "UnkObj188_b43a3 layout");

void fn_7100d36688(UnkObj188_b43a3* self, UnkObj188_b43a3* other);  // move constructor

void fn_7100c77bcc(base::UnkVector<UnkObj188_b43a3>* self, u32 n) {
    if (self->capacity < n) {
        UnkObj188_b43a3* p =
            static_cast<UnkObj188_b43a3*>(self->allocator->vf_0x40((u64)n * sizeof(UnkObj188_b43a3), 8, self->flags));
        if (UnkObj188_b43a3* old = self->data) {
            UnkObj188_b43a3* dst = p;
            for (u32 i = 0; i < self->count; ++i) {
                fn_7100d36688(dst, old);
                old->vf_0x8();
                ++dst;
                ++old;
            }
            self->allocator->vf_0x48(self->data);
        }
        self->data = p;
        self->capacity = n;
    }
    for (u32 i = self->count; i < n; ++i)
        fn_7100d36624(reinterpret_cast<UnkObj_71007e44b8*>(&self->data[i]));
    self->count = n;
}
