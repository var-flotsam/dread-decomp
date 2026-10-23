// Destructors (complete and deleting) of a class (vtable unk_7101a1e248) that returns two pooled
// objects at 0x68 and 0x70 to their pool (fn_7100161b90), clears the word at 0x80 and releases a
// string id at 0x150, above the base whose destructor is fn_710019e82c: fn_710019bfb8 and
// fn_710019c058. fn_7101141558 is a relative. Called only through the vtable.

#include <new>

#include "base/UnkDestructors.h"
#include "types.h"

extern char unk_7101a1e248[];
void fn_7100161b90(void* obj);
void fn_710019e82c(void* self);

struct UnkPooledPair_710019bfb8 {
    char* vtable;
    u8 pad_0x8[0x60];
    void* pooled_0x68;
    void* pooled_0x70;
    u64 field_0x78;
    u64 field_0x80;
    u8 pad_0x88[0xc8];
    base::UnkStrIdMember id;
};

__attribute__((always_inline)) static inline void Destroy(UnkPooledPair_710019bfb8* self) {
    self->vtable = unk_7101a1e248 + 0x10;
    if (self->pooled_0x68) {
        fn_7100161b90(self->pooled_0x68);
        self->pooled_0x68 = nullptr;
    }
    if (self->pooled_0x70) {
        fn_7100161b90(self->pooled_0x70);
        self->pooled_0x70 = nullptr;
    }
    self->field_0x80 = 0;
    using Id = base::UnkStrIdMember;
    self->id.~Id();
    fn_710019e82c(self);
}

void fn_710019bfb8(UnkPooledPair_710019bfb8* self) { Destroy(self); }

void fn_710019c058(UnkPooledPair_710019bfb8* self) {
    Destroy(self);
    operator delete(self);
}
