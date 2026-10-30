// Destructors (complete and deleting) of four classes with three vectors at 0x230, 0x258 and
// 0x280: the objects the first one points to have their first word cleared, the three are
// emptied and freed, then the base destructor (fn_71011d8e90 for the first
// two) runs.
#include "base/UnkVector.h"
#include "game/UnkComponent.h"
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

template <void** Vtable, u32 At = 0x230>
struct UnkLinkOwner {
    void** vtable;
    u8 unk_0x8[At - 0x8];
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

template <void** Vtable, u32 At, class Base = UnkGuiDisplayObject, void (*BaseDtor)(Base*) = fn_71011d8e90>
__attribute__((always_inline)) static inline void Destroy(UnkLinkOwner<Vtable, At>* self) {
    self->~UnkLinkOwner();
    BaseDtor(reinterpret_cast<Base*>(self));
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

// The same pair for two more classes, with their own vtables and base destructors (a component's,
// fn_71010c4194, and fn_710085c814) and the vectors at 0x58 and 0x18a0.
void fn_710085c814(char*);

extern void* g_unk_7101a786e0[];
extern void* g_unk_7101a8d098[];

void fn_710084e8fc(UnkLinkOwner<g_unk_7101a786e0, 0x18a0>* self) { Destroy<g_unk_7101a786e0, 0x18a0, char, fn_710085c814>(self); }
void fn_710084ea1c(UnkLinkOwner<g_unk_7101a786e0, 0x18a0>* self) {
    Destroy<g_unk_7101a786e0, 0x18a0, char, fn_710085c814>(self);
    operator delete(self);
}
void fn_7100a1f520(UnkLinkOwner<g_unk_7101a8d098, 0x58>* self) {
    Destroy<g_unk_7101a8d098, 0x58, UnkBaseComponent, fn_71010c4194>(self);
}
void fn_7100a1f640(UnkLinkOwner<g_unk_7101a8d098, 0x58>* self) {
    Destroy<g_unk_7101a8d098, 0x58, UnkBaseComponent, fn_71010c4194>(self);
    operator delete(self);
}
