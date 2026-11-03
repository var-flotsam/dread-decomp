// In the CCharClassWaterPoolComponent unit: fn_71009085dc runs the base step (fn_7100878310), hands
// the member at +0xcc0 a counted copy of the owner's (+0x18) name id at +0x30 (fn_710090869c, the
// shape of base::refl_CallWithOwnerId in base/UnkCloneHelpers.h), registers the object with the
// game manager under the id g_unk_7172735088 (fn_7100de9540) and clears the flag at +0xd48. Names and
// layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::UnkStrIdMember;

extern u64 g_unk_7172735088 __attribute__((visibility("hidden")));
void fn_7100878310(char*);
void fn_710090869c(char* member, UnkStrIdMember* id);

void fn_71009085dc(char* self) {
    fn_7100878310(self);
    char* m = self + 0xcc0;
    base::global::UnkStringData* d =
        *reinterpret_cast<base::global::UnkStringData* const*>(*reinterpret_cast<char**>(self + 0x18) + 0x30);
    {
        union Id {
            UnkStrIdMember id;
            Id() {}
            ~Id() { id.~UnkStrIdMember(); }
        } u;
        u.id.data = d;
        if (&g_unk_7101d47108->empty != d)
            ++d->ref_count;
        fn_710090869c(m, &u.id);
    }
    fn_7100de9540(g_unk_7101d47310, &g_unk_7172735088, reinterpret_cast<UnkWeakTarget*>(self));
    self[0xd48] = 0;
}
