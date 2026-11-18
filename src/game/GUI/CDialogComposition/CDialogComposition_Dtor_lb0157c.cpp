// GUI::CDialogComposition's unit (tools/tu_map.py unit 1665): the dialog's destructor. Releases the counted object
// at +0x318 and the string ids, clears the back pointer of every linked object (+0x268), empties and frees the
// three arrays in reverse order, releases the page id, unregisters both callbacks and runs the GUI container's
// destructor (fn_71011d8e90). Names and layouts are placeholders.
#include "base/UnkDeferredSet.h"
#include "base/UnkDestructors.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/GUI/CDialogComposition.h"
#include "game/UnkGuiDisplayObject.h"
#include "types.h"

using base::global::CStrId;

// A callback registered with a deferred set: unregistered when destroyed.
struct UnkDlgCallback_lb0157c {
    base::UnkDeferredSet<void>* set;
    void* object;
    void (*fn)();

    ~UnkDlgCallback_lb0157c() {
        if (set) {
            fn_71004f3ea4(set, &set);
            set = nullptr;
        }
        fn = nullptr;
        object = nullptr;
    }
};

struct UnkDlgLinked_lb0157c {
    void* back;
};

struct UnkDialogComposition_lb0157c {
    void** vtable;
    u8 pad_0x8[0x228];
    UnkDlgCallback_lb0157c field_0x230;
    UnkDlgCallback_lb0157c field_0x248;
    CStrId page;  // 0x260
    base::global::CRntVector<UnkDlgLinked_lb0157c*> linked;  // 0x268
    base::global::CRntVector<u64> field_0x290;
    base::global::CRntVector<u64> field_0x2b8;
    bool field_0x2e0;
    CStrId field_0x2e8;
    u64 field_0x2f0;
    u64 field_0x2f8;
    CStrId field_0x300;
    u64 field_0x308;
    u64 field_0x310;
    base::UnkRefCounted* field_0x318;
};
static_assert(__builtin_offsetof(UnkDialogComposition_lb0157c, field_0x318) == 0x318, "layout");

void fn_7100b0157c(UnkDialogComposition_lb0157c* self) {
    self->vtable = g_unk_7101a99f18 + 2;
    if (base::UnkRefCounted* p = self->field_0x318) {
        if (--p->ref_count == 0)
            delete p;
    }
    self->field_0x300.~CStrId();
    self->field_0x2e8.~CStrId();
    for (UnkDlgLinked_lb0157c **it = self->linked.data, **end = self->linked.data + self->linked.count; it != end;
         ++it)
        (*it)->back = nullptr;
    self->linked.count = 0;
    self->field_0x290.count = 0;
    self->field_0x2b8.count = 0;
    self->field_0x2b8.Free();
    self->field_0x290.Free();
    self->linked.Free();
    self->page.~CStrId();
    self->field_0x248.~UnkDlgCallback_lb0157c();
    self->field_0x230.~UnkDlgCallback_lb0157c();
    fn_71011d8e90(reinterpret_cast<UnkGuiDisplayObject*>(self));
}
