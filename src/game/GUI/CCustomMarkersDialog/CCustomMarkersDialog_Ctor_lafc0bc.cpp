// GUI::CCustomMarkersDialog's unit (tools/tu_map.py unit 1660): the dialog's constructor. After the
// display object's base, it clears the references and the list, starts with no selection, binds its
// callback (fn_7100afe1ac) and points its three id and path pairs at their assets.
// Names and layouts are placeholders.
#include <new>

#include "base/UnkStrIdPathPair.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/GUI/CCustomMarkersDialog.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkTU2527.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

u32 fn_7100afe1ac(char* p0);

__attribute__((visibility("hidden"))) extern CFilePathStrId g_unk_717275d3e8;
__attribute__((visibility("hidden"))) extern CFilePathStrId g_unk_717275d3f0;
__attribute__((visibility("hidden"))) extern CFilePathStrId g_unk_717275d3f8;

struct UnkMarkersRef_lafc0bc {
    void* ptr;

    UnkMarkersRef_lafc0bc() : ptr(nullptr) {}
};

struct UnkMarkersRefs_lafc0bc {
    void** data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkMarkersRefs_lafc0bc()
        : data(nullptr), count(0), capacity(0), step(10), field_0x14(true), flags(15),
          allocator(fn_7100083990()) {}
};

// A bound callback: the object and the function called with it.
struct UnkMarkersCallback_lafc0bc {
    void* set;
    char* object;
    u32 (*function)(char*);

    explicit UnkMarkersCallback_lafc0bc(char* obj)
        : set(nullptr), object(obj), function(&fn_7100afe1ac) {}
};

// An id and path pair copied from two existing ids.
struct UnkMarkersIds_lafc0bc {
    CStrId id;
    CFilePathStrId path;

    UnkMarkersIds_lafc0bc(const CStrId& i, const CFilePathStrId& p) : id(i), path(p) {}
};

struct UnkMarkersAsset_lafc0bc {
    CStrId id;
    CFilePathStrId path;
    UnkCountedResource* resource;

    UnkMarkersAsset_lafc0bc() : id(base::global::CStrId::Empty()), path(g_unk_7101d470e8), resource(nullptr) {}

    void Set(const UnkMarkersIds_lafc0bc& ids) {
        fn_71000267f8(reinterpret_cast<UnkStrIdPathResource*>(this),
                      reinterpret_cast<const UnkStrIdPathPair&>(ids));
    }
};

struct UnkCustomMarkersDialog_lafc0bc {
    void** vtable;
    u8 pad_0x8[0x228];
    UnkMarkersRef_lafc0bc refs[6];     // 0x230
    UnkMarkersRefs_lafc0bc list;       // 0x260
    CStrId name;                       // 0x288
    u32 field_0x290;
    s32 field_0x294;
    s32 field_0x298;
    bool field_0x29c;
    bool field_0x29d;
    bool field_0x29e;
    UnkMarkersCallback_lafc0bc callback;  // 0x2a0
    UnkMarkersAsset_lafc0bc field_0x2b8;
    UnkMarkersAsset_lafc0bc field_0x2d0;
    UnkMarkersAsset_lafc0bc field_0x2e8;
};
static_assert(__builtin_offsetof(UnkCustomMarkersDialog_lafc0bc, callback) == 0x2a0, "callback");
static_assert(sizeof(UnkCustomMarkersDialog_lafc0bc) == 0x300, "layout");

// An empty string id: the default constructor, inlined.
static inline void InitEmptyStrId(CStrId* id) {
    id->data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    id->data = &g_unk_7101d47108->empty;
}

void fn_7100afc0bc(UnkCustomMarkersDialog_lafc0bc* self) {
    fn_71011d8d24(self);
    self->vtable = g_unk_7101a99520 + 2;
    for (UnkMarkersRef_lafc0bc& ref : self->refs)
        new (&ref) UnkMarkersRef_lafc0bc();
    new (&self->list) UnkMarkersRefs_lafc0bc();
    InitEmptyStrId(&self->name);
    self->field_0x290 = 0;
    self->field_0x294 = -1;
    self->field_0x298 = -1;
    self->field_0x29c = false;
    self->field_0x29d = false;
    self->field_0x29e = false;
    new (&self->callback) UnkMarkersCallback_lafc0bc(reinterpret_cast<char*>(self));
    new (&self->field_0x2b8) UnkMarkersAsset_lafc0bc();
    new (&self->field_0x2d0) UnkMarkersAsset_lafc0bc();
    new (&self->field_0x2e8) UnkMarkersAsset_lafc0bc();

    self->field_0x2b8.Set(UnkMarkersIds_lafc0bc(g_unk_71727b9d30, g_unk_717275d3f8));
    self->field_0x2d0.Set(UnkMarkersIds_lafc0bc(g_unk_71727b9d30, g_unk_717275d3e8));
    self->field_0x2e8.Set(UnkMarkersIds_lafc0bc(g_unk_71727b9d30, g_unk_717275d3f0));
}
