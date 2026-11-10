// Unit 2661 (tools/tu_map.py; CCutscene::CTunableCutscene by reflection): the cutscene's entry array
// (0x30-byte entries at +0x150, count at +0x158) and the helpers on it: whether every entry is set,
// creating the entries' objects, finding an entry's object by name (the name's text found inside the
// entry key's text before its first underscore), a string-id holder's vector appends and moves, and the
// big settings record's copy assignment. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/UnkRenderManager.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

// A shared string: the string at +0x10 and its reference count at +0x44 (the count is a union member
// so its increment forces the reloads the original has).
struct UnkSharedRep_b0181 {
    u8 pad_0x0[0x10];
    CRntString str;  // 0x10
    u8 pad_0x40[4];
    union {
        u32 ref_count;  // 0x44
    };
};
struct UnkSharedStr_b0181 {
    UnkSharedRep_b0181* rep;
    // Passing a shared string on takes a reference that is not given back here.
    void Take() const { ++rep->ref_count; }
};

struct UnkCutsceneEntry_b0181 {
    u8 pad_0x0[0x10];
    UnkSharedStr_b0181* key;  // 0x10
    void* field_0x18;
    void* field_0x20;
    u8 pad_0x28[2];
    u8 flag_0x2a;
    u8 pad_0x2b[5];
};

struct UnkCutscene_b0181 {
    u8 pad_0x0[0x150];
    UnkCutsceneEntry_b0181* entries;  // 0x150
    u32 entry_count;                  // 0x158
};

void fn_7101066a70(UnkCutsceneEntry_b0181* entry);
void* fn_7101066438(UnkCutsceneEntry_b0181* entry);

// Whether every entry is either without an object or flagged.
bool fn_710106cd50(UnkCutscene_b0181* self) {
    u32 n = 0;
    for (UnkCutsceneEntry_b0181 *it = self->entries, *end = it + self->entry_count; it != end; ++it)
        n += it->field_0x20 == nullptr || it->flag_0x2a != 0;
    return n == self->entry_count;
}

// Creates the missing objects and runs each entry.
void fn_710106cf10(UnkCutscene_b0181* self) {
    for (UnkCutsceneEntry_b0181 *it = self->entries, *end = it + self->entry_count; it != end; ++it) {
        if (!it->field_0x20)
            it->field_0x20 = fn_7101066438(it);
        if (it->field_0x20)
            fn_7101066a70(it);
    }
}

s32 fn_710007fb54(const CRntString* self, const char* str, u32 start);
s32 fn_710007facc(const CRntString* self, char c, u32 start);

// The object of the first entry whose key's text holds the name's text before its first underscore.
void* fn_710106caa4(UnkCutscene_b0181* self, UnkSharedStr_b0181* name) {
    for (UnkCutsceneEntry_b0181 *it = self->entries, *end = it + self->entry_count; it != end; ++it) {
        UnkSharedStr_b0181* key = it->key;
        key->Take();
        const CRntString* key_str = &key->rep->str;
        name->Take();
        const char* text = name->rep->str.data;
        s32 pos = fn_710007fb54(key_str, text ? text : g_unk_710162e2ed, 0);
        key->Take();
        s32 limit = fn_710007facc(&key->rep->str, '_', 0);
        if (limit < 0)
            limit = key->rep->str.length;
        if (pos >= 0 && pos < limit)
            return it->field_0x18;
    }
    return nullptr;
}

struct UnkSubInner_b0181 {
    u8 pad_0x0[0x138];
    void* field_0x138;
};
struct UnkSubObj_b0181 {
    u8 pad_0x0[0x50];
    UnkSubInner_b0181* field_0x50;
};
struct UnkCutsceneActor_b0181 {
    u8 pad_0x0[0x228];
    UnkSubObj_b0181* field_0x228;
    u8 pad_0x230[0x50];
    void* field_0x280;
};

void fn_71009f19b0(void* object, bool flag);
struct UnkResetOwner;
void fn_71001c3270(UnkResetOwner* object);

// The same lookup, then applies the flags to the object found.
void fn_710106cdb0(UnkCutscene_b0181* self, UnkSharedStr_b0181* name, bool flag, bool flag2) {
    for (UnkCutsceneEntry_b0181 *it = self->entries, *end = it + self->entry_count; it != end; ++it) {
        UnkSharedStr_b0181* key = it->key;
        key->Take();
        const CRntString* key_str = &key->rep->str;
        name->Take();
        const char* text = name->rep->str.data;
        s32 pos = fn_710007fb54(key_str, text ? text : g_unk_710162e2ed, 0);
        key->Take();
        s32 limit = fn_710007facc(&key->rep->str, '_', 0);
        if (limit < 0)
            limit = key->rep->str.length;
        if (pos >= 0 && pos < limit) {
            UnkCutsceneActor_b0181* actor = static_cast<UnkCutsceneActor_b0181*>(it->field_0x18);
            if (!actor)
                return;
            if (UnkSubObj_b0181* sub = actor->field_0x228) {
                fn_71009f19b0(sub, flag);
                if (flag2 && sub->field_0x50)
                    fn_71001c3270(static_cast<UnkResetOwner*>(sub->field_0x50->field_0x138));
            }
            if (actor->field_0x280)
                fn_71009f19b0(actor->field_0x280, flag);
            return;
        }
    }
}

extern void* g_unk_7101cac988[];

// A 0x30-byte holder: a destructor slot, a word and a 27-byte payload.
struct UnkHolder_b0181;
struct UnkHolderVtable_b0181 {
    void (*dtor)(UnkHolder_b0181* self);
};
struct UnkHolder_b0181 {
    const UnkHolderVtable_b0181* vtable;
    u64 field_0x8;
    u8 payload[27];

    UnkHolder_b0181(UnkHolder_b0181&& other)
        : vtable(reinterpret_cast<const UnkHolderVtable_b0181*>(g_unk_7101cac988 + 2)), field_0x8(other.field_0x8) {
        __builtin_memcpy(payload, other.payload, sizeof(payload));
    }
    ~UnkHolder_b0181() { vtable->dtor(this); }
};

void fn_7101065c4c(UnkHolder_b0181* slot, void* a, void* b);

// Appends a holder built by fn_7101065c4c and returns it.
UnkHolder_b0181* fn_7101068b1c(base::UnkVector<UnkHolder_b0181>* self, void* a, void* b) {
    u32 n = self->count;
    u32 new_capacity;
    UnkHolder_b0181* end;
    if (n == self->capacity) {
        if (self->step == 0)
            goto hang;
        new_capacity = (n / self->step + 1) * self->step;
        if ((s32)n >= (s32)new_capacity)
            goto hang;
        if (n < new_capacity) {
            UnkHolder_b0181* p = static_cast<UnkHolder_b0181*>(
                self->allocator->vf_0x40((u64)new_capacity * sizeof(UnkHolder_b0181), 8, self->flags));
            if (self->data) {
                UnkHolder_b0181* src = self->data;
                UnkHolder_b0181* dst = p;
                for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
                    new (dst) UnkHolder_b0181(static_cast<UnkHolder_b0181&&>(*src));
                    src->~UnkHolder_b0181();
                }
                self->allocator->vf_0x48(self->data);
            }
            n = self->count;
            self->data = p;
            self->capacity = new_capacity;
        }
    }
    fn_7101065c4c(&self->data[n], a, b);
    n = self->count + 1;
    end = &self->data[n];
    self->count = n;
    return n ? end - 1 : end;
hang:
    for (;;) {
    }
}

struct UnkRenderPool;
struct UnkSceneObject_b0181;

// An entry (0x18 bytes) that owns a scene node: dropped from the render manager when destroyed.
struct UnkNodeEntry_b0181 {
    UnkSceneObject_b0181* owner;
    u32 field_0x8;
    u32 field_0xc;
    UnkSceneNode* node;  // 0x10

    UnkNodeEntry_b0181(UnkNodeEntry_b0181&& other) { __builtin_memcpy(this, &other, sizeof(*this)); }
    ~UnkNodeEntry_b0181() {
        fn_71002199d8(g_unk_7101d47300, reinterpret_cast<UnkItemBase_71011a91c4*>(node), 0);
        node = nullptr;
    }
};

void fn_7100b74b9c(UnkRenderPool* pool, void* name, UnkSceneNode** out, const CStrId* id);

// Appends an entry whose node the render pool creates for the owner, and returns it.
UnkNodeEntry_b0181* fn_7101068980(base::UnkVector<UnkNodeEntry_b0181>* self, UnkSceneObject_b0181* owner,
                                  const u32* a, const u32* b) {
    u32 n = self->count;
    u32 new_capacity;
    UnkNodeEntry_b0181* end;
    if (n == self->capacity) {
        if (self->step == 0)
            goto hang;
        new_capacity = (n / self->step + 1) * self->step;
        if ((s32)n >= (s32)new_capacity)
            goto hang;
        if (n < new_capacity) {
            UnkNodeEntry_b0181* p = static_cast<UnkNodeEntry_b0181*>(
                self->allocator->vf_0x40((u64)new_capacity * sizeof(UnkNodeEntry_b0181), 8, self->flags));
            if (self->data) {
                UnkNodeEntry_b0181* src = self->data;
                UnkNodeEntry_b0181* dst = p;
                for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
                    new (dst) UnkNodeEntry_b0181(static_cast<UnkNodeEntry_b0181&&>(*src));
                    src->~UnkNodeEntry_b0181();
                }
                self->allocator->vf_0x48(self->data);
            }
            n = self->count;
            self->data = p;
            self->capacity = new_capacity;
        }
    }
    {
        UnkNodeEntry_b0181* entry = &self->data[n];
        u32 first = *a;
        u32 second = *b;
        entry->owner = owner;
        entry->field_0x8 = first;
        entry->field_0xc = second;
        UnkRenderManager* manager = g_unk_7101d47300;
        UnkSceneNode* node = nullptr;
        fn_7100b74b9c(manager->field_0x218, reinterpret_cast<u8*>(owner) + 0x10, &node, &g_unk_7101d470e0);
        fn_7100219ae0(manager, node, 0);
        entry->node = node;
    }
    n = self->count + 1;
    end = &self->data[n];
    self->count = n;
    return n ? end - 1 : end;
hang:
    for (;;) {
    }
}

// ---- the tuple holder: a word and a holder, by value (tag 1) or by pointers (tag 2)
struct UnkTupleHolder_b0181 {
    char* vtable;
    u64 field_0x8;
    u8 payload[27];
};
struct UnkTupleVals_b0181 {
    u32 a;
    UnkTupleHolder_b0181 h;
};
struct UnkTuplePtrs_b0181 {
    u32* a;
    UnkTupleHolder_b0181* h;
};
struct UnkTuple_b0181 {
    union {
        UnkTupleVals_b0181 v;
        UnkTuplePtrs_b0181 p;
    };
    u32 tag;  // 0x38
};

static inline u32 GetA(const UnkTuple_b0181* t) { return t->tag == 2 ? *t->p.a : t->v.a; }
static inline u32* RefA(UnkTuple_b0181* t) { return t->tag == 2 ? t->p.a : &t->v.a; }
static inline UnkTupleHolder_b0181* RefH(UnkTuple_b0181* t) { return t->tag == 2 ? t->p.h : &t->v.h; }
static inline const UnkTupleHolder_b0181* GetH(const UnkTuple_b0181* t) { return t->tag == 2 ? t->p.h : &t->v.h; }

// Assignment: the word and the holder's word and payload.
void fn_710106eb44(UnkTuple_b0181* self, const UnkTuple_b0181* other) {
    if (self == other)
        return;
    u32 a = GetA(other);
    *RefA(self) = a;
    const UnkTupleHolder_b0181* src = GetH(other);
    UnkTupleHolder_b0181* dst = RefH(self);
    dst->field_0x8 = src->field_0x8;
    __builtin_memcpy(dst->payload, src->payload, sizeof(dst->payload));
}

// The same, for a set tuple only.
void fn_710106ebdc(UnkTuple_b0181* self, const UnkTuple_b0181* other) {
    if (self == other)
        return;
    if (other->tag == 1) {
        u32 a = other->v.a;
        *RefA(self) = a;
        UnkTupleHolder_b0181* dst = RefH(self);
        dst->field_0x8 = other->v.h.field_0x8;
        __builtin_memcpy(dst->payload, other->v.h.payload, sizeof(dst->payload));
    } else if (other->tag == 2) {
        u32 a = *other->p.a;
        *RefA(self) = a;
        const UnkTupleHolder_b0181* src = other->p.h;
        UnkTupleHolder_b0181* dst = RefH(self);
        dst->field_0x8 = src->field_0x8;
        __builtin_memcpy(dst->payload, src->payload, sizeof(dst->payload));
    }
}

// ---- the settings record's copy assignment
struct UnkRefObj_b0181 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    union {
        s32 ref_count;  // 0x8
    };
};

struct UnkInner_b0181 {
    u8 data[0xb0];
};
void fn_710002f90c(void* dst, const void* src);

struct UnkBigState_b0181 {
    void* vtable;          // 0x0
    u8 block[0x20];        // 0x8
    UnkRefObj_b0181* ref;  // 0x28
    CStrId name;           // 0x30
    u32 field_0x38;
    u32 field_0x3c;
    u32 field_0x40;
    u32 field_0x44;
    u32 field_0x48;
    u32 field_0x4c;
    u32 field_0x50;
    u32 field_0x54;
    u32 field_0x58;
    u32 field_0x5c;
    u32 field_0x60;
    u32 field_0x64;
    u32 field_0x68;
    u32 field_0x6c;
    u32 field_0x70;
    u32 field_0x74;
    u32 field_0x78;
    u32 field_0x7c;
    u32 field_0x80;
    u32 field_0x84;
    u32 field_0x88;
    u32 field_0x8c;
    u32 field_0x90;
    u32 field_0x94;
    u32 field_0x98;
    u32 field_0x9c;
    u32 field_0xa0;
    u32 field_0xa4;
    u32 field_0xa8;
    u8 pad_0xac[4];
    UnkInner_b0181 inner;  // 0xb0
    u64 field_0x160;
    u8 field_0x168;
    u8 pad_0x169[3];
    u32 field_0x16c;
    u32 field_0x170;
    u16 field_0x174;
};
static_assert(__builtin_offsetof(UnkBigState_b0181, inner) == 0xb0, "layout");
static_assert(__builtin_offsetof(UnkBigState_b0181, field_0x160) == 0x160, "layout");

UnkBigState_b0181* fn_71010718fc(UnkBigState_b0181* self, const UnkBigState_b0181* other) {
    __builtin_memcpy(self->block, other->block, sizeof(self->block));
    UnkRefObj_b0181* incoming = other->ref;
    if (self->ref != incoming) {
        if (incoming)
            ++incoming->ref_count;
        if (UnkRefObj_b0181* old = self->ref) {
            if (--old->ref_count == 0)
                old->vf_0x10();
        }
        self->ref = incoming;
    }
    self->name = other->name;
    if (other != self) {
        self->field_0x38 = other->field_0x38;
        self->field_0x3c = other->field_0x3c;
        self->field_0x40 = other->field_0x40;
        self->field_0x44 = other->field_0x44;
        self->field_0x48 = other->field_0x48;
        self->field_0x4c = other->field_0x4c;
        self->field_0x50 = other->field_0x50;
        self->field_0x54 = other->field_0x54;
        self->field_0x58 = other->field_0x58;
        self->field_0x5c = other->field_0x5c;
        self->field_0x60 = other->field_0x60;
        self->field_0x64 = other->field_0x64;
        self->field_0x68 = other->field_0x68;
        self->field_0x6c = other->field_0x6c;
        self->field_0x70 = other->field_0x70;
        self->field_0x74 = other->field_0x74;
        self->field_0x78 = other->field_0x78;
        self->field_0x7c = other->field_0x7c;
        self->field_0x80 = other->field_0x80;
        self->field_0x84 = other->field_0x84;
        self->field_0x88 = other->field_0x88;
        self->field_0x8c = other->field_0x8c;
        self->field_0x90 = other->field_0x90;
    }
    self->field_0x94 = other->field_0x94;
    self->field_0x98 = other->field_0x98;
    self->field_0x9c = other->field_0x9c;
    self->field_0xa0 = other->field_0xa0;
    self->field_0xa4 = other->field_0xa4;
    self->field_0xa8 = other->field_0xa8;
    fn_710002f90c(&self->inner, &other->inner);
    u8 flag = other->field_0x168;
    self->field_0x160 = other->field_0x160;
    self->field_0x168 = flag;
    self->field_0x16c = other->field_0x16c;
    self->field_0x170 = other->field_0x170;
    self->field_0x174 = other->field_0x174;
    return self;
}
