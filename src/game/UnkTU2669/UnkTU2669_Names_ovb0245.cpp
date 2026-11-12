// Unit 2669 (tools/tu_map.py, no class hint): the script manager's name table. A walk over the items that
// loads each one and copies the names whose load comes back as 3 into an optional list (fn_710109dc90), the
// unlink of one item from its bucket chain and the doubly linked item list (fn_710109da60), and the
// base name of a path (fn_710109e0a0: the text between the last '/' and the first '.'). Names and layouts are
// placeholders.
#include "UnkLeftoverReg.h"
#include "base/UnkNamedRegistry.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CRntStringAssign.h"
#include "game/UnkTU2669.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CRntString;

// A string (header of 0x30 bytes) with 0x100 bytes of inline storage.
struct UnkStackStr_ovb0245 {
    const char* data;
    u32 length;
    u8 pad_0xc[0x24];
    void* storage_vtable;  // 0x30
    u8 pad_0x38[0x100];
    void* field_0x138;
    u32 field_0x140;

    CRntString* Str() { return reinterpret_cast<CRntString*>(this); }

    UnkStackStr_ovb0245() {
        fn_710007e6e4(Str(), reinterpret_cast<base::UnkAllocator*>(&storage_vtable));
        storage_vtable = g_unk_7101a194b8 + 2;
        field_0x138 = nullptr;
        field_0x140 = 0;
    }
    ~UnkStackStr_ovb0245() {
        fn_710007ed40(Str());
        fn_710007ecd4(Str());
    }
};

struct UnkUnit2669Str;
struct UnkUnit2669Tag : UnkLeftoverReg {};

void fn_710109e0a0(UnkUnit2669Tag, void* source, UnkUnit2669Str* out_) {
    CRntString* out = reinterpret_cast<CRntString*>(out_);
    fn_710007e77c(out, g_unk_710162e2ed);
    UnkStackStr_ovb0245 whole;
    fn_710007eaa0(whole.Str(), static_cast<const CRntString*>(source));
    s32 dot = fn_710007fb10(whole.Str(), '.', -1);
    if (dot >= 0) {
        u32 start = fn_710007fb10(whole.Str(), '/', -1) + 1;
        s32 length = dot - start;
        const char* text = whole.data + start;
        UnkStackStr_ovb0245 name;
        fn_710007e7f8(name.Str(), text, length);
        fn_710007eaa0(out, name.Str());
    }
}

struct UnkLuaNameList_b0160;
struct UnkScriptManager_ovb0085;

struct UnkScriptNode_ovb0245 {
    u8 pad_0x0[0x30];
    UnkScriptNode_ovb0245* next;  // 0x30 (in a list of names), else the source at +0x30
};

struct UnkScriptEntry_ovb0245 {
    u8 pad_0x0[0x30];
    void* source;                 // 0x30
    UnkScriptEntry_ovb0245* next;  // 0x38
};

struct UnkScriptManagerView_ovb0245 {
    u8 pad_0x0[0x40];
    UnkScriptEntry_ovb0245* first;  // 0x40
};

struct UnkNameListView_ovb0245 {
    void* vtable;
    u32 count;  // 0x8
    UnkScriptNode_ovb0245* head;  // 0x10
    UnkScriptNode_ovb0245* tail;  // 0x18
    base::UnkAllocator* allocator;  // 0x20
    u32 flags;  // 0x28
};

void fn_710109dc90(UnkScriptManager_ovb0085* self_, UnkLuaNameList_b0160* out_) {
    auto* self = reinterpret_cast<UnkScriptManagerView_ovb0245*>(self_);
    auto* out = reinterpret_cast<UnkNameListView_ovb0245*>(out_);
    for (UnkScriptEntry_ovb0245* e = self->first; e; e = e->next) {
        u32 r = fn_710109dd6c(self, e->source, reinterpret_cast<UnkUnit2669Str*>(e), true, true);
        if (out && r == 3) {
            auto* node = static_cast<UnkScriptNode_ovb0245*>(out->allocator->vf_0x40(0x38, 8, out->flags));
            if (node) {
                fn_710007ea60(reinterpret_cast<base::UnkNamedEntry*>(node), reinterpret_cast<base::UnkNamedEntry*>(e));
                node->next = nullptr;
                u32 count;
                if (out->count == 0) {
                    out->head = node;
                    out->tail = node;
                    count = 1;
                } else {
                    UnkScriptNode_ovb0245* old_head = out->head;
                    __builtin_memcpy(&node->next, &old_head, sizeof(old_head));
                    out->head = node;
                    count = out->count + 1;
                }
                out->count = count;
            }
        }
    }
}

// ---- fn_710109da60 / fn_710109d890 ----
struct UnkScriptData_ovb0245;

struct UnkScriptItem_ovb0245 {
    CRntString name;                  // 0x0
    UnkScriptData_ovb0245* data;      // 0x30
    union {
        UnkScriptItem_ovb0245* next;  // 0x38
    };
    union {
        UnkScriptItem_ovb0245* prev;  // 0x40
    };
    union {
        UnkScriptItem_ovb0245* chain;  // 0x48
    };
};

struct UnkScriptTable_ovb0245 {
    base::UnkAllocator* allocator;  // 0x0
    u32 flags_0x8;
    u32 count;                      // 0xc
    UnkScriptItem_ovb0245** buckets;  // 0x10
    u32 bucket_count_0x18;
    u32 bucket_count;               // 0x1c
    base::UnkAllocator* allocator_0x20;
    u32 pad_0x28[4];
    UnkScriptItem_ovb0245* tail;    // 0x38
    UnkScriptItem_ovb0245* head;    // 0x40
};

struct UnkCompared_0x30;

// Removes `key`'s item from the table; returns the item after it in the list, or null.
UnkScriptItem_ovb0245* fn_710109da60(UnkScriptTable_ovb0245* self, UnkScriptItem_ovb0245* key) {
    u32 hash = *reinterpret_cast<u32*>(reinterpret_cast<u8*>(key) + 0x20) ^ *reinterpret_cast<u32*>(reinterpret_cast<u8*>(key) + 0x24);
    u32 index = hash % self->bucket_count;
    UnkScriptItem_ovb0245** slot = &self->buckets[index];
    UnkScriptItem_ovb0245* node = *slot;
    if (!node)
        return nullptr;
    UnkScriptItem_ovb0245* prev;
    if (fn_710007f120(reinterpret_cast<const UnkCompared_0x30*>(&node->name), reinterpret_cast<const UnkCompared_0x30*>(&key->name))) {
        prev = nullptr;
    } else {
        do {
            prev = node;
            node = node->chain;
            if (!node)
                return nullptr;
        } while (!fn_710007f120(reinterpret_cast<const UnkCompared_0x30*>(&node->name), reinterpret_cast<const UnkCompared_0x30*>(&key->name)));
    }
    (prev ? prev->chain : *slot) = node->chain;
    if (self->head == node)
        self->head = node->next;
    if (self->tail == node)
        self->tail = node->prev;
    if (node->prev)
        node->prev->next = node->next;
    UnkScriptItem_ovb0245* result;
    if (node->next) {
        node->next->prev = node->prev;
        result = node->next;
    } else {
        result = nullptr;
    }
    fn_710007ecd4(&node->name);
    self->allocator->vf_0x48(node);
    self->count = self->count - 1;
    return result;
}
