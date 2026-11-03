// CMaterialFXComponent unit (tools/tu_map.py, the name from reflection): the destructor: this class's
// vtable (g_unk_7101a833b0), the ids of the 50 effect slots from +0x218 (0x58 bytes each) released last
// to first, then the component base destructor (fn_71010c4194). Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "game/UnkComponent.h"
#include "types.h"

extern char g_unk_7101a833b0[];

struct UnkMaterialFXSlot {
    base::UnkStrIdMember id;
    u8 unk_0x8[0x50];
};

struct UnkMaterialFXComponent {
    char* vtable;
    u8 unk_0x8[0x210];
    UnkMaterialFXSlot slots[50];  // 0x218
};

static inline __attribute__((always_inline)) void Destroy(UnkMaterialFXComponent* self) {
    self->vtable = g_unk_7101a833b0 + 0x10;
    for (u64 i = 50; i != 0; --i) {
        using Id = base::UnkStrIdMember;
        self->slots[i - 1].id.~Id();
    }
    fn_71010c4194(reinterpret_cast<UnkBaseComponent*>(self));
}

void fn_710092737c(UnkMaterialFXComponent* self) {
    Destroy(self);
}

// The deleting form.
void fn_7100927420(UnkMaterialFXComponent* self) {
    Destroy(self);
    operator delete(self);
}
