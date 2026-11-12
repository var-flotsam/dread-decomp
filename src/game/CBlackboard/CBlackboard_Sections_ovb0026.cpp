// CBlackboard's unit (tools/tu_map.py, the name from reflection): moving and destroying sections,
// removing one from the parallel arrays, the named groups of sections (a hash map at +0x30 whose
// nodes are listed from +0x70, a fallback group at +0x78) and adding sections to them, flagging
// every section, a property's type letter, the lower-case file names, clearing the blackboard and
// writing it (or one group of it) to a file.
// Names and layouts are placeholders.
#include <new>

#include "base/UnkCloneHelpers.h"
#include "base/UnkNamedRegistry.h"
#include "base/UnkParallelFind.h"
#include "base/UnkVector.h"
#include "base/animation/CBlendSpaceResource.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CBlackboard.h"
#include "game/UnkTU83.h"
#include "game/UnkTU92.h"
#include "types.h"

struct UnkClone_fn_710104afb4;
extern template void base::UnkCloneKeyedArray<UnkClone_fn_710104afb4>::Reserve(u32);

template <class T>
struct UnkKeyedArray_b62a1;
template <base::global::UnkStringPool*& Pool>
struct UnkMovedId_b62a1;
using UnkMovedStrId_b62a1 = UnkMovedId_b62a1<g_unk_7101d47108>;
void fn_71000d97fc(UnkKeyedArray_b62a1<UnkMovedStrId_b62a1>* self, u32 capacity);

using base::global::CRntString;
using base::global::CStrId;
using base::reflection::UnkReflValue;

void fn_7100af0030(UnkSectionData* self, const UnkSectionData* other);
s32 fn_7100024140(const base::reflection::CType* type);  // the type's letter (b, f, i, s, v)

struct UnkStrIdData_b0026 {
    u8 pad_0x0[0x30];
    u32 hash_lo;
    u32 hash_hi;
};

struct UnkNameSet_b0026 {
    CStrId* values;
    CStrId* ids;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
};

// The sections as parallel arrays (the blackboard's at +0x8, a group's at +0x38).
struct UnkSectionArray_b0026 {
    CBlackboard::CSection** values;
    CStrId* ids;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;

    // Releases the ids and empties the arrays (the storage is kept).
    void Clear() {
        if (ids) {
            for (u64 i = 0; i < count; ++i)
                ids[i].Reset();
        }
        count = 0;
    }
    // The same, testing the count first.
    void ClearIfAny() {
        if (count && ids) {
            for (u64 i = 0; i < count; ++i)
                ids[i].Reset();
        }
        count = 0;
    }
};

// A named group of sections (created by fn_71010486bc): the names it takes sections under and
// the sections it holds; a section's field_0x10 points at its group.
struct UnkSectionGroup_b0026 {
    CBlackboard* owner;
    CStrId name;
    UnkNameSet_b0026 names;          // 0x10
    UnkSectionArray_b0026 sections;  // 0x38
    bool field_0x60;

    UnkSectionGroup_b0026(CBlackboard* owner_, const CStrId& name_) : owner(owner_), name(name_) {
        names.values = nullptr;
        names.ids = nullptr;
        names.count = 0;
        names.capacity = 0;
        names.step = 10;
        names.flags = 0xe;
        names.allocator = fn_7100083990();
        sections.values = nullptr;
        sections.ids = nullptr;
        sections.count = 0;
        sections.capacity = 0;
        sections.step = 10;
        sections.flags = 0x15;
        sections.allocator = fn_7100083990();
        field_0x60 = false;
    }
};
static_assert(sizeof(UnkSectionGroup_b0026) == 0x68, "layout");

// The groups by name: a chained hash map whose nodes are also listed in creation order.
struct UnkGroupNode_b0026 {
    CStrId key;
    UnkSectionGroup_b0026* value;
    // Union members: the original reloads the list ends after each link store.
    union {
        UnkGroupNode_b0026* next;
    };
    union {
        UnkGroupNode_b0026* prev;
    };
    UnkGroupNode_b0026* chain;
};

struct UnkGroupMap_b0026 {
    base::UnkAllocator* allocator;
    u32 flags;
    u32 count;
    UnkGroupNode_b0026** buckets;
    u32 bucket_count;
    u32 modulus;
    u8 pad_0x20[0x18];
    UnkGroupNode_b0026* last;   // 0x38
    UnkGroupNode_b0026* first;  // 0x40
};

struct UnkBlackboardGroups_b0026 {
    u8 pad_0x0[0x30];
    UnkGroupMap_b0026 groups;  // 0x30
};

// Move-constructs a section: the key is taken over, the data copied (fn_7100af0030).
void fn_7101049ee4(CBlackboard::CSection* self, CBlackboard::CSection* other) {
    self->key.data = other->key.data;
    other->key.data = &g_unk_7101d47108->empty;
    __builtin_memcpy(&self->blackboard, &other->blackboard, 0x10);
    fn_7100af0030(&self->data, &other->data);
    self->data.field_0x38 = other->data.field_0x38;
}

// Destroys a section: its properties' ids and values, their arrays, then its key.
void fn_7101048420(CBlackboard::CSection* self) {
    UnkPropertyArrays& p = self->data.properties;
    if (p.ids) {
        for (u64 i = 0; i < p.count; ++i)
            p.ids[i].Reset();
    }
    if (p.values) {
        for (u64 i = 0; i < p.count; ++i)
            fn_71000a1c60(&static_cast<UnkReflValue*>(p.values)[i]);
    }
    p.count = 0;
    if (p.ids)
        p.allocator->vf_0x48(p.ids);
    if (p.values)
        p.allocator->vf_0x48(p.values);
    self->key.Reset();
}

// Removes the entry in `slot`: its id is released and the last entry moved into its place.
// Returns the slot. The moves are untyped (the original reloads the arrays after each).
CBlackboard::CSection** fn_7101048588(UnkSectionArray_b0026* self, CBlackboard::CSection** slot) {
    u32 i = slot - self->values;
    self->ids[i].Reset();
    u32 last = self->count - 1;
    if (last != i) {
        __builtin_memcpy(&self->values[i], &self->values[last], sizeof(*slot));
        CStrId* ids = self->ids;
        last = self->count - 1;
        __builtin_memcpy(&ids[i], &ids[last], sizeof(CStrId));
        base::global::UnkStringData* empty = &g_unk_7101d47108->empty;
        __builtin_memcpy(&ids[last], &empty, sizeof(empty));
        self->ids[self->count - 1].Reset();
    }
    --self->count;
    return &self->values[i];
}

// A section's properties with the value type spelled out, for base/UnkParallelFind.h.
struct UnkPropertyValues_b0026 {
    UnkReflValue* values;
    CStrId* ids;
    u32 count;
};

// The type letter of the property `name` of the section `key` names ('n' when there is none).
s32 fn_71010475d0(CBlackboard* self, const CStrId& key, const CStrId& name) {
    if (CBlackboard::CSection* section = fn_71010472e0(self, key)) {
        auto* properties = reinterpret_cast<UnkPropertyValues_b0026*>(&section->data.properties);
        UnkReflValue* value = UnkParallelFindSlot(properties, name);
        if (value != &properties->values[properties->count] && value->type &&
            base::reflection::refl_ArgValue<void>(value))
            return fn_7100024140(reinterpret_cast<const base::reflection::CType*>(value->type));
    }
    return 'n';
}

UnkSectionGroup_b0026* fn_71010486bc(UnkBlackboardGroups_b0026* self, const CStrId& key) {
    auto* group = new (fn_71000839a0(sizeof(UnkSectionGroup_b0026), 0x15))
        UnkSectionGroup_b0026(reinterpret_cast<CBlackboard*>(self), key);
    UnkGroupMap_b0026& map = self->groups;
    UnkGroupNode_b0026** bucket = nullptr;
    if (map.bucket_count) {
        const UnkStrIdData_b0026* data = reinterpret_cast<const UnkStrIdData_b0026*>(key.data);
        u32 i = (data->hash_lo ^ data->hash_hi) % map.modulus;
        bucket = map.buckets + i;
    }
    UnkGroupNode_b0026* old = *bucket;
    auto* node = static_cast<UnkGroupNode_b0026*>(map.allocator->vf_0x40(sizeof(UnkGroupNode_b0026), 8, map.flags));
    if (node) {
        new (&node->key) CStrId(key);
        node->value = group;
        node->next = nullptr;
        node->prev = nullptr;
        node->chain = nullptr;
    }
    node->chain = old;
    *bucket = node;
    if (!map.first) {
        map.first = node;
    } else if (map.last) {
        node->prev = map.last;
        map.last->next = node;
    }
    map.last = node;
    ++map.count;
    return group;
}

// A list of names (CRntStrings linked through +0x30) starting at +0x10.
struct UnkNameListItem_b0026 {
    u8 pad_0x0[0x30];
    UnkNameListItem_b0026* next;
};
struct UnkNameList_b0026 {
    u8 pad_0x0[0x10];
    UnkNameListItem_b0026* first;
};

// A string id built from a string out of line (fn_7100000850), released like CStrId.
struct UnkBuiltStrId_b0026 {
    union {
        CStrId id;
    };

    explicit UnkBuiltStrId_b0026(const base::global::CRntString& str) { fn_7100000850(&id, str, true); }
    ~UnkBuiltStrId_b0026() { id.~CStrId(); }
};

// Constructs a copy of `src` at `dst`; the pointer is stored untyped (the original reloads the
// array after it).
static inline void CopyId(CStrId* dst, const CStrId& src) {
    base::global::UnkStringData* data = src.data;
    __builtin_memcpy(dst, &data, sizeof(data));
    if (&g_unk_7101d47108->empty != data)
        ++data->ref_count;
}

// Creates the group named `key` with the names on `list`.
UnkSectionGroup_b0026* fn_7101048820(UnkBlackboardGroups_b0026* self, const CStrId& key, UnkNameList_b0026* list) {
    UnkSectionGroup_b0026* group = fn_71010486bc(self, key);
    for (UnkNameListItem_b0026* item = list->first; item; item = item->next) {
        UnkBuiltStrId_b0026 built(*reinterpret_cast<const base::global::CRntString*>(item));
        const CStrId& name = built.id;
        if (group->names.count == group->names.capacity)
            fn_71000d97fc(reinterpret_cast<UnkKeyedArray_b62a1<UnkMovedStrId_b62a1>*>(&group->names),
                          group->names.count + group->names.step);
        CopyId(&group->names.ids[group->names.count], name);
        CopyId(&group->names.values[group->names.count], name);
        ++group->names.count;
    }
    return group;
}

// The blackboard's sections and its groups: the map's node list at +0x30 (fn_71010486bc) and a
// fallback group at +0x78.
class UnkBlackboardView_b0026 {
public:
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    // Adds `section` under `key` (fn_71010489c4).
    virtual void vf_0x28(const CStrId& key, CBlackboard::CSection* section);
    // Removes the section in `slot`; returns the slot that follows (fn_71010482d4).
    virtual CBlackboard::CSection** vf_0x30(CBlackboard::CSection** slot);

    UnkSectionArray_b0026 sections;  // 0x8
    UnkGroupMap_b0026 groups;        // 0x30
    UnkSectionGroup_b0026* field_0x78;
};
static_assert(__builtin_offsetof(UnkBlackboardView_b0026, field_0x78) == 0x78, "layout");

// Adds the section to the group, and `key` to the group's names when it is new (not matched yet).
void fn_71010467e8(UnkSectionGroup_b0026* self, const CStrId& key, CBlackboard::CSection* section);

// Adds `section` under `key`, then to the first group that names `key` (the fallback group when
// none does).
void fn_71010489c4(UnkBlackboardView_b0026* self, const CStrId& key, CBlackboard::CSection* section) {
    section->blackboard = reinterpret_cast<CBlackboard*>(self);
    if (self->sections.count == self->sections.capacity)
        reinterpret_cast<base::UnkCloneKeyedArray<UnkClone_fn_710104afb4>*>(&self->sections)
            ->Reserve(self->sections.count + self->sections.step);
    CopyId(&self->sections.ids[self->sections.count], key);
    __builtin_memcpy(&self->sections.values[self->sections.count], &section, sizeof(section));
    ++self->sections.count;
    for (UnkGroupNode_b0026* node = self->groups.first; node; node = node->next) {
        UnkSectionGroup_b0026* group = node->value;
        if (UnkParallelFindSlot(&group->names, key) != &group->names.values[group->names.count]) {
            fn_71010467e8(group, key, section);
            return;
        }
    }
    if (self->field_0x78)
        fn_71010467e8(self->field_0x78, key, section);
}

// Removes every section, then empties every group's section list.
void fn_7101047ea8(UnkBlackboardView_b0026* self) {
    for (CBlackboard::CSection** it = self->sections.values; &self->sections.values[self->sections.count] != it;)
        it = self->vf_0x30(it);
    self->sections.ClearIfAny();
    for (UnkGroupNode_b0026* node = self->groups.first; node; node = node->next)
        node->value->sections.Clear();
}

// Flags every section, and the group each belongs to.
void fn_7101048af8(CBlackboard* self) {
    for (CBlackboard::CSection **it = self->sections, **end = it + self->count; it != end; ++it) {
        CBlackboard::CSection* section = *it;
        section->data.field_0x38 = true;
        if (auto* group = static_cast<UnkSectionGroup_b0026*>(section->field_0x10))
            group->field_0x60 = true;
    }
}

// A string with 0x100 bytes of inline storage (0x148 bytes), as in src/base/UnkTunableRegistry.cpp.
struct UnkInlineString_b0026 {
    u8 str[0x30];
    char* storage_vtable;  // +0x30
    u8 buffer[0x100];
    u64 field_0x138;
    u32 field_0x140;

    CRntString* Str() { return reinterpret_cast<CRntString*>(str); }
    UnkInlineString_b0026() {
        fn_710007e6e4(Str(), reinterpret_cast<base::UnkAllocator*>(&storage_vtable));
        storage_vtable = reinterpret_cast<char*>(g_unk_7101a194b8 + 2);
        field_0x138 = 0;
        field_0x140 = 0;
    }
    ~UnkInlineString_b0026() {
        fn_710007ed40(Str());
        fn_710007ecd4(Str());
    }
};

// The blackboard's two strings (base/CBlackboard.cpp's constructor builds them).
struct UnkBlackboardStrings_b0026 {
    u8 pad_0x0[0x80];
    CRntString field_0x80;
    CRntString field_0xb0;
};

// Replaces `str` by its lower-case copy.
static inline void ToLower(CRntString* str) {
    const char* text = str->data ? str->data : g_unk_710162e2ed;
    char lower[0x100];
    lower[0] = '\0';
    u32 length = __builtin_strlen(text);
    if (length) {
        for (u32 i = 0; i < length; ++i) {
            char c = text[i];
            lower[i] = (u8)(c - 'A') < 26 ? c + ('a' - 'A') : c;
        }
        lower[length] = '\0';
    }
    UnkInlineString_b0026 tmp;
    fn_710007e77c(tmp.Str(), lower);
    fn_710007eaa0(str, tmp.Str());
}

// The two strings joined, in lower case.
void fn_71010476a0(UnkBlackboardStrings_b0026* self, CRntString* out) {
    fn_710007eaa0(out, &self->field_0x80);
    fn_710007f3b4(out, &self->field_0xb0);
    ToLower(out);
}

// What a blackboard's user names itself by: a string at +0x10 and a counter at +0x44.
struct UnkBlackboardUserInfo_b0026 {
    u8 pad_0x0[0x10];
    CRntString name;
    u8 pad_0x40[0x4];
    // A union member: the original reloads the pointer to it after the increment.
    union {
        s32 field_0x44;
    };
};

struct UnkBlackboardUser_b0026 {
    UnkBlackboardStrings_b0026* blackboard;
    UnkBlackboardUserInfo_b0026* info;
};

// The user's name joined with the blackboard's second string, in lower case.
void fn_7101047b2c(UnkBlackboardUser_b0026* self, CRntString* out) {
    ++self->info->field_0x44;
    fn_710007eaa0(out, &self->info->name);
    {
        base::UnkNamedEntry suffix;
        fn_710007ea60(&suffix, reinterpret_cast<const base::UnkNamedEntry*>(&self->blackboard->field_0xb0));
        fn_710007f3b4(out, reinterpret_cast<CRntString*>(&suffix));
        fn_710007ecd4(reinterpret_cast<CRntString*>(&suffix));
    }
    ToLower(out);
}

struct UnkStrIdTable;
void fn_710104b27c(UnkStrIdTable* self);

// Copy-assigns the sections' arrays: the old ids are released, the arrays resized to the other's
// count when their capacity differs, the values copied and the ids copied in.
UnkSectionArray_b0026* fn_710104b114(UnkSectionArray_b0026* self, const UnkSectionArray_b0026* other) {
    if (self != other) {
        self->Clear();
        if (other->count != self->capacity) {
            fn_710104b27c(reinterpret_cast<UnkStrIdTable*>(self));
            reinterpret_cast<base::UnkCloneKeyedArray<UnkClone_fn_710104afb4>*>(self)->Reserve(other->count);
        }
        __builtin_memcpy(self->values, other->values, (u64)other->count * sizeof(*self->values));
        self->count = other->count;
        self->count = other->count;
        for (u64 i = 0; i < self->count; ++i)
            CopyId(&self->ids[i], other->ids[i]);
    }
    return self;
}

struct UnkVersionRegistry;
struct UnkVersionEntry;

// A typed pointer to a value the reader fills (a CName id here).
struct UnkReadArg_b0026 {
    const base::reflection::CType* type;
    u64* value;
};
void fn_71000ee9fc(void* reader, u32 flags, UnkReadArg_b0026* arg, const base::global::CStrId* name);
const base::reflection::CType* fn_710009c640(base::reflection::UnkReflObject* obj);

struct UnkBlackboardReader_b0026 {
    u8 pad_0x0[0x30];
    void* reader;
};
s32 fn_710104b640(UnkBlackboardReader_b0026* self, base::reflection::UnkReflObject* obj, const base::reflection::CType* type);

// Reads the object's name id, and, for a pointer, the id of the type it points to (the registered
// type replaces the object's own when there is one), then reads the object as that type.
s32 fn_710104b540(UnkBlackboardReader_b0026* self, base::reflection::UnkReflObject* obj) {
    UnkReadArg_b0026 arg;
    u64 name = (u64)-1;
    void* reader = self->reader;
    arg.type = fn_710000fd7c();
    arg.value = &name;
    fn_71000ee9fc(reader, 0, &arg, &g_unk_7101d470e0);
    const base::reflection::CType* type = fn_710009c640(obj);
    if (fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(obj->vf_0x0()),
                      reinterpret_cast<const base::reflection::CType*>(fn_710009bedc()))) {
        u64 id = (u64)-1;
        reader = self->reader;
        arg.type = fn_710000fd7c();
        arg.value = &id;
        fn_71000ee9fc(reader, 1, &arg, &g_unk_7101d470e0);
        if (id != (u64)-1)
            type = reinterpret_cast<const base::reflection::CType*>(fn_71000a5118(g_unk_7101d4c8d0, &id));
    }
    return fn_710104b640(self, obj, type);
}

// ---- writing the blackboard ----

struct UnkVersionWriter_A1b71;
void fn_71000ed224(UnkVersionWriter_A1b71* self, base::global::CRntFile* file, UnkVersionEntry* entry);

// The writer's state (0xc90 bytes, src/base/UnkUnit90/UnkVersionWriter_A1_b71.cpp).
struct UnkWriter_b0026 {
    u8 data[0xc90];
};

// The objects still to write: up to 100 entries of three values each.
struct UnkWriteEntry_b0026 {
    UnkReflValue a;
    u8 pad_0x18[0x18];
    UnkReflValue b;
    UnkReflValue c;
    u8 pad_0x60[0x8];
};
struct UnkWriteStack_b0026 {
    u8 pad_0x0[0x14];
    u32 count;
    UnkWriteEntry_b0026 entries[100];
    u8 pad_0x28b8[0x8];
};
static_assert(sizeof(UnkWriteStack_b0026) == 0x28c0, "layout");

struct UnkWriteRoot_b0026 {
    char* type;
    void* object;
};

void* fn_71000edd0c(UnkWriter_b0026* self);
struct UnkWriteCallbacks_b0026 {
    u64 words[6];
};
extern UnkWriteCallbacks_b0026 g_unk_71729fd068 __attribute__((visibility("hidden")));

// What fn_710104b540 is given: this unit's callbacks, the objects to write and the writer.
struct UnkWriteContext_b0026 {
    UnkWriteCallbacks_b0026 callbacks;
    UnkWriteStack_b0026* stack;
    UnkWriter_b0026* writer;
};

// Writes the blackboard to `file`; when it is written, every section and group is unflagged.
void fn_7101047988(CBlackboard* self, base::global::CRntFile* file) {
    auto* object = reinterpret_cast<base::reflection::UnkReflObject*>(self);
    UnkWriteContext_b0026 ctx;
    UnkWriteStack_b0026 stack;
    {
        UnkWriteRoot_b0026 root = {fn_710009dcd8(const_cast<char*>(reinterpret_cast<const char*>(object->vf_0x0())), self), self};
        fn_71000ee314(&stack, &root, 2);
    }
    UnkWriter_b0026 writer;
    fn_71000ed224(reinterpret_cast<UnkVersionWriter_A1b71*>(&writer), file,
                  reinterpret_cast<UnkVersionEntry*>(const_cast<base::reflection::CClass*>(object->vf_0x0())));
    if (stack.count) {
        const UnkWriteCallbacks_b0026* callbacks = &g_unk_71729fd068;
        ctx.callbacks.words[0] = callbacks->words[0];
        ctx.callbacks.words[1] = callbacks->words[1];
        ctx.callbacks.words[2] = callbacks->words[2];
        ctx.callbacks.words[3] = callbacks->words[3];
        ctx.callbacks.words[4] = callbacks->words[4];
        ctx.callbacks.words[5] = callbacks->words[5];
        ctx.stack = &stack;
        ctx.writer = &writer;
        if (fn_710104b540(reinterpret_cast<UnkBlackboardReader_b0026*>(&ctx),
                          static_cast<base::reflection::UnkReflObject*>(fn_71000edd0c(&writer)))) {
            for (CBlackboard::CSection **it = self->sections, **end = it + self->count; it != end; ++it)
                if (*it)
                    (*it)->data.field_0x38 = false;
            for (UnkGroupNode_b0026* node = reinterpret_cast<UnkBlackboardView_b0026*>(self)->groups.first; node;
                 node = node->next)
                if (node->value)
                    node->value->field_0x60 = false;
        }
    }
    for (u64 i = 0; i < stack.count; ++i) {
        fn_71000a1c60(&stack.entries[i].c);
        fn_71000a1c60(&stack.entries[i].b);
        fn_71000a1c60(&stack.entries[i].a);
    }
}

// The group a blackboard is writing (+0xe0), set while fn_7101048b94 runs.
struct UnkBlackboardWriting_b0026 {
    u8 pad_0x0[0xe0];
    UnkSectionGroup_b0026* writing;
};

// Writes the group's blackboard to `file` with the group marked as the one being written; when it
// is written, the group is unflagged.
void fn_7101048b94(UnkSectionGroup_b0026* self, base::global::CRntFile* file) {
    // Stored untyped: the original reloads the owner after it.
    __builtin_memcpy(&reinterpret_cast<UnkBlackboardWriting_b0026*>(self->owner)->writing, &self, sizeof(self));
    auto* object = reinterpret_cast<base::reflection::UnkReflObject*>(self->owner);
    UnkWriteContext_b0026 ctx;
    UnkWriteStack_b0026 stack;
    {
        UnkWriteRoot_b0026 root = {fn_710009dcd8(const_cast<char*>(reinterpret_cast<const char*>(object->vf_0x0())), object), object};
        fn_71000ee314(&stack, &root, 2);
    }
    UnkWriter_b0026 writer;
    fn_71000ed224(reinterpret_cast<UnkVersionWriter_A1b71*>(&writer), file,
                  reinterpret_cast<UnkVersionEntry*>(const_cast<base::reflection::CClass*>(
                      reinterpret_cast<base::reflection::UnkReflObject*>(self->owner)->vf_0x0())));
    if (stack.count) {
        const UnkWriteCallbacks_b0026* callbacks = &g_unk_71729fd068;
        ctx.callbacks.words[0] = callbacks->words[0];
        ctx.callbacks.words[1] = callbacks->words[1];
        ctx.callbacks.words[2] = callbacks->words[2];
        ctx.callbacks.words[3] = callbacks->words[3];
        ctx.callbacks.words[4] = callbacks->words[4];
        ctx.callbacks.words[5] = callbacks->words[5];
        ctx.stack = &stack;
        ctx.writer = &writer;
        if (fn_710104b540(reinterpret_cast<UnkBlackboardReader_b0026*>(&ctx),
                          static_cast<base::reflection::UnkReflObject*>(fn_71000edd0c(&writer))))
            self->field_0x60 = false;
    }
    reinterpret_cast<UnkBlackboardWriting_b0026*>(self->owner)->writing = nullptr;
    for (u64 i = 0; i < stack.count; ++i) {
        fn_71000a1c60(&stack.entries[i].c);
        fn_71000a1c60(&stack.entries[i].b);
        fn_71000a1c60(&stack.entries[i].a);
    }
}
