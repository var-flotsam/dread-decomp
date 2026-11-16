// CTunableAIManager unit: adds a key to (or removes it from) the key list at +0x68; when that
// changes whether the list is empty, sets or clears flag 8 on the collision named g_unk_71727bc7c8
// of every door life and door Emmy FX component. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkTU39.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkCollisionContainer;
struct UnkCollisionObject;

UnkCollisionObject* fn_7100863814(UnkCollisionContainer* self, const CStrId* id, const u32* extra);
void fn_71008639a8(UnkCollisionContainer* self, const CStrId* id, const u32* extra);

extern CStrId g_unk_71727bc7c8 __attribute__((visibility("hidden")));

struct UnkDoorEntry_dae8a4 {
    u8 unk_0x0[0x10];
    UnkReflObject* component;
};

struct UnkDoorEntries_dae8a4 {
    UnkDoorEntry_dae8a4** data;
    u32 count;
};

// The registered door life and door Emmy FX components.
UnkDoorEntries_dae8a4* fn_71008cdf6c();
struct UnkDoorList_b0193;
UnkDoorList_b0193* fn_71007b5d90();

struct UnkDoorOwner_dae8a4 {
    u8 unk_0x0[0x2a0];
    UnkCollisionContainer* collision;
};

struct UnkDoorComponent_dae8a4 {
    u8 unk_0x0[0x30];
    UnkDoorOwner_dae8a4* owner;
};

struct UnkDoorKeyOwner_dae8a4 {
    u8 unk_0x0[0x68];
    UnkGuiKeyList keys;
};

static inline void SetDoorFlag(UnkDoorEntries_dae8a4* doors, const base::reflection::CClass* (*get_class)(),
                               bool set) {
    for (u64 i = 0; i < doors->count; ++i) {
        UnkDoorEntry_dae8a4* entry = doors->data[i];
        if (!entry)
            continue;
        UnkReflObject* component = entry->component;
        if (!component)
            continue;
        if (!fn_710009d50c(component->vf_0x0(), get_class()))
            continue;
        UnkCollisionContainer* collision =
            reinterpret_cast<UnkDoorComponent_dae8a4*>(component)->owner->collision;
        if (!collision)
            continue;
        if (set) {
            u32 flag = 8;
            fn_7100863814(collision, &g_unk_71727bc7c8, &flag);
        } else {
            u32 flag = 8;
            fn_71008639a8(collision, &g_unk_71727bc7c8, &flag);
        }
    }
}

void fn_7100dae8a4(UnkDoorKeyOwner_dae8a4* self, bool add, CStrId* key) {
    bool was = fn_710002fb70(&self->keys);
    if (add)
        fn_710002fd10(&self->keys, key);
    else
        fn_710002ffb4(&self->keys, key);
    if (fn_710002fb70(&self->keys) == was)
        return;
    SetDoorFlag(fn_71008cdf6c(), fn_71008cb078, add);
    SetDoorFlag(reinterpret_cast<UnkDoorEntries_dae8a4*>(fn_71007b5d90()), fn_71007b55e4, add);
}
