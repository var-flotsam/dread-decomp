// Destructors (complete and deleting) of two classes with three vectors at 0x230, 0x258 and
// 0x280: the objects the first one points to have their first word cleared, the three are
// emptied and freed, then the base destructor (fn_71011d8e90) runs.
#include "base/UnkVector.h"
#include "game/UnkGuiDisplayObject.h"
#include "types.h"

struct UnkLinked {
    void* owner;
};

// A vector of plain values whose destructor frees the array.
template <class T>
struct UnkFreedVector : base::UnkVector<T> {
    __attribute__((always_inline)) ~UnkFreedVector() { this->Free(); }
};

template <void** Vtable>
struct UnkLinkOwner {
    void** vtable;
    u8 unk_0x8[0x228];
    UnkFreedVector<UnkLinked*> links;
    UnkFreedVector<void*> field_0x258;
    UnkFreedVector<void*> field_0x280;

    __attribute__((always_inline)) ~UnkLinkOwner() {
        vtable = Vtable + 2;
        for (UnkLinked **p = links.data, **end = p + links.count; p != end; ++p)
            (*p)->owner = nullptr;
        links.count = 0;
        field_0x258.count = 0;
        field_0x280.count = 0;
    }
};
static_assert(__builtin_offsetof(UnkLinkOwner<nullptr>, field_0x280) == 0x280, "UnkLinkOwner layout");

template <void** Vtable>
__attribute__((always_inline)) static inline void Destroy(UnkLinkOwner<Vtable>* self) {
    self->~UnkLinkOwner();
    fn_71011d8e90(reinterpret_cast<UnkGuiDisplayObject*>(self));
}

extern void* g_unk_7101a9f820[];
extern void* g_unk_7101a9fd08[];

void fn_7100b85000(UnkLinkOwner<g_unk_7101a9f820>* self) { Destroy(self); }
void fn_7100b85120(UnkLinkOwner<g_unk_7101a9f820>* self) {
    Destroy(self);
    operator delete(self);
}
void fn_7100b88cc4(UnkLinkOwner<g_unk_7101a9fd08>* self) { Destroy(self); }
void fn_7100b88de4(UnkLinkOwner<g_unk_7101a9fd08>* self) {
    Destroy(self);
    operator delete(self);
}
