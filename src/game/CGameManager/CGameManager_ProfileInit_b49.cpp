// CGameManager's unit (tools/tu_map.py unit 2553, CGameManager::CTunableGameManager by hint): sets the
// profile file's name parts ("game", ".bmssv" at +0xc78 and +0xca8; the two names held in
// g_unk_7101aa99b0 and g_unk_7101aa99b8 at +0x8a0 and +0x8d0), then tells the user data's owner
// (created on first use; fn_7101203768), finishes the pending profile operation (fn_7100dee46c) and
// hands the owner a callback (fn_7100df81fc through fn_71012037bc). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2768.h"
#include "types.h"

using base::global::CRntString;

struct UnkUserDataOwner;
void fn_71012037bc(UnkUserDataOwner* self, void (*callback)());
void fn_7100df81fc();

extern const char* g_unk_7101aa99b0;
extern const char* g_unk_7101aa99b8;

static inline UnkUserDataOwner* GetOwner() {
    if (!g_unk_7101d47308) {
        UnkUserDataOwner* owner = static_cast<UnkUserDataOwner*>(fn_71000839a0(0x2b8, 0x3f));
        fn_710120320c(owner);
        g_unk_7101d47308 = owner;
    }
    return g_unk_7101d47308;
}

struct UnkGameManagerProfile {
    u8 pad_0x0[0x8a0];
    CRntString field_0x8a0;
    CRntString field_0x8d0;
    u8 pad_0x900[0xc78 - 0x900];
    CRntString field_0xc78;
    CRntString field_0xca8;
};
static_assert(__builtin_offsetof(UnkGameManagerProfile, field_0xca8) == 0xca8, "layout");

void fn_7100df8128(UnkGameManagerProfile* self) {
    fn_710007e77c(&self->field_0xc78, "game");
    fn_710007e77c(&self->field_0xca8, ".bmssv");
    const char* a = g_unk_7101aa99b0;
    const char* b = g_unk_7101aa99b8;
    fn_710007e77c(&self->field_0x8a0, a);
    fn_710007e77c(&self->field_0x8d0, b);
    fn_7101203768(GetOwner());
    fn_7100dee46c(reinterpret_cast<UnkGameManager*>(self));
    fn_71012037bc(GetOwner(), fn_7100df81fc);
}
