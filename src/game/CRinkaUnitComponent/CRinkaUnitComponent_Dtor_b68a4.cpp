// The CRinkaUnitComponent unit. fn_710065f364: the component's deleting destructor. Members (three value
// holders at +0x58, +0xc8 and +0x138 whose base part, vtable 0x7101a5be00, is refl_ValueHolderDtor in
// base/UnkCloneHelpers.h; the zeroed record at +0x1a8, vtable 0x7101a5be58, clears 0x10..0x24; the counted
// reference at +0x260) are destroyed in reverse order before the component base (fn_71010c4194). Names and
// layouts are placeholders.
#include "base/UnkVector.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

extern void* g_unk_7101a5bc30[];
extern void* g_unk_7101a5be58[];
extern void* g_unk_7101a5be00[];

struct UnkRinkaPair_b68a4 {
    u64 a, b;
    ~UnkRinkaPair_b68a4() {
        a = 0;
        b = 0;
    }
};

struct UnkRinkaHolderBase_b68a4 {
    void* vtable;
    u64 field_0x8;
    u32 id;  // 0x10
    base::reflection::UnkReflValue value;
    ~UnkRinkaHolderBase_b68a4() {
        vtable = reinterpret_cast<char*>(g_unk_7101a5be00) + 0x10;
        fn_71000a1c60(&value);
    }
};

// The derived part's pairs are cleared (their destructors) before the base part runs.
struct UnkRinkaHolder_b68a4 : UnkRinkaHolderBase_b68a4 {
    UnkRinkaPair_b68a4 pairs[4];
};

struct UnkRinkaZeroed_b68a4 {
    void* vtable;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
    u32 field_0x20;
    ~UnkRinkaZeroed_b68a4() {
        vtable = reinterpret_cast<char*>(g_unk_7101a5be58) + 0x10;
        field_0x10 = 0;
        field_0x18 = 0;
        field_0x20 = 0;
    }
};

struct UnkRinkaBase_b68a4 : UnkBaseComponent {
    u8 pad_0x40[0x8];
    s32 field_0x48;
    s32 field_0x4c;
    s32 field_0x50;
    u8 pad_0x54[0x4];
    ~UnkRinkaBase_b68a4() { fn_71010c4194(this); }
};

struct UnkRinkaUnit_b68a4 : UnkRinkaBase_b68a4 {
    UnkRinkaHolder_b68a4 holder_0x58;
    UnkRinkaHolder_b68a4 holder_0xc8;
    UnkRinkaHolder_b68a4 holder_0x138;
    UnkRinkaZeroed_b68a4 zeroed_0x1a8;
    u8 pad_0x1d0[0x7c];
    u32 field_0x24c;
    u16 field_0x250;
    u8 pad_0x252[0x6];
    u32 field_0x258;
    UnkRefObject* ref;  // 0x260
    u64 field_0x268;
    u64 field_0x270;
    ~UnkRinkaUnit_b68a4() {
        vtable = reinterpret_cast<void**>(reinterpret_cast<char*>(g_unk_7101a5bc30) + 0x10);
        if (UnkRefObject* p = ref) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};
static_assert(__builtin_offsetof(UnkRinkaUnit_b68a4, field_0x24c) == 0x24c, "layout");
static_assert(__builtin_offsetof(UnkRinkaUnit_b68a4, ref) == 0x260, "layout");

void fn_710065f364(UnkRinkaUnit_b68a4* self) {
    self->~UnkRinkaUnit_b68a4();
    fn_7100083c00(self);
}
