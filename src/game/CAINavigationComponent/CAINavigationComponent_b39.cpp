// CAINavigationComponent unit (tools/tu_map.py unit 1113): a destructor. It stores its vtable, detaches
// itself from the owner of the counted link at +0x18 (fn_7101137c84) and drops the link's reference
// (released through vtable slot 0x10 at zero); fn_71006f538c is the deleting form, freeing the object with
// fn_7100083c00. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

extern void* g_unk_7101a6b780[];

struct UnkLinkOwner_ain;

struct UnkLink_ain {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Release();  // 0x10
    u32 ref_count;  // 0x8
    UnkLinkOwner_ain* owner;  // 0x10
};

struct UnkListener_ain {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkLink_ain* link;  // 0x18
};

void fn_7101137c84(UnkLinkOwner_ain* owner, UnkListener_ain* listener);

static inline __attribute__((always_inline)) void Destroy_ain(UnkListener_ain* self) {
    self->vtable = &g_unk_7101a6b780[2];
    UnkLink_ain* link = self->link;
    if (!link)
        return;
    if (UnkLinkOwner_ain* owner = link->owner) {
        fn_7101137c84(owner, self);
        link = self->link;
        if (!link)
            return;
    }
    if (--link->ref_count == 0)
        link->Release();
}

void fn_71006f4344(UnkListener_ain* self) { Destroy_ain(self); }

void fn_71006f538c(UnkListener_ain* self) {
    Destroy_ain(self);
    fn_7100083c00(self);
}
