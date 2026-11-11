// CCharClassLogicSoundEvent's unit: the default constructor on the event base (fn_7100067ea4): the
// vtable, then two empty string ids (+0x10, +0x18), the id constructor inlined. Names and
// layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/global/timeline/CEvent.h"
#include "game/CCharClassLogicSoundEvent.h"
#include "types.h"

using base::global::CStrId;

struct UnkLogicSoundEvent {
    void** vtable;
    u8 unk_0x8[0x8];
    CStrId id_0x10;
    CStrId id_0x18;
};

// An empty id: the string id constructor for "" (the length folded) inlined.
__attribute__((always_inline)) static inline void InitEmptyId(CStrId* id) {
    id->data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    id->data = &g_unk_7101d47108->empty;
}

void fn_7100cc60dc(UnkLogicSoundEvent* self) {
    fn_7100067ea4(self);
    self->vtable = reinterpret_cast<void**>(g_unk_7101aa5d10) + 2;
    InitEmptyId(&self->id_0x10);
    InitEmptyId(&self->id_0x18);
}
