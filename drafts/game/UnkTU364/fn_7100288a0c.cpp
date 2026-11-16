// Unit 364: calls each callback of a deferred set with a 4-byte id-and-kind key (base::refl_DeferredInvoke),
// dropping those that return non-zero, with its own out-of-line slot accessor (fn_7100288c48).
// Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkDeferredSet.h"
#include "base/UnkIdKindKey.h"
#include "base/global/CRntVector.h"
#include "types.h"

using base::UnkDeferredCallback;
using base::UnkDeferredSet;
using base::global::CRntVector;

struct UnkDeferredInvoke_7100288a0c;

UnkDeferredCallback<UnkIdKindKey>** fn_7100288c48(CRntVector<UnkDeferredCallback<UnkIdKindKey>*>* v, u32 i);

template <>
struct base::UnkDeferredInvokeTraits<UnkDeferredInvoke_7100288a0c> {
    static UnkDeferredCallback<UnkIdKindKey>** at(CRntVector<UnkDeferredCallback<UnkIdKindKey>*>* v, u32 i) {
        return fn_7100288c48(v, i);
    }
};

// Calls one callback with the key.
inline u32 Call(UnkDeferredCallback<UnkIdKindKey>* cb, UnkIdKindKey key) {
    return cb->fn(cb->context, key);
}

void fn_7100288a0c(UnkDeferredSet<UnkDeferredCallback<UnkIdKindKey>>* s, const UnkIdKindKey* key) {
    base::refl_DeferredInvokeWith<UnkDeferredInvoke_7100288a0c>(
        s, [key](UnkDeferredCallback<UnkIdKindKey>* cb) { return Call(cb, *key); });
}
