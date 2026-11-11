#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

void fn_7100dec7a0(void* self);
void fn_7100400364(void* dst, void* src);

struct UnkTgmScreenFlags_b205 {
    bool field_0x0;
    bool field_0x1;
    bool field_0x2;
    bool field_0x3;
    u8 pad_0x4[4];
    u8 field_0x8[0x100];
    bool field_0x108;
};
struct UnkTgmScreen_b205 {
    u8 pad_0x0[0x2328];
    s32 mode;
    u32 value;
    CStrId id;
    UnkTgmScreenFlags_b205 flags;
};
static_assert(__builtin_offsetof(UnkTgmScreen_b205, flags) == 0x2338, "layout");

static inline __attribute__((always_inline)) void SetScreen(UnkTgmScreen_b205* self, s32 mode, u32 value, bool a,
                                                            bool keep, const CStrId* id, bool c) {
    bool same = self->mode == mode;
    bool either = same | keep;
    bool kept;
    if (same) {
        if (!keep)
            kept = self->flags.field_0x1;
        else
            kept = either;
    } else {
        kept = either;
    }
    self->mode = mode;
    self->value = value;
    self->flags.field_0x0 = a;
    self->flags.field_0x1 = kept;
    self->id = *id;
    self->flags.field_0x2 = c;
}

void fn_7100df3300(UnkTgmScreen_b205* self, s32 mode, u32 value, bool a, bool keep, const CStrId* id, bool c, void* x7) {
    SetScreen(self, mode, value, a, keep, id, c);
    if (x7) {
        self->flags.field_0x3 = true;
        fn_7100400364(self->flags.field_0x8, x7);
    }
    if (!self->flags.field_0x108)
        fn_7100dec7a0(self);
}

void fn_7100df3418(UnkGameManager* manager, bool a, bool b, void* x3) {
    CStrId id = g_unk_7101d470e0;
    fn_7100df3300(reinterpret_cast<UnkTgmScreen_b205*>(manager), 2, 0, a, b, &id, false, x3);
}

static inline __attribute__((always_inline)) void SetScreenCopy(UnkTgmScreen_b205* self, s32 mode, u32 value, bool a,
                                                                bool keep, CStrId id, bool c) {
    SetScreen(self, mode, value, a, keep, &id, c);
    if (!self->flags.field_0x108)
        fn_7100dec7a0(self);
}

void fn_7100df31b8(UnkGameManager* manager, bool a, bool b, u32 c, CStrId id, bool d) {
    UnkTgmScreen_b205* self = reinterpret_cast<UnkTgmScreen_b205*>(manager);
    SetScreenCopy(self, 1, c, a, b, id, d);
}
