// Unit 2545: a destructor. Destroys eight reflected value holders and five pairs of string ids,
// last to first, then runs the base destructor (its vtable and cleared fields). Names and
// layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::global::CStrId;

struct UnkTU2545_Holder;
extern char g_unk_7101aa97a8[];
template <>
struct base::UnkValueHolderTraits<UnkTU2545_Holder> {
    static char* vtable() { return g_unk_7101aa97a8; }
};

extern char g_unk_7101aa9800[];

// The base: its destructor stores its vtable and clears its fields.
struct UnkTU2545_Base {
    char* vtable;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
    u32 field_0x20;

    void Destroy() {
        vtable = g_unk_7101aa9800 + 0x10;
        field_0x10 = 0;
        field_0x18 = 0;
        field_0x20 = 0;
    }
};

struct UnkTU2545_IdPair {
    CStrId first;
    CStrId second;
    u8 pad_0x10[0x18];
};

struct UnkRushSub_b0195 : UnkTU2545_Base {
    u8 pad_0x28[0x1a8 - 0x28];
    UnkTU2545_IdPair ids[5];                                  // 0x1a8
    base::UnkValueHolder<UnkTU2545_Holder> values[8];         // 0x270
};
static_assert(__builtin_offsetof(UnkRushSub_b0195, ids) == 0x1a8, "UnkRushSub_b0195 layout");
static_assert(__builtin_offsetof(UnkRushSub_b0195, values) == 0x270, "UnkRushSub_b0195 layout");
static_assert(sizeof(UnkRushSub_b0195) == 0x5f0, "UnkRushSub_b0195 layout");

void fn_7100dd27ec(UnkRushSub_b0195* self) {
    for (int i = 7; i >= 0; --i)
        base::refl_ValueHolderDtor(&self->values[i]);
    for (int i = 4; i >= 0; --i)
        self->ids[i].~UnkTU2545_IdPair();
    self->Destroy();
}
