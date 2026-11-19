// Unit 2661 (tools/tu_map.py, hint CCutscene::CTunableCutscene): the destructor matching fn_71010673fc: unloads
// the stream (fn_710106838c), drops its clips, its render items, its resource and its models' paths, then
// destroys its members in reverse order and leaves the counted media base. Names and layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "game/UnkBufferStringPool.h"
#include "game/UnkCutsceneRecord_lc8c338.h"
#include "game/UnkModelManager.h"
#include "base/global/CStrId.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU2200.h"
#include "game/UnkTU2661.h"
#include "game/UnkTU2739.h"
#include "game/media/CMediaStream.h"
#include "../../game_aca57c/UnkBufferString.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntVector;
using base::global::CStrId;

struct UnkUnit1134Object_b49a3;
void fn_710070d38c(UnkUnit1134Object_b49a3* self);

// A clip: polymorphic, 0x30 bytes, destroyed in place.
struct UnkCutsceneClip_l067f90 {
    virtual ~UnkCutsceneClip_l067f90();
    u8 pad_0x8[0x28];
};

// A render item the stream added; destroying the entry takes it out of the render manager.
struct UnkCutsceneItem_l067f90 {
    u64 field_0x0;
    u64 field_0x8;
    UnkItemBase_71011a91c4* item;  // 0x10

    ~UnkCutsceneItem_l067f90() {
        fn_71002199d8(g_unk_7101d47300, item, 0);
        item = nullptr;
    }
};

// A reference-counted object freed through its third virtual.
struct UnkCutsceneShared_l067f90 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Release();
    u32 ref_count;
};

struct UnkCutsceneSharedRef_l067f90 {
    UnkCutsceneShared_l067f90* object;

    ~UnkCutsceneSharedRef_l067f90() {
        if (UnkCutsceneShared_l067f90* p = object)
            if (--p->ref_count == 0)
                p->Release();
    }
};

struct UnkCutsceneStream_l067f90 {
    void** vtable;
    u32 ref_count;  // 0x8
    u8 pad_0xc[4];
    UnkRecord_lc8c338 record;  // 0x10
    UnkCountedResource* resource;      // 0x120
    CRntVector<UnkCutsceneItem_l067f90> items;  // 0x128
    CRntVector<UnkCutsceneClip_l067f90> clips;  // 0x150
    u64 field_0x178;
    u8 bone_filter[0x178];  // 0x180
    u64 field_0x2f8;
    u8 sound[0x10];  // 0x300
    CFilePathStrId path;  // 0x310
    CFilePathStrId field_0x318;
    UnkBufferString<0x80, g_unk_7101a1b750> name;  // 0x320
    UnkBufferString<0x80, g_unk_7101a1b750> field_0x3e8;
    u8 pad_0x4b0[0x40];
    UnkCutsceneSharedRef_l067f90 field_0x4f0;
    UnkCutsceneSharedRef_l067f90 field_0x4f8;
};
static_assert(__builtin_offsetof(UnkCutsceneStream_l067f90, path) == 0x310, "layout");
static_assert(__builtin_offsetof(UnkCutsceneStream_l067f90, field_0x4f8) == 0x4f8, "layout");

// The model manager, made on first use.
static inline UnkModelManager* ModelManager() {
    UnkModelManager* manager = g_unk_71726b0cd8;
    if (!manager) {
        manager = static_cast<UnkModelManager*>(fn_71000839a0(sizeof(UnkModelManager), 0x3f));
        fn_71011a870c(manager);
        g_unk_71726b0cd8 = manager;
    }
    return manager;
}

static inline void ReleaseResource(UnkCountedResource* resource) {
    if (--resource->ref_count == 0)
        fn_710001737c(resource);
}

void fn_7101067f90(UnkCutsceneStream_l067f90* self) {
    self->vtable = g_unk_7101cac9b0 + 2;
    fn_710106838c(self);
    self->clips.Clear();
    self->items.Clear();
    if (UnkCountedResource* resource = self->resource) {
        ReleaseResource(resource);
        self->resource = nullptr;
    }
    self->field_0x2f8 = 0;
    for (UnkPathEntry_lc8c338 entry : self->record.entries)
        fn_71011aa5a4(reinterpret_cast<UnkFxCache*>(ModelManager()), &entry.path);

    self->field_0x4f8.~UnkCutsceneSharedRef_l067f90();
    self->field_0x4f0.~UnkCutsceneSharedRef_l067f90();
    self->field_0x3e8.~UnkBufferString();
    self->name.~UnkBufferString();
    self->field_0x318.~CFilePathStrId();
    self->path.~CFilePathStrId();
    fn_710070d38c(reinterpret_cast<UnkUnit1134Object_b49a3*>(self->bone_filter));
    self->clips.Free();
    self->items.Free();
    if (UnkCountedResource* resource = self->resource)
        ReleaseResource(resource);
    self->record.~UnkRecord_lc8c338();
    self->vtable = reinterpret_cast<void**>(g_unk_7101a203f8 + 0x10);
}
