// Deleting destructor of a GUI object (vtable 0x7101a9ebd8) with a node list at 0x238 (its own
// vtable 0x7101a9ee50; the nodes are freed through the list's allocator), then the base's
// destructor (fn_71011d8e90).
#include "base/global/CRntVector.h"
#include "game/GUI/CScrollMenuEntryItemRenderer.h"
#include "game/UnkGuiDisplayObject.h"
#include "types.h"
#include "base/UnkVtableList.h"

struct UnkListNode {
    u64 field_0x0;
    UnkListNode* next;
};

struct UnkNodeList : base::UnkVtableList<UnkListNode> {
    ~UnkNodeList() {
        vtable = g_unk_7101a9ee50 + 2;
        Clear();
    }
};

struct UnkListScreen {
    void** vtable;
    u8 unk_0x8[0x230];
    UnkNodeList field_0x238;
};

void fn_7100b75394(UnkListScreen* self) {
    self->vtable = g_unk_7101a9ebd8 + 2;
    self->field_0x238.~UnkNodeList();
    fn_71011d8e90(reinterpret_cast<UnkGuiDisplayObject*>(self));
    operator delete(self);
}
