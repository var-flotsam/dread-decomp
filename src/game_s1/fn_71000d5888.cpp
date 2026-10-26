// Copies for other element types, naming their own callees and data (the comments are adapted from the original's): of
// fn_7100ad2004 (src/game_aca57c/fn_7100ad2004.cpp): fn_71000d5888, fn_71000e027c, fn_710027370c; and of fn_7100c8f61c
// (src/game_aca57c/fn_7100ad2800.cpp): fn_71000e0e8c, fn_7100273dbc, fn_71000d5f4c. Move-assignments of engine arrays
// (base/UnkVector.h), and the reflected End of the same arrays, whose elements' destructor is out of line (0x18-byte
// elements destroyed by fn_71000a1c60, 0x30-byte ones by fn_710007ecd4, 0x50-byte ones by fn_710026ead0).
#include "base/UnkCollectionIter.h"
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::UnkVector;

// An element of `Size` bytes that is a T, destroyed out of line.
template <u32 Size, class T, void (*Destroy)(T*)>
struct UnkOutOfLineElement {
    u64 unk_0x0[Size / 8];

    ~UnkOutOfLineElement() { Destroy(static_cast<T*>(static_cast<void*>(this))); }
};

void fn_710026ead0(void* self);
// The 0x30-byte elements are strings (base/global/CRntString.h).
inline void DestroyString(void* self) { fn_710007ecd4(static_cast<base::global::CRntString*>(self)); }
using UnkElement18 = UnkOutOfLineElement<0x18, base::reflection::UnkReflValue, fn_71000a1c60>;
using UnkElement30 = UnkOutOfLineElement<0x30, void, DestroyString>;
using UnkElement50 = UnkOutOfLineElement<0x50, void, fn_710026ead0>;

UnkVector<UnkElement18>& fn_71000d5888(UnkVector<UnkElement18>* self, UnkVector<UnkElement18>&& other) {
    return *self = static_cast<UnkVector<UnkElement18>&&>(other);
}
UnkVector<UnkElement30>& fn_71000e027c(UnkVector<UnkElement30>* self, UnkVector<UnkElement30>&& other) {
    return *self = static_cast<UnkVector<UnkElement30>&&>(other);
}
UnkVector<UnkElement50>& fn_710027370c(UnkVector<UnkElement50>* self, UnkVector<UnkElement50>&& other) {
    return *self = static_cast<UnkVector<UnkElement50>&&>(other);
}

using UnkElement18Iter = base::UnkCollectionIter<UnkElement18>;
using UnkElement30Iter = base::UnkCollectionIter<UnkElement30>;
using UnkElement50Iter = base::UnkCollectionIter<UnkElement50>;

void fn_71000e0e8c(const UnkVector<UnkElement30>* v, UnkElement30Iter* out) {
    *out = UnkElement30Iter(v->count, v->data + v->count);
}
void fn_7100273dbc(const UnkVector<UnkElement50>* v, UnkElement50Iter* out) {
    *out = UnkElement50Iter(v->count, v->data + v->count);
}
void fn_71000d5f4c(const UnkVector<UnkElement18>* v, UnkElement18Iter* out) {
    *out = UnkElement18Iter(v->count, v->data + v->count);
}
