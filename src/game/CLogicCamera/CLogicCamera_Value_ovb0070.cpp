// CLogicCamera's unit (tools/tu_map.py unit 2655): this unit's copy of fn_7100407018
// (src/game/CCentralUnitAIComponent/CCentralUnitAIComponent_Value_b49.cpp): reads the object behind
// the lazily resolved handle at +0x38 of the holder (source +0x8, state +0x30) as a reflected value of
// the type fn_71010510b4 describes, holding a counted reference to it. The resolver (fn_7101052a30)
// and the handle assignment (fn_7101052314) are this unit's copies of fn_7100353be8 and
// fn_710031d208. Names and layouts are placeholders.
#include "base/UnkErasedVector.h"
#include "base/UnkRefHandle.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflValue;

struct UnkEnumType;
UnkEnumType* fn_71010510b4();
base::UnkRefHandle* fn_7101052314(base::UnkRefHandle* self, const base::UnkRefHandle* other);

struct UnkCountedCamera_ovb0070 : base::UnkRefCounted {
    void* field_0x10;
};

struct UnkCameraRef_ovb0070 {
    UnkCountedCamera_ovb0070* ptr;
    ~UnkCameraRef_ovb0070() {
        if (ptr && --ptr->ref_count == 0)
            delete ptr;
    }
};

UnkCameraRef_ovb0070 fn_7101052a30(void* from);

struct UnkCameraHolder_ovb0070 {
    u8 pad_0x0[0x8];
    u8 source[0x28];
    u32 state;
    u32 pad_0x34;
    base::UnkRefHandle handle;
};

UnkReflValue fn_7101052e7c(UnkCameraHolder_ovb0070* holder) {
    if (holder->state == 1) {
        {
            UnkCameraRef_ovb0070 fresh = fn_7101052a30(holder->source);
            fn_7101052314(&holder->handle, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
        }
        holder->state = 2;
    }
    UnkCameraRef_ovb0070 ref;
    ref.ptr = nullptr;
    if (auto* p = static_cast<UnkCountedCamera_ovb0070*>(holder->handle.ptr)) {
        ++p->ref_count;
        ref.ptr = p;
    }
    auto* type = reinterpret_cast<const base::reflection::CType*>(fn_71010510b4());
    UnkReflValue value;
    value.kind = 2;
    value.field_0x0 = nullptr;
    value.type = nullptr;
    fn_71000a18d4(&value, type, &ref, 0);
    return value;
}

extern void* g_unk_7101a20998[];
extern void* g_unk_7101cac850[];

// The callback set behind the camera's lazily resolved handle: callbacks at +0x8, the state at
// +0x30 (1: not resolved yet, 2: resolved) and the handle at +0x38.
struct UnkCameraCallbackBase_ovb0070 {
    void** vtable;
    UnkOwnedErasedVector callbacks;
    u32 state;

    UnkCameraCallbackBase_ovb0070(const UnkCameraCallbackBase_ovb0070& other) asm("fn_7100da4dcc");
    ~UnkCameraCallbackBase_ovb0070() { vtable = &g_unk_7101a20998[2]; }
};

struct UnkCameraCallbackSet_ovb0070 : UnkCameraCallbackBase_ovb0070 {
    base::UnkRefHandle handle;

    // The copy resolves its own handle from the source.
    UnkCameraCallbackSet_ovb0070(const UnkCameraCallbackSet_ovb0070& other)
        : UnkCameraCallbackBase_ovb0070(other) {
        vtable = &g_unk_7101cac850[2];
        handle.ptr = nullptr;
        if (state == 1) {
            {
                UnkCameraRef_ovb0070 fresh = fn_7101052a30(&callbacks);
                fn_7101052314(&handle, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
            }
            if (handle.ptr && static_cast<UnkCountedCamera_ovb0070*>(handle.ptr)->field_0x10)
                state = 2;
        }
    }
    ~UnkCameraCallbackSet_ovb0070() {
        vtable = &g_unk_7101cac850[2];
        if (base::UnkRefCounted* p = handle.ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
    }
    UnkCameraCallbackSet_ovb0070& operator=(UnkCameraCallbackSet_ovb0070&& other) {
        fn_7101052314(&handle, &other.handle);
        state = other.state;
        UnkErasedVector* from = &other.callbacks;
        fn_710023b29c(&callbacks, from);
        other.state = 0;
        fn_7100ad3588(from);
        if (base::UnkRefCounted* p = other.handle.ptr) {
            if (--p->ref_count == 0)
                delete p;
            other.handle.ptr = nullptr;
        }
        return *this;
    }
};

// The copy assignment: through a resolved copy of `other`, moved in.
UnkCameraCallbackSet_ovb0070* fn_710105312c(UnkCameraCallbackSet_ovb0070* self,
                                            const UnkCameraCallbackSet_ovb0070* other) {
    if (self != other) {
        UnkCameraCallbackSet_ovb0070 copy(*other);
        *self = static_cast<UnkCameraCallbackSet_ovb0070&&>(copy);
    }
    return self;
}
