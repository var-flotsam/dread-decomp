// CEmmySancAIComponent's unit (tools/tu_map.py unit 793): the component's constructor (vtable 0x7101a46180),
// the counterpart of the destructor in CEmmySancAIComponent_Dtor_A3_b69.cpp: the base constructor
// fn_710053e5c0, a cleared word, an empty array of 0x40-byte elements (flags 0xf), a byte, two empty callback
// lists (base vtable 0x7101a20998, then 0x7101a20950), then the material path at +0x147f0 set from a literal.
// Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "game/UnkTU811.h"
#include "types.h"

#include <new>

extern void* g_unk_7101a46180[];
extern void* g_unk_7101a20950[];
extern void* g_unk_7101a20998[];

// As src/game/CEmmyCaveAIComponent/CEmmyCaveAIComponent.cpp declares it.
struct UnkComponent;

// A file-path id copied in (as src/base/UnkUnit25/UnkIdPathIterAssign.cpp's UnkUnit25PathId).
struct UnkSancPathId_A3_b71 {
    base::global::UnkStringData* data;

    UnkSancPathId_A3_b71& operator=(const base::global::CFilePathStrId& other) {
        if (data != other.data) {
            base::global::UnkStringData* d = other.data;
            base::global::UnkStringPool* pool = g_unk_7101d47200;
            if (d != &pool->empty)
                ++d->ref_count;
            if (data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0)
                    fn_71000803a4(pool, data);
            }
            data = d;
        }
        return *this;
    }
};

struct UnkSancElem_A3_b71 {
    u8 raw[0x40];
};

struct UnkSancListVt_A3_b71 {
    char* vtable;
    UnkSancListVt_A3_b71() { vtable = reinterpret_cast<char*>(g_unk_7101a20998) + 0x10; }
};

struct UnkSancListBase_A3_b71 : UnkSancListVt_A3_b71 {
    base::UnkVector<void*> callables{0xf};
    u32 field_0x30 = 0;
};

struct UnkSancList_A3_b71 : UnkSancListBase_A3_b71 {
    void* owner;
    UnkSancList_A3_b71() {
        vtable = reinterpret_cast<char*>(g_unk_7101a20950) + 0x10;
        owner = nullptr;
    }
};
static_assert(sizeof(UnkSancList_A3_b71) == 0x40, "UnkSancList_A3_b71 layout");

struct UnkSancVt_A3_b71 {
    char* vtable;
    u8 pad_0x8[0x147e8];
    UnkSancPathId_A3_b71 path;
    u8 pad_0x147f8[0x2a6f8 - 0x147f8];
    UnkSancVt_A3_b71() {
        fn_710053e5c0(reinterpret_cast<UnkComponent*>(this));
        vtable = reinterpret_cast<char*>(g_unk_7101a46180) + 0x10;
    }
};

struct UnkSanc_A3_b71 : UnkSancVt_A3_b71 {
    u64 field_0x2a6f8 = 0;
    base::UnkVector<UnkSancElem_A3_b71> elems{0xf};
    bool field_0x2a728 = false;
    UnkSancList_A3_b71 first;
    UnkSancList_A3_b71 second;

    UnkSanc_A3_b71() {
        path = base::global::CFilePathStrId(base::global::UnkStrIdInline(),
                                            "actors/characters/emmy/fx/imats/conepatrolsearch_sanc_material.bsmat",
                                            true);
    }
};
static_assert(__builtin_offsetof(UnkSanc_A3_b71, first) == 0x2a730, "UnkSanc_A3_b71 layout");

__attribute__((flatten)) void fn_71004ff028(UnkSanc_A3_b71* self) {
    new (self) UnkSanc_A3_b71;
}
