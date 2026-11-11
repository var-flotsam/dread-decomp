// From unit 229 (msapi::material::SAlpha and the unit's other material types): looks up, in a
// material's program, the uniform location of every named value block for one shader stage or for
// all three. Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/UnkTU230.h"
#include "game/msapi/material/SAlpha.h"
#include "types.h"

using base::global::CStrId;

// The value block's vtable (reached through the GOT); its slot 1 is the complete destructor.
extern char unk_7101a1e4d8[];

// The element's plain part (copied as a whole).
struct UnkMaterialValuesData_l1a77a4 {
    u8 field_0x10;
    s32 field_0x14;
    u64 locations[3];  // 0x18: the block's uniform location in each shader stage, 0 for none
    void* field_0x30;
};

// A named value block (0x38 bytes).
struct UnkMaterialValues_l1a77a4 {
    char* vtable;
    CStrId name;
    UnkMaterialValuesData_l1a77a4 d;

    UnkMaterialValues_l1a77a4(const UnkMaterialValues_l1a77a4& other)
        : vtable(unk_7101a1e4d8 + 0x10), name(other.name), d(other.d) {}

    void Destroy() {
        reinterpret_cast<void (*)(UnkMaterialValues_l1a77a4*)>(reinterpret_cast<void**>(vtable)[1])(this);
    }
};
static_assert(sizeof(UnkMaterialValues_l1a77a4) == 0x38, "UnkMaterialValues_l1a77a4 placeholder layout");

// Grows a value block array to n elements, default-constructing the new ones.
struct UnkMaterialValuesVector;

struct UnkMaterialValuesVector_l1a77a4 : base::UnkVector<UnkMaterialValues_l1a77a4> {
    // Grows the array to hold n elements, copying them over and destroying the old ones.
    void Reserve(u32 n) {
        if (capacity < n) {
            UnkMaterialValues_l1a77a4* p = static_cast<UnkMaterialValues_l1a77a4*>(
                allocator->vf_0x40((u64)n * sizeof(UnkMaterialValues_l1a77a4), 8, flags));
            if (data) {
                UnkMaterialValues_l1a77a4* src = data;
                UnkMaterialValues_l1a77a4* dst = p;
                for (u32 i = 0; i < count; ++i, ++dst, ++src) {
                    new (dst) UnkMaterialValues_l1a77a4(*src);
                    src->Destroy();
                }
                allocator->vf_0x48(data);
            }
            data = p;
            capacity = n;
        }
    }

    // The element at `index`; a growable array is first extended to reach it.
    UnkMaterialValues_l1a77a4& At(u32 index) {
        u32 n = count;
        if (field_0x14 && n <= index) {
            if (capacity <= index && step != 0)
                Reserve((index / step + 1) * step);
            fn_71001a9bd0(reinterpret_cast<UnkMaterialValuesVector*>(this), index + 1);
        }
        return data[index];
    }
};

// The location of the uniform named `name` of a program's stage, 0 when it has none.
struct UnkGfxStrId;

// The material as this function sees it: its program at 0x390.
struct UnkMaterial_l1a77a4 {
    u8 pad_0x0[0x390];
    void* program;
};

constexpr s32 kAllStages = 0x7fffffff;

void fn_71001a77a4(UnkMaterial_l1a77a4* self, s32 stage, UnkMaterialValuesVector_l1a77a4& values) {
    if (!self->program)
        return;
    s32 n = values.count;
    for (s32 i = 0; i < n; ++i) {
        UnkMaterialValues_l1a77a4& block = values.At(i);
        const UnkGfxStrId* name = reinterpret_cast<const UnkGfxStrId*>(&block.name);
        for (s32 s = 0; s < 3; ++s) {
            block.d.locations[s] = 0;
            if (stage == s || stage == kAllStages)
                fn_71001aa0cc(self->program, s, name, &block.d.locations[s]);
        }
    }
}
