// A spawn point's update when it spawns `actor`: resolves the weak reference to its spawner once
// (fn_710023b580), tells the spawner's object (vf_0x178) and calls the Lua function named at +0xd8
// with the owner and the actor. Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "base/global/CFilePathStrId.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct lua_State;
extern lua_State* g_unk_7172a01d18;
s32 fn_71010c0bb0(lua_State* L, const char* name);
void fn_7100fbc4e0(lua_State* L, void* arg);
s32 fn_71010c0d80(lua_State* L, s32 count, char* out, u32 out_size, void* a, void* b);

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

struct UnkSpawnerRef_71011275d8 {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

void* fn_71007c04f0(void* obj);
void* fn_7100a506a8(void* obj);
void fn_7100d7b964(void* actor, void* self);

struct UnkSpawnPoint_71011275d8 {
    u8 pad_0x0[0x30];
    UnkOwnerDef_71011275d8* field_0x30;
    u8 pad_0x38[0x30];
    u8 field_0x68[0x28];
    u32 field_0x90;
    u8 pad_0x94[0x4];
    UnkSpawnerRef_71011275d8* field_0x98;
    u8 pad_0xa0[0x38];
    const char* field_0xd8;
    u8 pad_0xe0[0x30];
    s32 field_0x110;
    u8 pad_0x114[0x1a4];
    bool field_0x2b8;

    void ResolveSpawner() {
        if (field_0x90 == 1) {
            {
                UnkRefPtr ref = fn_710023b580(field_0x68);
                fn_7100234f78(reinterpret_cast<base::UnkRefHandle*>(&field_0x98),
                              reinterpret_cast<const base::UnkRefHandle*>(&ref));
            }
            field_0x90 = 2;
        }
    }
    UnkSpawner_71011275d8* Spawner() {
        if (field_0x98 && field_0x98->field_0x10)
            return static_cast<UnkSpawner_71011275d8*>(fn_71007c04f0(field_0x98->field_0x10));
        return nullptr;
    }
};
static_assert(__builtin_offsetof(UnkSpawnPoint_71011275d8, field_0x2b8) == 0x2b8, "");

void fn_71011275d8(UnkSpawnPoint_71011275d8* self, void* actor) {
    if (!actor)
        return;
    if (self->field_0x110 < 0) {
        self->ResolveSpawner();
        if (UnkSpawner_71011275d8* spawner = self->Spawner())
            if (spawner->field_0x13a)
                self->field_0x2b8 = false;
    }
    fn_7100d7b964(actor, self);
    self->ResolveSpawner();
    if (UnkSpawner_71011275d8* spawner = self->Spawner()) {
        UnkOwnerDef_71011275d8* def = self->field_0x30;
        ++def->name->use_count;
        const char* str = def->name->str;
        spawner->vf_0x178(str ? str : g_unk_710162e2ed, actor);
    }
    if (self->field_0x30 && fn_7100a506a8(self->field_0x30) && self->field_0xd8 && self->field_0xd8[0] &&
        fn_71010c0bb0(g_unk_7172a01d18, self->field_0xd8)) {
        fn_7100fbc4e0(g_unk_7172a01d18, self->field_0x30);
        fn_7100fbc4e0(g_unk_7172a01d18, actor);
        char out[0x100];
        fn_71010c0d80(g_unk_7172a01d18, 2, out, 0x100, nullptr, nullptr);
    }
}
