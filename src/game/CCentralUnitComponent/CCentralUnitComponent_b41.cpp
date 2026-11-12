// CCentralUnitComponent unit. fn_7100770354: a release step; after the base step (fn_710024fad0) it
// unregisters the component from the game manager (fn_7100de9730 with its key), resets the render
// pass colour g_unk_710164aa90 to (0, 0, 0, 0), releases its two members at +0x7f8 and +0x8a8, and
// sets two render parameters (named by their hashes) to zero. Names and layouts are placeholders.
#include "base/math/CVector4D.h"
#include "game/UnkGameManager.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU39.h"
#include "types.h"

struct UnkSpawnGroupKey;
extern UnkSpawnGroupKey g_unk_7172715400 __attribute__((visibility("hidden")));
void fn_710021a0b4(UnkRenderManager* self, const u64* hash, const base::math::CVector4D* value);
extern const base::math::CVector4D g_unk_71016477b4;
extern const u64 g_unk_710164aa90;

struct UnkSelf_cu41 {
    u8 pad_0x0[0x7f8];
    u8 field_0x7f8[0xb0];
    u8 field_0x8a8[0x8];
};

void fn_710024fad0(UnkSelf_cu41* self);

void fn_7100770354(UnkSelf_cu41* self) {
    fn_710024fad0(self);
    fn_7100de9730(g_unk_7101d47310, &g_unk_7172715400, self);
    fn_710021a0b4(g_unk_7101d47300, &g_unk_710164aa90, &g_unk_71016477b4);
    fn_7100030ce0(self->field_0x7f8);
    fn_710002fa90(self->field_0x8a8);
    f32 a = 0.0f;
    f32 b = 0.0f;
    fn_7100219294(g_unk_7101d47300, 0x0b3b14aea1e3e17full, &a, 0);
    fn_7100219294(g_unk_7101d47300, 0x9ad56a86fea3f8d7ull, &b, 0);
}
