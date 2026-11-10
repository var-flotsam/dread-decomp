// Unit 2744 (gui/scripts/guimanager.lua): the GUI root's helpers. fn_71011b5d94 drops the focus (and the
// follow-up reference) when it is on the object and unlinks the object's weak handle from the root's
// list (fn_71011b77e8); fn_71011bbf08 names an object, attaches it to its parent and queues it on the
// root; fn_71011b94ac creates a counted record in a string-id map and fn_71011b97bc counts one more use
// of the record a name has, or creates it; fn_71011b6030 loads a composition from a script and hands
// out its weak handle; fn_71011b63c4 builds a display object by copying a reflected definition.
// Names and layouts are placeholders.
#include <new>

#include "../../game_aca57c/UnkBufferString.h"
#include "../../game_s7/UnkPropBagS7.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CRntStringAssign.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflObjectRead.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiEvent.h"
#include "game/UnkGuiHandle.h"
#include "game/UnkGuiRoot.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CStrId;
using base::reflection::UnkReflObject;
using base::reflection::UnkReflValue;
using unk_prop_bag::UnkPropBag;

// ---- fn_71011b5d94 ----

// A counted reference to a weak handle, as the list unlink takes it.
struct UnkFocusLink_b0210 {
    UnkWeakHandle* handle;
    UnkFocusLink_b0210(UnkWeakHandle* h) : handle(nullptr) {
        if (h) {
            ++h->ref_count;
            handle = h;
        }
    }
    ~UnkFocusLink_b0210() {
        if (handle) {
            if (--handle->ref_count == 0)
                handle->vf_0x10();
        }
    }
};

// The focused object, which takes the lost-focus event in vtable slot 0xd8.
struct UnkFocusTarget_b0210 {
    virtual void vf_0x0(); virtual void vf_0x8(); virtual void vf_0x10(); virtual void vf_0x18(); virtual void vf_0x20();
    virtual void vf_0x28(); virtual void vf_0x30(); virtual void vf_0x38(); virtual void vf_0x40(); virtual void vf_0x48();
    virtual void vf_0x50(); virtual void vf_0x58(); virtual void vf_0x60(); virtual void vf_0x68(); virtual void vf_0x70();
    virtual void vf_0x78(); virtual void vf_0x80(); virtual void vf_0x88(); virtual void vf_0x90(); virtual void vf_0x98();
    virtual void vf_0xa0(); virtual void vf_0xa8(); virtual void vf_0xb0(); virtual void vf_0xb8(); virtual void vf_0xc0();
    virtual void vf_0xc8(); virtual void vf_0xd0(); virtual void vf_0xd8(UnkGuiEvent* event);
};

void fn_71011b77e8(void* list, UnkFocusLink_b0210* link);

void fn_71011b5d94(UnkGuiRoot* root, UnkGuiDisplayObject* object) {
    if (UnkWeakHandle* h = root->field_0x230) {
        if (h->target && h->target == reinterpret_cast<UnkWeakTarget*>(object)) {
            UnkGuiEvent event;
            s32 kind = 3;
            fn_710120235c(&event, object, &kind, nullptr);
            reinterpret_cast<UnkFocusTarget_b0210*>(root->field_0x230->target)->vf_0xd8(&event);
            ResetRef(&root->field_0x230);
            fn_7101202488(&event);
        }
    }
    if (UnkWeakHandle* h = root->field_0x238) {
        if (h->target && h->target == reinterpret_cast<UnkWeakTarget*>(object))
            ResetRef(&root->field_0x238);
    }
    void* list = reinterpret_cast<char*>(root) + 0x1b8;
    UnkWeakHandle* handle = GetWeakHandle(reinterpret_cast<UnkWeakTarget*>(object));
    UnkFocusLink_b0210 link(handle);
    fn_71011b77e8(list, &link);
}

// ---- fn_71011bbf08 ----

bool fn_71011ceac0(UnkGuiDisplayObject* self);
bool fn_71011ceb34(UnkGuiDisplayObject* self);
void fn_71011cd424(UnkGuiDisplayObject* self, UnkGuiDisplayObject* parent);

struct UnkGuiRootQueue_b0210 {
    u8 pad_0x0[0xa0];
    base::UnkVector<UnkGuiDisplayObject*> queue;  // 0xa0
};

struct UnkQueuedObject_b0210 {
    u8 pad_0x0[0x130];
    CStrId id;  // 0x130
    u8 pad_0x138[0x4c];
    bool field_0x184;
    u8 pad_0x185[0x2];
    bool field_0x187;
    bool field_0x188;
    UnkGuiDisplayObject* Object() { return reinterpret_cast<UnkGuiDisplayObject*>(this); }
};

void fn_71011bbf08(UnkGuiRootQueue_b0210* root, UnkQueuedObject_b0210* item, const CStrId* id,
                   UnkGuiDisplayObject* parent) {
    if (!item)
        return;
    item->id = *id;
    if (parent)
        fn_71011cd424(item->Object(), parent);
    if (!item->field_0x187 &&
        (item->field_0x188 || item->field_0x184 ||
         (fn_71011cde14(item->Object())->field_0x3c &&
          (fn_71011ceac0(item->Object()) || fn_71011ceb34(item->Object()))))) {
        UnkGuiDisplayObject* object = item->Object();
        root->queue.PushBack(object);
        item->field_0x187 = true;
    }
}

// ---- fn_71011b94ac and fn_71011b97bc: the map of counted records ----

// The string data's hash, as the first function reads it whole and the second as two words.
struct UnkStringHash64_b0210 {
    u8 pad_0x0[0x30];
    u64 hash;
};
struct UnkStringHashView_b0210 {
    u8 pad_0x0[0x30];
    u32 hash_lo;
    u32 hash_hi;
};

// A counted record: a reference count (a union member, so the callers' string ids are reloaded after it
// is bumped), an empty path and a list of 8-byte entries.
struct UnkRecord_b0210 {
    union {
        s32 ref_count;
    };
    CFilePathStrId path;  // 0x8
    u64 field_0x10;
    base::UnkVector<void*> items;  // 0x18
};

struct UnkRecordNode_b0210 {
    CStrId key;
    UnkRecord_b0210* value;
    UnkRecordNode_b0210* next;   // 0x10: the list of all nodes
    UnkRecordNode_b0210* prev;   // 0x18
    UnkRecordNode_b0210* chain;  // 0x20: the next node in the bucket
};

struct UnkRecordMap_b0210 {
    base::UnkAllocator* allocator;
    u32 flags;
    u32 count;
    UnkRecordNode_b0210** buckets;  // 0x10
    u32 bucket_count;               // 0x18
    u32 bucket_capacity;            // 0x1c
    u8 pad_0x20[0x18];
    UnkRecordNode_b0210* tail;  // 0x38
    UnkRecordNode_b0210* head;  // 0x40

    UnkRecordNode_b0210** Bucket(const CStrId& id) {
        const auto* d = reinterpret_cast<const UnkStringHashView_b0210*>(id.data);
        u32 h = d->hash_lo ^ d->hash_hi;
        u32 index = h % bucket_capacity;
        return &buckets[index];
    }
    UnkRecordNode_b0210* Find(const CStrId& id) {
        if (bucket_count) {
            UnkRecordNode_b0210** slot = Bucket(id);
            if (slot) {
                for (UnkRecordNode_b0210* n = *slot; n; n = n->chain) {
                    if (n->key.data == id.data)
                        return n;
                }
            }
        }
        return nullptr;
    }
};

UnkRecord_b0210* fn_71011b94ac(UnkRecordMap_b0210* self, const CStrId* key) {
    auto* record = static_cast<UnkRecord_b0210*>(fn_71000839a0(0x40, 0xc));
    record->ref_count = 0;
    new (&record->path) CFilePathStrId(g_unk_710162e2ed, true);
    record->field_0x10 = 0;
    new (&record->items) base::UnkVector<void*>(0xc);
    record->items.Reserve(0x28);
    ++record->ref_count;

    UnkRecordNode_b0210** slot = nullptr;
    if (self->bucket_count) {
        u64 h = reinterpret_cast<const UnkStringHash64_b0210*>(key->data)->hash;
        u32 index = ((u32)(h >> 32) ^ (u32)h) % self->bucket_capacity;
        slot = &self->buckets[index];
    }
    UnkRecordNode_b0210* chain = *slot;
    auto* n = static_cast<UnkRecordNode_b0210*>(self->allocator->vf_0x40(0x28, 8, self->flags));
    if (n) {
        new (&n->key) CStrId(*key);
        n->value = record;
        n->next = nullptr;
        n->prev = nullptr;
        n->chain = nullptr;
    }
    n->chain = chain;
    *slot = n;
    if (!self->head) {
        self->head = n;
    } else if (self->tail) {
        // A raw store: the tail is reloaded after it, as in the original.
        *reinterpret_cast<UnkRecordNode_b0210**>(reinterpret_cast<char*>(n) + __builtin_offsetof(UnkRecordNode_b0210, prev)) = self->tail;
        self->tail->next = n;
    }
    self->tail = n;
    ++self->count;
    return record;
}

struct UnkRecordOwner_b0210 {
    u8 pad_0x0[8];
    UnkRecordMap_b0210* map;  // 0x8
};

// Room for a string id that the out-of-line constructor (fn_7100000850) builds; destroyed like any id.
union UnkIdSlot_b0210 {
    CStrId id;
    UnkIdSlot_b0210() {}
    ~UnkIdSlot_b0210() { id.~CStrId(); }
};

void fn_71011b9968(UnkRecordOwner_b0210* self, const CRntString* name);

void fn_71011b97bc(UnkRecordOwner_b0210* self, const CRntString* name) {
    if (UnkRecordMap_b0210* map = self->map) {
        UnkRecordNode_b0210* found;
        {
            CStrId id(name->data ? name->data : g_unk_710162e2ed, true);
            found = map->Find(id);
        }
        if (found) {
            UnkRecordMap_b0210* m = self->map;
            UnkIdSlot_b0210 slot;
            fn_7100000850(&slot.id, *name, true);
            UnkRecordNode_b0210* n = *m->Bucket(slot.id);
            while (n->key.data != slot.id.data)
                n = n->chain;
            ++n->value->ref_count;
            return;
        }
    }
    fn_71011b9968(self, name);
}

// ---- fn_71011b6030: loads a composition ----

s32 fn_710007fb10(const base::global::CRntString* self, char c, s32 start);

struct UnkScriptManager_ovb0085;
extern UnkScriptManager_ovb0085* g_unk_71727bed90;
void fn_710109d810(UnkScriptManager_ovb0085* self);
struct UnkUnit2669Str;
u32 fn_710109dd6c(void* self, void* source, UnkUnit2669Str* name, bool a, bool b);
UnkWeakTarget* fn_71011b5f5c(u32 script, const char* const* name, const CRntString* source, void* bag);

static inline UnkScriptManager_ovb0085* ScriptManager() {
    UnkScriptManager_ovb0085* manager = g_unk_71727bed90;
    if (!manager) {
        manager = static_cast<UnkScriptManager_ovb0085*>(fn_71000839a0(0x88, 0x3f));
        fn_710109d810(manager);
        g_unk_71727bed90 = manager;
    }
    return manager;
}

UnkGuiHandle fn_71011b6030(UnkGuiRoot* root, const CRntString& name, const CRntString& path, UnkPropBag* bag) {
    s32 slash = fn_710007fb10(&path, '/', -1);
    u32 start = slash + 1;
    s32 dot = fn_710007fb10(&path, '.', -1);
    s32 end = dot == -1 ? (s32)path.length : dot;
    u32 length = end - start;
    const char* text = path.data + start;
    UnkBufferString<0x100, g_unk_7101a194b8> part;
    fn_710007e7f8(part.Str(), text, length);
    UnkScriptManager_ovb0085* manager = ScriptManager();
    UnkWeakHandle* handle = nullptr;
    if (u32 script = fn_710109dd6c(manager, const_cast<CRntString*>(&path),
                                   reinterpret_cast<UnkUnit2669Str*>(part.Str()), false, false)) {
        if (UnkWeakTarget* object = fn_71011b5f5c(script, reinterpret_cast<const char* const*>(&name), part.Str(), bag))
            handle = GetWeakHandle(object);
    }
    UnkGuiHandle result;
    result.ptr = nullptr;
    if (handle) {
        ++handle->ref_count;
        result.ptr = reinterpret_cast<UnkGuiHandleObj*>(handle);
    }
    return result;
}

// ---- fn_71011b63c4: builds a display object from a reflected definition ----

struct UnkButtonTemplate_ovb0108;

struct UnkWriteStack_b0026;
struct UnkWriteRoot_b0026;
void fn_71000ee314(UnkWriteStack_b0026* self, UnkWriteRoot_b0026* root, u32 count);

// The objects to copy: up to 100 entries of three values each (UnkWriteStack_b0026's layout).
struct UnkCopyEntry_b0210 {
    UnkReflValue a;
    u8 pad_0x18[0x18];
    UnkReflValue b;
    UnkReflValue c;
    u8 pad_0x60[0x8];
};
struct UnkCopyStack_b0210 {
    u8 pad_0x0[0x14];
    u32 count;
    UnkCopyEntry_b0210 entries[100];
    u8 pad_0x28b8[0x8];
};
static_assert(sizeof(UnkCopyStack_b0210) == 0x28c0, "UnkCopyStack_b0210 layout");

struct UnkCopyRoot_b0210 {
    char* type;
    void* object;
};

struct UnkCopyCallbacks_b0210 {
    u64 words[6];
};
extern UnkCopyCallbacks_b0210 g_unk_7172a0d188 __attribute__((visibility("hidden")));

// What fn_7100b09720 is given: the callbacks, the objects to copy and the target.
struct UnkCopyContext_b0210 {
    UnkCopyCallbacks_b0210 callbacks;
    UnkCopyStack_b0210* stack;
    UnkReadTarget* target;
};

struct UnkDisplayReader_ovb0108;
s32 fn_7100b09720(UnkDisplayReader_ovb0108* self, UnkReflObject* obj);

// The type of std::unique_ptr<GUI::CDisplayObjectContainer> (src/reflection_types/).
const base::reflection::CType* fn_71011bcf1c();

struct UnkOwnedObject_b0210 {
    UnkGuiDisplayObject* ptr = nullptr;

    UnkGuiDisplayObject* Release() {
        UnkGuiDisplayObject* p = ptr;
        if (p)
            ptr = nullptr;
        return p;
    }
    ~UnkOwnedObject_b0210() {
        UnkGuiDisplayObject* p = ptr;
        ptr = nullptr;
        if (p)
            p->vf_0x10();
    }
};

UnkGuiDisplayObject* fn_71011b63c4(UnkGuiRoot* root, UnkButtonTemplate_ovb0108* def) {
    UnkCopyContext_b0210 ctx;
    UnkOwnedObject_b0210 object;
    UnkCopyStack_b0210 stack;
    auto* source = reinterpret_cast<UnkReflObject*>(def);
    {
        UnkCopyRoot_b0210 copy_root = {
            fn_710009dcd8(const_cast<char*>(reinterpret_cast<const char*>(source->vf_0x0())), source), source};
        fn_71000ee314(reinterpret_cast<UnkWriteStack_b0026*>(&stack), reinterpret_cast<UnkWriteRoot_b0026*>(&copy_root),
                      2);
    }
    UnkReadTarget target;
    {
        const base::reflection::CType* type = fn_71011bcf1c();
        UnkReflValue value;
        value.field_0x0 = nullptr;
        value.type = nullptr;
        value.kind = 2;
        fn_71000a18d4(&value, type, &object.ptr, true);
        fn_71000f0568(&target, &value, 0xc);
        fn_71000a1c60(&value);
    }
    if (stack.count) {
        ctx.stack = &stack;
        ctx.target = &target;
        const UnkCopyCallbacks_b0210* callbacks = &g_unk_7172a0d188;
        ctx.callbacks.words[0] = callbacks->words[0];
        ctx.callbacks.words[1] = callbacks->words[1];
        ctx.callbacks.words[2] = callbacks->words[2];
        ctx.callbacks.words[3] = callbacks->words[3];
        ctx.callbacks.words[4] = callbacks->words[4];
        ctx.callbacks.words[5] = callbacks->words[5];
        UnkReadEntry* end = &target.entries[target.count];
        UnkReflObject* obj = target.count ? static_cast<UnkReflObject*>(end[-1].field_0x0->field_0x10) : nullptr;
        fn_7100b09720(reinterpret_cast<UnkDisplayReader_ovb0108*>(&ctx), obj);
    }
    UnkGuiDisplayObject* result = object.Release();
    for (u64 i = 0; i < target.count; ++i) {
        UnkReadEntry& e = target.entries[i];
        if (e.ops) {
            e.ops->destroy(e.storage);
            e.ops = nullptr;
        }
    }
    for (u64 i = 0; i < stack.count; ++i) {
        fn_71000a1c60(&stack.entries[i].c);
        fn_71000a1c60(&stack.entries[i].b);
        fn_71000a1c60(&stack.entries[i].a);
    }
    return result;
}
