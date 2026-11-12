// Unit 2744 (gui/scripts/guimanager.lua): the GUI manager's string-id keyed maps and screens, ninth
// batch. Holds the sprite-sheet and script registry checks (fn_71011b8d10, fn_71011b9968), the
// maps of layers (fn_71011ba3b0), text entries (fn_71011bd280, fn_71011b8344, fn_71011bbe4c) and
// scene nodes (fn_71011b364c), the screen scale setter (fn_71011b33cc), the
// focus check (fn_71011b3778), the script call (fn_71011b685c) and the name list add
// (fn_71011b8be0). Names and layouts are placeholders.
#include <new>
#include <string.h>

#include "base/UnkCloneRecords.h"
#include "base/UnkDestructors.h"
#include "base/UnkRegistry.h"
#include "base/UnkStrIdPathPair.h"
#include "base/UnkVector.h"
#include "base/animation/CAnimationResource.h"
#include "base/file/CThreadSafeDevice.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/math/UnkSegment2D.h"
#include "base/tunable/CTunable.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2669.h"
#include "game/UnkTU2768.h"
#include "game/UnkTU39.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkStringData;
using FilePathId = base::UnkStrIdMemberOf<g_unk_7101d47200>;

// A string id's data as the maps hash it: two words at +0x30.
struct UnkIdHashData_b0209 {
    u8 pad_0x0[0x30];
    u32 hash_a;
    u32 hash_b;
};

// ---- fn_71011ba240: a position centred on an object, through the object's anchor ----
struct UnkGuiSize_b0209 {
    u8 pad_0x0[0x1c];
    f32 w, h;
};
struct UnkGuiParent_b0209 {
    u8 pad_0x0[0x120];
    void* anchor;
};

UnkVec2Result fn_71011cf12c(UnkGuiDisplayObject* self, const UnkVec2Result& point);

UnkVec2Result fn_71011ba240(void* unused, UnkGuiDisplayObject* obj, const f32* pos) {
    UnkGuiSize_b0209* s = reinterpret_cast<UnkGuiSize_b0209*>(fn_71011cde14(obj));
    f32 hw = s->w * 0.5f;
    f32 hh = s->h * 0.5f;
    UnkVec2Result p(pos[0] - hw, pos[1] - hh);
    UnkVec2Result r = p;
    void* anchor = reinterpret_cast<UnkGuiParent_b0209*>(obj)->anchor;
    if (anchor) {
        UnkVec2Result t = fn_71011cf12c(reinterpret_cast<UnkGuiDisplayObject*>(anchor), p);
        const u32* tw = reinterpret_cast<const u32*>(&t);
        u32* rw = reinterpret_cast<u32*>(&r);
        u32 a = tw[0], b = tw[1];
        rw[0] = a;
        rw[1] = b;
    }
    return r;
}

// ---- the layer map and fn_71011ba3b0 ----
struct UnkGuiNode_b0209;

// The chained hash map of the GUI manager (the layout of src/base/UnkUnit122/UnkChainedMapErase_b54.cpp).
struct UnkGuiMap_b0209 {
    base::UnkAllocator* allocator;  // 0x0
    u32 flags;                      // 0x8
    u32 count;                      // 0xc
    UnkGuiNode_b0209** buckets;     // 0x10
    u32 bucket_count;               // 0x18
    u32 bucket_capacity;            // 0x1c
    u8 pad_0x20[0x18];
    UnkGuiNode_b0209* tail;  // 0x38
    UnkGuiNode_b0209* head;  // 0x40
};
static_assert(sizeof(UnkGuiMap_b0209) == 0x48, "layout");

struct UnkGuiNode_b0209 {
    CStrId key;
    void* value;
    UnkGuiNode_b0209* next;   // 0x10: the list of all nodes
    UnkGuiNode_b0209* prev;   // 0x18
    UnkGuiNode_b0209* chain;  // 0x20: the next node in the bucket

    UnkGuiNode_b0209*& Next() { return next; }
    UnkGuiNode_b0209*& Prev() { return prev; }
};

struct UnkGuiLayer_b0209 {
    u64 field_0x0;
    u64 field_0x8;
    u32 field_0x10;
    bool field_0x14;
    u32 field_0x18;
    base::UnkAllocator* allocator;
    u32 field_0x28;
    bool field_0x2c;
    bool field_0x2d;
    f32 max_x, max_y;
    f32 min_x, min_y;
    u32 field_0x40;
};
static_assert(sizeof(UnkGuiLayer_b0209) == 0x48, "layout");

UnkGuiLayer_b0209* fn_71011ba3b0(UnkGuiMap_b0209* self, const CStrId* key) {
    auto* layer = static_cast<UnkGuiLayer_b0209*>(fn_71000839a0(0x48, 0xc));
    layer->field_0x0 = 0;
    layer->field_0x8 = 0;
    layer->field_0x10 = 10;
    layer->field_0x14 = true;
    layer->field_0x18 = 7;
    layer->allocator = fn_7100083990();
    layer->field_0x28 = 0;
    layer->field_0x2c = false;
    layer->field_0x2d = true;
    layer->max_x = 3.4028235e38f;
    layer->max_y = 3.4028235e38f;
    layer->min_x = -3.4028235e38f;
    layer->min_y = -3.4028235e38f;
    layer->field_0x40 = 0;

    UnkGuiNode_b0209** slot = nullptr;
    if (self->bucket_count) {
        auto* d = reinterpret_cast<UnkIdHashData_b0209*>(key->data);
        u32 hash = d->hash_a ^ d->hash_b;
        u32 index = hash % self->bucket_capacity;
        UnkGuiNode_b0209** buckets = self->buckets;
        slot = buckets + index;
    }
    UnkGuiNode_b0209* chain = *slot;
    auto* n = static_cast<UnkGuiNode_b0209*>(self->allocator->vf_0x40(0x28, 8, self->flags));
    if (n) {
        new (&n->key) CStrId(*key);
        n->value = layer;
        n->next = nullptr;
        n->prev = nullptr;
        n->chain = nullptr;
    }
    n->chain = chain;
    *slot = n;
    if (!self->head) {
        self->head = n;
    } else if (self->tail) {
        n->Prev() = self->tail;
        self->tail->Next() = n;
    }
    self->tail = n;
    ++self->count;
    return layer;
}

// ---- the text map: fn_71011bd280 (add), fn_71011b8344 (erase), fn_71011bbe4c (node destructor) ----
// The entry a node takes over.
struct UnkGuiEntry_b0209 {
    FilePathId path;
    base::global::CRntString text;  // 0x8
    u32 field_0x38;
    u8 field_0x3c;
};

struct UnkGuiTextNode_b0209 {
    base::UnkStrIdMember key;
    FilePathId path;
    base::global::CRntString text;  // 0x10
    u32 field_0x40;
    u8 field_0x44;
    UnkGuiTextNode_b0209* next;   // 0x48
    UnkGuiTextNode_b0209* prev;   // 0x50
    UnkGuiTextNode_b0209* chain;  // 0x58

    UnkGuiTextNode_b0209*& Next() { return next; }
    UnkGuiTextNode_b0209*& Prev() { return prev; }
};
static_assert(sizeof(UnkGuiTextNode_b0209) == 0x60, "layout");

struct UnkGuiTextMap_b0209 {
    base::UnkAllocator* allocator;
    u32 flags;
    u32 count;
    UnkGuiTextNode_b0209** buckets;
    u32 bucket_count;
    u32 bucket_capacity;
    u8 pad_0x20[0x18];
    UnkGuiTextNode_b0209* tail;
    UnkGuiTextNode_b0209* head;
};

UnkGuiTextNode_b0209* fn_71011bd280(UnkGuiTextMap_b0209* self, const CStrId* key, UnkGuiEntry_b0209* entry) {
    UnkGuiTextNode_b0209** slot = nullptr;
    if (self->bucket_count) {
        auto* d = reinterpret_cast<UnkIdHashData_b0209*>(key->data);
        u32 hash = d->hash_a ^ d->hash_b;
        u32 index = hash % self->bucket_capacity;
        UnkGuiTextNode_b0209** buckets = self->buckets;
        slot = buckets + index;
    }
    UnkGuiTextNode_b0209* chain = *slot;
    auto* n = static_cast<UnkGuiTextNode_b0209*>(self->allocator->vf_0x40(0x60, 8, self->flags));
    if (n) {
        n->key.data = key->data;
        if (&g_unk_7101d47108->empty != key->data)
            ++key->data->ref_count;
        new (&n->path) FilePathId(static_cast<FilePathId&&>(entry->path));
        fn_710007eab4(&n->text, &entry->text);
        u32 a = entry->field_0x38;
        u8 b = entry->field_0x3c;
        n->field_0x44 = b;
        n->field_0x40 = a;
        n->next = nullptr;
        n->prev = nullptr;
        n->chain = nullptr;
    }
    n->chain = chain;
    *slot = n;
    if (!self->head) {
        self->head = n;
    } else if (self->tail) {
        n->Prev() = self->tail;
        self->tail->Next() = n;
    }
    self->tail = n;
    ++self->count;
    return n;
}

// The node's destructor, released through its members.
__attribute__((noinline)) void fn_71011bbe4c(UnkGuiTextNode_b0209* self) {
    self->~UnkGuiTextNode_b0209();
}

UnkGuiTextNode_b0209* fn_71011b8344(UnkGuiTextMap_b0209* self, const CStrId* key) {
    auto* d = reinterpret_cast<UnkIdHashData_b0209*>(key->data);
    u32 hash = d->hash_a ^ d->hash_b;
    u32 index = hash % self->bucket_capacity;
    UnkGuiTextNode_b0209** bucket = self->buckets + index;
    UnkGuiTextNode_b0209* n = *bucket;
    if (!n)
        return nullptr;
    UnkGuiTextNode_b0209* before = nullptr;
    while (n->key.data != key->data) {
        before = n;
        n = n->chain;
        if (!n)
            return nullptr;
    }
    // keeps &before->chain out of the search loop
    asm("" : "+r"(before));
    *(before ? &before->chain : bucket) = n->chain;
    if (self->head == n)
        self->head = n->next;
    if (self->tail == n)
        self->tail = n->prev;
    if (n->Prev())
        n->Prev()->Next() = n->Next();
    UnkGuiTextNode_b0209* r = nullptr;
    if (n->Next()) {
        n->Next()->Prev() = n->Prev();
        r = n->Next();
    }
    fn_71011bbe4c(n);
    self->allocator->vf_0x48(n);
    --self->count;
    return r;
}


// ---- fn_71011b8d10 and fn_71011b9968: a name, looked up in the registry (g_unk_7101d4b5a0, created on first
// use), handed to the loader of its file kind when found ----

struct UnkKeyTemp_b0209 {
    void* id;
    void* path;
    ~UnkKeyTemp_b0209() { fn_710008d8e8(reinterpret_cast<UnkStrIdPathPair*>(this)); }
};
struct UnkTextTemp_b0209 {
    u64 words[41];
    ~UnkTextTemp_b0209() {
        fn_710007ed40(reinterpret_cast<CRntString*>(this));
        fn_710007ecd4(reinterpret_cast<CRntString*>(this));
    }
};
struct UnkEntry_0x30;
struct UnkStrPair_0x10;
UnkTextTemp_b0209 fn_71011b8a80(void* unused, const char* const* name);
UnkTextTemp_b0209 fn_71011b965c(void* unused, const char* const* name);
void fn_71011b8dc4(UnkGuiRoot* root, const UnkStrPair_0x10* path);
void fn_71011b9ce4(UnkGuiRoot* root, const UnkStrPair_0x10* path);

// The registry, created on first use.
static inline UnkRegistry_0x70* Registry() {
    UnkRegistry_0x70* reg = g_unk_7101d4b5a0;
    if (!reg) {
        reg = static_cast<UnkRegistry_0x70*>(fn_71000839a0(0x70, 0x3f));
        fn_7100087388(reg);
        g_unk_7101d4b5a0 = reg;
    }
    return reg;
}

#define REG_BODY(TEXT_FN, USE_FN)                                                                               \
    UnkTextTemp_b0209 text = TEXT_FN(self, name);                                                               \
    UnkRegistry_0x70* reg = Registry();                                                                         \
    bool found;                                                                                                 \
    {                                                                                                           \
        UnkKeyTemp_b0209 key;                                                                                   \
        fn_710008d4c0(reinterpret_cast<UnkStrPair_0x10*>(&key), reinterpret_cast<const UnkEntry_0x30*>(&text)); \
        found = fn_7100087850(reg, reinterpret_cast<const UnkStrPair_0x10*>(&key));                             \
    }                                                                                                           \
    if (found) {                                                                                                \
        UnkKeyTemp_b0209 key;                                                                                   \
        fn_710008d4c0(reinterpret_cast<UnkStrPair_0x10*>(&key), reinterpret_cast<const UnkEntry_0x30*>(&text)); \
        USE_FN(reinterpret_cast<UnkGuiRoot*>(self), reinterpret_cast<const UnkStrPair_0x10*>(&key));            \
    }

__attribute__((noinline)) void fn_71011b8d10(void* self, const char* const* name) {
    REG_BODY(fn_71011b8a80, fn_71011b8dc4)
}

void fn_71011b9968(void* self, const char* const* name) {
    REG_BODY(fn_71011b965c, fn_71011b9ce4)
}
#undef REG_BODY

// ---- fn_71011b3778: whether the GUI is free of a pending focus ----
struct UnkUserDataOwner;

struct UnkGuiFlags_b0209 {
    u8 pad_0x0[0xca8];
    u8 timer[0x88];   // 0xca8
    bool field_0xd30;
    u8 pad_0xd31[0x27];
    bool field_0xd58;
    u8 pad_0xd59[0xd7];
    bool field_0xe30;
};
static_assert(__builtin_offsetof(UnkGuiFlags_b0209, field_0xd58) == 0xd58, "layout");
static_assert(__builtin_offsetof(UnkGuiFlags_b0209, field_0xe30) == 0xe30, "layout");

bool fn_71011b3778(UnkGuiFlags_b0209* self) {
    if (self->field_0xe30)
        return true;
    if (fn_710002fb70(self->timer) && !self->field_0xd58)
        return false;
    if (!g_unk_7101d47308) {
        UnkUserDataOwner* owner = static_cast<UnkUserDataOwner*>(fn_71000839a0(0x2b8, 0x3f));
        fn_710120320c(owner);
        g_unk_7101d47308 = owner;
    }
    if (fn_71012037dc(g_unk_7101d47308) == 1)
        return false;
    const u8* flag = *reinterpret_cast<const u8* const*>(reinterpret_cast<const u8*>(g_unk_7101d47310) + 0x5d0);
    return !flag || *flag == 0;
}


// ---- fn_71011b33cc: the scale of the second layout against the screen size ----

struct UnkGuiWidget_b0209 {
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
    virtual void vf_0x58(f32 value);
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
};

struct UnkGuiLayout_b0209 {
    u8 pad_0x0[8];
    s32 kind;  // 0x8
    u8 pad_0xc[0x18];
    f32 width;  // 0x24
    u8 pad_0x28[4];
    f32 height;  // 0x2c
    u8 pad_0x30[0x18];
};
static_assert(sizeof(UnkGuiLayout_b0209) == 0x48, "layout");

struct UnkGuiScreen_b0209 {
    u8 pad_0x0[0x40];
    UnkGuiLayout_b0209* layouts;  // 0x40
    u8 pad_0x48[8];
    u32 layout_count;  // 0x50
    u8 pad_0x54[0x14];
    UnkGuiWidget_b0209* target;  // 0x68
    u8 pad_0x70[0xd28];
    u8 pad_0xd98[8];
    f32 scale_x;  // 0xda0
    f32 scale_y;  // 0xda4
};
static_assert(__builtin_offsetof(UnkGuiScreen_b0209, scale_x) == 0xda0, "layout");

struct UnkScreenSize_b0209 {
    f32 width;
    f32 height;
};

static inline UnkScreenSize_b0209 ScreenSize() {
    f32 w = static_cast<f32>(fn_7100132fc0());
    f32 h = static_cast<f32>(fn_7100132fd0());
    return UnkScreenSize_b0209{w, h};
}

// Normalises the second layout (kind 2) by the screen size and refreshes the target.
void fn_71011b33cc(UnkGuiScreen_b0209* self) {
    UnkGuiLayout_b0209* layout = nullptr;
    UnkGuiLayout_b0209* end = self->layouts + self->layout_count;
    for (UnkGuiLayout_b0209* p = self->layouts; p != end; ++p) {
        if (p->kind == 2) {
            layout = p;
            break;
        }
    }
    static UnkScreenSize_b0209 screen = ScreenSize();
    f32 lw = layout->width;
    f32 lh = layout->height;
    UnkScreenSize_b0209 size = screen;
    self->scale_x = lw / size.width;
    self->scale_y = lh / size.height;
    if (self->target) {
        self->target->vf_0x88();
        self->target->vf_0x58(0.0f);
        self->target->vf_0x60();
    }
}

// ---- fn_71011b685c: the weak handle of a script-created object ----
// The path text's storage (0x148 bytes, string first).
struct UnkPathText_b0209 {
    u8 raw[0x30];
    void** storage_vtable;
    u8 pad_0x38[0x100];
    u64 field_0x138;
    u32 field_0x140;
    u8 pad_0x144[4];

    CRntString* Str() { return reinterpret_cast<CRntString*>(raw); }
    ~UnkPathText_b0209() {
        fn_710007ed40(Str());
        fn_710007ecd4(Str());
    }
};
static_assert(sizeof(UnkPathText_b0209) == 0x148, "layout");

struct UnkMovedStr_b0209 {
    u64 words[6];
    ~UnkMovedStr_b0209() { fn_710007ecd4(reinterpret_cast<CRntString*>(this)); }
};

UnkPathText_b0209 fn_71011b659c(void* self, const char* const* name);
struct UnkUnit2669Str;
struct UnkScriptManager_ovb0085;
UnkWeakTarget* fn_71011b5f5c(u32 unused, const char* const* path, const CRntString* name, void* arg);

static inline UnkScriptManager_ovb0085* ScriptManager() {
    UnkScriptManager_ovb0085* manager = g_unk_71727bed90;
    if (!manager) {
        manager = static_cast<UnkScriptManager_ovb0085*>(fn_71000839a0(0x88, 0x3f));
        fn_710109d810(manager);
        g_unk_71727bed90 = manager;
    }
    return manager;
}

UnkRefPtr fn_71011b685c(void* self, const char* const* path, CRntString* name, void* arg) {
    UnkMovedStr_b0209 key;
    {
        UnkPathText_b0209 text = fn_71011b659c(self, reinterpret_cast<const char* const*>(name));
        fn_710007eab4(reinterpret_cast<CRntString*>(&key), text.Str());
    }
    UnkScriptManager_ovb0085* manager = ScriptManager();
    u32 loaded = fn_710109dd6c(manager, &key, reinterpret_cast<UnkUnit2669Str*>(name), false, false);
    UnkWeakTarget* target = fn_71011b5f5c(loaded, path, name, arg);
    UnkWeakHandle* handle = target ? GetWeakHandle(target) : nullptr;
    UnkRefPtr out;
    out.field_0x0 = nullptr;
    if (handle) {
        ++handle->ref_count;
        out.field_0x0 = handle;
    }
    return out;
}


// ---- fn_71011b364c: puts each scene node on each of the five layers, then empties the map ----
struct UnkGuiSceneNode_b0209 {
    FilePathId key;
    UnkItemBase_71011a91c4* value;
    UnkGuiSceneNode_b0209* next;  // 0x10
    UnkGuiSceneNode_b0209* prev;
    UnkGuiSceneNode_b0209* chain;
};

struct UnkGuiSceneMap_b0209 {
    base::UnkAllocator* allocator;  // 0x0
    u32 flags;
    u32 count;  // 0xc
    UnkGuiSceneNode_b0209** buckets;
    u32 bucket_count;
    u32 bucket_capacity;  // 0x1c
    u8 pad_0x20[0x18];
    UnkGuiSceneNode_b0209* tail;  // 0x38
    UnkGuiSceneNode_b0209* head;  // 0x40
};

struct UnkGuiScenesRoot_b0209 {
    u8 pad_0x0[0xf8];
    UnkGuiSceneMap_b0209 map;  // 0xf8
};
static_assert(__builtin_offsetof(UnkGuiScenesRoot_b0209, map.head) == 0x138, "layout");

void fn_71011b364c(UnkGuiScenesRoot_b0209* self) {
    for (u32 layer = 0; layer != 5; ++layer) {
        for (UnkGuiSceneNode_b0209* n = self->map.head; n; n = n->next)
            fn_71002199d8(g_unk_7101d47300, n->value, layer);
        UnkGuiSceneMap_b0209* map = &self->map;
        if (map->count) {
            for (UnkGuiSceneNode_b0209* n = map->head; n;) {
                UnkGuiSceneNode_b0209* next = n->next;
                n->key.~FilePathId();
                map->allocator->vf_0x48(n);
                n = next;
            }
            memset(map->buckets, 0, map->bucket_capacity * u64(8));
            map->count = 0;
            map->head = nullptr;
            map->tail = nullptr;
        }
    }
}


// ---- fn_71011b8be0: adds a name to the list (and has fn_71011b8d10 look it up) ----
struct UnkGuiNames_b0209 {
    u8 pad_0x0[0x10];
    base::UnkStrIdVec ids;  // 0x10
};

void fn_71011b8be0(UnkGuiNames_b0209* self, const char* const* name) {
    const char* text = *name;
    if (!text)
        text = g_unk_710162e2ed;
    CStrId id(text, true);
    const CStrId* p = self->ids.data;
    for (u64 n = self->ids.count; n; --n, ++p)
        if (p->data == id.data)
            return;
    self->ids.PushBack(id);
    fn_71011b8d10(self, name);
}
