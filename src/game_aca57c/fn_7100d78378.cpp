// An object's weak handle, created on first use, and a setter making the weak reference at 0x398
// refer to another object's handle.
#include "game_s2/UnkWeakHandle.h"

UnkWeakHandle* fn_7100d78378(UnkWeakTarget* self) { return GetWeakHandle(self); }

struct UnkWeakRef398 {
    u8 pad_0x0[0x398];
    UnkWeakHandle* field_0x398;
};

void fn_7100d7b964(UnkWeakRef398* self, UnkWeakTarget* other) {
    UnkWeakHandle* handle = other ? GetWeakHandle(other) : nullptr;
    AssignRef(&self->field_0x398, handle);
}

// The same setter for weak references at other offsets, in other units.
template <u32 Offset>
struct UnkWeakRefAt {
    u8 pad_0x0[Offset];
    UnkWeakHandle* ref;
};
template <>
struct UnkWeakRefAt<0> {
    UnkWeakHandle* ref;
};

template <u32 Offset>
__attribute__((always_inline)) static inline void SetWeakRef(UnkWeakRefAt<Offset>* self, UnkWeakTarget* other) {
    UnkWeakHandle* handle = other ? GetWeakHandle(other) : nullptr;
    AssignRef(&self->ref, handle);
}

void fn_71006b16b0(UnkWeakRefAt<0x18>* self, UnkWeakTarget* other) { SetWeakRef(self, other); }
void fn_710086f0e0(UnkWeakRefAt<0x200>* self, UnkWeakTarget* other) { SetWeakRef(self, other); }
void fn_710092ef94(UnkWeakRefAt<0>* self, UnkWeakTarget* other) { SetWeakRef(self, other); }
void fn_71010e6038(UnkWeakRefAt<0xc30>* self, UnkWeakTarget* other) { SetWeakRef(self, other); }
void fn_71010e6158(UnkWeakRefAt<0xc38>* self, UnkWeakTarget* other) { SetWeakRef(self, other); }
