// CMultiLockOnBlockComponent's unit (tools/tu_map.py unit 1339): for each linked block, numbers it and
// files its "try_to_activate_point_<n>" and "relax_point_<n>" keys in the component's map by the linked
// object. Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/timeline/CEvent.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/UnkCallbackBase.h"
#include "game/UnkTU341.h"
#include "game/UnkTU437.h"
#include "game/UnkTU57.h"
#include "game/UnkTUInit.h"
#include "types.h"

using base::global::CRntString;

struct UnkCallbackSet : UnkCallbackBase {
    virtual void vf8();
    UnkRefCounted* owner;

    UnkCallbackSet(const UnkCallbackSet& other) : owner(nullptr) {
        UnkRefCounted* p = other.owner;
        if (p != owner) {
            if (p)
                ++p->ref_count;
            if (UnkRefCounted* old = owner) {
                if (--old->ref_count == 0)
                    old->Delete();
            }
            owner = p;
        }
        field_0x30 = other.field_0x30;
        fn_710023b29c(&callbacks, &other.callbacks);
    }

    UnkCallbackSet(UnkCallbackSet&& other);  // fn_71007b1928

    __attribute__((always_inline)) ~UnkCallbackSet() {
        if (UnkRefCounted* p = owner) {
            if (--p->ref_count == 0)
                p->Delete();
        }
    }
};

// The unit's data (its statics, merged into one block).
struct UnkMultiLockOnBlockTU {
    UnkTUHeader header;
    u64 field_0x18;
    u64 field_0x20;
    CStrId try_to_activate_point;  // 0x28
    CStrId relax_point;            // 0x30
    UnkReflType tunable_type;      // 0x38
    u8 field_0x38_rest[0x170 - 0x38 - sizeof(UnkReflType)];
    UnkReflType type;              // 0x170
};

extern UnkMultiLockOnBlockTU g_unk_717272b730 __attribute__((visibility("hidden")));

struct UnkMissionLogName_ovb0079;
void fn_710007f688(UnkMissionLogName_ovb0079* self, u32 value);

// A string built from a string id out of line (fn_710007eb20) and destroyed as one.
struct UnkLockOnIdText_l882cdc {
    union {
        CRntString str;
    };

    explicit UnkLockOnIdText_l882cdc(const CStrId* id) {
        CRntString_ConstructFromStrId(&str, reinterpret_cast<const UnkPooledStrId*>(id));
    }
    ~UnkLockOnIdText_l882cdc() { CRntString_Destroy(&str); }
};

// The resolved link's weak handle: its target at +0x10.
struct UnkLockOnLinkHandle_l882cdc {
    u8 pad_0x0[0x10];
    void* target;  // 0x10
};

struct UnkLockOnPoints_l882cdc {
    bool field_0x0;
    u32 field_0x4;
    u64 activate_point;  // 0x8
    u64 relax_point;     // 0x10
    bool field_0x18;
};

struct UnkLockOnPointNode_l882cdc {
    void* key;
    UnkLockOnPoints_l882cdc value;       // 0x8
    UnkLockOnPointNode_l882cdc* next;    // 0x28
    UnkLockOnPointNode_l882cdc* prev;    // 0x30
    UnkLockOnPointNode_l882cdc* chain;   // 0x38

    UnkLockOnPointNode_l882cdc(void* key, const UnkLockOnPoints_l882cdc& value)
        : key(key), value(value), next(nullptr), prev(nullptr), chain(nullptr) {}
};

struct UnkLockOnPointMap_l882cdc {
    base::UnkAllocator* allocator;
    u32 flags;
    u32 count;
    base::global::CRntVector<UnkLockOnPointNode_l882cdc*> buckets;
    UnkLockOnPointNode_l882cdc* last;   // 0x38
    UnkLockOnPointNode_l882cdc* first;  // 0x40

    void Insert(void* key, const UnkLockOnPoints_l882cdc& value) {
        UnkLockOnPointNode_l882cdc** bucket =
            buckets.count ? &buckets.data[(u32)(u64)key % buckets.capacity] : nullptr;
        UnkLockOnPointNode_l882cdc* chained = *bucket;
        void* p = allocator->vf_0x40(sizeof(UnkLockOnPointNode_l882cdc), 8, flags);
        UnkLockOnPointNode_l882cdc* node = static_cast<UnkLockOnPointNode_l882cdc*>(p);
        if (node)
            new (node) UnkLockOnPointNode_l882cdc(key, value);
        node->chain = chained;
        *bucket = node;
        if (!first) {
            first = node;
        } else if (last) {
            node->prev = last;
            last->next = node;
        }
        last = node;
        ++count;
    }
};

struct UnkMultiLockOnBlock_l882cdc {
    u8 pad_0x0[0x40];
    u32 point_count;  // 0x40
    u8 pad_0x44[0xa8 - 0x44];
    base::global::CRntVector<UnkCallbackSet> links;  // 0xa8
    UnkLockOnPointMap_l882cdc points;  // 0xd0
};
static_assert(__builtin_offsetof(UnkMultiLockOnBlock_l882cdc, points) == 0xd0, "points");

// The string's key (its CRC-64).
inline u64 KeyOf_l882cdc(const CRntString& str) {
    u64 key;
    fn_7100066630(&key, &str);
    return key;
}

// Resolves the link once (while its state is 1) and returns its target, or null.
inline void* LockOnLinkTarget_l882cdc(UnkCallbackSet& link) {
    if (link.field_0x30 == 1) {
        {
            base::UnkRefPtrMember ref = fn_7100353be8(&link.callbacks);
            fn_710031d208(reinterpret_cast<base::UnkRefHandle*>(&link.owner),
                          reinterpret_cast<const base::UnkRefHandle*>(&ref));
        }
        link.field_0x30 = 2;
    }
    UnkLockOnLinkHandle_l882cdc* handle = reinterpret_cast<UnkLockOnLinkHandle_l882cdc*>(link.owner);
    return handle ? handle->target : nullptr;
}

void fn_7100882cdc(UnkMultiLockOnBlock_l882cdc* self) {
    for (UnkCallbackSet* it = self->links.data, *end = it + self->links.count; it != end; ++it) {
        UnkCallbackSet link(*it);
        if (!LockOnLinkTarget_l882cdc(link))
            continue;
        ++self->point_count;
        UnkLockOnIdText_l882cdc name(&g_unk_717272b730.try_to_activate_point);
        fn_710007f688(reinterpret_cast<UnkMissionLogName_ovb0079*>(&name.str), self->point_count);
        u64 activate_point = KeyOf_l882cdc(name.str);
        {
            UnkLockOnIdText_l882cdc relax(&g_unk_717272b730.relax_point);
            CRntString_Assign(&name.str, &relax.str);
        }
        fn_710007f688(reinterpret_cast<UnkMissionLogName_ovb0079*>(&name.str), self->point_count);
        u64 relax_point = KeyOf_l882cdc(name.str);

        self->points.Insert(LockOnLinkTarget_l882cdc(link),
                            UnkLockOnPoints_l882cdc{false, 0, activate_point, relax_point, false});
    }
}
