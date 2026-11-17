// Unit 2661 (CCutscene::CTunableCutscene): loads a cutscene from its file. Reads the CCutSceneDef
// it holds, then sets up what the definition names: its shot models, sounds, lights, action set,
// actors and the "streams_cutscenes_<name>_wav" stream path. Names and layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "base/UnkFileReader.h"
#include "base/UnkHashStream.h"
#include "base/UnkStrIdPathPair.h"
#include "base/UnkStrPair.h"
#include "base/animation/CBlendSpace1D.h"
#include "base/animation/CBlendSpaceResource.h"
#include "base/file/CThreadSafeDevice.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflObjectRead.h"
#include "base/reflection/UnkReflValue.h"
#include "base/snd/CSoundSystem.h"
#include "game/UnkGameManager.h"
#include "game/UnkModelManager.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU23.h"
#include "game/UnkTU2527.h"
#include "game/UnkTU2661.h"
#include "game/UnkTU2677.h"
#include "game/UnkTU27.h"
#include "game/UnkTU344.h"
#include "game/UnkTU56.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CRntVector;
using base::global::CStrId;

// This unit's parser callbacks.
extern UnkParseCallbacks g_unk_7172a014c8 __attribute__((visibility("hidden")));

// A counted reference to a resource, as the registry returns it (in memory).
struct UnkAssetResRef {
    UnkCountedResource* ptr;

    UnkAssetResRef() : ptr(nullptr) {}
    UnkAssetResRef& operator=(const UnkAssetResRef& other) {
        if (ptr != other.ptr) {
            UnkCountedResource* p = other.ptr;
            if (p)
                ++p->ref_count;
            UnkCountedResource* old = ptr;
            if (old && --old->ref_count == 0)
                fn_710001737c(old);
            ptr = p;
        }
        return *this;
    }
    ~UnkAssetResRef() {
        if (ptr && --ptr->ref_count == 0)
            fn_710001737c(ptr);
    }
};

// A 0x30-byte holder: a destructor slot, a word and a 27-byte payload.
struct UnkHolder_b0181;
struct UnkHolderVtable_b0181 {
    void (*dtor)(UnkHolder_b0181* self);
};
struct UnkHolder_b0181 {
    const UnkHolderVtable_b0181* vtable;
    u64 field_0x8;
    u8 payload[27];

    UnkHolder_b0181(UnkHolder_b0181&& other)
        : vtable(reinterpret_cast<const UnkHolderVtable_b0181*>(g_unk_7101cac988 + 2)), field_0x8(other.field_0x8) {
        __builtin_memcpy(payload, other.payload, sizeof(payload));
    }
    ~UnkHolder_b0181() { vtable->dtor(this); }
};

// A shot's model as the render manager holds it; giving it up hands the item back.
struct UnkCutsceneModel_l067698 {
    u64 field_0x0;
    u64 field_0x8;
    UnkItemBase_71011a91c4* item;

    ~UnkCutsceneModel_l067698() {
        fn_71002199d8(g_unk_7101d47300, item, 0);
        item = nullptr;
    }
};

struct UnkCutsceneShot_l067698 {
    u64 field_0x0;
    u32 frame_count;  // 0x8
    u8 pad_0xc[0x34];
};
static_assert(sizeof(UnkCutsceneShot_l067698) == 0x40, "shot size");

struct UnkCutsceneActor_l067698 {
    u64 field_0x0;
    CFilePathStrId path;  // 0x8
    u8 pad_0x10[0x90];
};
static_assert(sizeof(UnkCutsceneActor_l067698) == 0xa0, "actor size");

struct UnkCutsceneLight_l067698 {
    u64 field_0x0;
    u32 field_0x8;
    u32 field_0xc;
};

struct UnkCutscene_l067698 {
    u8 pad_0x0[0x10];
    void* def_vtable;                                 // 0x10: the CCutSceneDef read from the file
    CStrId name;                                      // 0x18
    CFilePathStrId action_set_path;                   // 0x20
    u8 pad_0x28[0x10];
    CStrId field_0x38;                                // 0x38
    u8 pad_0x40[0x38];
    CRntVector<UnkCutsceneShot_l067698> shots;        // 0x78
    CRntVector<UnkCutsceneActor_l067698> actors;      // 0xa0
    CRntVector<CFilePathStrId> sounds;                // 0xc8
    CRntVector<UnkCutsceneLight_l067698> lights;      // 0xf0
    u8 pad_0x118[0x8];
    UnkAssetResRef action_set;                        // 0x120
    CRntVector<UnkCutsceneModel_l067698> models;      // 0x128
    CRntVector<UnkHolder_b0181> holders;              // 0x150
    u8 pad_0x178[0x198];
    CFilePathStrId path;                              // 0x310
    CFilePathStrId stream_path;                       // 0x318
    u8 pad_0x320[0x190];
    u32 state;                                        // 0x4b0
    u32 pad_0x4b4;
    u32 frame_count;                                  // 0x4b8
    u8 pad_0x4bc[0x67];
    bool field_0x523;                                 // 0x523
};
static_assert(__builtin_offsetof(UnkCutscene_l067698, shots) == 0x78, "layout");
static_assert(__builtin_offsetof(UnkCutscene_l067698, action_set) == 0x120, "layout");
static_assert(__builtin_offsetof(UnkCutscene_l067698, path) == 0x310, "layout");
static_assert(__builtin_offsetof(UnkCutscene_l067698, state) == 0x4b0, "layout");
static_assert(__builtin_offsetof(UnkCutscene_l067698, field_0x523) == 0x523, "layout");

// The ids a resource is looked up by: its type and its path.
struct UnkResourceIds_l067698 {
    CStrId type;
    CFilePathStrId path;
};

void fn_7101068980(CRntVector<UnkCutsceneModel_l067698>* models, UnkCutsceneShot_l067698* shot, u32* first,
                   u32* total);
UnkHolder_b0181* fn_7101068b1c(CRntVector<UnkHolder_b0181>* self, void* a, void* b);
UnkAssetResRef fn_71011934b0(UnkActorDefFactory* factory, const CFilePathStrId& path);
void fn_71011aa0d4(UnkModelManager* self, u32 a, UnkCutsceneLight_l067698* light, u32 b, u32 c);

// The "actionset" resource type's id (the first word of its descriptor).
static inline CStrId ActionSetType() {
    return *reinterpret_cast<const CStrId*>(&g_unk_71727b9630);
}

// The model manager, created on first use.
static inline UnkModelManager* ModelManager() {
    UnkModelManager* m = g_unk_71726b0cd8;
    if (!m) {
        m = static_cast<UnkModelManager*>(fn_71000839a0(0x1e8, 0x3f));
        fn_71011a870c(m);
        g_unk_71726b0cd8 = m;
    }
    return m;
}

// A string followed by a text.
static inline CRntString Concat(const CRntString& a, const char* b) {
    return fn_710007ef90(&a, b);
}

// Sets `target` up to read the cutscene definition (a CCutSceneDef) at `def`, in `mode`.
static inline void SetUpRead(UnkReadTarget* target, void* def, u32 mode) {
    const base::reflection::CClass* type = fn_7100c81418();
    base::reflection::UnkReflValue value;
    value.field_0x0 = nullptr;
    value.type = nullptr;
    value.kind = 2;
    fn_71000a18d4(&value, type, def, true);
    fn_71000f0568(target, &value, mode);
    fn_71000a1c60(&value);
}

// The resource as an action set: a counted reference, or none when it is not one.
static inline UnkAssetResRef AsActionSet(const UnkAssetResRef& found) {
    UnkAssetResRef set;
    UnkCountedResource* r = found.ptr;
    if (r && fn_710009d50c(reinterpret_cast<base::reflection::UnkReflObject*>(r)->vf_0x0(), fn_7100b8bbf4())) {
        ++r->ref_count;
        set.ptr = r;
    }
    return set;
}

// Parses the file into the target, starting from its last entry's node; whether it succeeded.
static inline bool Parse(UnkFileReader* reader, UnkReadTarget* target) {
    UnkParseContext ctx(g_unk_7172a014c8, reader, target);
    UnkReadEntry* end = target->entries + target->count;
    return fn_710000f750(&ctx, target->count ? end[-1].field_0x0->field_0x10 : nullptr) != 0;
}

// Loads the cutscene from `path`; whether it was read and set up.
bool fn_7101067698(UnkCutscene_l067698* self, const CFilePathStrId* path) {
    self->state = 0;
    self->path = *path;

    UnkFileReader reader;
    {
        UnkStrPair_0x10 s;
        fn_710008d2ec(&s, path);
        fn_710007be8c(&reader.file, 0x16, nullptr);
        reader.field_0x38 = 3;
        reader.field_0x3c = 2;
        bool opened;
        {
            CRntString mode("rb");
            opened = fn_710007c1ac(&reader, &s, &mode);
        }
        if (opened)
            fn_710000ccd8(&reader);
        fn_710008d8e8(reinterpret_cast<UnkStrIdPathPair*>(&s));
    }

    UnkReadTarget target;
    SetUpRead(&target, &self->def_vtable, 0x55);

    bool ok = false;
    if (reader.file.position < reader.file.size && reader.field_0x38 == 0) {
        if (Parse(&reader, &target)) {
            // The shots' models, and the total length.
            self->frame_count = 0;
            self->models.Reserve(self->shots.count);
            for (UnkCutsceneShot_l067698 *shot = self->shots.data, *last = shot + self->shots.count; shot != last;
                 ++shot) {
                u32 first = 0;
                fn_7101068980(&self->models, shot, &first, &self->frame_count);
                self->frame_count += shot->frame_count;
            }

            for (CFilePathStrId *sound = self->sounds.data, *last = sound + self->sounds.count; sound != last; ++sound)
                fn_710010808c(g_unk_7101d4d820, reinterpret_cast<UnkPathNode*>(sound));

            for (UnkCutsceneLight_l067698 *light = self->lights.data, *last = light + self->lights.count;
                 light != last; ++light)
                fn_71011aa0d4(ModelManager(), light->field_0x8, light, light->field_0xc, 0);

            // The action set, kept only when the resource is one.
            {
                UnkRegistry_7100d92158* registry = g_unk_7101d4bbc0;
                CStrId type = ActionSetType();
                UnkResourceIds_l067698 ids{type, self->action_set_path};
                UnkAssetResRef found =
                    fn_710002038c(registry, reinterpret_cast<const UnkStrIdPathPair*>(&ids), true, nullptr);
                self->action_set = AsActionSet(found);
            }

            // The actors whose definitions resolve.
            self->holders.Reserve(self->actors.count);
            for (UnkCutsceneActor_l067698 *actor = self->actors.data, *last = actor + self->actors.count;
                 actor != last; ++actor) {
                UnkAssetResRef def = fn_71011934b0(g_unk_71726a0220, CFilePathStrId(actor->path));
                if (def.ptr)
                    fn_7101068b1c(&self->holders, actor, self);
            }

            self->state = 1;

            // Reports the cutscene's name, outside a game in progress.
            if (self->field_0x38.data != &g_unk_7101d47108->empty) {
                CStrId name = self->field_0x38;
                if (!self->field_0x523) {
                    if (!fn_7100deac04(g_unk_7101d47310) && !fn_7100deabe4(g_unk_7101d47310)) {
                        ++name.data->reads;
                        fn_71010c0e70(name.data->text, g_unk_7101d58d10.data, 0);
                    }
                }
            }

            ++self->name.data->reads;
            CRntString stream = Concat(fn_710007f050("streams_cutscenes_", &self->name.data->String()), "_wav");
            self->stream_path = CFilePathStrId(stream.data ? stream.data : g_unk_710162e2ed, true);
            ok = self->state == 1;
        }
    }

    for (u64 i = 0; i < target.count; ++i) {
        UnkReadEntry& e = target.entries[i];
        if (e.ops) {
            e.ops->destroy(e.storage);
            e.ops = nullptr;
        }
    }
    fn_710007c0e4(&reader.file);
    return ok;
}
