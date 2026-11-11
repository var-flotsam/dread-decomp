// Unit 111 (base::tunable::CTunable): equality of cached iterator values (refl_IterValueEqualByValue on
// UnkIterValue, base/UnkCollectionIter.h) whose index is a 0x30-byte key compared by fn_710007f120
// and whose element is 64 bits (fn_710013812c, fn_71001384cc: reflection slot 0x58 of two types).
// Names are placeholders.
#include "base/UnkCollectionIter.h"
#include "base/global/CRntString.h"
#include "game/UnkTU57.h"
#include "types.h"

struct UnkCompared_0x30 {
    u64 words[6];
};

// The key (a base::global::CRntString) as an iterator index: compared through fn_710007f120 and
// assigned through fn_710007eaa0.
struct UnkTunableKey_b58 {
    UnkCompared_0x30 key;
    bool operator!=(const UnkTunableKey_b58& o) const { return !fn_710007f120(&key, &o.key); }
    UnkTunableKey_b58& operator=(const UnkTunableKey_b58& o) {
        fn_710007eaa0(reinterpret_cast<base::global::CRntString*>(this),
                      reinterpret_cast<const base::global::CRntString*>(&o));
        return *this;
    }
};

using UnkTunableIterValue_b58 = base::UnkIterValue<u64, UnkTunableKey_b58>;

bool fn_710013812c(const UnkTunableIterValue_b58& a, const UnkTunableIterValue_b58& b) {
    return base::refl_IterValueEqualByValue(a, b);
}

bool fn_71001384cc(const UnkTunableIterValue_b58& a, const UnkTunableIterValue_b58& b) {
    return base::refl_IterValueEqualByValue(a, b);
}

// Copy- and move-assignment of the same values (slots 0x48 and 0x50 of fn_710013812c's type).
void* fn_71001382b8(void* dst, const void* src) {
    return base::refl_IterValueCopy<u64, UnkTunableKey_b58>(dst, src);
}

UnkTunableIterValue_b58* fn_7100138370(UnkTunableIterValue_b58* self, UnkTunableIterValue_b58* other) {
    return &(*self = static_cast<UnkTunableIterValue_b58&&>(*other));
}
