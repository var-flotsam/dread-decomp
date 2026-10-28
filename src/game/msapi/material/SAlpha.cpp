// From unit 229 of tools/tu_map.py (0x71001a12f4-0x71001a9f3c, msapi::material::SAlpha and the
// unit's other material types): reading an array of named value blocks from a file. Each element
// (vtable unk_7101a1e4d8, destructors fn_71001a98dc and fn_71001a9978 in
// src/base/file/nx/CDeviceNX/fn_710002ddc0.cpp) has a string id at 8, a byte at 0x10, a count at 0x14 and an
// allocator-owned block of that many 4-byte values at 0x30. Names and layouts are placeholders.
// Holds fn_71001a6f54.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CRntFile.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CRntFile;
using base::global::CStrId;

// The element's vtable (reached through the GOT); its slot 1 is the complete destructor.
extern char unk_7101a1e4d8[];

// The element's plain part (copied as a whole).
struct UnkMaterialValuesData {
    u8 field_0x10;
    s32 field_0x14;  // the number of values
    u64 field_0x18;
    u64 field_0x20;
    u64 field_0x28;
    void* field_0x30;  // the values
};

struct UnkMaterialValues {
    char* vtable;
    CStrId id;
    UnkMaterialValuesData d;

    UnkMaterialValues(const UnkMaterialValues& other) : vtable(unk_7101a1e4d8 + 0x10), id(other.id), d(other.d) {}

    void Destroy() { reinterpret_cast<void (*)(UnkMaterialValues*)>(reinterpret_cast<void**>(vtable)[1])(this); }
};
static_assert(sizeof(UnkMaterialValues) == 0x38, "UnkMaterialValues placeholder layout");

// The array (base/UnkVector.h's layout); fn_71001a9bd0 resizes it.
struct UnkMaterialValuesVector : base::UnkVector<UnkMaterialValues> {
    // Grows the array to hold n elements, copying them over and destroying the old ones.
    void Reserve(u32 n) {
        if (capacity < n) {
            UnkMaterialValues* p =
                static_cast<UnkMaterialValues*>(allocator->vf_0x40((u64)n * sizeof(UnkMaterialValues), 8, flags));
            if (data) {
                UnkMaterialValues* src = data;
                UnkMaterialValues* dst = p;
                for (u32 i = 0; i < count; ++i, ++dst, ++src) {
                    new (dst) UnkMaterialValues(*src);
                    src->Destroy();
                }
                allocator->vf_0x48(data);
            }
            data = p;
            capacity = n;
        }
    }
    // The last element (the first when there is none).
    UnkMaterialValues* Back() {
        u32 n = count;
        UnkMaterialValues* end = data + n;
        return n ? end - 1 : end;
    }
};
void fn_71001a9bd0(UnkMaterialValuesVector* v, u32 n);

// A string id built in place to be moved into an element (so never destroyed).
struct UnkMovedId {
    union {
        CStrId id;
    };

    explicit UnkMovedId(const char* str) { new (&id) CStrId(str, true); }
    ~UnkMovedId() {}
};

// Reads `size` bytes (fn_710007c714 with one element).
u64 fn_710007c70c(CRntFile* file, void* dest, u64 size);
// The next string, in place (src/game_s7/UnkPropS7.h).
s64 fn_710007cee4(void* stream, const char** out);

// Reads the element count, then each element: its name, the byte, the count and the values.
void fn_71001a6f54(void*, CRntFile* file, void*, UnkMaterialValuesVector* values) {
    const char* name;
    s32 n = 0;
    fn_710007c70c(file, &n, 4);
    values->Reserve(n);
    for (s32 i = 0; i < n; ++i) {
        fn_71001a9bd0(values, values->count + 1);
        UnkMaterialValues* e = values->Back();
        fn_710007cee4(file, &name);
        fn_710007c714(file, &e->d.field_0x10, 1, 1);
        fn_710007c714(file, &e->d.field_0x14, 4, 1);
        e->id = static_cast<CStrId&&>(UnkMovedId(name).id);
        u64 size = (u64)e->d.field_0x14 * 4;
        void* p = fn_7100083990()->vf_0x40(size, 8, 0x35);
        e->d.field_0x30 = p;
        fn_710007c70c(file, p, size);
    }
}

// ---- fn_71001a6644: reading a material ----

// A pass of the material (0x60 bytes, vtable unk_7101a1e358 over the base fn_7100023838 builds): its
// named value blocks at 0x10 and a second array at 0x38.
struct UnkMaterialPass {
    char* vtable;
    u64 field_0x8;
    UnkMaterialValuesVector values;      // 0x10
    base::UnkVector<u64> field_0x38;
};
static_assert(sizeof(UnkMaterialPass) == 0x60, "UnkMaterialPass placeholder layout");
extern char unk_7101a1e358[];
void fn_7100023838(void* self);

// The material as this reads it: the header blocks read in place and their copies, the values at
// 0x330, the passes at 0x358 and the ids at 0x3a8 that are emptied after the read.
struct UnkMaterial {
    u8 pad_0x0[0x358];
    base::UnkVector<UnkMaterialPass> passes;
    u8 pad_0x380[0x28];
    base::UnkVector<CStrId> ids;  // 0x3a8
};
static_assert(__builtin_offsetof(UnkMaterial, ids) == 0x3a8, "UnkMaterial placeholder layout");

template <class T>
static inline T& At(UnkMaterial* self, u64 offset) {
    return *reinterpret_cast<T*>(reinterpret_cast<u8*>(self) + offset);
}

// The unit's readers: a block at `dst` (the first argument is not read), the material's own fields,
// a pass's second array, and the steps after the passes.
void fn_71001a69a0(void*, CRntFile* file, void* dst);
void fn_71001a6b5c(UnkMaterial* self);
void fn_71001a71b8(base::UnkVector<UnkMaterialPass>* passes, u32 n);  // Reserve
void fn_71001a7310(UnkMaterial* self, CRntFile* file, base::UnkVector<u64>* list);
void fn_71001a7410(UnkMaterial* self);
void fn_71001a836c(UnkMaterial* self, base::UnkVector<u64>* list);

// Reads the material from `file`: its header blocks (copied to their places), its values, then
// each pass with its blocks, values and second array; then empties the ids.
// NON_MATCHING(minor): the first argument of the block readers, which they do not read, is null
// here; the original leaves x0 as the previous call returned it.
void fn_71001a6644(UnkMaterial* self, CRntFile* file) {
    for (u64 o = 0x2e0; o < 0x330; o += 8)
        At<u64>(self, o) = 0;
    for (u64 o = 0x200; o < 0x240; o += 8)
        At<u64>(self, o) = 0;
    At<u64>(self, 0x1f0) = 0;
    At<u64>(self, 0x1f8) = 0;
    fn_710007c00c(file, 8, 0);
    fn_71001a69a0(nullptr, file, self);
    fn_71001a6b5c(self);
    At<u32>(self, 0x148) = At<u32>(self, 0x18);
    fn_71001a69a0(nullptr, file, &At<u8>(self, 0xb8));
    fn_71001a69a0(nullptr, file, &At<u8>(self, 0xd0));
    fn_71001a69a0(nullptr, file, &At<u8>(self, 0xe0));
    fn_71001a69a0(nullptr, file, &At<u8>(self, 0x108));
    fn_71001a69a0(nullptr, file, &At<u8>(self, 0x120));
    fn_71001a69a0(nullptr, file, &At<u8>(self, 0x130));
    At<u64>(self, 0x30) = At<u64>(self, 0xc0);
    At<u64>(self, 0x38) = At<u64>(self, 0xc8);
    At<u32>(self, 0x48) = At<u32>(self, 0xd8);
    for (u64 i = 0; i < 0x20; i += 8)
        At<u64>(self, 0x58 + i) = At<u64>(self, 0xe8 + i);
    At<u32>(self, 0x88) = At<u32>(self, 0x118);
    At<u64>(self, 0x80) = At<u64>(self, 0x110);
    At<u32>(self, 0x98) = At<u32>(self, 0x128);
    At<u8>(self, 0xb0) = At<u8>(self, 0x140);
    At<u64>(self, 0xa8) = At<u64>(self, 0x138);
    At<u32>(self, 0x18) = At<u32>(self, 0x148);
    fn_71001a6f54(nullptr, file, nullptr, &At<UnkMaterialValuesVector>(self, 0x330));
    s32 count = 0;
    fn_710007c714(file, &count, 4, 1);
    fn_71001a71b8(&self->passes, count);
    for (s32 i = 0; i < count; ++i) {
        u32 n = self->passes.count + 1;
        fn_71001a71b8(&self->passes, n);
        for (u32 k = self->passes.count; k < n; ++k) {
            UnkMaterialPass* p = &self->passes.data[k];
            fn_7100023838(p);
            p->vtable = unk_7101a1e358 + 0x10;
            new (&p->values) base::UnkVector<UnkMaterialValues>(0x35);
            new (&p->field_0x38) base::UnkVector<u64>(0x35);
        }
        self->passes.count = n;
        UnkMaterialPass* pass = n ? &self->passes.data[n - 1] : self->passes.data;
        fn_71001a69a0(nullptr, file, pass);
        fn_71001a6f54(nullptr, file, nullptr, &pass->values);
        fn_71001a7310(self, file, &pass->field_0x38);
    }
    fn_71001a7410(self);
    for (u64 i = 0; i < self->ids.count; ++i)
        self->ids.data[i].Reset();
    self->ids.count = 0;
    for (UnkMaterialPass* p = self->passes.data; p != self->passes.data + self->passes.count; ++p)
        fn_71001a836c(self, &p->field_0x38);
}
