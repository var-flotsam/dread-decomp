// GUI::CDisplayObjectDef's unit (tools/tu_map.py): the constructor and destructor of a definition
// over the base fn_7100023838 builds (vtable g_unk_7101a9a488 + 0x10): a name string (+0x8) and two
// arrays of owned objects, children deleted through fn_7100b0b96c (+0x38) and polymorphic objects
// (+0x60, 20 reserved), and a cleared word at +0x88. Also adding an item unless one has its name
// (fn_7100b0704c) and the case-insensitive search of the tree for a name (fn_7100b0694c). Names and
// layouts are placeholders.
#include "base/UnkNamedRegistry.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "types.h"

#include "../../../game_aca57c/UnkBufferString.h"

extern char g_unk_7101a9a488[];
void fn_7100023838(void* self);  // as src/base/tunable/CTunable/fn_7100138090.cpp declares it
void fn_710002384c(void* self);  // as src/base/file/nx/CDeviceNX/fn_710002ddc0.cpp declares it

struct UnkDisplayChild_ovb0108 {
    ~UnkDisplayChild_ovb0108() asm("fn_7100b0b96c");
};

struct UnkDisplayItem_ovb0108 {
    virtual void vf_0x0();
    virtual ~UnkDisplayItem_ovb0108();
    base::global::CRntString name;  // 0x8
};

struct UnkCompared_0x30;
bool fn_710007f120(const UnkCompared_0x30* a, const UnkCompared_0x30* b);

struct UnkDisplayObjectDef_ovb0108 {
    void* vtable;
    base::global::CRntString name;                         // 0x8
    base::UnkVector<UnkDisplayChild_ovb0108*> children;    // 0x38
    base::UnkVector<UnkDisplayItem_ovb0108*> items;        // 0x60
    void* field_0x88;
};
static_assert(sizeof(UnkDisplayObjectDef_ovb0108) == 0x90, "UnkDisplayObjectDef_ovb0108 layout");

void fn_7100b064bc(UnkDisplayObjectDef_ovb0108* self) {
    fn_7100023838(self);
    self->vtable = g_unk_7101a9a488 + 0x10;
    fn_710007e704(reinterpret_cast<base::UnkNamedEntry*>(&self->name), g_unk_710162e2ed);
    new (&self->children) base::UnkVector<UnkDisplayChild_ovb0108*>(0xf);
    new (&self->items) base::UnkVector<UnkDisplayItem_ovb0108*>(0xc);
    self->items.Reserve(20);
    self->field_0x88 = nullptr;
}

template <class T>
static inline void DeleteAll(base::UnkVector<T*>& v) {
    for (T** it = v.data; it != v.data + v.count; ++it) {
        if (*it) {
            delete *it;
            *it = nullptr;
        }
    }
    v.count = 0;
}

void fn_7100b065a8(UnkDisplayObjectDef_ovb0108* self) {
    self->vtable = g_unk_7101a9a488 + 0x10;
    DeleteAll(self->items);
    self->field_0x88 = nullptr;
    DeleteAll(self->children);
    self->items.Free();
    self->children.Free();
    fn_710007ecd4(&self->name);
    fn_710002384c(self);
}

// Adds an item unless one with the same name is there; whether it was added.
bool fn_7100b0704c(UnkDisplayObjectDef_ovb0108* self, UnkDisplayItem_ovb0108* item) {
    base::UnkVector<UnkDisplayItem_ovb0108*>& items = self->items;
    bool found = false;
    for (UnkDisplayItem_ovb0108** it = items.data; it != items.data + items.count;) {
        if (found)
            return false;
        UnkDisplayItem_ovb0108* other = *it++;
        found = fn_710007f120(reinterpret_cast<const UnkCompared_0x30*>(&other->name),
                              reinterpret_cast<const UnkCompared_0x30*>(&item->name));
    }
    if (found)
        return false;
    items.PushBack(item);
    return true;
}

using UnkDisplayName_ovb0108 = UnkBufferString<0x100, g_unk_7101a194b8>;

// Copies `src` lower-cased (ASCII) into `dst`.
static inline void ToLower(char* dst, const char* src) {
    dst[0] = '\0';
    u32 length = __builtin_strlen(src);
    if (length) {
        for (u32 i = 0; i < length; ++i) {
            char c = src[i];
            dst[i] = (u8)(c - 'A') < 26 ? c + ('a' - 'A') : c;
        }
        dst[length] = '\0';
    }
}

// The item named `name` (ignoring case) in the tree under this one, this one included.
UnkDisplayItem_ovb0108* fn_7100b0694c(UnkDisplayItem_ovb0108* self, const base::global::CRntString& name) {
    bool equal;
    {
        char buffer[0x100];
        ToLower(buffer, self->name.data);
        UnkDisplayName_ovb0108 a(buffer);
        ToLower(buffer, name.data);
        UnkDisplayName_ovb0108 b(buffer);
        equal = fn_710007f120(reinterpret_cast<const UnkCompared_0x30*>(a.Str()),
                              reinterpret_cast<const UnkCompared_0x30*>(b.Str()));
    }
    if (equal)
        return self;
    base::UnkVector<UnkDisplayItem_ovb0108*>& items = reinterpret_cast<UnkDisplayObjectDef_ovb0108*>(self)->items;
    for (UnkDisplayItem_ovb0108** it = items.data; it != items.data + items.count; ++it) {
        if (UnkDisplayItem_ovb0108* found = fn_7100b0694c(*it, name))
            return found;
    }
    return nullptr;
}
