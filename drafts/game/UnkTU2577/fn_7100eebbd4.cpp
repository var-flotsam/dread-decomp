// Unit 2577 (CSubAreaManager). For each of the three group kinds: gathers every group's members, drops the
// ones a sub area still refers to, and files the rest under that kind's "ORPHANS_Default" group.
// Names and layouts are placeholders.

#include "base/UnkErasedVector.h"
#include "base/UnkRefHandle.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

extern const u32 g_unk_710164bf1c[3];
extern char g_unk_7101a20998[];
extern char g_unk_7101a20950[];


struct UnkStrHashWords {
    u8 pad_0x0[0x30];
    u32 field_0x30;
    u32 field_0x34;
};

struct UnkMemberLink {
    u8 pad_0x0[0x8];
    void* member;
    UnkMemberLink* next;
};

struct UnkGroup {
    u8 pad_0x0[0x8];
    CStrId id;
    u8 pad_0x10[0xc];
    u32 field_0x1c;
    u8 pad_0x20[0x30];
    UnkMemberLink* field_0x50;
    u8 pad_0x58[0x8];
};
static_assert(sizeof(UnkGroup) == 0x60, "UnkGroup layout");

struct UnkGroupTable {
    u8 pad_0x0[0x20];
    u8 field_0x20[0x8];
    UnkGroup* groups;
    u8 pad_0x30[0x8];
    u32 group_count;
    u8 pad_0x3c[0x8c];
};
static_assert(sizeof(UnkGroupTable) == 0xc8, "UnkGroupTable layout");

struct UnkSubAreaItem {
    u8 pad_0x0[0x30];
    CStrId ids[3];
};

struct UnkSubArea {
    u8 pad_0x0[0x10];
    UnkSubAreaItem** items;
    u32 item_count;
    u8 pad_0x1c[0x16c];
};
static_assert(sizeof(UnkSubArea) == 0x188, "UnkSubArea layout");

struct UnkSubAreaManager {
    u8 pad_0x0[0x860];
    UnkSubArea* areas;
    u32 area_count;
};

struct UnkOwnerHandle {
    virtual void vf0();
    virtual void vf8();
    virtual void Delete();
    union {
        s32 ref_count;
    };
    void* target;
};

struct UnkOwnerRef {
    UnkOwnerHandle* ptr;
    explicit UnkOwnerRef(UnkOwnerHandle* p) : ptr(nullptr) {
        if (p) {
            ++p->ref_count;
            ptr = p;
        }
    }
    ~UnkOwnerRef() {
        if (ptr) {
            if (--ptr->ref_count == 0)
                ptr->Delete();
        }
    }
};

struct UnkSetBase {
    char* vtable;
    UnkOwnedErasedVector callbacks;
    u32 field_0x30;

    UnkSetBase() : vtable(&g_unk_7101a20998[0x10]), field_0x30(0) {}
    ~UnkSetBase() { vtable = &g_unk_7101a20998[0x10]; }
};

struct UnkSet : UnkSetBase {
    UnkOwnerHandle* owner;

    UnkSet() {
        vtable = &g_unk_7101a20950[0x10];
        owner = nullptr;
    }
    ~UnkSet() {
        vtable = &g_unk_7101a20950[0x10];
        if (UnkOwnerHandle* p = owner) {
            if (--p->ref_count == 0)
                p->Delete();
        }
    }
    void Destroy() { (*reinterpret_cast<void (**)(UnkSet*)>(vtable))(this); }
};
static_assert(sizeof(UnkSet) == 0x40, "UnkSet layout");


void fn_71002458a4(UnkSet* self, UnkSet* other);
u32 fn_7100faf7d0(u32 kind);
struct UnkSetVector;
UnkSetVector* fn_71011a3234(UnkGroupTable* tables, u32 kind, const CStrId* id);
UnkSetVector* fn_71011a3220(UnkGroupTable* tables, u32 kind, const CStrId* id);
UnkOwnerHandle* fn_7100232f18(void* member);
void fn_710025bda0(void* self);

struct UnkSetVector {
    UnkSet* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    void EmplaceBack(UnkSet* value) {
        u32 n = count;
        u32 old_capacity = capacity;
        u32 new_capacity = old_capacity;
        if (old_capacity <= n && step != 0)
            new_capacity = (n / step + 1) * step;
        if ((s32)n >= (s32)new_capacity)
            for (;;) {
            }
        UnkSet* p;
        if (old_capacity < new_capacity) {
            p = static_cast<UnkSet*>(allocator->vf_0x40((u64)new_capacity * sizeof(UnkSet), 8, flags));
            if (data) {
                UnkSet* src = data;
                UnkSet* dst = p;
                for (u32 i = 0; i < count; ++i, ++dst, ++src) {
                    fn_71002458a4(dst, src);
                    src->Destroy();
                }
                allocator->vf_0x48(data);
            }
            n = count;
            data = p;
            capacity = new_capacity;
        } else {
            p = data;
        }
        fn_71002458a4(&p[n], value);
        ++count;
    }
};


struct UnkOrphanNode {
    CStrId key;
    base::UnkVector<void*> members;
    UnkOrphanNode* next;
    UnkOrphanNode* prev;
    UnkOrphanNode* chain;

    explicit UnkOrphanNode(const CStrId& id) : key(id), members(0xf), next(nullptr), prev(nullptr), chain(nullptr) {}

    static void* operator new(unsigned long size, base::UnkAllocator* allocator, u32 flags) noexcept {
        return allocator->vf_0x40(size, 8, flags);
    }
    void Destroy() {
        members.Free();
        key.Reset();
    }
};
static_assert(sizeof(UnkOrphanNode) == 0x48, "UnkOrphanNode layout");

struct UnkOrphanMap;
struct UnkOrphanIter;


void fn_7100eecb08(UnkOrphanMap* map, const CStrId* key);
void fn_7100ef1abc(UnkOrphanIter* it);


struct UnkOrphanMap {
    base::UnkAllocator* allocator = fn_7100083990();
    u32 flags = 0xe;
    u32 count = 0;
    base::UnkVector<UnkOrphanNode*> buckets{0xe};
    UnkOrphanNode* tail = nullptr;
    UnkOrphanNode* head = nullptr;

    UnkOrphanMap() {
        u32 n = buckets.count;
        if (n < 11) {
            buckets.Reserve(11);
            __builtin_memset(&buckets.data[n], 0, (u64)(11 - n) * sizeof(UnkOrphanNode*));
            buckets.count = 11;
        } else if (n > 11) {
            buckets.count = 11;
        }
    }

    UnkOrphanNode* Add(const CStrId& key) {
        const UnkStrHashWords* words = reinterpret_cast<const UnkStrHashWords*>(key.data);
        UnkOrphanNode** bucket =
            buckets.count ? &buckets.data[(words->field_0x34 ^ words->field_0x30) % buckets.capacity] : nullptr;
        UnkOrphanNode* chain = *bucket;
        UnkOrphanNode* node = new (allocator, flags) UnkOrphanNode(key);
        node->chain = chain;
        *bucket = node;
        if (!head) {
            head = node;
        } else if (tail) {
            node->prev = tail;
            tail->next = node;
        }
        tail = node;
        ++count;
        return node;
    }

    ~UnkOrphanMap() {
        if (count != 0) {
            for (UnkOrphanNode* node = head; node;) {
                UnkOrphanNode* next = node->next;
                node->Destroy();
                allocator->vf_0x48(node);
                node = next;
            }
            __builtin_memset(buckets.data, 0, (u64)buckets.capacity * sizeof(UnkOrphanNode*));
            count = 0;
            tail = nullptr;
            head = nullptr;
        }
        buckets.Free();
    }
};

// The cached entry is empty (0), a copy (1) or refers to the node's own fields (2).
struct UnkOrphanIter {
    struct Value {
        CStrId key;
        base::UnkVector<void*> members;
    };
    struct Ref {
        CStrId* key;
        base::UnkVector<void*>* members;
    };
    UnkOrphanNode* node;
    alignas(8) u8 storage[0x30];
    u32 state;

    explicit UnkOrphanIter(UnkOrphanNode* n) : node(n), state(0) { fn_7100ef1abc(this); }
    void Next() {
        node = node->next;
        fn_7100ef1abc(this);
    }
    const base::UnkVector<void*>& Members() {
        return state == 2 ? *reinterpret_cast<Ref*>(storage)->members : reinterpret_cast<Value*>(storage)->members;
    }
    ~UnkOrphanIter() {
        if (state == 2) {
        } else if (state == 1) {
            Value* value = reinterpret_cast<Value*>(storage);
            value->members.Free();
            value->key.Reset();
        } else {
            return;
        }
        state = 0;
    }
};
static_assert(sizeof(UnkOrphanIter) == 0x40, "UnkOrphanIter layout");

inline bool AtEnd(const UnkOrphanIter& it) {
    UnkOrphanIter end(nullptr);
    return it.node == end.node;
}


void fn_7100eebbd4(UnkSubAreaManager* self) {
    for (const u32* kind_ptr = g_unk_710164bf1c; kind_ptr != g_unk_710164bf1c + 3; ++kind_ptr) {
        u32 kind = *kind_ptr;
        UnkGroupTable* tables = reinterpret_cast<UnkGroupTable*>(g_unk_7101d47310->field_0x608);
        UnkOrphanMap map;

        UnkGroupTable* table = &tables[kind];
        for (UnkGroup *group = table->groups, *group_end = group + table->group_count; group != group_end; ++group) {
            UnkOrphanNode* node = map.Add(group->id);
            node->members.Reserve(group->field_0x1c);
            for (UnkMemberLink* link = group->field_0x50; link; link = link->next) {
                void* member = link->member;
                node->members.PushBack(member);
            }

            for (UnkSubArea *area = self->areas, *area_end = area + self->area_count; area != area_end; ++area) {
                for (UnkSubAreaItem **item = area->items, **item_end = item + area->item_count; item != item_end;
                     ++item) {
                    UnkSubAreaItem* object = *item;
                    CStrId id(object->ids[fn_7100faf7d0(kind)]);
                    UnkSetVector* sets = fn_71011a3234(tables, kind, &id);
                    if (sets) {
                        for (UnkSet *set = sets->data, *set_end = set + sets->count; set != set_end; ++set) {
                            if (set->field_0x30 == 1) {
                                {
                                    UnkRefPtr ref = fn_710023b580(&set->callbacks);
                                    fn_7100234f78(reinterpret_cast<base::UnkRefHandle*>(&set->owner),
                                                  reinterpret_cast<const base::UnkRefHandle*>(&ref));
                                }
                                set->field_0x30 = 2;
                            }
                            if (set->owner) {
                                if (void* target = set->owner->target)
                                    node->members.Remove(target);
                            }
                        }
                    }
                }
            }

            if (node->members.count == 0)
                fn_7100eecb08(&map, &group->id);
        }

        if (map.count != 0) {
            UnkSetVector* orphans;
            switch (kind) {
            case 0: {
                CStrId id("eg_ORPHANS_Default", true);
                orphans = fn_71011a3220(tables, 0, &id);
                break;
            }
            case 1: {
                CStrId id("sg_ORPHANS_Default", true);
                orphans = fn_71011a3220(tables, 1, &id);
                break;
            }
            case 2: {
                CStrId id("lg_ORPHANS_Default", true);
                orphans = fn_71011a3220(tables, 2, &id);
                break;
            }
            default:
                orphans = nullptr;
                break;
            }

            {
                UnkOrphanIter it(map.head);
                while (!AtEnd(it)) {
                    const base::UnkVector<void*>& members = it.Members();
                    for (void **member = members.data, **member_end = member + members.count; member != member_end;
                         ++member) {
                        void* object = *member;
                        UnkSet set;
                        {
                            UnkOwnerRef owner(fn_7100232f18(object));
                            fn_7100234f78(reinterpret_cast<base::UnkRefHandle*>(&set.owner),
                                          reinterpret_cast<const base::UnkRefHandle*>(&owner));
                            set.field_0x30 = (owner.ptr && owner.ptr->target) ? 2 : 0;
                        }
                        orphans->EmplaceBack(&set);
                    }
                    it.Next();
                }
            }
            fn_710025bda0(table->field_0x20);
        }
    }
}
