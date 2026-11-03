// CChangeStageNavMeshItemComponent unit (tools/tu_map.py): when the stage test (fn_71009b9f6c) holds
// (fn_71009ba334) or fails (fn_71009ba3a0) and the target at +0x30 resolves (fn_71002b3b84), sends
// it the id of the stage name at +0x58 or +0x60 of the settings (+0x40) when that is not empty
// (fn_71009c0f1c), then refreshes it (fn_71009bffdc). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

struct UnkWeakTarget;
struct UnkOwner;
void* fn_71002b3b84(UnkWeakTarget*);
void fn_71009c0f1c(void* object, u64 id);
void fn_71009bffdc(UnkOwner* self, bool arg);

struct UnkStageNameData_b49a4 {
    u8 pad_0x0[0x30];
    u64 hash;
};
struct UnkStageSettings_b49a4 {
    u8 pad_0x0[0x58];
    UnkStageNameData_b49a4* name_a;
    UnkStageNameData_b49a4* name_b;
};
struct UnkChangeStageItem_b49a4 {
    u8 pad_0x0[0x30];
    UnkWeakTarget* target;
    u8 pad_0x38[0x8];
    UnkStageSettings_b49a4* settings;
};

bool fn_71009b9f6c(UnkChangeStageItem_b49a4* self);

static inline void Send(UnkChangeStageItem_b49a4* self, UnkStageNameData_b49a4* UnkStageSettings_b49a4::*name) {
    if (void* target = fn_71002b3b84(self->target)) {
        UnkStageNameData_b49a4* data = self->settings->*name;
        if (__builtin_expect(data != reinterpret_cast<UnkStageNameData_b49a4*>(g_unk_7101d470e0.data), 1)) {
            fn_71009c0f1c(target, data->hash);
            fn_71009bffdc(static_cast<UnkOwner*>(target), true);
        }
    }
}

void fn_71009ba334(UnkChangeStageItem_b49a4* self) {
    if (fn_71009b9f6c(self))
        Send(self, &UnkStageSettings_b49a4::name_a);
}

void fn_71009ba3a0(UnkChangeStageItem_b49a4* self) {
    if (!fn_71009b9f6c(self))
        Send(self, &UnkStageSettings_b49a4::name_b);
}
