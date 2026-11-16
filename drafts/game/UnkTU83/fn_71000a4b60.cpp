// Unit 83 (base::reflection): the registry's pass over every registered type at start-up: each type is
// added to the registry's name set and to its parent's list of derived types (types without a parent are
// gathered, sorted and walked into the registry's list). Names and layouts are placeholders.
#include <new>

#include "base/file/UnkFileCacheSet.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection.h"
#include "types.h"

using base::global::CRntVector;
using base::global::CStrId;
using base::reflection::CType;

struct UnkVersionRegistry;
struct UnkTreeNode_ovb0173;

extern void* g_unk_7101d5bdc0;  // the registry's list of types (linked through CType+0x88)

UnkFileCacheSetNode** fn_71000a5c1c(base::global::CRntVector<UnkFileCacheSetNode*>* self, u32 index);
void fn_710009ea28(CType* type);
// Sorts the root types (std::sort over [first, last); src/game_s7/fn_71000a5e00.cpp).
void fn_71000a5e00(CType** first, CType** last);
void fn_71000a5020(UnkTreeNode_ovb0173* node, base::global::CRntVector<UnkTreeNode_ovb0173*>* out);

// The registry as far as this pass uses it.
struct UnkTypeRegistry_l0a4b60 {
    char* vtable;
    UnkFileCacheSet names;
    CRntVector<UnkTreeNode_ovb0173*> roots;
};
static_assert(__builtin_offsetof(UnkTypeRegistry_l0a4b60, roots) == 0x98, "layout");

// A node of the name set: a type's name and the type.
struct UnkTypeNode_l0a4b60 {
    CStrId name;
    CType* type;
    UnkTypeNode_l0a4b60* next;

    UnkTypeNode_l0a4b60(const CStrId& name, CType* type) : name(name), type(type), next(nullptr) {}
};

// Adds `type` to the set under `name`, at the front of its bucket.
static inline void AddType(UnkFileCacheSet* set, const CStrId& name, CType* type) {
    ++set->field_0x48;
    UnkTypeNode_l0a4b60* node =
        static_cast<UnkTypeNode_l0a4b60*>(set->allocator->vf_0x40(sizeof(UnkTypeNode_l0a4b60), 8, set->flags));
    if (node)
        new (node) UnkTypeNode_l0a4b60(name, type);
    u32 index = set->buckets.count ? name.Hash() % set->buckets.capacity : (u32)-1;
    u32 first = (u32)set->field_0x28;
    set->field_0x28 = index < first ? index : first;
    if (index <= first)
        set->field_0x30 = reinterpret_cast<UnkFileCacheSetNode*>(node);
    UnkTypeNode_l0a4b60** slot = reinterpret_cast<UnkTypeNode_l0a4b60**>(fn_71000a5c1c(&set->buckets, index));
    node->next = *slot;
    *slot = node;
    ++set->count;
}

void fn_71000a4b60(UnkVersionRegistry* self_) {
    auto* self = reinterpret_cast<UnkTypeRegistry_l0a4b60*>(self_);
    CRntVector<CType*> roots(0xf);
    roots.Reserve(0x880);
    for (CType* type = static_cast<CType*>(g_unk_7101d5bdc0); type; type = static_cast<CType*>(type->field_0x88)) {
        {
            CStrId name = type->m_name;
            AddType(&self->names, name, type);
        }
        if (const CType* parent = type->m_parent) {
            void* derived = type;
            const_cast<CType*>(parent)->m_array_0x90.PushBack(derived);
        } else {
            roots.PushBackCopy(type);
        }
    }
    for (CType* type = static_cast<CType*>(g_unk_7101d5bdc0); type; type = static_cast<CType*>(type->field_0x88))
        fn_710009ea28(type);
    fn_71000a5e00(roots.data, roots.data + roots.count);
    for (CType** root = roots.data; root != roots.data + roots.count; ++root)
        fn_71000a5020(reinterpret_cast<UnkTreeNode_ovb0173*>(*root), &self->roots);
    roots.Free();
}
