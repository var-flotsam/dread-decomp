// Destructors (complete and deleting) of a reflection class (vtable unk_7101a19fc0) holding a
// string id at 8 and a string (base::global::CRntString) at 0x10: fn_710002ddc0, fn_710002de3c.
// The deleting one frees the object without resetting the id to the empty string. Likewise for
// a class (vtable unk_7101a1f380) with a string id at 8 and an array of plain values at 0x10:
// fn_71001f9c3c, fn_71001f9cd4. Also a class (vtable unk_7101a20d68) with the id at 0x58 and the
// array at 0x68, above a base whose destructor is fn_7100016f88: fn_710026cd30, fn_710026cdcc.
// And a class (vtable unk_7101a1e4d8) with the id at 8 and an allocator-owned block at 0x30,
// above a base whose destructor is fn_710002384c: fn_71001a98dc, fn_71001a9978.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

extern char unk_7101a19fc0[];
extern char unk_7101a1f380[];
extern char unk_7101a20d68[];
void fn_7100016f88(void* self);

struct UnkNamedId_710002ddc0 {
    char* vtable;
    base::UnkStrIdMember id;
    base::global::CRntString name;

    __attribute__((always_inline)) ~UnkNamedId_710002ddc0() { vtable = unk_7101a19fc0 + 0x10; }
};

void fn_710002ddc0(UnkNamedId_710002ddc0* self) { self->~UnkNamedId_710002ddc0(); }

// The id's release, without the reset.
__attribute__((always_inline)) static inline void Release(base::UnkStrIdMember* id) {
    if (UnkStringPool* pool = g_unk_7101d47108) {
        UnkStringData*& data = id->data;
        if (data && data != &pool->empty) {
            --data->ref_count;
            if (data->ref_count == 0)
                fn_71000803a4(pool, data);
        }
    }
}

void fn_710002de3c(UnkNamedId_710002ddc0* self) {
    self->vtable = unk_7101a19fc0 + 0x10;
    self->name.~CRntString();
    Release(&self->id);
    operator delete(self);
}

struct UnkIdValues_71001f9c3c {
    char* vtable;
    base::UnkStrIdMember id;
    base::UnkVector<u32> values;

    __attribute__((always_inline)) ~UnkIdValues_71001f9c3c() {
        vtable = unk_7101a1f380 + 0x10;
        values.Free();
    }
};

void fn_71001f9c3c(UnkIdValues_71001f9c3c* self) { self->~UnkIdValues_71001f9c3c(); }

void fn_71001f9cd4(UnkIdValues_71001f9c3c* self) {
    self->vtable = unk_7101a1f380 + 0x10;
    self->values.Free();
    Release(&self->id);
    operator delete(self);
}

struct UnkIdValues_710026cd30 {
    char base[0x58];
    base::UnkStrIdMember id;
    u64 field_0x60;
    base::UnkVector<u32> values;
};

__attribute__((always_inline)) static inline void Destroy(UnkIdValues_710026cd30* self) {
    *reinterpret_cast<char**>(self) = unk_7101a20d68 + 0x10;
    self->values.Free();
    using Id = base::UnkStrIdMember;
    self->id.~Id();
    fn_7100016f88(self);
}

void fn_710026cd30(UnkIdValues_710026cd30* self) { Destroy(self); }

void fn_710026cdcc(UnkIdValues_710026cd30* self) {
    Destroy(self);
    operator delete(self);
}

extern char unk_7101a1e4d8[];
void fn_710002384c(void* self);

struct UnkIdOwned_71001a98dc {
    char* vtable;
    base::UnkStrIdMember id;
    u8 pad_0x10[0x20];
    void* owned;
};

__attribute__((always_inline)) static inline void Destroy(UnkIdOwned_71001a98dc* self) {
    self->vtable = unk_7101a1e4d8 + 0x10;
    if (self->owned) {
        fn_7100083990()->vf_0x48(self->owned);
        self->owned = nullptr;
    }
    using Id = base::UnkStrIdMember;
    self->id.~Id();
    fn_710002384c(self);
}

void fn_71001a98dc(UnkIdOwned_71001a98dc* self) { Destroy(self); }

void fn_71001a9978(UnkIdOwned_71001a98dc* self) {
    Destroy(self);
    operator delete(self);
}
