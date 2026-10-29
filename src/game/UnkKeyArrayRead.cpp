// From unit 1728 of tools/tu_map.py (0x7100b892d4-0x7100b8bb94, no class hint): an array of 12-byte
// keys (two words and a value that defaults to 30) read from a file. Names and layouts are
// placeholders.
// Holds fn_7100b8948c.
#include "base/UnkVector.h"
#include "base/global/CRntFile.h"
#include "types.h"

struct UnkKey12 {
    u32 field_0x0 = 0;
    u32 field_0x4 = 0;
    f32 field_0x8 = 30.0f;
};
static_assert(sizeof(UnkKey12) == 0xc, "UnkKey12 placeholder layout");

struct UnkKey12Array : base::UnkVector<UnkKey12> {
    // Grows the array to exactly n elements' room, copying the old ones over.
    void Grow(u32 n) {
        auto* p = static_cast<UnkKey12*>(allocator->vf_0x40((u64)n * sizeof(UnkKey12), 8, flags));
        if (data) {
            for (u32 i = 0; i < count; ++i)
                __builtin_memcpy(&p[i], &data[i], sizeof(UnkKey12));
            allocator->vf_0x48(data);
        }
        data = p;
        capacity = n;
    }
};

// Reads the key count, then each key's 12 bytes into a new default key at the end.
// NON_MATCHING(major): written from the original's behaviour, not compared instruction by instruction.
void fn_7100b8948c(UnkKey12Array* self, base::global::CRntFile* file) {
    u32 n;
    fn_710007c714(file, &n, 4, 1);
    if (self->capacity < n)
        self->Grow(n);
    for (u32 i = 0; i < n; ++i) {
        u32 c = self->count + 1;
        if (self->capacity < c)
            self->Grow(c);
        for (u32 k = self->count; k < c; ++k)
            new (&self->data[k]) UnkKey12();
        self->count = c;
        fn_710007c714(file, &self->data[c - 1], sizeof(UnkKey12), 1);
    }
}
