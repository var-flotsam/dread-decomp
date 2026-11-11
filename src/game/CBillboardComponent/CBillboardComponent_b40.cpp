// CBillboardComponent unit (tools/tu_map.py unit 1154): the destructor body (fn_7100726e9c). The
// vtable, the array of 0x148-byte entries at +0x50 (delete[]: each by fn_7100745b24), the two entries
// at +0x48 and +0x4c in the billboard manager g_unk_71726da048 (released with fn_7100e1c9d8 unless -1,
// then both reset to -1), the two render items at +0x120 and +0x128 (fn_71002199d8), the member at
// +0xc0 (fn_7100216934), then the component base (fn_71010c4194). Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkVector.h"
#include "game/UnkComponent.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU2560.h"
#include "types.h"

extern void* g_unk_7101a6d218[];
extern void* g_unk_71726da048;
void fn_7100e1c9d8(void* manager, u32 id);

struct UnkEntry_bbd {
    u8 data[0x148];
    ~UnkEntry_bbd() asm("fn_7100745b24");
};

struct UnkBillboard_bbd : UnkBaseComponent {
    u32 field_0x40;
    u32 field_0x44;
    u32 id[2];  // 0x48
    UnkEntry_bbd* field_0x50;
    u8 pad_0x58[0x68];
    u8 field_0xc0[0x60];
    UnkItemBase_71011a91c4* item[2];  // 0x120
};

static inline __attribute__((always_inline)) void Release_bbd(const u32* id) {
    if (*id == 0xffffffff)
        return;
    void* manager = g_unk_71726da048;
    if (!manager) {
        manager = fn_71000839a0(0xe0, 0x3f);
        fn_7100e1bca4(manager);
        g_unk_71726da048 = manager;
    }
    fn_7100e1c9d8(manager, *id);
}

void fn_7100726e9c(UnkBillboard_bbd* self) {
    self->vtable = g_unk_7101a6d218 + 2;
    if (self->field_0x50) {
        delete[] self->field_0x50;
        self->field_0x50 = nullptr;
    }
    Release_bbd(&self->id[0]);
    Release_bbd(&self->id[1]);
    *reinterpret_cast<u64*>(self->id) = 0xffffffffffffffffull;
    for (u32 i = 0; i < 2; ++i) {
        if (self->item[i]) {
            fn_71002199d8(g_unk_7101d47300, self->item[i], 0);
            self->item[i] = nullptr;
        }
    }
    fn_7100216934(reinterpret_cast<char*>(self->field_0xc0));
    fn_71010c4194(self);
}
