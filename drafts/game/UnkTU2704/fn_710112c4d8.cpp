#include "game/UnkGameManager.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

extern u32 g_unk_71727bebb8;

struct UnkSpawnKey_ovb0171 {
    u8 pad_0x0[0x30];
    u64 id;  // 0x30
};

struct UnkSpawnWeakHolder_ovb0171 {
    u8 pad_0x0[0x380];
    UnkWeakHandle* handle;  // 0x380
};

void fn_710112c4d8(UnkSpawnWeakHolder_ovb0171* self, UnkSpawnKey_ovb0171** key) {
    UnkSpawnKey_ovb0171* k = *key;
    UnkGameManager* manager = g_unk_7101d47310;
    u64 id;
    alignas(8) u32 flags;
    u32* flags_src = &g_unk_71727bebb8;
    id = k->id;
    flags = *flags_src;
    UnkWeakTarget* target = static_cast<UnkWeakTarget*>(fn_7100deba8c(manager, &id, &flags));
    if (target)
        AssignRef(&self->handle, GetWeakHandle(target));
    else
        ResetRef(&self->handle);
}
