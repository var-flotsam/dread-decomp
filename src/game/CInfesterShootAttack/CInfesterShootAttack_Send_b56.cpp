// The same function in the CInfesterShootAttack unit (fn_71005c70e0) and the CNailongThornsAttack
// unit (fn_710060cd30): sends the hash of the string id `id` to the attack's target (+0x58) with
// mode 2 (descriptors as UnkSpitSend, src/game_s2/UnkSpitSend.h; the shape of
// src/game/CAttack/CAttack_Send_b55.cpp's send). Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkShootAttack_b56a3 {
    u8 pad_0x0[0x58];
    void* target;
};

static inline __attribute__((always_inline)) void Send_b56a3(UnkShootAttack_b56a3* self, const CStrId* id) {
    u64 hash = *reinterpret_cast<u64*>(reinterpret_cast<char*>(id->data) + 0x30);
    void* target = self->target;
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = 2;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    fn_7101176690(target, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
}

void fn_71005c70e0(UnkShootAttack_b56a3* self, const CStrId* id) { Send_b56a3(self, id); }
void fn_710060cd30(UnkShootAttack_b56a3* self, const CStrId* id) { Send_b56a3(self, id); }
