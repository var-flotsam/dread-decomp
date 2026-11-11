#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

using base::global::CStrId;

struct UnkWeakTarget;
struct UnkSpawnRegistered_ovb0065;
extern u32 g_unk_71727bebb8;
UnkWeakTarget* fn_7100debbc8(UnkGameManager* g_unk_7101d47310, const u64* key, const u32* kind);
UnkSpawnRegistered_ovb0065* fn_7100360044(void* object);
void fn_7100df31b8(UnkGameManager* self, u32 a, bool b, bool c, CStrId id, u32 d);
void fn_7100df3418(UnkGameManager* self, u32 a, bool b, void* c);

struct UnkRegisteredLink_b0181 {
    u8 pad_0x0[0x10];
    UnkWeakTarget* field_0x10;
};
struct UnkRegistered_b0181 {
    u8 pad_0x0[0x40];
    UnkRegisteredLink_b0181* field_0x40;
};

struct UnkSlotNode_b0181 {
    u8 pad_0x0[0x18];
    struct UnkSlotNameHolder_b0181* name;  // 0x18
};
struct UnkSlotNameHolder_b0181 {
    u8 pad_0x0[0x30];
    u64 hash;  // 0x30
};
struct UnkSlotOwner_b0181 {
    u8 pad_0x0[0x10];
    UnkSlotNode_b0181* node;  // 0x10
};

void fn_71010661a0(UnkSlotOwner_b0181* self, u8* a, u8* b);

UnkWeakTarget* fn_7101066438(UnkSlotOwner_b0181* self) {
    u64 key;
    alignas(8) u32 kind;
    u8 first = 0;
    u8 second = 0;
    fn_71010661a0(self, &first, &second);
    u8 changed = first;
    if (changed) {
        fn_7100df31b8(g_unk_7101d47310, 1, true, true, g_unk_7101d470e0, 0);
    } else if (second) {
        fn_7100df3418(g_unk_7101d47310, 1, true, nullptr);
    }
    u32 kind_value = g_unk_71727bebb8;
    key = self->node->name->hash;
    kind = kind_value;
    UnkWeakTarget* target = fn_7100debbc8(g_unk_7101d47310, &key, &kind);
    if (!target)
        return nullptr;
    if (second | changed) {
        key = fn_7100001570("stop_fx", 7);
        fn_7100d7c828(reinterpret_cast<UnkActor*>(target), &key, false);
        key = fn_7100001570("stop_materialtransition", 0x17);
        fn_7100d7c828(reinterpret_cast<UnkActor*>(target), &key, false);
        return target;
    }
    UnkRegistered_b0181* registered = reinterpret_cast<UnkRegistered_b0181*>(fn_7100360044(target));
    if (!registered)
        return target;
    return registered->field_0x40 ? registered->field_0x40->field_0x10 : nullptr;
}
