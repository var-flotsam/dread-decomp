// Two shared_ptr control blocks in the CBlendSpace1D unit that own a pointer through a
// std::function deleter (libc++'s __shared_ptr_pointer<T*, std::function<void(T*)>, allocator<T>>
// layout: the pointer at 0x20, the function at 0x30). For each: the destructor (it stores the
// block's vtable, destroys the function, then runs std::__shared_weak_count's destructor), the
// deleting destructor, and __on_zero_shared (calls the deleter on the pointer, then destroys the
// deleter). Each names its own vtable. Names and layouts are placeholders.
#include <functional>
#include <memory>

#include "base/UnkVector.h"
#include "base/animation/CBlendSpace1D.h"
#include "types.h"

struct UnkSharedFnPointee;

template <char* Vtable>
struct UnkSharedFnBlock {
    char* vtable;
    u64 shared_owners;
    u64 shared_weak_owners;
    std::__compressed_pair<std::__compressed_pair<UnkSharedFnPointee*, std::function<void(UnkSharedFnPointee*)>>,
                           std::allocator<UnkSharedFnPointee>>
        data;
};

// std::__shared_weak_count's base-object destructor (protected; the original calls the D2 symbol).
void UnkSharedWeakCountDtor(void* self) asm("_ZNSt3__119__shared_weak_countD2Ev");

template <char* Vtable>
__attribute__((always_inline)) static inline void DestroyBlock(UnkSharedFnBlock<Vtable>* self) {
    self->vtable = Vtable + 0x10;
    self->data.first().second().~function();
    UnkSharedWeakCountDtor(self);
}

template <char* Vtable>
__attribute__((always_inline)) static inline void DeleteBlock(UnkSharedFnBlock<Vtable>* self) {
    DestroyBlock(self);
    fn_7100083c00(self);
}

template <char* Vtable>
__attribute__((always_inline)) static inline void OnZeroShared(UnkSharedFnBlock<Vtable>* self) {
    self->data.first().second()(std::move(self->data.first().first()));
    self->data.first().second().~function();
}

void fn_7100008d6c(UnkSharedFnBlock<g_unk_7101a19230>* self) { DestroyBlock(self); }
void fn_7100008dc8(UnkSharedFnBlock<g_unk_7101a19230>* self) { DeleteBlock(self); }
void fn_7100008e2c(UnkSharedFnBlock<g_unk_7101a19230>* self) { OnZeroShared(self); }
void fn_7100009054(UnkSharedFnBlock<g_unk_7101a19268>* self) { DestroyBlock(self); }
void fn_71000090b0(UnkSharedFnBlock<g_unk_7101a19268>* self) { DeleteBlock(self); }
void fn_7100009114(UnkSharedFnBlock<g_unk_7101a19268>* self) { OnZeroShared(self); }
