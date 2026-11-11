// CDemolitionBlockComponent's unit (tools/tu_map.py): moves a lazily resolved holder (0x40 bytes:
// vtable, the callback vector at +0x8, the state at +0x30, the refcounted handle at +0x38) into a
// newly constructed one, as fn_710111fd80 (src/game/CSpawnGroupComponent/
// CSpawnGroupComponent_Holders_ovb0065.cpp) does for another holder type (vtable g_unk_7101a33fa0).
// Names and layouts are placeholders.
#include <new>

#include "base/UnkErasedVector.h"
#include "base/UnkRefHandle.h"
#include "base/UnkVector.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/UnkTU341.h"
#include "types.h"

struct UnkDemolitionHolderBase_ovb0112 {
    char* vtable;
    UnkOwnedErasedVector callbacks;  // 0x8
    u32 state;                       // 0x30

    UnkDemolitionHolderBase_ovb0112() : vtable(reinterpret_cast<char*>(g_unk_7101a20998) + 0x10), state(0) {}
};

struct UnkDemolitionHolder_ovb0112 : UnkDemolitionHolderBase_ovb0112 {
    base::UnkRefHandle handle;  // 0x38

    __attribute__((always_inline)) UnkDemolitionHolder_ovb0112(UnkDemolitionHolder_ovb0112& other) {
        vtable = reinterpret_cast<char*>(g_unk_7101a33fa0) + 0x10;
        handle.ptr = nullptr;
        handle.Set(other.handle.ptr);
        UnkErasedVector* from = &other.callbacks;
        state = other.state;
        fn_710023b29c(&callbacks, from);
        other.state = 0;
        fn_7100ad3588(&other.callbacks);
        if (base::UnkRefCounted* p = other.handle.ptr) {
            if (--p->ref_count == 0)
                delete p;
            other.handle.ptr = nullptr;
        }
    }
};

void fn_71007b1928(UnkDemolitionHolder_ovb0112* self, UnkDemolitionHolder_ovb0112* other) {
    new (self) UnkDemolitionHolder_ovb0112(*other);
}
