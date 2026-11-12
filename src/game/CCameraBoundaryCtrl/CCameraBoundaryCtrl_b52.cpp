// CCameraBoundaryCtrl unit: when the controlled object's weak reference at +0xc30 names a live
// target, takes a copy of the name of the target its reference at +0xc38 names (the empty id
// otherwise), lets the sub-area manager (g_unk_7101d47360, created on first use) update
// (fn_7100eeb834) and clears the reference at +0xc30 (fn_71010e6038). Names and layouts are
// placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/UnkTU2577.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

// As src/game_aca57c/fn_7100d78378.cpp defines it.
template <u32 Offset>
struct UnkWeakRefAt {
    u8 pad_0x0[Offset];
    UnkWeakHandle* ref;
};
void fn_71010e6038(UnkWeakRefAt<0xc30>* self, UnkWeakTarget* other);

struct UnkSubAreaManagerView;
void fn_7100eeb834(UnkSubAreaManagerView* self);
struct UnkManager;
extern UnkManager* g_unk_7101d47360;

struct UnkCameraBoundaryNamed {
    u8 pad_0x0[0x20];
    CStrId name;
};

struct UnkCameraBoundaryObject {
    u8 pad_0x0[0xc30];
    UnkWeakHandle* ref_0xc30;
    UnkWeakHandle* ref_0xc38;
};

struct UnkCameraBoundaryOwner {
    u8 pad_0x0[0x10];
    UnkCameraBoundaryObject* object;
};

struct UnkCameraBoundaryCtrl {
    u8 pad_0x0[0xb0];
    UnkCameraBoundaryOwner* owner;
};

static inline UnkSubAreaManagerView* SubAreaManager() {
    if (!g_unk_7101d47360) {
        void* m = fn_71000839a0(0xae8, 0x3f);
        fn_7100ee1f34(m);
        g_unk_7101d47360 = static_cast<UnkManager*>(m);
    }
    return reinterpret_cast<UnkSubAreaManagerView*>(g_unk_7101d47360);
}

void fn_7100766934(UnkCameraBoundaryCtrl* self) {
    UnkCameraBoundaryObject* object = self->owner->object;
    if (!object->ref_0xc30 || !object->ref_0xc30->target)
        return;
    UnkWeakHandle* named = object->ref_0xc38;
    CStrId name(named && named->target
                    ? reinterpret_cast<UnkCameraBoundaryNamed*>(named->target)->name
                    : g_unk_7101d470e0);
    fn_7100eeb834(SubAreaManager());
    UnkCameraBoundaryOwner* owner = self->owner;
    fn_71010e6038(reinterpret_cast<UnkWeakRefAt<0xc30>*>(owner ? owner->object : nullptr), nullptr);
}
