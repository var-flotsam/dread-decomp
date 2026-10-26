// Binds an actor and caches values from its component (0x240 -> 0x1610): two computed ids,
// several parameters, three values from its transform, the table entry for its key in the
// navmesh manager's current mesh (game/UnkNavMeshManager.h; the manager is created on first
// use), and two derived pointers.
#include "base/UnkMatrix44.h"
#include "base/UnkVector.h"
#include "game/UnkNavMeshManager.h"
#include "types.h"

struct UnkEntry_7100286798 {
    u8 unk_0x0[0x28];
};

// The navigation mesh, as far as this file reads it: the key/value table at +0x338.
struct UnkNavMesh {
    u8 unk_0x0[0x338];
    UnkEntry_7100286798* values;
    void** keys;
    u32 count;

    UnkEntry_7100286798* Find(void* const& key) {
        for (u32 i = 0; i < count; ++i) {
            if (keys[i] == key)
                return &values[i];
        }
        return &values[count];
    }
    UnkEntry_7100286798* Get(void* const& key) {
        UnkEntry_7100286798* it = Find(key);
        return it == &values[count] ? nullptr : it;
    }
};

static inline UnkNavMeshManager* GetManager() {
    if (!g_unk_71726a4510) {
        UnkNavMeshManager* m = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(m);
        g_unk_71726a4510 = m;
    }
    return g_unk_71726a4510;
}

struct UnkSource_7100286798 {
    u8 unk_0x0[0xec];
    u32 field_0xec;
    u32 field_0xf0;
    u8 unk_0xf4[0xd4];
    void* key;  // 0x1c8
    u64 field_0x1d0;
    u8 unk_0x1d8[0x20];
    u32 field_0x1f8;
    u8 unk_0x1fc[0xc];
    u32 field_0x208;
    u8 unk_0x20c[0xc];
    u32 field_0x218;
    u8 unk_0x21c[0xc];
    u32 field_0x228;
    u8 unk_0x22c[0xc];
    u32 field_0x238;
};
u32 fn_7100413fd0(UnkSource_7100286798* self);
u32 fn_7100413fd8(UnkSource_7100286798* self);

struct UnkComponent_7100286798 {
    u8 unk_0x0[0x1610];
    UnkSource_7100286798* source;
};
void* fn_71010fb398(UnkComponent_7100286798* self);
void* fn_71010fb288(UnkComponent_7100286798* self);

struct UnkActor_7100286798 {
    u8 unk_0x0[0x38];
    u8 field_0x38;  // the transform (UnkXform); fn_710002614c returns its matrix
    u8 unk_0x39[0x207];
    UnkComponent_7100286798* component;  // 0x240
};

struct UnkStruct_7100286798 {
    u8 unk_0x0[0x50];
    UnkActor_7100286798* actor;
    u8 unk_0x58[0x8];
    u32 field_0x60;
    u32 field_0x64;
    u32 field_0x68;
    u32 field_0x6c;
    u8 unk_0x70[0xe4];
    u32 field_0x154;
    u32 field_0x158;
    u32 field_0x15c;
    u32 field_0x160;
    u32 field_0x164;
    u32 field_0x168;
    u32 field_0x16c;
    u32 field_0x170;
    u8 unk_0x174[4];
    UnkEntry_7100286798* field_0x178;
    u64 field_0x180;
    u8 unk_0x188[8];
    void* field_0x190;
    void* field_0x198;
};

// The bit pattern of a float in memory (a word load, as the original reads it).
static inline u32 AsU32(const f32& value) {
    u32 bits;
    __builtin_memcpy(&bits, &value, 4);
    return bits;
}

void fn_7100286798(UnkStruct_7100286798* self, UnkActor_7100286798* actor) {
    self->actor = actor;
    if (!actor || !actor->component)
        return;
    UnkSource_7100286798* src = actor->component->source;
    if (!src)
        return;
    self->field_0x60 = fn_7100413fd0(src);
    self->field_0x64 = fn_7100413fd8(src);
    self->field_0x68 = src->field_0x218;
    self->field_0x6c = src->field_0x228;
    self->field_0x154 = src->field_0x208;
    self->field_0x158 = src->field_0x1f8;
    self->field_0x15c = src->field_0x238;
    self->field_0x160 = src->field_0xec;
    self->field_0x164 = src->field_0xf0;
    const UnkMatrix44& t = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(&self->actor->field_0x38));
    u32 a = AsU32(t.m[1]);  // the second column, as bit patterns
    u32 b = AsU32(t.m[5]);
    u32 c = AsU32(t.m[9]);
    self->field_0x168 = a;
    self->field_0x16c = b;
    self->field_0x170 = c;
    UnkNavMesh* table = GetManager()->navmesh;
    self->field_0x178 = table ? table->Get(src->key) : nullptr;
    self->field_0x180 = src->field_0x1d0;
    self->field_0x190 = fn_71010fb398(self->actor->component);
    self->field_0x198 = fn_71010fb288(self->actor->component);
}
