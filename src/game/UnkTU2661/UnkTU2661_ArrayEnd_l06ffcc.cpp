// Unit 2661 (CCutscene::CTunableCutscene): the reflected End of an engine array of 0x30-byte holders.
// Names and layouts are placeholders.
#include "base/UnkCollectionIter.h"
#include "base/global/CRntVector.h"
#include "game/UnkTU2661.h"
#include "game/UnkTU341.h"
#include "types.h"

struct UnkTunableHolder_l06ffcc;
struct UnkBoundObj_l06ffcc {
    u8 pad_0x0[0xc8];
    UnkTunableHolder_l06ffcc* field_0xc8[1];
};

// A holder that registers itself with the object it refers to and releases it when it dies.
struct UnkTunableHolder_l06ffcc {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    UnkBoundObj_l06ffcc* owner;
    u64 pad_0x20[2];

    ~UnkTunableHolder_l06ffcc() {
        vtable = g_unk_7101cac988 + 2;
        if (owner->field_0xc8[0] == this)
            owner->field_0xc8[0] = nullptr;
        fn_7100232124(reinterpret_cast<UnkBoundObj*>(owner));
    }
};
static_assert(sizeof(UnkTunableHolder_l06ffcc) == 0x30, "holder size");

void fn_710106ffcc(const base::global::CRntVector<UnkTunableHolder_l06ffcc>* v,
                   base::UnkCollectionIter<UnkTunableHolder_l06ffcc>* out) {
    base::refl_End(v, out);
}
