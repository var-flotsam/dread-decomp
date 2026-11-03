// CAreaFXComponent unit (tools/tu_map.py unit 1263). Its destructors: the vtable, the render object at +0xe70 taken off the render manager
// (fn_71002199d8, layer 0) and cleared, the word at +0xcd8 cleared, the file-path id at +0xcc8 reset to
// the pool's empty id (released when its count drops to zero), then the component destructor
// fn_71010c4194; fn_7100802d44 also frees the object. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "game/UnkComponent.h"
#include "game/UnkRenderManager.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

extern void* g_unk_7101a746f8[];

// A render object: its item for the render manager follows the vtable.
struct UnkFxRenderObject_b49a3 {
    void** vtable;
    u8 item[0x8];
};

struct UnkAreaFXComponent_b49a3 {
    void** vtable;
    u8 pad_0x8[0xcc0];
    UnkStringData* path;  // 0xcc8, a CFilePathStrId
    u8 pad_0xcd0[0x8];
    u32 field_0xcd8;
    u8 pad_0xcdc[0x194];
    UnkFxRenderObject_b49a3* item;  // 0xe70
};

__attribute__((always_inline)) static inline void Destroy_b49a3(UnkAreaFXComponent_b49a3* self) {
    self->vtable = g_unk_7101a746f8 + 2;
    if (UnkFxRenderObject_b49a3* item = self->item) {
        fn_71002199d8(g_unk_7101d47300, reinterpret_cast<UnkItemBase_71011a91c4*>(item->item), 0);
        self->item = nullptr;
    }
    self->field_0xcd8 = 0;
    if (UnkStringPool* pool = g_unk_7101d47200) {
        UnkStringData** path = &self->path;
        if (*path && *path != &pool->empty) {
            --(*path)->ref_count;
            if ((*path)->ref_count == 0) {
                fn_71000803a4(pool, *path);
                pool = g_unk_7101d47200;
            }
        }
        *path = &pool->empty;
    }
    fn_71010c4194(reinterpret_cast<UnkBaseComponent*>(self));
}

void fn_7100802ca0(UnkAreaFXComponent_b49a3* self) {
    Destroy_b49a3(self);
}

void fn_7100802d44(UnkAreaFXComponent_b49a3* self) {
    Destroy_b49a3(self);
    operator delete(self);
}
