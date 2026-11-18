// Unit 2661 (CCutscene::CTunableCutscene): the reflected Begin of an engine array of 0x30-byte holders.
// Names and layouts are placeholders.
#include "base/UnkCollectionIter.h"
#include "base/global/CRntVector.h"
#include "game/UnkTU2661.h"
#include "game/UnkTU341.h"
#include "types.h"

struct UnkTunableHolder_l06fe80;
struct UnkBoundObj_6fe80 {
    u8 pad_0x0[0xc8];
    UnkTunableHolder_l06fe80* field_0xc8;
};

// A holder that registers itself with the object it refers to and releases it when it dies.
struct UnkTunableHolder_l06fe80 {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    UnkBoundObj_6fe80* owner;
    u64 pad_0x20[2];

    ~UnkTunableHolder_l06fe80() {
        vtable = g_unk_7101cac988 + 2;
        if (owner->field_0xc8 == this)
            owner->field_0xc8 = nullptr;
        fn_7100232124(reinterpret_cast<UnkBoundObj*>(owner));  // the shared UnkBoundObj has no layout yet
    }
};
static_assert(sizeof(UnkTunableHolder_l06fe80) == 0x30, "holder size");

void fn_710106fe80(const base::global::CRntVector<UnkTunableHolder_l06fe80>* v,
                   base::UnkCollectionIter<UnkTunableHolder_l06fe80>* out) {
    base::refl_Begin(v, out);
}
