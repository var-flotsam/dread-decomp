// GUI::CDialogComposition's unit (tools/tu_map.py unit 1665): the dialog's constructor, on the GUI display
// object base: two callbacks bound to the dialog, an empty page id, three arrays, and two copies of the
// default string id. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/GUI/CDialogComposition.h"
#include "game/UnkGuiDisplayObject.h"
#include "types.h"

using base::global::CStrId;

u32 fn_7100b026b0(void* self);
u32 fn_7100b02684(void* self);

// A callback bound to its owner.
struct UnkDlgCallback_lb013b8 {
    void* field_0x0;
    void* owner;
    u32 (*fn)(void* self);
};

struct UnkDialogComposition_lb013b8 {
    void** vtable;
    u8 pad_0x8[0x228];
    UnkDlgCallback_lb013b8 field_0x230;
    UnkDlgCallback_lb013b8 field_0x248;
    CStrId page;  // 0x260
    base::global::CRntVector<void*> lists[3];  // 0x268
    bool field_0x2e0;
    CStrId field_0x2e8;
    u64 field_0x2f0;
    u64 field_0x2f8;
    CStrId field_0x300;
    u64 field_0x308;
    u64 field_0x310;
    u64 field_0x318;
};
static_assert(__builtin_offsetof(UnkDialogComposition_lb013b8, field_0x318) == 0x318, "layout");

void fn_7100b013b8(UnkDialogComposition_lb013b8* self) {
    fn_71011d8d24(self);
    self->vtable = g_unk_7101a99f18 + 2;
    self->field_0x230.field_0x0 = nullptr;
    self->field_0x230.owner = self;
    self->field_0x230.fn = fn_7100b026b0;
    self->field_0x248.field_0x0 = nullptr;
    self->field_0x248.owner = self;
    self->field_0x248.fn = fn_7100b02684;
    self->page.data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->page.data = &g_unk_7101d47108->empty;
    for (base::global::CRntVector<void*>& list : self->lists)
        new (&list) base::global::CRntVector<void*>(0xf);
    self->field_0x2e0 = false;
    new (&self->field_0x2e8) CStrId(base::global::CStrId::Empty());
    self->field_0x2f0 = 0;
    self->field_0x2f8 = 0;
    new (&self->field_0x300) CStrId(base::global::CStrId::Empty());
    self->field_0x308 = 0;
    self->field_0x310 = 0;
    self->field_0x318 = 0;
}
