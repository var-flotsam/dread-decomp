// Finds a display object by a dotted path: splits the path on '.' and walks child containers by name.
#include "base/UnkCloneHelpers.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"

using base::global::CStrId;
using base::global::UnkStringData;
using base::reflection::CClass;

// A string id built out of line (fn_7100000850) and released like CStrId.
struct UnkIdHolder_71011d9508 {
    UnkStringData* data;
    ~UnkIdHolder_71011d9508() {
        if (base::global::UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                if (--data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

// A token of the split path: a string with inline storage, then the link to the next token.
struct UnkToken_71011d9508 {
    base::global::CRntString str;
    u8 unk_0x30[0x58];
    UnkToken_71011d9508* next;
};

struct UnkTokenList_0x70 {
    void* vtable;
    u32 field_0x8;
    UnkToken_71011d9508* head;
    void* field_0x18;
    void* allocator;
    u32 field_0x28;
    void* vtable2;
    void* field_0x38;
    UnkBlockFreeList field_0x40;
    u32 field_0x68;
    u8 field_0x6c;
};

struct UnkDisplayObject_71011d9508;

struct UnkChildLink_71011d9508 {
    UnkDisplayObject_71011d9508* child;
    UnkChildLink_71011d9508* next;
};

struct UnkDisplayObject_71011d9508 {
    virtual const CClass* vf_0x0();
    u8 unk_0x8[0x128];
    UnkStringData* name;
    u8 unk_0x138[0x98];
    UnkChildLink_71011d9508* children;
};

void fn_71009319dc(const char* str, const char* sep, UnkTokenList_0x70* out);
void fn_7100931c34(UnkTokenList_0x70* list);
extern char g_unk_7101a838c8[];
extern char g_unk_7172a0f120[] __attribute__((visibility("hidden")));

UnkDisplayObject_71011d9508* fn_71011d9508(UnkDisplayObject_71011d9508* self, const char* const* path) {
    UnkTokenList_0x70 list;
    list.field_0x8 = 0;
    list.head = nullptr;
    list.field_0x18 = nullptr;
    list.allocator = fn_7100083990();
    list.field_0x28 = 0x13;
    list.vtable = g_unk_7101a838c8 + 0x10;
    list.vtable2 = g_unk_7101a1a4c8 + 0x10;
    list.field_0x38 = nullptr;
    fn_710008377c(&list.field_0x40);
    list.field_0x68 = 0;
    list.field_0x6c = 0;
    fn_71009319dc(*path, ".", &list);
    UnkDisplayObject_71011d9508* cur = self;
    for (UnkToken_71011d9508* tok = list.head; tok; tok = tok->next) {
        if (!cur)
            break;
        const CClass* type = cur->vf_0x0();
        fn_71011d7b84();
        if (!fn_710009d50c(type, reinterpret_cast<const base::reflection::CType*>(g_unk_7172a0f120))) {
            cur = nullptr;
            break;
        }
        UnkIdHolder_71011d9508 id;
        fn_7100000850(reinterpret_cast<base::global::UnkStrIdBase*>(&id), tok->str, true);
        UnkDisplayObject_71011d9508* found = nullptr;
        if (UnkChildLink_71011d9508* l = cur->children) {
            do {
                found = l->child->name == id.data ? l->child : nullptr;
                l = l->next;
            } while (l && !found);
        }
        cur = found;
    }
    fn_7100931c34(&list);
    return cur;
}

// ---- Lookups among a container's children (the list at +0x1d0) ----

#include "game/UnkGuiDisplayObject.h"

static inline UnkDisplayObject_71011d9508* AsContainer(UnkGuiDisplayObject* object) {
    return reinterpret_cast<UnkDisplayObject_71011d9508*>(object);
}

// The direct child named `name`, or null.
// NON_MATCHING(minor): the loop's exit test compares the link before the result (the original tests
// the result first in its conditional compare); the behaviour is the same.
UnkGuiDisplayObject* fn_71011d94d4(UnkGuiDisplayObject* container, const CStrId& name) {
    UnkChildLink_71011d9508* l = AsContainer(container)->children;
    if (!l)
        return nullptr;
    UnkDisplayObject_71011d9508* found;
    do {
        found = l->child;
        l = l->next;
        if (found->name != name.data)
            found = nullptr;
    } while (!found && l);
    return reinterpret_cast<UnkGuiDisplayObject*>(found);
}

// The child whose name's hash is `id`: first among the direct children, then, depth first, inside
// the children that are containers themselves.
// NON_MATCHING(minor): the original walks the first pass through the address of each link and
// reloads the list's head for the second; Clang keeps the head in a register and walks the links
// themselves, which lays the passes out differently. The behaviour is the same.
UnkGuiDisplayObject* fn_71011d98ec(UnkGuiDisplayObject* container, u64 id) {
    UnkDisplayObject_71011d9508* self = AsContainer(container);
    for (UnkChildLink_71011d9508* l = self->children; l; l = l->next) {
        UnkDisplayObject_71011d9508* child = l->child;
        if (child && *reinterpret_cast<const u64*>(reinterpret_cast<const char*>(child->name) + 0x30) == id)
            return reinterpret_cast<UnkGuiDisplayObject*>(child);
    }
    for (UnkChildLink_71011d9508* l = self->children; l; l = l->next) {
        UnkDisplayObject_71011d9508* child = l->child;
        if (!child)
            continue;
        const CClass* type = child->vf_0x0();
        fn_71011d7b84();
        if (fn_710009d50c(type, reinterpret_cast<const base::reflection::CType*>(g_unk_7172a0f120))) {
            if (UnkGuiDisplayObject* found = fn_71011d98ec(reinterpret_cast<UnkGuiDisplayObject*>(child), id))
                return found;
        }
    }
    return nullptr;
}
