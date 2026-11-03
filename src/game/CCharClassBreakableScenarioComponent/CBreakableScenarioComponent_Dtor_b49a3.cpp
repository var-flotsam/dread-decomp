// CBreakableScenarioComponent's destructors (tools/tu_map.py unit 1165): the vtable, the link at +0x70
// taken out of its list (fn_71002aeb9c) and the words at +0x78 and +0x80 cleared, the array of 0x40-byte
// polymorphic entries at +0x48 (each destroyed through its vtable, then the array freed), then the
// component destructor fn_71010c4194; fn_7100751708 also frees the object. Names and layouts are
// placeholders.
#include "base/UnkVector.h"
#include "game/UnkComponent.h"
#include "types.h"

extern void* g_unk_7101a6e128[];

struct UnkLinkList;
void fn_71002aeb9c(UnkLinkList* list, UnkLinkList** slot);

struct UnkBreakableEntry_b49a3 {
    virtual ~UnkBreakableEntry_b49a3();
    u8 pad_0x8[0x38];
};

struct UnkBreakableScenarioComponent_b49a3 : UnkBaseComponent {
    u8 pad_0x40[0x8];
    base::UnkVector<UnkBreakableEntry_b49a3> entries;  // 0x48
    UnkLinkList* link;  // 0x70
    u64 field_0x78;
    u64 field_0x80;
};

__attribute__((always_inline)) static inline void Destroy_b49a3(UnkBreakableScenarioComponent_b49a3* self) {
    self->vtable = g_unk_7101a6e128 + 2;
    UnkLinkList** slot = &self->link;
    if (*slot) {
        fn_71002aeb9c(*slot, slot);
        *slot = nullptr;
    }
    self->field_0x78 = 0;
    self->field_0x80 = 0;
    self->entries.Free();
    fn_71010c4194(self);
}

void fn_710075163c(UnkBreakableScenarioComponent_b49a3* self) {
    Destroy_b49a3(self);
}

void fn_7100751708(UnkBreakableScenarioComponent_b49a3* self) {
    Destroy_b49a3(self);
    operator delete(self);
}
