// CDropterAIComponent unit (tools/tu_map.py unit 780): after the base step, loads the two effect models at
// +0x14870 and +0x14878 when they are missing and resets their colour parameter to zero.
// Names and layouts are placeholders.
#include <new>
#include "base/global/CFilePathStrId.h"
#include "base/math/CVector4D.h"
#include "base/math/CVectorOps.h"
#include "game/UnkBehaviorTree.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::math::CVector4D;

extern const u64 g_unk_71726db100[] __attribute__((visibility("hidden")));
extern const u64 g_unk_71726db108[] __attribute__((visibility("hidden")));
extern const base::math::CVector4D g_unk_71016477b4;  // (0, 0, 0, 0)

void fn_71001494f8(void* slot, u32 kind, u32 mode, CFilePathStrId* path, u32 flags);
struct UnkPlanktonInstance_b44a2;
UnkPlanktonInstance_b44a2* fn_71001be428(void* model);

struct UnkDropterEffectMaterial {
    u8 pad_0x0[0x30];
    CVector4D* params;  // 0x30
};

struct UnkDropterEffectInstance {
    u8 pad_0x0[0x17c];
    CVector4D color;  // 0x17c
    u8 pad_0x18c[0x208 - 0x18c];
    UnkDropterEffectMaterial* material;  // 0x208
};
static_assert(__builtin_offsetof(UnkDropterEffectInstance, material) == 0x208, "UnkDropterEffectInstance layout");

struct UnkDropterEffect {
    u8 pad_0x0[0x130];
    void* model;  // 0x130
};

struct UnkDropterAI {
    u8 pad_0x0[0x14870];
    UnkDropterEffect* effect_a;  // 0x14870
    UnkDropterEffect* effect_b;  // 0x14878
};

// storage holds the temporary copy of the path for the load.
static inline void LoadEffect(UnkDropterEffect*& slot, const CFilePathStrId& path, void* storage) {
    if (slot)
        return;
    CFilePathStrId* id = new (storage) CFilePathStrId(path);
    fn_71001494f8(&slot, 0x190, 1, id, 4);
    id->~CFilePathStrId();
    UnkDropterEffectInstance* instance = reinterpret_cast<UnkDropterEffectInstance*>(fn_71001be428(slot->model));
    Assign(instance->color, g_unk_71016477b4);
    if (instance->material)
        *instance->material->params = g_unk_71016477b4;
}

void fn_71004ec434(UnkDropterAI* self) {
    alignas(8) u8 storage_a[sizeof(CFilePathStrId)];
    alignas(8) u8 storage_b[sizeof(CFilePathStrId)];
    fn_71003695b8(reinterpret_cast<UnkBehaviorTreeAI*>(self));
    LoadEffect(self->effect_a, *reinterpret_cast<const CFilePathStrId*>(g_unk_71726db100), storage_a);
    LoadEffect(self->effect_b, *reinterpret_cast<const CFilePathStrId*>(g_unk_71726db108), storage_b);
}
