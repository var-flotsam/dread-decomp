// GUI::CPopUpGeneric (unit 1709 of tools/tu_map.py): the constructor. After the display object's
// constructor it sets the vtable, two id records, two groups of three vectors and two bound callbacks.
// Names and layouts are placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/GUI/CPopUpGeneric.h"
#include "game/UnkGuiDisplayObject.h"
#include "types.h"

using base::global::CRntVector;
using base::global::UnkStringData;
using base::global::UnkStringPool;

void fn_7100b61e50();
void fn_7100b61e34();

// A string id copied from a global id, its count incremented unless it is the pool's empty string.
struct UnkPopUpId_lb60fb0 {
    UnkStringData* data;

    UnkPopUpId_lb60fb0(const base::global::UnkStrIdBase& other, UnkStringPool* const& pool) : data(other.data) {
        if (&pool->empty != data)
            ++data->ref_count;
    }
    UnkPopUpId_lb60fb0(const base::global::UnkFilePathStrIdBase& other, UnkStringPool* const& pool)
        : data(other.data) {
        if (&pool->empty != data)
            ++data->ref_count;
    }
};

// A name and a file path, both empty, and a cleared word.
struct UnkPopUpRecord_lb60fb0 {
    UnkPopUpId_lb60fb0 name;
    UnkPopUpId_lb60fb0 path;
    u64 field_0x10;

    UnkPopUpRecord_lb60fb0()
        : name(g_unk_7101d470e0, g_unk_7101d47108), path(g_unk_7101d470e8, g_unk_7101d47200), field_0x10(0) {}
};

// Three vectors (allocation flags 0xf) and a flag.
struct UnkPopUpLists_lb60fb0 {
    CRntVector<void*> lists[3];
    bool field_0x78;

    UnkPopUpLists_lb60fb0() : lists{CRntVector<void*>(0xf), CRntVector<void*>(0xf), CRntVector<void*>(0xf)},
                              field_0x78(false) {}
};

// A callback bound to its owner.
struct UnkPopUpCallback_lb60fb0 {
    void* field_0x0;
    void* owner;
    void (*function)();
};

struct UnkPopUpGeneric_lb60fb0 {
    void* vtable;
    u8 pad_0x8[0x228];
    bool field_0x230;
    bool field_0x231;
    u8 pad_0x232[0x6];
    void* field_0x238;
    void* field_0x240;
    u8 pad_0x248[0x30];
    UnkPopUpRecord_lb60fb0 records[2];   // 0x278
    UnkPopUpLists_lb60fb0 lists[2];      // 0x2a8
    UnkPopUpCallback_lb60fb0 field_0x3a8;
    UnkPopUpCallback_lb60fb0 field_0x3c0;
    bool field_0x3d8;
};
static_assert(__builtin_offsetof(UnkPopUpGeneric_lb60fb0, records) == 0x278, "UnkPopUpGeneric_lb60fb0 layout");
static_assert(__builtin_offsetof(UnkPopUpGeneric_lb60fb0, lists) == 0x2a8, "UnkPopUpGeneric_lb60fb0 layout");
static_assert(__builtin_offsetof(UnkPopUpGeneric_lb60fb0, field_0x3d8) == 0x3d8, "UnkPopUpGeneric_lb60fb0 layout");

void fn_7100b60fb0(UnkPopUpGeneric_lb60fb0* self) {
    fn_71011d8d24(self);
    self->vtable = g_unk_7101a9da40 + 0x10;
    self->field_0x230 = false;
    self->field_0x231 = false;
    self->field_0x238 = nullptr;
    self->field_0x240 = nullptr;
    new (&self->records[0]) UnkPopUpRecord_lb60fb0();
    new (&self->records[1]) UnkPopUpRecord_lb60fb0();
    new (&self->lists[0]) UnkPopUpLists_lb60fb0();
    new (&self->lists[1]) UnkPopUpLists_lb60fb0();
    self->field_0x3a8.field_0x0 = nullptr;
    self->field_0x3a8.owner = self;
    self->field_0x3a8.function = fn_7100b61e50;
    self->field_0x3c0.field_0x0 = nullptr;
    self->field_0x3c0.owner = self;
    self->field_0x3c0.function = fn_7100b61e34;
    self->field_0x3d8 = false;
}
