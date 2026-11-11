// CGoliathXBurstProjectionBombMovement's unit (tools/tu_map.py unit 840): its destructor
// (fn_710058a368): the two arrays of 128 effect models (+0x520, +0x920) are handed back to the
// model manager (fn_71011a91c4, created on first use), the count at +0x518 cleared, the three
// arrays at +0xd48, +0xd20 and +0x4b8 freed, then the base's destructor runs (fn_7100aa9f3c).
// Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/CBombMovement.h"
#include "game/UnkModelManager.h"
#include "types.h"

extern char g_unk_7101a4bd48[];  // the class's vtable
struct UnkPbMovementBase_ovb0015;
void fn_71011a91c4(UnkModelManager* self, void* handle, u32 id);

static inline UnkModelManager* ModelManager() {
    if (!g_unk_71726b0cd8) {
        UnkModelManager* m = static_cast<UnkModelManager*>(fn_71000839a0(0x1e8, 0x3f));
        fn_71011a870c(m);
        g_unk_71726b0cd8 = m;
    }
    return g_unk_71726b0cd8;
}

struct UnkGoliathBombMovement_b0119 {
    char* vtable;
    u8 pad_0x8[0x4b0];
    base::UnkVector<u64> field_0x4b8;
    u8 pad_0x4e0[0x38];
    u32 field_0x518;
    u8 pad_0x51c[0x4];
    void* models_a[128];  // 0x520
    void* models_b[128];  // 0x920
    base::UnkVector<u64> field_0xd20;
    base::UnkVector<u64> field_0xd48;
};
static_assert(__builtin_offsetof(UnkGoliathBombMovement_b0119, field_0xd48) == 0xd48,
              "UnkGoliathBombMovement_b0119 layout");

void fn_710058a368(UnkGoliathBombMovement_b0119* self) {
    self->vtable = g_unk_7101a4bd48 + 0x10;
    for (u32 i = 0; i < 128; ++i) {
        fn_71011a91c4(ModelManager(), self->models_a[i], 0);
        fn_71011a91c4(ModelManager(), self->models_b[i], 0);
    }
    self->field_0x518 = 0;
    self->field_0xd48.Free();
    self->field_0xd20.Free();
    self->field_0x4b8.Free();
    fn_7100aa9f3c(reinterpret_cast<UnkPbMovementBase_ovb0015*>(self));
}
