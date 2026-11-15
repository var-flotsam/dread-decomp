// Unit 2702: the entry of the navigation mesh's key table for this AI component's target. Names and layouts
// are placeholders.
#include "base/global/CRntVector.h"
#include "game/UnkNavMeshManager.h"
#include "types.h"

struct UnkLookupEntry_l0fb288 {
    u8 pad_0x0[0x28];
};

struct UnkLookupMesh_l0fb288 {
    u8 pad_0x0[0x2e8];
    UnkLookupEntry_l0fb288* values;  // 0x2e8
    u64* keys;                       // 0x2f0
    u32 count;                       // 0x2f8
};

struct UnkLookupTarget_l0fb288 {
    u8 pad_0x0[0x30];
    u64 key;  // 0x30
};

struct UnkLookupConfig_l0fb288 {
    u8 pad_0x0[0x3a0];
    bool enabled;  // 0x3a0
};

struct UnkLookupAI_l0fb288;
struct UnkLookupVtable_l0fb288 {
    void* pad_0x0[0x7c];
    UnkLookupTarget_l0fb288* (*vf_0x3e0)(UnkLookupAI_l0fb288* self);
};

struct UnkLookupAI_l0fb288 {
    UnkLookupVtable_l0fb288* vtable;
    u8 pad_0x8[0x1608];
    UnkLookupConfig_l0fb288* config;  // 0x1610
};

static inline UnkLookupEntry_l0fb288* Find_l0fb288(UnkLookupMesh_l0fb288* t, const u64& key) {
    for (u32 i = 0; i < t->count; ++i)
        if (t->keys[i] == key)
            return &t->values[i];
    return &t->values[t->count];
}

struct UnkComponent_7100286798;

void* fn_71010fb288(UnkComponent_7100286798* component) {
    UnkLookupAI_l0fb288* self = reinterpret_cast<UnkLookupAI_l0fb288*>(component);
    if (!self->config->enabled)
        return nullptr;
    UnkNavMeshManager* manager = g_unk_71726a4510;
    if (!manager) {
        manager = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(manager);
        g_unk_71726a4510 = manager;
    }
    UnkLookupMesh_l0fb288* mesh = reinterpret_cast<UnkLookupMesh_l0fb288*>(manager->navmesh);
    if (!mesh)
        return nullptr;
    UnkLookupTarget_l0fb288* target = self->vtable->vf_0x3e0(self);
    if (!target)
        return nullptr;
    UnkLookupEntry_l0fb288* entry = Find_l0fb288(mesh, target->key);
    return entry == &mesh->values[mesh->count] ? nullptr : entry;
}
