// The GUI root (game/UnkGuiRoot.h, the global g_unk_717271e8f8), the unit tools/tu_map.py names
// after gui/scripts/guimanager.lua. The unit also holds the reflected pointer types
// std::unique_ptr<GUI::CGUIManager::SkinContainer> and ...::SpriteSheetContainer, so the root is
// most likely GUI::CGUIManager. Holds the per-frame update (fn_71011b51c4), the composition file
// reader (fn_71011b6208), the focus setter (fn_71011b6bbc), the setter of the reference at +0x260
// (fn_71011b732c) and the sprite sheet file reader (fn_71011b9ce4). The file reader's types repeat
// those of src/game/UnkObjectFileRead.cpp (the same names and layouts). Names and layouts are
// placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkHashStream.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntString.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflObjectRead.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiEvent.h"
#include "game/UnkGuiRoot.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"
#include "UnkClone.h"

#include "../../game_s7/UnkPropBagS7.h"

using unk_prop_bag::UnkPropBag;

// The event's property bag (+0x10), created on first use and then owned by the event.
UnkPropBag* fn_71012024f4(UnkGuiEvent* ev);
bool fn_71011cffe4(UnkGuiDisplayObject* object, UnkGuiDisplayObject* container);
extern const base::global::CStrId g_unk_7172a0d0d0 __attribute__((visibility("hidden")));

// The root's objects as fn_71011b51c4 updates them: the list it walks (+0x78, kept sorted by
// fn_71011bc0b0), the objects added since the last update (+0xa0) and references to objects to
// report to fn_71011b57c4 afterwards (+0xc8).
struct UnkWeakRef_71011b51c4 {
    UnkWeakHandle* ptr;

    ~UnkWeakRef_71011b51c4() {
        if (UnkWeakHandle* p = ptr) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

struct UnkGuiRootObjects {
    u8 unk_0x0[0x70];
    bool updating;
    base::UnkVector<UnkGuiDisplayObject*> objects;
    base::UnkVector<UnkGuiDisplayObject*> added;
    base::UnkVector<UnkWeakRef_71011b51c4> reported;
};
static_assert(__builtin_offsetof(UnkGuiRootObjects, objects) == 0x78, "UnkGuiRootObjects placeholder layout");
static_assert(__builtin_offsetof(UnkGuiRootObjects, reported) == 0xc8, "UnkGuiRootObjects placeholder layout");

// The element accessor of the object list is the shared out-of-line copy (src/clones/, fn_71011b5674).
struct UnkClone_fn_71011b5674;
using UnkObjectSlot = UnkCloneElem<UnkClone_fn_71011b5674, 8>;
extern template UnkObjectSlot& base::UnkVector<UnkObjectSlot>::GrowAt(u32);

static inline UnkGuiDisplayObject* ObjectAt(base::UnkVector<UnkGuiDisplayObject*>* v, u32 index) {
    return *reinterpret_cast<UnkGuiDisplayObject**>(
        reinterpret_cast<base::UnkVector<UnkObjectSlot>*>(v)->GrowAt(index).data);
}

// The display object's per-frame update (vtable slot 0x58).
struct UnkGuiUpdatable {
    virtual void vf_0x0(); virtual void vf_0x8(); virtual void vf_0x10(); virtual void vf_0x18();
    virtual void vf_0x20(); virtual void vf_0x28(); virtual void vf_0x30(); virtual void vf_0x38();
    virtual void vf_0x40(); virtual void vf_0x48(); virtual void vf_0x50();
    virtual void vf_0x58(f32 dt);
};

// The libc++ sort of the object list (src/game_s7/fn_71011bc0b0.cpp names it in assembly; its
// comparator argument is dropped).
extern "C" void fn_71011bc0b0(UnkGuiDisplayObject** first, UnkGuiDisplayObject** last);
bool fn_71011ceac0(UnkGuiDisplayObject* self);
bool fn_71011ceb34(UnkGuiDisplayObject* self);

// Room for 0x800 objects on the stack: a count, then the pointers (4-byte aligned).
struct UnkGuiObjectList {
    u32 count;
    u32 storage[0x800 * 2];

    UnkGuiDisplayObject** begin() { return reinterpret_cast<UnkGuiDisplayObject**>(storage); }
    UnkGuiDisplayObject** end() { return begin() + count; }
    void PushBack(UnkGuiDisplayObject* p) {
        begin()[count] = p;
        ++count;
    }
};

static inline u64 IndexOf(base::UnkVector<UnkGuiDisplayObject*>* v, UnkGuiDisplayObject* p) {
    for (u64 i = 0; i < v->count; ++i)
        if (v->data[i] == p)
            return i;
    return v->count;
}

static inline void EraseAt(base::UnkVector<UnkGuiDisplayObject*>* v, u32 i) {
    if (i < v->count) {
        UnkGuiDisplayObject** p = &v->data[i];
        __builtin_memcpy(p, p + 1, (u64)(v->count - i - 1) * 8);
        --v->count;
    }
}

static inline bool Contains(base::UnkVector<UnkGuiDisplayObject*>* v, UnkGuiDisplayObject* p) {
    for (u32 i = 0; i < v->count; ++i)
        if (v->data[i] == p)
            return true;
    return false;
}

// The root's per-frame update: takes in the objects added since the last one, sorts the list,
// updates each object and drops the ones that are done (not +0x188, not +0x184, and not shown or
// neither fn_71011ceac0 nor fn_71011ceb34 holds), then reports the referenced objects.
// NON_MATCHING(minor): register allocation (&objects is formed after the sort instead of before it,
// so the removal loop's registers shift by one), and the two searches of the removal loop end on
// `<` instead of `!=` with their blocks laid out differently; the same loads, stores and calls.
void fn_71011b51c4(UnkGuiRoot* root, f32 dt) {
    UnkGuiRootObjects* self = reinterpret_cast<UnkGuiRootObjects*>(root);
    self->updating = true;
    UnkGuiObjectList done;
    done.count = 0;
    for (UnkGuiDisplayObject** it = self->added.data; it != self->added.data + self->added.count; ++it)
        self->objects.PushBack(*it);
    self->added.count = 0;
    fn_71011bc0b0(self->objects.data, self->objects.data + self->objects.count);
    // No code: keeps the later accesses from sharing one address register with the loop above.
    asm("" : "+r"(self));
    s32 n = self->objects.count;
    for (s32 i = 0; i < n; ++i) {
        UnkGuiDisplayObject* obj = ObjectAt(&self->objects, i);
        reinterpret_cast<UnkGuiUpdatable*>(obj)->vf_0x58(dt);
        const char* raw = reinterpret_cast<const char*>(obj);
        if (!raw[0x188] && !raw[0x184] &&
            (!fn_71011cde14(obj)->field_0x3c || (!fn_71011ceac0(obj) && !fn_71011ceb34(obj)))) {
            if (Contains(&self->objects, obj))
                done.PushBack(obj);
        }
    }
    self->updating = false;
    for (UnkGuiDisplayObject** it = done.begin(); it != done.end(); ++it) {
        UnkGuiDisplayObject* obj = *it;
        u32 i = 0;
        for (; i < self->added.count; ++i)
            if (self->added.data[i] == obj)
                break;
        if (i < self->added.count)
            EraseAt(&self->added, i);
        else
            EraseAt(&self->objects, IndexOf(&self->objects, obj));
        reinterpret_cast<char*>(obj)[0x187] = 0;
    }
    done.count = 0;
    for (UnkWeakRef_71011b51c4* it = self->reported.data; it != self->reported.data + self->reported.count; ++it) {
        if (it->ptr && it->ptr->target)
            fn_71011b57c4(reinterpret_cast<UnkGuiRoot*>(self), it->ptr->target);
    }
    self->reported.Clear();
}

// The two weak references the root keeps: the focused object (+0x230) and the container the
// focus is kept inside (+0x238).
static inline UnkWeakHandle*& FocusRef(UnkGuiRoot* root) {
    return *reinterpret_cast<UnkWeakHandle**>(reinterpret_cast<char*>(root) + 0x230);
}
static inline UnkWeakHandle* ScopeRef(UnkGuiRoot* root) {
    return *reinterpret_cast<UnkWeakHandle**>(reinterpret_cast<char*>(root) + 0x238);
}
static inline UnkGuiDisplayObject* TargetOf(UnkWeakHandle* h) {
    return reinterpret_cast<UnkGuiDisplayObject*>(h->target);
}

void fn_71011b6bbc(UnkGuiRoot* root, UnkGuiDisplayObject* object, bool value) {
    UnkWeakHandle* cur = FocusRef(root);
    if ((cur ? TargetOf(cur) : nullptr) == object)
        return;
    if (cur && TargetOf(cur)) {
        s32 kind = 3;
        UnkGuiEvent ev;
        fn_710120235c(&ev, TargetOf(cur), &kind, nullptr);
        fn_71011b6e0c(root, &ev);
        fn_7101202488(&ev);
    }
    UnkWeakHandle* handle = nullptr;
    if (object && fn_71011cde14(object)->field_0x3e && object->field_0x181) {
        UnkWeakHandle* scope = ScopeRef(root);
        UnkGuiDisplayObject* container = scope ? TargetOf(scope) : nullptr;
        if (container && container != object) {
            if (!fn_710009d50c(container->GetClass(), fn_71011d7b84()))
                container = nullptr;
            if (!fn_71011cffe4(object, container))
                goto assign;
        }
        handle = GetWeakHandle(reinterpret_cast<UnkWeakTarget*>(object));
    }
assign:
    AssignRef(&FocusRef(root), handle);
    if (handle && TargetOf(handle)) {
        s32 kind = 2;
        UnkGuiEvent ev;
        fn_710120235c(&ev, TargetOf(handle), &kind, nullptr);
        fn_710050f4ac(fn_71012024f4(&ev), g_unk_7172a0d0d0, value);
        fn_71011b6e0c(root, &ev);
        fn_7101202488(&ev);
    }
}

// Points the weak reference at +0x260 at `object`. When `notify` is set the old object is told it
// lost it (event 0x1a) and the new one that it got it (event 0x19, unless the flag at +0x258 is set).
void fn_71011b732c(UnkGuiRoot* root, UnkGuiDisplayObject* object, bool notify) {
    UnkWeakHandle*& ref = *reinterpret_cast<UnkWeakHandle**>(reinterpret_cast<char*>(root) + 0x260);
    UnkWeakHandle* cur = ref;
    if ((cur ? TargetOf(cur) : nullptr) == object)
        return;
    if (notify && cur && TargetOf(cur)) {
        s32 kind = 0x1a;
        UnkGuiEvent ev;
        fn_710120235c(&ev, TargetOf(cur), &kind, nullptr);
        fn_71011b6e0c(root, &ev);
        fn_7101202488(&ev);
    }
    UnkWeakHandle* handle = object ? GetWeakHandle(reinterpret_cast<UnkWeakTarget*>(object)) : nullptr;
    AssignRef(&ref, handle);
    if (notify && handle && TargetOf(handle) && !*(reinterpret_cast<bool*>(root) + 0x258)) {
        s32 kind = 0x19;
        UnkGuiEvent ev;
        fn_710120235c(&ev, TargetOf(handle), &kind, nullptr);
        fn_71011b6e0c(root, &ev);
        fn_7101202488(&ev);
    }
}

// The id and path pair (base/UnkStrIdPathPair.h) the caller passes, as
// src/game_hi/fn_71011b69d8.cpp names it.
struct UnkStrPair_0x10 {
    void* a;
    void* b;
};

// The file read into memory and two words the opener sets (+0x38 is zero after a good read).
struct UnkFileReader {
    base::global::CRntFile file;
    u32 field_0x38;
    u32 field_0x3c;
};

bool fn_710007c1ac(UnkFileReader* self, const UnkStrPair_0x10* path, const base::global::CRntString* mode);
void fn_710000ccd8(UnkFileReader* self);

// This unit's parser callbacks.
extern UnkParseCallbacks g_unk_7172a0d128 __attribute__((visibility("hidden")));

void fn_710000f750(UnkParseContext* self, void* root);

// Parses the file into the target, starting from its last entry's node.
static inline void Parse(UnkFileReader* reader, UnkReadTarget* target) {
    UnkParseContext ctx(g_unk_7172a0d128, reader, target);
    UnkReadEntry* end = target->entries + target->count;
    fn_710000f750(&ctx, target->count ? end[-1].field_0x0->field_0x10 : nullptr);
}

// The type of std::unique_ptr<GUI::CDisplayObjectContainer> (src/reflection_types/).
const base::reflection::CType* fn_71011bcf1c();

// The loaded composition, as src/game_hi/fn_71011b69d8.cpp declares it (a display-object
// container; vf_0x10 is its deleting destructor, vf_0x28 its weak-handle slot).
struct UnkTag_71011b69d8 {};
struct UnkRefHandle_71011b69d8;
struct UnkRefSource_71011b69d8 {
    virtual void vf_0x0(); virtual void vf_0x8(); virtual void vf_0x10(); virtual void vf_0x18(); virtual void vf_0x20();
    virtual UnkRefHandle_71011b69d8* vf_0x28();
};

// The std::unique_ptr the file is read into.
struct UnkOwnedSource {
    UnkRefSource_71011b69d8* ptr = nullptr;

    UnkRefSource_71011b69d8* Release() {
        UnkRefSource_71011b69d8* p = ptr;
        if (p)
            ptr = nullptr;
        return p;
    }
    ~UnkOwnedSource() {
        UnkRefSource_71011b69d8* p = ptr;
        ptr = nullptr;
        if (p)
            p->vf_0x10();
    }
};

// Reads the composition file `path` names into a new object and returns it (null when the file
// gave none).
UnkRefSource_71011b69d8* fn_71011b6208(UnkTag_71011b69d8, const UnkStrPair_0x10* path) {
    UnkOwnedSource object;
    UnkFileReader reader;
    fn_710007be8c(&reader.file, 0x16, nullptr);
    reader.field_0x38 = 3;
    reader.field_0x3c = 2;
    bool opened;
    {
        base::global::CRntString mode("rb");
        opened = fn_710007c1ac(&reader, path, &mode);
    }
    if (opened)
        fn_710000ccd8(&reader);
    UnkReadTarget target;
    {
        const base::reflection::CType* type = fn_71011bcf1c();
        base::reflection::UnkReflValue value;
        value.field_0x0 = nullptr;
        value.type = nullptr;
        value.kind = 2;
        fn_71000a18d4(&value, type, &object.ptr, true);
        fn_71000f0568(&target, &value, 0xc);
        fn_71000a1c60(&value);
    }
    if (reader.file.position < reader.file.size && reader.field_0x38 == 0)
        Parse(&reader, &target);
    UnkRefSource_71011b69d8* result = object.Release();
    for (u64 i = 0; i < target.count; ++i) {
        UnkReadEntry& e = target.entries[i];
        if (e.ops) {
            e.ops->destroy(e.storage);
            e.ops = nullptr;
        }
    }
    fn_710007c0e4(&reader.file);
    return result;
}

// GUI::CGUIManager::SpriteSheetContainer (its std::unique_ptr's type is fn_71011b1d2c): sprite
// sheets by name, a string-id hash map (base/UnkCloneHelpers.h). fn_7100b80fe8 destroys it.
struct UnkSpriteSheetTag;
using UnkSpriteSheetMap = base::UnkStrIdHashMap<UnkSpriteSheetTag>;
using UnkSpriteSheetNode = base::UnkStrIdHashNode<UnkSpriteSheetTag>;

const base::reflection::CType* fn_71011b1d2c();
void fn_7100b80fe8(UnkSpriteSheetMap* self);

// The std::unique_ptr the sprite sheet file is read into.
struct UnkOwnedSpriteSheets {
    UnkSpriteSheetMap* ptr = nullptr;

    ~UnkOwnedSpriteSheets() {
        UnkSpriteSheetMap* p = ptr;
        ptr = nullptr;
        if (p) {
            fn_7100b80fe8(p);
            operator delete(p);
        }
    }
};

// The root's sprite sheets (a std::unique_ptr at +8).
static inline UnkSpriteSheetMap*& SpriteSheets(UnkGuiRoot* root) {
    return *reinterpret_cast<UnkSpriteSheetMap**>(reinterpret_cast<char*>(root) + 8);
}

// Reads the sprite sheet file `path` names: the first one read becomes the root's container,
// later ones add their entries to it.
void fn_71011b9ce4(UnkGuiRoot* root, const UnkStrPair_0x10* path) {
    UnkOwnedSpriteSheets loaded;
    UnkFileReader reader;
    fn_710007be8c(&reader.file, 0x16, nullptr);
    reader.field_0x38 = 3;
    reader.field_0x3c = 2;
    bool opened;
    {
        base::global::CRntString mode("rb");
        opened = fn_710007c1ac(&reader, path, &mode);
    }
    if (opened)
        fn_710000ccd8(&reader);
    UnkReadTarget target;
    {
        const base::reflection::CType* type = fn_71011b1d2c();
        base::reflection::UnkReflValue value;
        value.field_0x0 = nullptr;
        value.type = nullptr;
        value.kind = 2;
        fn_71000a18d4(&value, type, &loaded.ptr, true);
        fn_71000f0568(&target, &value, 0xc);
        fn_71000a1c60(&value);
    }
    if (reader.file.position < reader.file.size && reader.field_0x38 == 0)
        Parse(&reader, &target);
    if (!SpriteSheets(root)) {
        UnkSpriteSheetMap* p = loaded.ptr;
        loaded.ptr = nullptr;
        SpriteSheets(root) = p;
    } else {
        for (UnkSpriteSheetNode* n = loaded.ptr->head; n; n = n->next)
            base::refl_StrIdHashInsert(SpriteSheets(root), reinterpret_cast<const base::global::CStrId*>(&n->key),
                                       reinterpret_cast<const u64*>(n->unk_0x8));
    }
    for (u64 i = 0; i < target.count; ++i) {
        UnkReadEntry& e = target.entries[i];
        if (e.ops) {
            e.ops->destroy(e.storage);
            e.ops = nullptr;
        }
    }
    fn_710007c0e4(&reader.file);
}

// ---- fn_71011b250c: a pooled container's constructor ----

// A container whose 0x18-byte nodes come from an embedded block pool (base/UnkCloneHelpers.h's
// UnkBlockPool), as UnkPoolList lays it out but with the flags before the allocator and an
// optional parent for the pool at +0x30. Built by fn_71011b1f30.
struct UnkGuiPoolList {
    char* vtable;
    u32 count;
    void* head;
    void* tail;
    u32 flags;
    base::UnkAllocator* allocator;
    base::UnkAllocator* pool_parent;  // the pool's parent when set, else the allocator
    base::UnkBlockPool pool;
};
static_assert(sizeof(UnkGuiPoolList) == 0x78, "UnkGuiPoolList placeholder layout");

extern char unk_7101cb58a8[];  // the base part's vtable (reached through the GOT)
extern char unk_7101cb5888[];  // the container's vtable (reached through the GOT)

void fn_71011b250c(UnkGuiPoolList* self, u32 n, u32 /*unused*/, u32 flags, base::UnkAllocator* a) {
    self->vtable = unk_7101cb58a8 + 0x10;
    self->count = 0;
    self->flags = flags;
    self->head = nullptr;
    self->tail = nullptr;
    if (!a)
        a = fn_7100083990();
    self->allocator = a;
    self->pool_parent = nullptr;
    self->vtable = unk_7101cb5888 + 0x10;
    self->pool.Init();
    base::UnkAllocator* parent = self->pool_parent;
    self->pool.field_0x38 = 0;
    self->pool.field_0x3c = false;
    if (!parent)
        parent = self->allocator;
    u32 f = self->flags;
    self->pool.Release();
    if (n) {
        if (!parent)
            parent = fn_7100083990();
        self->pool.parent = parent;
        u64 size = (u64)n * 0x18;
        fn_710008378c(&self->pool.blocks, parent->vf_0x40(size, 8, f), size, 0x18);
    }
    // The pool's address, formed as an integer (as in UnkPoolList::Init).
    self->allocator = reinterpret_cast<base::UnkAllocator*>(reinterpret_cast<u64>(self) + 0x38);
}

// ---- fn_71011ba714: the skin file ----

// GUI::CGUIManager::SkinContainer (its std::unique_ptr's type is fn_71011b1924): skins by name, a
// string-id hash map like the sprite sheets'. fn_7100121268 destroys it.
struct UnkSkinTag;
using UnkSkinMap = base::UnkStrIdHashMap<UnkSkinTag>;
using UnkSkinNode = base::UnkStrIdHashNode<UnkSkinTag>;

const base::reflection::CType* fn_71011b1924();
void fn_7100121268(UnkSkinMap* self);

// The std::unique_ptr the skin file is read into.
struct UnkOwnedSkins {
    UnkSkinMap* ptr = nullptr;

    ~UnkOwnedSkins() {
        UnkSkinMap* p = ptr;
        ptr = nullptr;
        if (p) {
            fn_7100121268(p);
            operator delete(p);
        }
    }
};

// The root's skins (a std::unique_ptr at +0x1b0).
static inline UnkSkinMap*& Skins(UnkGuiRoot* root) {
    return *reinterpret_cast<UnkSkinMap**>(reinterpret_cast<char*>(root) + 0x1b0);
}

// Reads the skin file `path` names: the first one read becomes the root's container, later ones
// add their entries to it (fn_71011b9ce4's twin).
void fn_71011ba714(UnkGuiRoot* root, const UnkStrPair_0x10* path) {
    UnkOwnedSkins loaded;
    UnkFileReader reader;
    fn_710007be8c(&reader.file, 0x16, nullptr);
    reader.field_0x38 = 3;
    reader.field_0x3c = 2;
    bool opened;
    {
        base::global::CRntString mode("rb");
        opened = fn_710007c1ac(&reader, path, &mode);
    }
    if (opened)
        fn_710000ccd8(&reader);
    UnkReadTarget target;
    {
        const base::reflection::CType* type = fn_71011b1924();
        base::reflection::UnkReflValue value;
        value.field_0x0 = nullptr;
        value.type = nullptr;
        value.kind = 2;
        fn_71000a18d4(&value, type, &loaded.ptr, true);
        fn_71000f0568(&target, &value, 0xc);
        fn_71000a1c60(&value);
    }
    if (reader.file.position < reader.file.size && reader.field_0x38 == 0)
        Parse(&reader, &target);
    if (!Skins(root)) {
        UnkSkinMap* p = loaded.ptr;
        loaded.ptr = nullptr;
        Skins(root) = p;
    } else {
        for (UnkSkinNode* n = loaded.ptr->head; n; n = n->next)
            base::refl_StrIdHashInsert(Skins(root), reinterpret_cast<const base::global::CStrId*>(&n->key),
                                       reinterpret_cast<const u64*>(n->unk_0x8));
    }
    for (u64 i = 0; i < target.count; ++i) {
        UnkReadEntry& e = target.entries[i];
        if (e.ops) {
            e.ops->destroy(e.storage);
            e.ops = nullptr;
        }
    }
    fn_710007c0e4(&reader.file);
}
