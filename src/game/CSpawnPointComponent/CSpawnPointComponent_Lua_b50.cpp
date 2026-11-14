// A spawn point's update when it spawns `actor`: resolves the weak reference to its spawner once
// (fn_710023b580), tells the spawner's object (vf_0x178) and calls the Lua function named at +0xd8
// with the owner and the actor. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "game/CDrop.h"
#include "game/CSpawnPointComponent.h"
#include "game/CLuaCallsLogicAction.h"
#include "game/UnkTU2668.h"
#include "game/UnkTU2677.h"
#include "game/lua_lib.h"
#include "types.h"

struct lua_State;

struct UnkName_71011275d8 {
    u8 pad_0x0[0x10];
    const char* str;
    u8 pad_0x18[0x2c];
    union {
        u32 use_count;
    };
};

struct UnkOwnerDef_71011275d8 {
    u8 pad_0x0[0x30];
    UnkName_71011275d8* name;
};

struct UnkSpawner_71011275d8 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual void vf_0xe8();
    virtual void vf_0xf0();
    virtual void vf_0xf8();
    virtual void vf_0x100();
    virtual void vf_0x108();
    virtual void vf_0x110();
    virtual void vf_0x118();
    virtual void vf_0x120();
    virtual void vf_0x128();
    virtual void vf_0x130();
    virtual void vf_0x138();
    virtual void vf_0x140();
    virtual void vf_0x148();
    virtual void vf_0x150();
    virtual void vf_0x158();
    virtual void vf_0x160();
    virtual void vf_0x168();
    virtual void vf_0x170();
    virtual void vf_0x178(const char* name, void* actor);  // notified with the owner's name and the actor
    u8 pad_0x8[0x132];
    bool field_0x13a;
};

void fn_7100d7b964(void* actor, void* self);

// The object told of each spawn: the target of the link at +0x60, seen through fn_71007c04f0.
static UnkSpawner_71011275d8* Spawner(CSpawnPointComponent* self) {
    if (CActor* target = self->field_0x60.Target())
        return static_cast<UnkSpawner_71011275d8*>(fn_71007c04f0(target));
    return nullptr;
}

void fn_71011275d8(CSpawnPointComponent* self, void* actor) {
    if (!actor)
        return;
    if (self->iMaxNumToGenerate < 0) {
        self->ResolveField0x60();
        if (UnkSpawner_71011275d8* spawner = Spawner(self))
            if (spawner->field_0x13a)
                self->field_0x2b8 = false;
    }
    fn_7100d7b964(actor, self);
    self->ResolveField0x60();
    if (UnkSpawner_71011275d8* spawner = Spawner(self)) {
        UnkOwnerDef_71011275d8* def = self->Owner<UnkOwnerDef_71011275d8>();
        ++def->name->use_count;
        const char* str = def->name->str;
        spawner->vf_0x178(str ? str : g_unk_710162e2ed, actor);
    }
    if (self->field_0x30 && fn_7100a506a8(self->field_0x30) && self->sOnEntityGenerated.data &&
        self->sOnEntityGenerated.data[0] && fn_71010c0bb0(g_unk_7172a01d18, self->sOnEntityGenerated.data)) {
        fn_7100fbc4e0(g_unk_7172a01d18, self->field_0x30);
        fn_7100fbc4e0(g_unk_7172a01d18, actor);
        char out[0x100];
        fn_71010c0d80(g_unk_7172a01d18, 2, out, 0x100, nullptr, nullptr);
    }
}
