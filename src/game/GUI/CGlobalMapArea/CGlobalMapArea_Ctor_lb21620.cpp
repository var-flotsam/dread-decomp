// GUI::CGlobalMapArea's unit (tools/tu_map.py unit 1679): the area's constructor: the display object
// container's, then its vtable, six cleared words, ten empty ids and a cleared flag. Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "game/UnkGuiDisplayObject.h"
#include "types.h"

using base::global::CStrId;

extern char g_unk_7101a9b0a0[];

struct UnkGlobalMapArea_b21620 {
    void** vtable;
    u8 pad_0x8[0x230 - 0x8];  // GUI::CDisplayObjectContainer's members
    void* field_0x230;
    void* field_0x238;
    void* field_0x240;
    void* field_0x248;
    void* field_0x250;
    void* field_0x258;
    CStrId id_0x260;
    CStrId id_0x268;
    CStrId id_0x270;
    CStrId id_0x278;
    CStrId id_0x280;
    bool flag_0x288;
    CStrId id_0x290;
    CStrId id_0x298;
    CStrId id_0x2a0;
    CStrId id_0x2a8;
    CStrId id_0x2b0;
};
static_assert(__builtin_offsetof(UnkGlobalMapArea_b21620, id_0x2b0) == 0x2b0, "layout");

// An empty id, creating the pool first when there is none.
static inline void MakeEmpty(CStrId* id) {
    id->data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    id->data = &g_unk_7101d47108->empty;
}

void fn_7100b21620(UnkGlobalMapArea_b21620* self) {
    fn_71011d8d24(self);
    self->vtable = reinterpret_cast<void**>(g_unk_7101a9b0a0 + 0x10);
    self->field_0x230 = nullptr;
    self->field_0x238 = nullptr;
    self->field_0x240 = nullptr;
    self->field_0x248 = nullptr;
    self->field_0x250 = nullptr;
    self->field_0x258 = nullptr;
    MakeEmpty(&self->id_0x260);
    MakeEmpty(&self->id_0x268);
    MakeEmpty(&self->id_0x270);
    MakeEmpty(&self->id_0x278);
    MakeEmpty(&self->id_0x280);
    self->flag_0x288 = false;
    MakeEmpty(&self->id_0x290);
    MakeEmpty(&self->id_0x298);
    MakeEmpty(&self->id_0x2a0);
    MakeEmpty(&self->id_0x2a8);
    MakeEmpty(&self->id_0x2b0);
}
