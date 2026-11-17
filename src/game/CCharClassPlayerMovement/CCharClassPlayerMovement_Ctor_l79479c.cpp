// CCharClassPlayerMovement's unit (tools/tu_map.py unit 1197): a constructor over the base (fn_710078e668):
// its own vtable, then five empty string ids. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CCharClassCharacterMovement.h"
#include "types.h"

using base::global::CStrId;

extern char g_unk_7101a6f760[];

struct UnkPlayerMovementClass_l79479c {
    void** vtable;
    u8 pad_0x8[0x68];
    CStrId field_0x70;
    CStrId field_0x78;
    CStrId field_0x80;
    CStrId field_0x88;
    CStrId field_0x90;
};
static_assert(__builtin_offsetof(UnkPlayerMovementClass_l79479c, field_0x90) == 0x90, "layout");

// An empty id, creating the pool first when there is none.
static inline void MakeEmpty(CStrId* id) {
    id->data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    id->data = &g_unk_7101d47108->empty;
}

void fn_710079479c(UnkPlayerMovementClass_l79479c* self) {
    fn_710078e668(self);
    self->vtable = reinterpret_cast<void**>(g_unk_7101a6f760 + 0x10);
    MakeEmpty(&self->field_0x70);
    MakeEmpty(&self->field_0x78);
    MakeEmpty(&self->field_0x80);
    MakeEmpty(&self->field_0x88);
    MakeEmpty(&self->field_0x90);
}
