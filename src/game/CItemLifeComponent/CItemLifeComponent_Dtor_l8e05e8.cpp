// CItemLifeComponent's unit (tools/tu_map.py): the destructor. The listener at +0x318, the three arrays at
// +0x290 (the first one's entries point to slots that are cleared), the string-id table at +0x268 and the
// name at +0x260, then the base destructor fn_710115f91c. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkStrIdTable.h"
#include "base/global/CRntVector.h"
#include "game/CAbilityGhostAura.h"
#include "game/CItemLifeComponent.h"
#include "game/CLifeComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkItemLifeLink_l8e05e8 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Release();  // 0x10
    u32 ref_count;           // 0x8
    UnkWeakTarget* owner;    // 0x10
};

// A listener on a counted link; it detaches itself from the link's owner on destruction.
struct UnkItemLifeListener_l8e05e8 {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkItemLifeLink_l8e05e8* link;  // 0x18

    ~UnkItemLifeListener_l8e05e8() {
        vtable = &g_unk_7101a22d48[2];
        if (UnkItemLifeLink_l8e05e8* l = link) {
            if (UnkWeakTarget* owner = l->owner) {
                fn_710117b3f0(owner, this);
                l = link;
                if (!l)
                    return;
            }
            if (--l->ref_count == 0)
                l->Release();
        }
    }
};

struct UnkItemLifeTable_l8e05e8 : UnkStrIdTable {
    ~UnkItemLifeTable_l8e05e8() {
        fn_71004f11f8(this);
        if (keys)
            allocator->vf_0x48(keys);
        if (values)
            allocator->vf_0x48(values);
    }
};

struct UnkItemLifeComponent_l8e05e8 {
    void** vtable;
    u8 pad_0x8[0x258];
    base::UnkStrIdMember name;                   // 0x260
    UnkItemLifeTable_l8e05e8 table;              // 0x268
    base::global::CRntVector<void**> lists[3];   // 0x290
    u8 pad_0x308[0x10];
    UnkItemLifeListener_l8e05e8 listener;        // 0x318
};
static_assert(__builtin_offsetof(UnkItemLifeComponent_l8e05e8, listener) == 0x318, "layout");

void fn_71008e05e8(void* p) {
    auto* self = static_cast<UnkItemLifeComponent_l8e05e8*>(p);
    self->vtable = &g_unk_7101a7f028[2];
    self->listener.~UnkItemLifeListener_l8e05e8();
    base::global::CRntVector<void**>& links = self->lists[0];
    for (void*** it = links.data, ***end = links.data + links.count; it != end; ++it)
        **it = nullptr;
    for (base::global::CRntVector<void**>& list : self->lists)
        list.Clear();
    self->lists[2].Free();
    self->lists[1].Free();
    self->lists[0].Free();
    self->table.~UnkItemLifeTable_l8e05e8();
    using base::UnkStrIdMember;
    self->name.~UnkStrIdMember();
    fn_710115f91c(self);
}
