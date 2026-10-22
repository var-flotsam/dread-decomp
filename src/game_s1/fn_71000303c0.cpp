// Calls of a deferred callback set (base::refl_DeferredInvoke, as in fn_7100566480 for a 4-byte
// argument), each with its own out-of-line slot accessor: a bool argument in fn_71000303c0
// (fn_71000305d8), a 2-byte one in fn_7100280934 (fn_7100280b4c).
#include "base/UnkCloneHelpers.h"
#include "base/UnkDeferredSet.h"
#include "base/UnkVector.h"
#include "types.h"

using base::UnkDeferredCallback;
using base::UnkDeferredSet;
using base::UnkVector;

struct UnkDeferredInvoke_71000303c0;
struct UnkDeferredInvoke_7100280934;

UnkDeferredCallback<bool>** fn_71000305d8(UnkVector<UnkDeferredCallback<bool>*>* v, u32 i);
UnkDeferredCallback<u16>** fn_7100280b4c(UnkVector<UnkDeferredCallback<u16>*>* v, u32 i);

template <>
struct base::UnkDeferredInvokeTraits<UnkDeferredInvoke_71000303c0> {
    static UnkDeferredCallback<bool>** at(UnkVector<UnkDeferredCallback<bool>*>* v, u32 i) {
        return fn_71000305d8(v, i);
    }
};
template <>
struct base::UnkDeferredInvokeTraits<UnkDeferredInvoke_7100280934> {
    static UnkDeferredCallback<u16>** at(UnkVector<UnkDeferredCallback<u16>*>* v, u32 i) { return fn_7100280b4c(v, i); }
};

__attribute__((flatten)) void fn_71000303c0(UnkDeferredSet<UnkDeferredCallback<bool>>* s, const bool* arg) {
    base::refl_DeferredInvoke<UnkDeferredInvoke_71000303c0, bool>(s, arg);
}

__attribute__((flatten)) void fn_7100280934(UnkDeferredSet<UnkDeferredCallback<u16>>* s, const u16* arg) {
    base::refl_DeferredInvoke<UnkDeferredInvoke_7100280934, u16>(s, arg);
}
