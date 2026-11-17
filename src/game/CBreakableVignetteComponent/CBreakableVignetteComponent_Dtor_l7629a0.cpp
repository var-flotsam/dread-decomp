// CBreakableVignetteComponent unit (tools/tu_map.py unit 1168): the destructor. Names and layouts are
// placeholders.
#include "base/UnkDestructors.h"
#include "base/global/CRntVector.h"
#include "game/CActorComponent.h"
#include "game/UnkTU883.h"
#include "types.h"

extern void* g_unk_7101a6e5b0[];
extern void* g_unk_7101a81968[];

// A callback link registered with a manager; unregistered when destroyed.
struct UnkVignetteLink_l7629a0 {
    void* registration;
    void* target;
    void* callback;

    ~UnkVignetteLink_l7629a0() {
        if (registration) {
            fn_71005c4828(registration, &registration);
            registration = nullptr;
        }
        target = nullptr;
        callback = nullptr;
    }
};

// The component's shape (owned: deleted through slot 2).
struct UnkVignetteArea_l7629a0 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
};

struct UnkBreakableVignette_l7629a0 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkVignetteArea_l7629a0* area;  // 0x30
    u8 pad_0x38[0x8];
    base::UnkStrIdMember name;                  // 0x40
    UnkVignetteLink_l7629a0 link;               // 0x48
    base::global::CRntVector<void**> lists[3];  // 0x60
};
static_assert(__builtin_offsetof(UnkBreakableVignette_l7629a0, lists) == 0x60, "layout");

void fn_71007629a0(UnkBreakableVignette_l7629a0* self) {
    self->vtable = &g_unk_7101a6e5b0[2];
    base::global::CRntVector<void**>& links = self->lists[0];
    for (void*** it = links.data, ***end = links.data + links.count; it != end; ++it)
        **it = nullptr;
    for (base::global::CRntVector<void**>& list : self->lists)
        list.Clear();
    self->lists[2].Free();
    self->lists[1].Free();
    self->lists[0].Free();
    self->link.~UnkVignetteLink_l7629a0();
    using base::UnkStrIdMember;
    self->name.~UnkStrIdMember();
    self->vtable = &g_unk_7101a81968[2];
    UnkVignetteArea_l7629a0* area = self->area;
    self->area = nullptr;
    if (area)
        area->vf_0x10();
    fn_710024f6d4(reinterpret_cast<UnkBaseComponent*>(self));
}
