#include "base/UnkCloneCalls.h"
#include "base/UnkVector.h"
#include "types.h"

struct UnkRecord98_b54a1;
struct UnkKeyedArray98_b54a1;
struct UnkNode_b0253;
struct UnkObject_710019506c;
struct UnkChild_710019b304;

u64 fn_7100001570(const char* str, u64 length);
void fn_710019b7e8(UnkKeyedArray98_b54a1* self, u32 capacity);
void fn_7100194ba4(UnkRecord98_b54a1* self, UnkRecord98_b54a1* other);
void fn_7100194a9c(UnkNode_b0253* self);
void fn_710019506c(UnkObject_710019506c* self);
void fn_710019b304(UnkChild_710019b304* child);

struct UnkBoneArrayTag_b0261;

struct UnkBoneRecord_b0261 {
    u8 data[0x98];
};

struct UnkBoneDefInfo_b0261 {
    u8 pad_0x0[0x8];
    const char* name;    // 0x8
    const char* parent;  // 0x10
};

struct UnkBoneDefEntry_b0261 {
    u8* transform;  // 0x0: the transform data (+0x24 is passed on)
    const char* name;
    u8 pad_0x10[8];
    bool flag;  // 0x18
};

struct UnkBoneDefNode_b0261 {
    UnkBoneDefEntry_b0261* entry;  // 0x0
    UnkBoneDefNode_b0261* next;    // 0x8
};

struct UnkBoneDefList_b0261 {
    u32 count;  // 0x0
    u8 pad_0x4[4];
    UnkBoneDefNode_b0261* first;  // 0x8
};

struct UnkChildOwner_b0261 {
    u8 pad_0x0[0x28];
    u32 child_count;  // 0x28
    u8 pad_0x2c[0x14];
    UnkChild_710019b304** children;  // 0x40
};

struct UnkBoneSkeleton_b0261 {
    UnkChildOwner_b0261* owner;      // 0x0
    UnkBoneRecord_b0261* values;     // 0x8
    u64* keys;                       // 0x10
    u32 count;                       // 0x18
    u32 capacity;                    // 0x1c
    u32 step;                        // 0x20
    u32 flags;                       // 0x24
    base::UnkAllocator* allocator;   // 0x28
    bool done;                       // 0x30
};

static inline UnkBoneRecord_b0261* FindBone_b0261(UnkBoneSkeleton_b0261* self, u64 key) {
    for (u64 i = 0; i < self->count; ++i)
        if (self->keys[i] == key)
            return &self->values[i];
    return &self->values[self->count];
}

void fn_7100194cfc(UnkBoneRecord_b0261* self, u8* transform, UnkBoneRecord_b0261* parent, const char* name, bool flag);

static inline u64 HashName_b0261(const char* s) {
    return fn_7100001570(s, __builtin_strlen(s));
}

void fn_710019b5ec(UnkBoneSkeleton_b0261* self, UnkBoneDefList_b0261* list, UnkChildOwner_b0261* owner) {
    self->owner = owner;
    self->values = nullptr;
    self->keys = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = 10;
    self->flags = 0x36;
    self->allocator = fn_7100083990();
    self->done = true;
    fn_710019b7e8(reinterpret_cast<UnkKeyedArray98_b54a1*>(&self->values), list->count);
    UnkBoneDefNode_b0261* node = list->first;
    for (u32 i = 0; i < list->count; i++) {
        if (node) {
            UnkBoneRecord_b0261* parent_bone;
            UnkBoneDefInfo_b0261* info = reinterpret_cast<UnkBoneDefInfo_b0261*>(node->entry);
            const char* parent_name = info->parent;
            if (parent_name) {
                u64 parent_key = HashName_b0261(parent_name);
                UnkBoneRecord_b0261* found = FindBone_b0261(self, parent_key);
                parent_bone = found == self->values + self->count ? nullptr : found;
            } else {
                parent_bone = nullptr;
            }
            u64 key = HashName_b0261(reinterpret_cast<UnkBoneDefInfo_b0261*>(node->entry)->name);
            alignas(8) char tmp[0x98];
            fn_7100194a9c(reinterpret_cast<UnkNode_b0253*>(tmp));
            if (self->count == self->capacity)
                fn_710019b7e8(reinterpret_cast<UnkKeyedArray98_b54a1*>(&self->values), self->step + self->count);
            __builtin_memcpy(&self->keys[self->count], &key, 8);
            fn_7100194ba4(reinterpret_cast<UnkRecord98_b54a1*>(&self->values[(u64)self->count]),
                          reinterpret_cast<UnkRecord98_b54a1*>(tmp));
            u32 n = self->count + 1;
            self->count = n;
            UnkBoneRecord_b0261* end = self->values + n;
            UnkBoneRecord_b0261* bone = n ? end - 1 : self->values;
            fn_710019506c(reinterpret_cast<UnkObject_710019506c*>(tmp));
            UnkBoneDefEntry_b0261* entry = node->entry;
            fn_7100194cfc(bone, entry->transform + 0x24, parent_bone, entry->name, entry->flag);
            node = node->next;
        }
    }
    UnkChildOwner_b0261* o = self->owner;
    for (u64 i = 0; i < o->child_count; i++)
        fn_710019b304(o->children[i]);
}
