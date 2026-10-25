// Sets up a Dropter AI component's sub-components: runs the base initialiser (fn_7100369490), casts
// the owner's object to the component's actor class, registers the state block at +0x146c8 (added to
// the component's list and initialised from the actor's data), then creates the two effect objects
// at +0x14890 and +0x14898 when they are missing (looked up through the manager's pool at +0x218)
// and configures them.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::CClass;

typedef u64 u64_a4 __attribute__((aligned(4)));

struct UnkActor {
    void** vtable;
    const CClass* vf_0x0() { return reinterpret_cast<const CClass* (*)(UnkActor*)>(vtable[0])(this); }
};

struct UnkEffect {
    u8 pad_0x0[0x130];
    void* field_0x130;
};

struct UnkPool;

struct UnkManager {
    u8 pad_0x0[0x218];
    UnkPool* field_0x218;
};

struct __attribute__((packed)) UnkComponent {
    u8 pad_0x0[0x10];
    UnkActor* field_0x10;
    u8 pad_0x18[0x1600];
    u32 field_0x1618;
    void* field_0x161c[0x1000];
    u8 pad_after[0x1];
};

// The actor pointer and the two effect slots, at +0x14738, +0x14890 and +0x14898 of the component.
struct UnkActorBlock {
    u64 actor;
    u8 pad_0x8[0x150];
    UnkEffect* effect_a;
    UnkEffect* effect_b;
};

extern UnkManager* g_unk_7101d47300;
extern const u64 g_unk_71726db100[] __attribute__((visibility("hidden")));
extern const u64 g_unk_71726db108[] __attribute__((visibility("hidden")));
bool fn_7100369490(UnkComponent* self);
void fn_71004e8ea0(void* state, char* data);
void fn_7100149930(UnkPool* pool, void* a, UnkEffect** out, base::global::CStrId* name);
void fn_7100219ae0(UnkManager* manager, void* entry, u32 flag);
void fn_71001be020(void* target, const u64* key);
void fn_7100149490(UnkEffect* effect, u32 flag);

__attribute__((always_inline)) static inline void CreateEffect(UnkEffect*& slot, const u64* key) {
    UnkManager* manager = g_unk_7101d47300;
    UnkEffect* effect = nullptr;
    fn_7100149930(manager->field_0x218, nullptr, &effect, &g_unk_7101d470e0);
    fn_7100219ae0(manager, effect ? reinterpret_cast<char*>(effect) + 8 : nullptr, 0);
    slot = effect;
    if (effect) {
        fn_71001be020(effect->field_0x130, key);
        fn_7100149490(slot, 0);
    }
}

bool fn_71004ec294(UnkComponent* self) {
    UnkActorBlock* ref = reinterpret_cast<UnkActorBlock*>(reinterpret_cast<char*>(self) + 0x14738);
    bool result = fn_7100369490(self);
    UnkActor* obj = self->field_0x10;
    char* actor = nullptr;
    if (obj && fn_710009d50c(obj->vf_0x0(), fn_71004eb2d0()))
        actor = reinterpret_cast<char*>(obj);
    ref->actor = (u64)actor;
    char* member = reinterpret_cast<char*>(self) + 0x146c8;
    char* list = reinterpret_cast<char*>(&self->field_0x1618) + 4;
    u32 n = self->field_0x1618;
    *reinterpret_cast<u64_a4*>(list + ((u64)n << 3)) = (u64)member;
    self->field_0x1618 = n + 1;
    fn_71004e8ea0(member, reinterpret_cast<char*>(ref->actor) + 0x528);
    if (!ref->effect_a)
        CreateEffect(ref->effect_a, g_unk_71726db100);
    if (!ref->effect_b)
        CreateEffect(ref->effect_b, g_unk_71726db108);
    return result;
}
