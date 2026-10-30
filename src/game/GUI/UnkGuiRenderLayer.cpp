// Unit 2754 of tools/tu_map.py (0x71011f5abc-0x71011f6044, no reflected class): a GUI object's
// render layer, an id (+0x0), a layer number (+0x8) and the item placed on it (+0x18, attached while
// +0x20 is set). Holds its release (fn_71011f5eb4). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkRenderManager.h"

void fn_7100219804(UnkRenderManager* self, u32 layer);  // drops the layer

struct UnkGuiRenderLayer {
    base::global::UnkStringData* id;
    u32 layer;
    u8 pad_0xc[0xc];
    char* item;  // 0x18: its +0x8 is handed to the render manager
    bool attached;          // 0x20
};

// Takes the item off the layer (when it was attached), drops the layer and releases the id.
void fn_71011f5eb4(UnkGuiRenderLayer* self) {
    if (self->item) {
        if (self->attached)
            fn_71002199d8(g_unk_7101d47300, reinterpret_cast<UnkItemBase_71011a91c4*>(self->item + 8), self->layer);
        self->item = nullptr;
    }
    fn_7100219804(g_unk_7101d47300, self->layer);
    if (base::global::UnkStringPool* pool = g_unk_7101d47108) {
        if (self->id && self->id != &pool->empty) {
            --self->id->ref_count;
            if (self->id->ref_count == 0) {
                fn_71000803a4(pool, self->id);
                pool = g_unk_7101d47108;
            }
        }
        self->id = &pool->empty;
    }
}
