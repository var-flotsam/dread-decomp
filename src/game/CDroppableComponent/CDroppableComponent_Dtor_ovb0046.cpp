// CDroppableComponent's unit (tools/tu_map.py, unit 1216): the component's destructor (vtable 0x7101a70cf0).
// It detaches the shared object at +0xa0 (fn_71009cdc54 on its +0x10, when set) and drops its reference,
// then destroys the id and path pair at +0x88 with its resource (fn_7100027044, then the resource's count)
// and runs the base component destructor (fn_71010c4194). Names and layouts are placeholders.
#include "base/UnkStrIdPathPair.h"
#include "game/UnkComponent.h"
#include "types.h"

struct UnkDropSub_ovb0046;
void fn_71009cdc54(UnkDropSub_ovb0046* self);
extern void* g_unk_7101a70cf0[];

// An intrusively counted object (count at +8, deleted through its slot 2).
struct UnkDropShared_ovb0046 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;
    UnkDropSub_ovb0046* sub;  // 0x10
};

struct UnkDropSharedPtr_ovb0046 {
    UnkDropShared_ovb0046* ptr;
    __attribute__((always_inline)) ~UnkDropSharedPtr_ovb0046() {
        if (ptr && --ptr->ref_count == 0)
            ptr->vf_0x10();
    }
};

struct UnkDropPath_ovb0046 {
    UnkStrIdPathResource value;
    __attribute__((always_inline)) ~UnkDropPath_ovb0046() {
        fn_7100027044(&value);
        if (UnkCountedResource* r = value.resource) {
            if (--r->ref_count == 0)
                fn_710001737c(r);
        }
    }
};

struct UnkDroppableDtor_ovb0046 {
    void** vtable;
    u8 pad_0x8[0x80];
    UnkDropPath_ovb0046 path;  // 0x88
    UnkDropSharedPtr_ovb0046 shared;  // 0xa0
};
static_assert(__builtin_offsetof(UnkDroppableDtor_ovb0046, shared) == 0xa0, "layout");

void fn_71007ba19c(UnkDroppableDtor_ovb0046* self) {
    self->vtable = &g_unk_7101a70cf0[2];
    if (self->shared.ptr && self->shared.ptr->sub)
        fn_71009cdc54(self->shared.ptr->sub);
    self->shared.~UnkDropSharedPtr_ovb0046();
    self->path.~UnkDropPath_ovb0046();
    fn_71010c4194(reinterpret_cast<UnkBaseComponent*>(self));
}
