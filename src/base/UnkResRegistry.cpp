// base::res's type registry: the object g_unk_7101d472f8 points at (0x1b8 bytes, created by
// fn_71000ea018 before the render resource types are registered), its constructor, the record a
// resource type is described by (UnkResDesc: built by fn_71000e9a7c, copied by fn_71000ec89c;
// src/game/msapi/res/CRenderRes/fn_71001b1dd0.cpp fills them in) and the growth of the registry's two parallel
// arrays of records and type names (fn_71000ec708). Names and layouts are placeholders inferred
// from the code.
#include <new>

#include "base/UnkBlockFreeList.h"
#include "base/UnkCloneHelpers.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::UnkAllocator;
using base::UnkBlockPool;
using base::UnkVector;
using base::global::CFilePathStrId;
using base::global::CStrId;
using base::global::UnkStringPool;

// A resource type (its first member is its name).
struct UnkResType {
    CStrId name;
};
struct UnkResFactory;
struct UnkResRegistry;
extern UnkResRegistry* g_unk_7101d472f8;

// The record's default callbacks: each forwards its second argument (fn_710001a0ac, fn_710001a39c).
void fn_71000204c4(void* context, void* arg);
void fn_71000204cc(void* context, void* arg);

// The record's list of file-path ids, laid out as an UnkVector of CFilePathStrId; its copy reserves
// the source's capacity (fn_7100018450) and appends the ids one by one.
struct UnkResPathVec {
    CFilePathStrId* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    UnkAllocator* allocator;

    explicit UnkResPathVec(u32 flags)
        : data(nullptr), count(0), capacity(0), step(10), field_0x14(true), flags(flags),
          allocator(fn_7100083990()) {}
    UnkResPathVec(const UnkResPathVec& other);
};
void fn_7100018450(UnkResPathVec* vec, u32 capacity);  // reserve

__attribute__((always_inline)) inline UnkResPathVec::UnkResPathVec(const UnkResPathVec& other)
    : data(nullptr), count(0), capacity(0), step(other.step), field_0x14(other.field_0x14),
      flags(other.flags), allocator(other.allocator) {
    fn_7100018450(this, other.capacity);
    for (const CFilePathStrId* p = other.data, *end = other.data + other.count; p != end; ++p) {
        u32 n = count;
        u32 new_capacity = capacity;
        if (new_capacity <= n && step != 0)
            new_capacity = (n / step + 1) * step;
        if ((s32)n >= (s32)new_capacity)
            for (;;) {
            }
        fn_7100018450(this, new_capacity);
        // Copied bitwise, then the reference taken: count is reloaded after the store.
        CFilePathStrId* slot = &data[count];
        base::global::UnkStringData* value = p->data;
        __builtin_memcpy(slot, &value, sizeof(value));
        if (&g_unk_7101d47200->empty != value)
            ++value->ref_count;
        ++count;
    }
}

// A callback: a context and a function, copied member by member.
struct UnkResCallback {
    void* context;
    void (*function)();

    UnkResCallback(void* context, void (*function)()) : context(context), function(function) {}
    UnkResCallback(const UnkResCallback& other) : context(other.context), function(other.function) {}
};

// A file-path id member of the record: CFilePathStrId's copies, and an empty id whose
// pointer is cleared before the pool is made sure of.
struct UnkResRegistryPathId : base::global::UnkFilePathStrIdBase {
    UnkResRegistryPathId() : UnkFilePathStrIdBase{nullptr} {
        if (!g_unk_7101d47200)
            base::global::UnkEnsureFilePathPool();
        data = &g_unk_7101d47200->empty;
    }
    explicit UnkResRegistryPathId(const UnkFilePathStrIdBase& other) : UnkFilePathStrIdBase(other) {
        if (&g_unk_7101d47200->empty != data)
            ++data->ref_count;
    }
    UnkResRegistryPathId(const UnkResRegistryPathId& other) : UnkResRegistryPathId(static_cast<const UnkFilePathStrIdBase&>(other)) {}
};

// The record's type and registry, set together (and copied as one block).
struct UnkResDescHead {
    const UnkResType* resource_type;
    void* registry;  // the registry's second base (+8), or null

    UnkResDescHead(const UnkResType* type, void* registry) : resource_type(type), registry(registry) {}
};

// The record a resource type is registered with (0x90 bytes).
struct UnkResDesc : UnkResDescHead {
    CStrId name;                   // 0x10
    UnkResRegistryPathId field_0x18;
    UnkResRegistryPathId field_0x20;
    UnkResCallback field_0x28;
    UnkResCallback field_0x38;
    UnkResFactory* factory;        // 0x48
    u32 type;                      // 0x50
    u64 field_0x58;
    UnkResPathVec paths;           // 0x60
    UnkResRegistryPathId field_0x88;

    UnkResDesc(const UnkResType* type, UnkResFactory* factory) asm("fn_71000e9a7c");
    ~UnkResDesc() asm("fn_71000e8dd0");
};
static_assert(sizeof(UnkResDesc) == 0x90, "");

// Clang 9, the matching compiler, ignores the asm label on the record's constructor and emits its
// mangled symbol, so this alias gives the definition the address's name; a newer Clang (the
// port's) names the definition by its label.
#if defined(__clang__) && __clang_major__ <= 9
void fn_71000e9a7c(UnkResDesc*, const UnkResType*, UnkResFactory*)
    __attribute__((alias("_ZN10UnkResDescC1EPK10UnkResTypeP13UnkResFactory")));
#endif

UnkResDesc::UnkResDesc(const UnkResType* type, UnkResFactory* factory)
    : UnkResDescHead(type, g_unk_7101d472f8 ? reinterpret_cast<char*>(g_unk_7101d472f8) + 8 : nullptr),
      name(g_unk_7101d470e0),
      field_0x18(g_unk_7101d470e8),
      field_0x20(),
      field_0x28(nullptr, reinterpret_cast<void (*)()>(fn_71000204c4)),
      field_0x38(nullptr, reinterpret_cast<void (*)()>(fn_71000204cc)),
      factory(factory),
      type(9),
      field_0x58(0),
      paths(0xf),
      field_0x88() {}

// The record's copy constructor, out of line.
__attribute__((noinline, flatten)) void fn_71000ec89c(UnkResDesc* self, const UnkResDesc* other) {
    new (self) UnkResDesc(*other);
}

// The registry's records and their type names: two parallel arrays sharing a count, a capacity
// and a growth step (embedded at +0xf0).
struct UnkResDescArrays {
    UnkResDesc* descs;
    CStrId* names;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    UnkAllocator* allocator;
};

// Grows both arrays to `new_capacity`: the records are copied and destroyed, the names moved.
void fn_71000ec708(UnkResDescArrays* self, u32 new_capacity) {
    if (self->capacity < new_capacity) {
        UnkResDesc* descs =
            static_cast<UnkResDesc*>(self->allocator->vf_0x40((u64)new_capacity * sizeof(UnkResDesc), 8, self->flags));
        CStrId* names = static_cast<CStrId*>(self->allocator->vf_0x40((u64)new_capacity * sizeof(CStrId), 8, self->flags));
        if (UnkResDesc* old = self->descs) {
            UnkResDesc* dst = descs;
            for (u32 i = 0; i < self->count; ++i, ++dst, ++old) {
                fn_71000ec89c(dst, old);
                old->~UnkResDesc();
            }
            self->allocator->vf_0x48(self->descs);
        }
        if (CStrId* old = self->names) {
            CStrId* dst = names;
            for (u32 i = 0; i < self->count; ++i, ++old, ++dst) {
                // The moved-from id is cleared through a union, so its pool is reloaded.
                union Slot {
                    base::global::UnkStringData* p;
                };
                dst->data = old->data;
                reinterpret_cast<Slot*>(&old->data)->p = &g_unk_7101d47108->empty;
                old->Reset();
            }
            self->allocator->vf_0x48(self->names);
        }
        self->descs = descs;
        self->names = names;
        self->capacity = new_capacity;
    }
}

// The registry's hash set of entries, laid out as the string pool's (base::global::UnkPoolSetWithNodes
// in base/global/UnkStringPoolBody.h), with 0x14-byte nodes; fn_71000ec184 sets it up.
struct UnkResSet {
    UnkVector<char*> buckets;
    u32 first_bucket;
    char* first_node;
    UnkAllocator* node_allocator;
    u32 node_flags;
    u32 node_count;
    u32 version;
    UnkBlockPool nodes;
};
static_assert(sizeof(UnkResSet) == 0x90, "");
void fn_71000ec184(UnkResSet* set, u32 buckets, u32 nodes);

struct UnkResRegistry {
    char* vtable;
    char* vtable_0x8;                // a second base's: records keep a pointer to it
    u8 field_0x10[0x50];             // built by fn_71000e9d60
    UnkResSet set;                   // 0x60
    UnkResDescArrays descs;          // 0xf0
    u8 field_0x118[0x30];            // built by fn_7100016b80
    u8 field_0x148[0x70];            // a pooled list (fn_71000e9f00)
};
static_assert(sizeof(UnkResRegistry) == 0x1b8, "");

extern char g_unk_7101a1b888[];  // the registry's vtable
void fn_71000e9d60(void* self, u32 a, u32 flags);
void fn_7100016b80(void* self);
void fn_71000e9f00(void* list, u32 n, u32, u32 flags, UnkAllocator* allocator);

// The registry's constructor.
void fn_71000e9c78(UnkResRegistry* self) {
    self->vtable = g_unk_7101a1b888 + 0x10;
    self->vtable_0x8 = g_unk_7101a1b888 + 0x60;
    fn_71000e9d60(self->field_0x10, 300, 0xe);
    UnkResSet* set = &self->set;
    new (&set->buckets) UnkVector<char*>(0xe);
    set->first_node = nullptr;
    set->first_bucket = -1;
    set->node_allocator = fn_7100083990();
    set->node_flags = 0xe;
    set->node_count = 0;
    set->version = 1;
    set->nodes.vtable = g_unk_7101a1a4c8 + 0x10;
    set->nodes.parent = nullptr;
    fn_710008377c(&set->nodes.blocks);
    set->nodes.field_0x38 = 0;
    set->nodes.field_0x3c = false;
    fn_71000ec184(set, 5, 0x14);
    self->descs.descs = nullptr;
    self->descs.names = nullptr;
    self->descs.count = 0;
    self->descs.capacity = 0;
    self->descs.step = 10;
    self->descs.flags = 0xe;
    self->descs.allocator = fn_7100083990();
    fn_7100016b80(self->field_0x118);
    fn_71000e9f00(self->field_0x148, 10, 0, 0x13, nullptr);
}
