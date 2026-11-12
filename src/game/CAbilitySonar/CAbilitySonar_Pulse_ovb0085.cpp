// CAbilitySonar's unit (tools/tu_map.py): a tunable's constructor (fn_710033462c: on fn_7100136ecc, three
// cleared vectors and three empty string ids), stopping the sonar (fn_7100335458), attaching an effect to
// the owner's effect object (fn_71003356ac), creating the sonar's model (fn_7100336760) and reading its
// model and pulse paths (fn_710033652c), and the pool's factories for models and switch objects
// (fn_7100339900, fn_7100339d48: the object is filed in the pool's chained hash map, keyed by its scene
// node at +0x8, as in src/game/UnkUnit347/UnkUnit347_Add_A1_b72.cpp; when the path is set and the
// resource does not load (fn_7100339a50, fn_7100339ea0 in CAbilitySonar.cpp) the object is deleted). Names
// and layouts are placeholders.
#include <new>
#include "base/UnkResKey.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntFile.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/snd/UnkSound.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkModelManager.h"
#include "game/UnkRenderManager.h"
#include "game/UnkSwitchObject.h"
#include "game/UnkTU134.h"
#include "game/UnkTU2739.h"
#include "game/UnkTUProperty.h"
#include "types.h"

extern char g_unk_7101a23768[];

struct UnkSonarVec_ovb0085 {
    f32 x, y, z;
    u32 pad;
};

struct UnkEmptyStrId_ovb0085 {
    base::global::UnkStringData* data;
};

__attribute__((always_inline)) static inline void InitEmptyId(UnkEmptyStrId_ovb0085* id) {
    id->data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    id->data = &g_unk_7101d47108->empty;
}

struct UnkSonarTunable_ovb0085 {
    void* vtable;
    u8 pad_0x8[0x28];
    UnkSonarVec_ovb0085 field_0x30;
    UnkSonarVec_ovb0085 field_0x40;
    UnkSonarVec_ovb0085 field_0x50;
    u8 pad_0x60[0x8];
    UnkEmptyStrId_ovb0085 field_0x68;
    UnkEmptyStrId_ovb0085 field_0x70;
    u64 field_0x78;
    UnkEmptyStrId_ovb0085 field_0x80;
};

static inline void Clear(UnkSonarVec_ovb0085* v) {
    v->x = 0.0f;
    v->y = 0.0f;
    v->z = 0.0f;
}

void fn_710033462c(UnkSonarTunable_ovb0085* self) {
    fn_7100136ecc(reinterpret_cast<UnkTunable*>(self));
    self->vtable = g_unk_7101a23768 + 0x10;
    Clear(&self->field_0x30);
    Clear(&self->field_0x40);
    Clear(&self->field_0x50);
    InitEmptyId(&self->field_0x68);
    InitEmptyId(&self->field_0x70);
    InitEmptyId(&self->field_0x80);
}

struct UnkSonarItem_ovb0085 {
    struct VTable {
        void* slots[4];
        void (*vf_0x20)(UnkSonarItem_ovb0085* self);
    };
    VTable* vtable;
};

struct UnkSonarSoundHandle_ovb0085 {
    UnkSound* sound;
    u64 id;
};

struct UnkSonarSoundRef_ovb0085 {
    UnkSonarSoundHandle_ovb0085* handle;
    u64 handle_id;
};

struct UnkAbilitySonar_ovb0085 {
    u8 pad_0x0[0x398];
    UnkSwitchObject* field_0x398;
    u8 pad_0x3a0[0x3d0 - 0x3a0];
    u32 field_0x3d0;
    u32 count;
    UnkSonarItem_ovb0085* items[1];
    u8 pad_0x3e0[0x790 - 0x3e0];
    UnkSonarSoundRef_ovb0085 sound_0x790;
    UnkSonarSoundRef_ovb0085 sound_0x7a0;
};
static_assert(__builtin_offsetof(UnkAbilitySonar_ovb0085, sound_0x7a0) == 0x7a0, "layout");

static inline void FadeOut(const UnkSonarSoundRef_ovb0085& ref) {
    UnkSonarSoundHandle_ovb0085* handle = ref.handle;
    if (handle && handle->id == ref.handle_id && ref.handle_id != ~0ull && handle->sound)
        fn_7100105a80(handle->sound, 0.2f, 0.0f);
}

void fn_7100335458(UnkAbilitySonar_ovb0085* self, bool flag) {
    self->field_0x3d0 = 0;
    u64 bytes = (u64)self->count * 8;
    asm("" : "+r"(bytes));  // as fn_7100336474 (CAbilitySonar_Reset_b46.cpp): the byte size is tested
    for (u64 offset = 0; offset != bytes; offset += 8) {
        UnkSonarItem_ovb0085* item =
            *reinterpret_cast<UnkSonarItem_ovb0085**>(reinterpret_cast<char*>(self->items) + offset);
        item->vtable->vf_0x20(item);
    }
    self->count = 0;
    FadeOut(self->sound_0x790);
    FadeOut(self->sound_0x7a0);
    if (UnkSwitchObject* object = self->field_0x398) {
        if (flag)
            fn_7100146dac(object, false);
        else
            fn_7100146e60(object, false);
    }
    f32 zero = 0.0f;
    fn_7100219294(g_unk_7101d47300, 0x90e4f789f8288231ull, &zero, 0);
}

using base::global::CStrId;

// As src/game/CCharClassFanComponent/CFanComponent_Effects_A4_b75.cpp declares it.
void fn_710115217c(void* object, void* effect, u32 a, u32 b, const base::global::CStrId* name,
                   const base::math::CVector3D& offset, const base::math::CVector3D& rotation, void* extra);

// The id's text, the way the string data is shared: its use count (+0x44) is raised.
// The count is written untyped: the original reads the id again afterwards.
static inline void* IdText(const CStrId& id) {
    u8* data = reinterpret_cast<u8*>(id.data);
    u32 uses;
    __builtin_memcpy(&uses, data + 0x44, sizeof(uses));
    ++uses;
    __builtin_memcpy(data + 0x44, &uses, sizeof(uses));
    return *reinterpret_cast<void**>(reinterpret_cast<u8*>(id.data) + 0x10);
}

struct UnkSonarEffect_ovb0085 {
    void** vtable;
    void vf_0x170() { reinterpret_cast<void (*)(UnkSonarEffect_ovb0085*)>(vtable[0x170 / 8])(this); }
};

struct UnkSonarEffectOwner_ovb0085 {
    u8 pad_0x0[0x280];
    void* object;  // 0x280
};

struct UnkAbilitySonar_ovb0085;

void fn_71003356ac(UnkAbilitySonar_ovb0085* self, UnkSonarEffect_ovb0085* effect, UnkSonarEffectOwner_ovb0085* owner) {
    if (!effect)
        return;
    effect->vf_0x170();
    void* object = owner->object;
    if (!object)
        return;
    base::math::CVector3D offset;
    offset.x = 0.0f;
    offset.y = 0.0f;
    offset.z = 0.0f;
    base::math::CVector3D rotation;
    rotation.x = 0.0f;
    rotation.y = 0.0f;
    rotation.z = 0.0f;
    fn_710115217c(object, effect, 0, 8, &g_unk_7101d470e0, offset, rotation,
                  IdText(GameManagerIdTable().field_0xd40));
}

using base::global::CFilePathStrId;
using base::global::CStrId;

bool fn_7100339ea0(UnkResHolder* self, const CFilePathStrId& path);
bool fn_7100339a50(UnkResHolder* self, const CFilePathStrId& path);

struct UnkRenderPool;
struct UnkSonarModel_ovb0085;
void fn_7100145bdc(UnkSonarModel_ovb0085* self, const CStrId* name);
void fn_710014699c(UnkSwitchObject* self, const CStrId* name);
void fn_71001c08d8(void* object);
void fn_71002084a8(void* object);

struct UnkSonarModel_ovb0085 {
    void** vtable;
    UnkSceneNode node;  // 0x8
    u8 pad_0x20[0x110];
    void* field_0x130;

    void vf_0x40(bool on) { reinterpret_cast<void (*)(UnkSonarModel_ovb0085*, bool)>(vtable[0x40 / 8])(this, on); }
    void vf_0x90(bool on) { reinterpret_cast<void (*)(UnkSonarModel_ovb0085*, bool)>(vtable[0x90 / 8])(this, on); }
};
static_assert(__builtin_offsetof(UnkSonarModel_ovb0085, field_0x130) == 0x130, "layout");

struct UnkPoolNode_ovb0085 {
    void* key;
    void* value;
    UnkPoolNode_ovb0085* next;   // 0x10: the list of all nodes
    UnkPoolNode_ovb0085* prev;   // 0x18
    UnkPoolNode_ovb0085* chain;  // 0x20: the next node in the bucket
};

struct UnkPoolMap_ovb0085 {
    using Node = UnkPoolNode_ovb0085;
    base::UnkAllocator* allocator;
    u32 flags;
    u32 count;
    Node** buckets;       // 0x10
    u32 bucket_count;     // 0x18
    u32 bucket_capacity;  // 0x1c
    u8 pad_0x20[0x18];
    Node* tail;  // 0x38
    Node* head;  // 0x40

    __attribute__((always_inline)) void Insert(void* key, void* value) {
        Node** slot = nullptr;
        if (bucket_count) {
            u32 i = (u32)(u64)key % bucket_capacity;
            asm("" : "+r"(i));  // the original computes the index before loading the buckets
            slot = &buckets[i];
        }
        Node* c = *slot;
        auto* n = static_cast<Node*>(allocator->vf_0x40(0x28, 8, flags));
        if (n) {
            n->key = key;
            n->value = value;
            n->next = nullptr;
            n->prev = nullptr;
            n->chain = nullptr;
        }
        n->chain = c;
        *slot = n;
        if (!head) {
            head = n;
        } else if (tail) {
            __builtin_memcpy(&n->prev, &tail, sizeof(Node*));  // the original reloads the tail
            tail->next = n;
        }
        tail = n;
        ++count;
    }
};

struct UnkDeletable_ovb0085 {
    void** vtable;
    void Delete() { reinterpret_cast<void (*)(UnkDeletable_ovb0085*)>(vtable[1])(this); }
};

template <class T, void (*Ctor)(T*, const CStrId*), u32 Size, bool (*Load)(UnkResHolder*, const CFilePathStrId&)>
__attribute__((always_inline)) inline void* Create(UnkRenderPool* pool, const CFilePathStrId* path, T** out,
                                                   const CStrId* name, void (*prepare)(T*)) {
    T* object = static_cast<T*>(fn_71000839a0(Size, 0x38));
    Ctor(object, name);
    void* node = nullptr;
    if (object) {
        prepare(object);
        if (path->data != &g_unk_7101d47200->empty && !Load(reinterpret_cast<UnkResHolder*>(object), *path)) {
            reinterpret_cast<UnkDeletable_ovb0085*>(object)->Delete();
            return nullptr;
        }
        node = reinterpret_cast<u8*>(object) + 8;
        reinterpret_cast<UnkPoolMap_ovb0085*>(pool)->Insert(node, object);
    }
    if (out)
        *out = object;
    return node;
}

static inline void PrepareModel(UnkSonarModel_ovb0085* object) { fn_71001c08d8(object->field_0x130); }
static inline void PrepareSwitch(UnkSwitchObject* object) { fn_71002084a8(object->field_0x28); }

void* fn_7100339900(UnkRenderPool* pool, const CFilePathStrId* path, UnkSonarModel_ovb0085** out,
                    const CStrId* name) {
    return Create<UnkSonarModel_ovb0085, fn_7100145bdc, 0x248, fn_7100339a50>(pool, path, out, name, PrepareModel);
}

// Declared with a void result in src/game/CScorpiusFXComponent/CScorpiusFXComponent_Gas_ovb0008.cpp.
void* fn_7100339d48(UnkRenderPool* pool, const CFilePathStrId* path, UnkSwitchObject** out, const CStrId* name) {
    return Create<UnkSwitchObject, fn_710014699c, 0x248, fn_7100339ea0>(pool, path, out, name, PrepareSwitch);
}

static inline UnkModelManager* ModelManager() {
    UnkModelManager* m = g_unk_71726b0cd8;
    if (!m) {
        m = static_cast<UnkModelManager*>(fn_71000839a0(0x1e8, 0x3f));
        fn_71011a870c(m);
        g_unk_71726b0cd8 = m;
    }
    return m;
}

// Creates the sonar's model from `path` (the model manager created first, on first use), puts its scene
// node on layer 0 and switches it off (vf_0x90, vf_0x40).
UnkSonarModel_ovb0085* fn_7100336760(UnkAbilitySonar_ovb0085* self, const CFilePathStrId* path) {
    if (!g_unk_71726b0cd8) {
        UnkModelManager* m = static_cast<UnkModelManager*>(fn_71000839a0(0x1e8, 0x3f));
        fn_71011a870c(m);
        g_unk_71726b0cd8 = m;
    }
    UnkRenderManager* render = g_unk_7101d47300;
    UnkSonarModel_ovb0085* model = nullptr;
    fn_7100339900(render->field_0x218, path, &model, &g_unk_7101d470e0);
    fn_7100219ae0(render, model ? &model->node : nullptr, 0);
    UnkSonarModel_ovb0085* created = model;
    if (created) {
        created->vf_0x90(false);
        created->vf_0x40(false);
    }
    return created;
}

// As src/game/CScorpiusFXComponent/CScorpiusFXComponent_Gas_ovb0008.cpp declares it.
// As src/game/CActorDef/CActorDef_ReadChecked.cpp declares it.
s64 fn_710007cee4(void*, const char**);

struct UnkSonarSwitch_ovb0085 {
    u8 pad_0x0[0x15a];
    bool field_0x15a;
};

struct UnkSonarData_ovb0085 {
    u8 pad_0x0[0x390];
    UnkSonarModel_ovb0085* model;  // 0x390
    UnkSwitchObject* pulse;        // 0x398
    u8 pad_0x3a0[0x28];
    f32 scale;  // 0x3c8
};

struct UnkSonarSource_ovb0085 {
    u8 pad_0x0[0x38];
    base::global::CRntFile file;  // 0x38
};

// Reads the sonar's scale and the paths of its model and its pulse (a switch object, put on layer 0 and
// switched off) when the file has data left; the old ones go back to the model manager first.
bool fn_710033652c(UnkSonarData_ovb0085* self, UnkSonarSource_ovb0085* source) {
    base::global::CRntFile* file = &source->file;
    if (file->position >= file->size)
        return false;
    UnkSwitchObject* pulse;
    const char* model_path;
    const char* pulse_path;
    f32 scale = 1.0f;
    fn_710007c714(file, &scale, 4, 1);
    fn_710007cee4(file, &model_path);
    fn_710007cee4(file, &pulse_path);
    self->scale = scale;
    if (UnkSonarModel_ovb0085* old = self->model)
        fn_71011a91c4(ModelManager(), old, 0);
    {
        CFilePathStrId path(model_path, true);
        self->model = fn_7100336760(reinterpret_cast<UnkAbilitySonar_ovb0085*>(self), &path);
    }
    if (UnkSwitchObject* old = self->pulse)
        fn_71011a91c4(ModelManager(), old, 0);
    CFilePathStrId path(pulse_path, true);
    ModelManager();
    UnkRenderManager* render = g_unk_7101d47300;
    pulse = nullptr;
    fn_7100339d48(render->field_0x218, &path, &pulse, &g_unk_7101d470e0);
    fn_7100219ae0(render, pulse ? reinterpret_cast<UnkSceneNode*>(reinterpret_cast<u8*>(pulse) + 8) : nullptr, 0);
    UnkSwitchObject* created = pulse;
    if (created) {
        reinterpret_cast<UnkSonarSwitch_ovb0085*>(created)->field_0x15a = false;
        fn_7100146dac(created, false);
    }
    self->pulse = created;
    return true;
}
