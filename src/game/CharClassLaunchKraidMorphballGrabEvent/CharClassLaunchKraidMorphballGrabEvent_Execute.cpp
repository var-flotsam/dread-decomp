// CharClassLaunchKraidMorphballGrabEvent (unit 1808 of tools/tu_map.py, the name from reflection):
// finds the entity named by the unit's string id (g_unk_7172770220, by its hash) in the game
// manager and, when its component at +0x240 is of the class fn_71005fac74 returns, hands it to
// fn_7100600c98. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::reflection::CClass;

struct UnkWeakTarget;
extern u32 g_unk_71727bebb8;
UnkWeakTarget* fn_7100debbc8(UnkGameManager* manager, const u64* key, const u32* kind);

struct UnkKraidNameData {
    u8 unk_0x0[0x30];
    u64 hash;
};
extern UnkKraidNameData* g_unk_7172770220 __attribute__((visibility("hidden")));  // a string id's data

struct UnkKraidComponent {
    void** vtable;
    const CClass* Type() { return reinterpret_cast<const CClass* (*)(UnkKraidComponent*)>(vtable[0])(this); }
};
struct UnkKraidEntity {
    u8 unk_0x0[0x240];
    UnkKraidComponent* component;
};
void fn_7100600c98(UnkKraidComponent* component);

void fn_7100bbaa5c() {
    UnkGameManager* manager = g_unk_7101d47310;
    u64 key;
    alignas(8) u32 kind;
    u32 kind_value = g_unk_71727bebb8;
    key = g_unk_7172770220->hash;
    kind = kind_value;
    UnkKraidEntity* entity = reinterpret_cast<UnkKraidEntity*>(fn_7100debbc8(manager, &key, &kind));
    if (!entity)
        return;
    UnkKraidComponent* component = entity->component;
    if (component && fn_710009d50c(component->Type(), fn_71005fac74()))
        fn_7100600c98(component);
}
