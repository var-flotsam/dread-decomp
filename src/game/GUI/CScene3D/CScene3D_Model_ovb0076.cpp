// GUI::CScene3D's unit (tools/tu_map.py, the name from reflection and the "CScene3D:" prefix of the
// name it registers under): a 3D model shown in a GUI frame. Its destructor, reset and
// registration with the item list (g_unk_7172729910), its unregistering clean-up, the two maps
// of string ids at +0x3d0 (built with 11 buckets, emptied), and dropping the model file
// (<root>maps/levels/c50_gui/<name>/<name>.bmssd). Names and layouts are placeholders.
#include <new>

#include "base/UnkChainedMap.h"
#include "base/UnkSList.h"
#include "base/UnkVector.h"
#include "base/file/UnkFileDevice.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "game/CInputComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiFrames.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2732.h"
#include "game/UnkTU315.h"
#include "types.h"

using base::global::CRntString;

extern void* g_unk_7101a9e968[];  // the scene's vtable
extern void* g_unk_7101a9ebb8[];  // the node list's vtable

struct UnkScene3DNode_ovb0076 {
    void* value;
    UnkScene3DNode_ovb0076* next;
};

// A linked list whose nodes come from its allocator.
struct UnkScene3DList_ovb0076 {
    void** vtable;
    u32 count;
    UnkScene3DNode_ovb0076* head;
    UnkScene3DNode_ovb0076* tail;
    base::UnkAllocator* allocator;
    u32 flags;

    // Frees every node.
    void Clear() {
        for (UnkScene3DNode_ovb0076* n = head; n;) {
            UnkScene3DNode_ovb0076* next = n->next;
            allocator->vf_0x48(n);
            n = next;
        }
        count = 0;
        head = nullptr;
        tail = nullptr;
    }
};

// One input of the frame: the key (bit), its kind (2: held) and its value.
struct UnkScene3DInput_ovb0076 {
    u32 key;
    u32 kind;
    u32 field_0x8;
    f32 value;  // 0xc
    u32 field_0x10;
};

// The model the scene shows (registered with the render manager).
struct UnkScene3DModel_ovb0076 {
    u8 pad_0x0[0x18];
    u8 camera[0xc4];  // 0x18
    f32 max_zoom;  // 0xdc
};

// A string assigned out of line (fn_710007eaa0).
struct UnkAssignedString_ovb0076 {
    u8 raw[0x30];

    UnkAssignedString_ovb0076& operator=(const CRntString& other) {
        fn_710007eaa0(reinterpret_cast<CRntString*>(this), &other);
        return *this;
    }
};

// What the scene registers with the item list: its name first.
struct UnkScene3DItem_ovb0076 {
    UnkAssignedString_ovb0076 name;
    u8 unk_0x30[0x4];
};

// The scene's two maps, keyed by string id (the key first in each node, the next node at 0x10).
struct UnkScene3DMaps_ovb0076 {
    UnkChainedMap first;
    UnkChainedMap second;
};

struct UnkScene3D_ovb0076 {
    void** vtable;
    u8 pad_0x8[0x80];
    f32 field_0x88;
    u8 pad_0x8c[0xf8];
    bool changed;  // 0x184: vf_0xa0 has run
    u8 pad_0x185[0x3];
    u8 dirty;  // 0x188
    u8 pad_0x189[0x97];
    f32 field_0x220;
    f32 field_0x224;
    f32 field_0x228;
    u8 pad_0x22c[0x8];
    f32 field_0x234;
    f32 field_0x238;
    f32 field_0x23c;
    u8 pad_0x240[0x48];
    CRntString name;   // 0x288: the scene's model
    s32 frame;         // 0x2b8
    f32 field_0x2bc;
    f32 field_0x2c0;
    f32 field_0x2c4;
    f32 field_0x2c8;
    f32 field_0x2cc;
    f32 field_0x2d0;
    f32 field_0x2d4;
    u32 field_0x2d8;
    u32 field_0x2dc;
    CRntString field_0x2e0;
    f32 field_0x310;
    u8 field_0x314[0x18];
    u8 pad_0x32c[0xc];
    CRntString field_0x338;
    u8 pad_0x368[0x8];
    UnkScene3DList_ovb0076 objects;  // 0x370
    u32 field_0x3a0;
    u32 field_0x3a4;
    u32 field_0x3a8;
    base::math::CVector2D field_0x3ac;
    base::math::CVector2D field_0x3b4;
    f32 field_0x3bc;
    f32 field_0x3c0;
    f32 field_0x3c4;
    bool field_0x3c8;
    u8 pad_0x3c9[0x7];
    UnkScene3DMaps_ovb0076 maps;  // 0x3d0
    f32 field_0x460;
    f32 field_0x464;
    bool field_0x468;
    bool field_0x469;  // registered with the item list
    u8 pad_0x46a[0x6];
    UnkScene3DItem_ovb0076 item;  // 0x470
    u8 field_0x4a4;
    u8 pad_0x4a5[0x3];
    UnkScene3DInput_ovb0076 inputs[120];  // 0x4a8
    u32 input_count;  // 0xe08
    u8 pad_0xe0c[0x4];
    UnkScene3DModel_ovb0076* model;  // 0xe10
    CRntString field_0xe18;
    f32 field_0xe48;
    f32 field_0xe4c;
    bool field_0xe50;
    bool field_0xe51;
};
static_assert(__builtin_offsetof(UnkScene3D_ovb0076, field_0xe18) == 0xe18, "UnkScene3D_ovb0076 layout");

void fn_7100b74158(UnkChainedMap* self);  // the maps' destructors
void fn_7100b74248(UnkChainedMap* self);
void fn_71011f3cfc(UnkScene3D_ovb0076* self);  // the base destructor

void fn_7100b72688(UnkScene3D_ovb0076* self) {
    self->vtable = &g_unk_7101a9e968[2];
    fn_710007ecd4(&self->field_0xe18);
    fn_710119ad3c(&self->item);
    UnkChainedMap* first = &self->maps.first;
    fn_7100b74158(&self->maps.second);
    fn_7100b74248(first);
    UnkScene3DList_ovb0076& objects = self->objects;
    objects.vtable = &g_unk_7101a9ebb8[2];
    objects.Clear();
    fn_710007ecd4(&self->field_0x338);
    fn_710007ecd4(&self->field_0x2e0);
    fn_710007ecd4(&self->name);
    fn_71011f3cfc(self);
}

void fn_71011f3e0c(UnkScene3D_ovb0076* self);  // the base's reset

void fn_7100b727c4(UnkScene3D_ovb0076* self) {
    fn_71011f3e0c(self);
    self->field_0x3c8 = false;
    self->field_0x2c4 = 0.0f;
    self->field_0x2bc = -1.0f;
    self->field_0x2dc = 0;
    fn_710007e77c(&self->field_0x2e0, g_unk_710162e2ed);
    self->field_0x460 = 5.0f;
    self->field_0x464 = 5.0f;
    self->field_0x224 = -1.0f;
    self->field_0x228 = 0.0f;
    self->field_0x220 = -1.0f;
    self->field_0x234 = -1.0f;
    self->field_0x238 = -1.0f;
    self->field_0x23c = -10000.0f;
    self->field_0x2c0 = -1.0f;
    self->field_0x2c8 = 0.0f;
    self->field_0x2cc = 0.0f;
    self->field_0x2d0 = -1.0f;
    self->field_0x2d4 = 0.0f;
    self->field_0x2d8 = 0;
    self->field_0xe48 = 0.0f;
    self->field_0xe4c = 1.0f;
    if (self->dirty != 1) {
        self->dirty = 1;
        reinterpret_cast<void (*)(UnkScene3D_ovb0076*)>(self->vtable[0xa0 / 8])(self);
    }
}

struct UnkListedItem;
struct UnkItemListTag;
using UnkItemList = base::UnkSList<UnkItemListTag>;
void fn_710119ba7c(UnkItemList* self, UnkListedItem* item);

static inline void* ItemList_ovb0076() {
    if (!g_unk_7172729910) {
        void* p = fn_71000839a0(0x2208, 0x3f);
        fn_710119aee8(p);
        g_unk_7172729910 = p;
    }
    return g_unk_7172729910;
}

CRntString fn_71011cd4f8(UnkScene3D_ovb0076* self);  // the object's name
CRntString fn_710007f050(const char* prefix, const CRntString* str);

static inline CRntString Prefixed_ovb0076(const char* prefix, const CRntString& str) {
    return fn_710007f050(prefix, &str);
}

void fn_7100b72e5c(UnkScene3D_ovb0076* self, bool on) {
    if (self->field_0x469 == on)
        return;
    self->field_0x469 = on;
    if (on) {
        self->item.name = Prefixed_ovb0076("CScene3D:", fn_71011cd4f8(self));
        self->field_0x4a4 = 1;
        fn_710119ba7c(static_cast<UnkItemList*>(ItemList_ovb0076()),
                      reinterpret_cast<UnkListedItem*>(&self->item));
    } else {
        fn_710119ae24(ItemList_ovb0076(), &self->item, true);
    }
}

struct UnkRemovedValue_b60;

// What a scene's list node holds: an object registered with the game manager.
struct UnkScene3DObject_ovb0076 {
    UnkRemovedValue_b60* value;
};

void fn_7100b72f64(UnkScene3D_ovb0076* self);
void fn_71011f426c(UnkScene3D_ovb0076* self);  // the base's

void fn_7100b72d44(UnkScene3D_ovb0076* self) {
    if (self->field_0x469) {
        self->field_0x469 = false;
        fn_710119ae24(ItemList_ovb0076(), &self->item, true);
    }
    if (UnkScene3DModel_ovb0076* model = self->model) {
        fn_71002199d8(g_unk_7101d47300, reinterpret_cast<UnkItemBase_71011a91c4*>(model), 0);
        self->model = nullptr;
    }
    UnkScene3DList_ovb0076& objects = self->objects;
    for (UnkScene3DNode_ovb0076* n = objects.head; n; n = n->next) {
        UnkScene3DObject_ovb0076* object = static_cast<UnkScene3DObject_ovb0076*>(n->value);
        if (object) {
            if (object->value)
                fn_7100debc28(g_unk_7101d47310, object->value);
            fn_7100083c00(object);
            n->value = nullptr;
        }
    }
    objects.Clear();
    fn_7100b72f64(self);
    fn_71011f426c(self);
}

// A chained map that starts with 11 empty buckets.
static inline __attribute__((always_inline)) void Construct11_ovb0076(UnkChainedMap* map) {
    new (map) UnkChainedMap();
    map->ResizeBuckets(11);
}

void fn_7100b724a4(UnkScene3DMaps_ovb0076* self) {
    Construct11_ovb0076(&self->first);
    Construct11_ovb0076(&self->second);
}

struct UnkScene3DMapNode_ovb0076 {
    base::global::CStrId key;
    u8 unk_0x8[0x8];
    UnkScene3DMapNode_ovb0076* next;
};

// Releases every node's key and frees the node, then empties the buckets.
static inline __attribute__((always_inline)) void Clear_ovb0076(UnkChainedMap* map) {
    if (map->count) {
        for (auto* node = reinterpret_cast<UnkScene3DMapNode_ovb0076*>(map->tail); node;) {
            UnkScene3DMapNode_ovb0076* next = node->next;
            node->key.Reset();
            map->allocator->vf_0x48(node);
            node = next;
        }
        __builtin_memset(map->buckets.data, 0, map->buckets.capacity * u64(8));
        map->count = 0;
        map->head = nullptr;
        map->tail = nullptr;
    }
}

void fn_7100b73454(UnkScene3DMaps_ovb0076* self) {
    Clear_ovb0076(&self->first);
    Clear_ovb0076(&self->second);
}

const CRntString* fn_7100def670(UnkGameManager* self);  // the data root
CRntString fn_710007ef90(const CRntString* a, const char* b);  // concatenation

static inline CRntString Concat_ovb0076(const CRntString& a, const CRntString& b) { return fn_710007eee0(&a, &b); }
static inline CRntString Concat_ovb0076(const CRntString& a, const char* b) { return fn_710007ef90(&a, b); }

static inline void Assign_ovb0076(CRntString* self, const CRntString& other) { fn_710007eaa0(self, &other); }

extern base::file::UnkFileDevice* g_unk_7101d4d828;

static inline base::file::UnkFileDevice* FileDevice_ovb0076() {
    if (!g_unk_7101d4d828) {
        auto* device = static_cast<base::file::UnkFileDevice*>(fn_71000839a0(0x1348, 0x3f));
        fn_71000893a8(device);
        g_unk_7101d4d828 = device;
    }
    return g_unk_7101d4d828;
}

// A file-path id made from a string (fn_710001a270), released as CFilePathStrId is.
struct UnkScenePathId_ovb0076 : base::global::UnkFilePathStrIdBase {
    explicit UnkScenePathId_ovb0076(const CRntString& str) { fn_710001a270(this, str, true); }
    ~UnkScenePathId_ovb0076() { reinterpret_cast<base::global::CFilePathStrId*>(this)->~CFilePathStrId(); }
};

bool fn_7100089ef4(base::file::UnkFileDevice* self, const base::global::UnkFilePathStrIdBase* path);  // unloads
void fn_71011f5cd0(UnkGuiFrame* frame);

static inline void Unload_ovb0076(base::file::UnkFileDevice* device, const UnkScenePathId_ovb0076& path) {
    fn_7100089ef4(device, &path);
}

// Drops the model: the frame's objects and the maps, the render manager's layer, and the file.
void fn_7100b72f64(UnkScene3D_ovb0076* self) {
    if (self->frame != 0x7fffffff) {
        fn_71011f5cd0(fn_71011b7a4c(g_unk_717271e8f8, self->frame));
        fn_7100b73454(&self->maps);
        fn_71002191a4(g_unk_7101d47300, 4);
        self->frame = 0x7fffffff;
    }
    if (self->name.data && self->name.data[0]) {
        // The model's file: <root>maps/levels/c50_gui/<name>/<name>.bmssd.
        CRntString path("maps/levels/");
        Assign_ovb0076(&path, Concat_ovb0076(Concat_ovb0076(Concat_ovb0076(Concat_ovb0076(Concat_ovb0076(Concat_ovb0076(
                                                                                   *fn_7100def670(g_unk_7101d47310), path),
                                                                               "c50_gui/"),
                                                                self->name),
                                                 "/"),
                                  self->name),
                   ".bmssd"));
        Unload_ovb0076(FileDevice_ovb0076(), UnkScenePathId_ovb0076(path));
        fn_710007e77c(&self->name, g_unk_710162e2ed);
    }
}
