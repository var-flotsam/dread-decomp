// Unit 2535 of tools/tu_map.py (no name hint): the copy constructor of a camera callback set's base (the sibling files
// declare it with the asm label fn_7100da4dcc; Clang 9 ignores the label, so an alias names the definition). Its source
// is really a pooled path name: "{EMPTY}" gives state 0, "{BROKEN}" state 2, any other is parsed into the callbacks.
// Names and layouts are placeholders.
#include "base/UnkErasedVector.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/UnkTU341.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

extern CStrId g_unk_71727bbfc8;
extern CStrId g_unk_71727bbfd0;
void fn_7100ad3118(UnkErasedVector* self, const CRntString* text);

// A pooled path name: the pointer to its record, which holds the text and a reference count.
struct UnkPathRecord_a4dcc {
    u8 pad_0x0[0x10];
    CRntString text;  // 0x10
    u8 pad_0x40[4];
    union {
        s32 ref_count;  // 0x44
    };
};

struct UnkPathId_a4dcc {
    UnkPathRecord_a4dcc* record;
};

// The array fn_7100ad3118 builds in place.
union UnkBuilt_a4dcc {
    UnkOwnedErasedVector values;
    UnkBuilt_a4dcc() {}
    ~UnkBuilt_a4dcc() { values.~UnkOwnedErasedVector(); }
};

struct UnkCameraCallbackBase_ovb0070 {
    void** vtable;
    UnkOwnedErasedVector callbacks;
    u32 state;

    UnkCameraCallbackBase_ovb0070(const UnkCameraCallbackBase_ovb0070& other) asm("fn_7100da4dcc");
};

// Clang 9 ignores the asm label on the constructor, so an alias gives the definition the address's name.
#if defined(__clang__) && __clang_major__ <= 9
void fn_7100da4dcc(UnkCameraCallbackBase_ovb0070*, const UnkCameraCallbackBase_ovb0070&)
    __attribute__((alias("_ZN29UnkCameraCallbackBase_ovb0070C1ERKS_")));
#endif

// The source is really a pooled path name (its record pointer is the first word).
UnkCameraCallbackBase_ovb0070::UnkCameraCallbackBase_ovb0070(const UnkCameraCallbackBase_ovb0070& other)
    : vtable(&g_unk_7101a20998[2]), callbacks(), state(0) {
    const UnkPathId_a4dcc& id = reinterpret_cast<const UnkPathId_a4dcc&>(other);
    if (reinterpret_cast<void*>(id.record) == reinterpret_cast<void*>(g_unk_71727bbfc8.data))
        state = 0;
    else if (reinterpret_cast<void*>(id.record) == reinterpret_cast<void*>(g_unk_71727bbfd0.data))
        state = 2;
    else {
        ++id.record->ref_count;
        {
            UnkBuilt_a4dcc built;
            fn_7100ad3118(&built.values, &id.record->text);
            fn_710023b29c(&callbacks, &built.values);
        }
        state = 1;
    }
}
