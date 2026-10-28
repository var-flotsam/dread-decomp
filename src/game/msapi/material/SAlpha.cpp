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
