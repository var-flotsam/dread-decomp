// CHydrogigaAIComponent unit: fn_71005c0e74 handles a message when the owner (+0x30) has both its parts at
// +0x228 and +0x2a0: it asks fn_71005b9ad8 for an object of the part at +0x149a0 and passes it, the owner and
// the key g_unk_710164a0d0 to fn_7100709f20 with 100 when the message id is g_unk_71726eb450, else 60.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CTentacle.h"
#include "game/UnkTU883.h"
#include "types.h"

struct UnkHydrogigaKey_b62a3;
extern UnkHydrogigaKey_b62a3 g_unk_710164a0d0 __attribute__((visibility("hidden")));

struct UnkHydrogigaOwner_b62a3 {
    u8 pad_0x0[0x228];
    void* field_0x228;
    u8 pad_0x230[0x70];
    void* field_0x2a0;
};

struct UnkHydrogigaTarget_b62a3;
UnkHydrogigaTarget_b62a3* fn_71005b9ad8(void* part, const u64* key);  // the entry with the id at key

struct UnkHydrogigaSpawnAI_b62a3 {
    u8 pad_0x0[0x30];
    UnkHydrogigaOwner_b62a3* owner;
    u8 pad_0x38[0x149a0 - 0x38];
    void* field_0x149a0;
};

void fn_71005c0e74(UnkHydrogigaSpawnAI_b62a3* self, base::global::CStrId* msg) {
    UnkHydrogigaOwner_b62a3* owner = self->owner;
    void* a = owner->field_0x228;
    void* b = owner->field_0x2a0;
    if (!a || !b)
        return;
    UnkHydrogigaTarget_b62a3* target = fn_71005b9ad8(self->field_0x149a0, reinterpret_cast<const u64*>(msg));
    if (!target)
        return;
    f32 value = msg->data == g_unk_71726eb450.data ? 100.0f : 60.0f;
    fn_7100709f20(target, self->owner, &g_unk_710164a0d0, value);
}
