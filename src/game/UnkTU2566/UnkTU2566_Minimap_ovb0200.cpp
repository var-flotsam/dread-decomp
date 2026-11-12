// CMinimapManager::CTunableMinimapManager's unit (tools/tu_map.py; the unit has no reliable name): the keyed array
// of ids and words (erase, insert, grow-to-index and copy assignment), the same for the string-id and word-pair
// records, the input queue's per-frame update, the hashed set's find-or-insert, the blackboard flag setters, a
// tolerance comparison of two tagged records, and the map menu toggle. Names and layouts are placeholders.
#include <new>

#include "base/UnkAtExit.h"
#include "base/UnkCloneRecords.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "engine/scene/UnkToggle.h"
#include "game/CAutsharpAIComponent.h"
#include "game/CBlackboard.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkMapMenuController.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU1689.h"
#include "game/UnkTU2547.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2566.h"
#include "game/UnkTU39.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using namespace base::global;

struct UnkIdValuesArray;

// A string id that starts out as the empty id.
struct UnkSlotId_b0200 {
    UnkStringData* data;

    UnkSlotId_b0200() {
        data = nullptr;
        if (!g_unk_7101d47108)
            UnkEnsureStringPool();
        data = &g_unk_7101d47108->empty;
    }
};

// A string id and a value; a new one is zeroed, then built.
struct UnkIdSlot_b0200 {
    UnkSlotId_b0200 id;
    u64 value = 0;
};

// The entry at `index`; when growing is allowed (field_0x14), an index past the end grows the array
// to the step's next multiple and then to index + 1 entries, value-initialising the new ones.
UnkIdSlot_b0200* fn_7100eb0058(void* array, u32 index) {
    auto* self = static_cast<base::UnkVector<UnkIdSlot_b0200>*>(array);
    if (self->field_0x14 && self->count <= index) {
        u32 cap = self->capacity;
        if (cap <= index && self->step != 0)
            cap = (index / self->step + 1) * self->step;
        fn_7100b48af4(reinterpret_cast<UnkIdValuesArray*>(self), cap);
        u32 n = index + 1;
        fn_7100b48af4(reinterpret_cast<UnkIdValuesArray*>(self), n);
        for (u32 i = self->count; i < n; ++i)
            new (&self->data[i]) UnkIdSlot_b0200();
        self->count = n;
    }
    return &self->data[index];
}

using base::global::CFilePathStrId;
using base::global::CStrId;

extern "C" int __cxa_guard_acquire(u64* guard);
extern "C" void __cxa_guard_release(u64* guard);

struct UnkMapData;
void fn_7100e9c43c(UnkMapData* map, const CStrId* id);
u32 fn_710119882c(void* manager, const CFilePathStrId* path);
extern void* g_unk_7101d47330;

// A cutscene path and the guard of its one-time construction.
struct UnkPathSlot_b0200 {
    u8 path[8];
    u64 guard;
};
extern UnkPathSlot_b0200 g_unk_71729cdb28 __attribute__((visibility("hidden")));

struct UnkMapEvent_b0200 {
    u8 pad_0x0[8];
    f32 value;   // 0x8
    bool valid;  // 0xc
};

// A string id and two words; moved with the id taken bitwise and the old one left empty, and released in
// two steps.
struct UnkIdWords_b0200 {
    UnkStringData* data;
    u32 field_0x8;
    u32 field_0xc;

    UnkIdWords_b0200(UnkIdWords_b0200&& other) {
        data = other.data;
        UnkStringData* empty = &g_unk_7101d47108->empty;
        __builtin_memcpy(&other.data, &empty, sizeof(empty));
        field_0x8 = other.field_0x8;
        field_0xc = other.field_0xc;
    }
    ~UnkIdWords_b0200() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

// Destroys the element at `index` and moves the later ones down.
void fn_7100eb408c(base::UnkVector<UnkIdWords_b0200>* self, u32 index) {
    if (index < self->count) {
        self->data[index].~UnkIdWords_b0200();
        u32 i = index;
        for (; i < (u32)(self->count - 1); ++i) {
            UnkIdWords_b0200* cur = &self->data[i];
            new (cur) UnkIdWords_b0200(static_cast<UnkIdWords_b0200&&>(cur[1]));
            (&self->data[i])[1].~UnkIdWords_b0200();
        }
        self->count = self->count - 1;
    }
}

struct UnkPlayer;

struct UnkInputEvent_b0200 {
    s32 type;   // 0x0
    u32 value;  // 0x4
    u8 pad_0x8[0x14];
};

struct UnkPlayerMode_b0200 {
    u8 pad_0x0[0x10];
    s32 mode;  // 0x10
};

struct UnkPlayerSub_b0200 {
    u8 pad_0x0[0x50];
    UnkPlayerMode_b0200* mode;  // 0x50
};

struct UnkPlayerView_b0200 {
    u8 pad_0x0[0x2b0];
    UnkPlayerSub_b0200* sub;  // 0x2b0
};

struct __attribute__((packed)) UnkInputQueue_b0200 {
    u32 count;                // 0x0
    UnkInputEvent_b0200* events;  // 0x4
    u8 pad_0xc[0x28c];
    bool enabled;  // 0x298
    u8 pad_0x299;
    bool toggled;  // 0x29a
};

struct UnkMinimapInput_b0200 {
    u8 pad_0x0[0xd63f2c];
    UnkInputQueue_b0200 queue;  // 0xd63f2c
};

void fn_7100e94254(UnkMinimapInput_b0200* self) {
    UnkInputQueue_b0200* queue = &self->queue;
    if (queue->count) {
        u64 i = 0;
        bool held = false;
        for (; i < queue->count; ++i) {
            const UnkInputEvent_b0200& e = queue->events[i];
            switch (e.type) {
            case 1:
            case 2:
                held = true;
                break;
            case 0x20:
            case 0x80:
            case 0x8000:
                held = e.value == 1;
                break;
            case 0x40:
                if (UnkPlayerView_b0200* player = reinterpret_cast<UnkPlayerView_b0200*>(fn_7100dec788(g_unk_7101d47310))) {
                    if (UnkPlayerSub_b0200* sub = player->sub)
                        held = sub->mode && sub->mode->mode == 2;
                }
                break;
            case 0x400000:
                if (queue->enabled && e.value == 1)
                    queue->toggled ^= 1;
                break;
            }
        }
        bool toggled = queue->toggled;
        queue->count = 0;
        if (toggled && held)
            queue->toggled = false;
    } else {
        queue->count = 0;
    }
}

using base::global::CStrId;

struct UnkKeyObject_b0200 {
    u8 pad_0x0[0x30];
    u32 hash_a;
    u32 hash_b;
};

struct UnkKeyRecord_b0200 {
    UnkKeyObject_b0200* key;
    u8 value[0x15f0];  // 0x8
    UnkKeyRecord_b0200* next;  // 0x15f8
};

struct UnkKeyBuckets_b0200 {
    UnkKeyRecord_b0200** data;
    u32 count;
    u32 capacity;
};

struct UnkKeyIter_b0200 {
    UnkKeyRecord_b0200* node;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
    UnkKeyIter_b0200(const UnkKeyIter_b0200& o)
        : node(o.node), field_0x8(o.field_0x8), field_0x10(o.field_0x10), field_0x18(o.field_0x18) {}
};

struct UnkKeyOwner_b0200 {
    u8 pad_0x0[0xd64210];
    UnkKeyBuckets_b0200 records;  // 0xd64210
    u8 pad_0xd64220[0xd64390 - 0xd64220];
    base::UnkStrIdVec keys;  // 0xd64390
};

// As src/game/CMinimapManager/fn_7100ea5c40.cpp declares it.
struct UnkRecord;
struct UnkRecordBuckets;
UnkKeyIter_b0200 fn_7100eb39f0(UnkKeyBuckets_b0200* buckets, const CStrId* key);
void fn_7100e94608(UnkKeyOwner_b0200* self, u32 unused, void* arg, void* value);

void* fn_7100e944bc(UnkKeyOwner_b0200* self, const CStrId* key, void* arg) {
    u32 unused;
    UnkKeyBuckets_b0200* buckets = &self->records;
    UnkKeyRecord_b0200* node = nullptr;
    if (buckets->count) {
        UnkKeyObject_b0200* object = reinterpret_cast<UnkKeyObject_b0200*>(key->data);
        u32 hash = object->hash_a ^ object->hash_b;
        u32 i = hash % buckets->capacity;
        if (i < buckets->count) {
            node = *reinterpret_cast<UnkKeyRecord_b0200**>(
                fn_7100eb3814(reinterpret_cast<UnkRecordBuckets*>(buckets), i));
            for (; node; node = node->next)
                if (node->key == reinterpret_cast<UnkKeyObject_b0200*>(key->data))
                    break;
        }
    }
    if (!node) {
        node = fn_7100eb39f0(buckets, key).node;
        self->keys.PushBack(*key);
        fn_7100e94608(self, unused, arg, node->value);
    }
    return node->value;
}

struct UnkSubId_7100fc4b34 {
    CStrId id;
    u64 field_0x8;
};
using UnkQualifiedName_7100fc4b34 = UnkQualifiedName;
const UnkSubId_7100fc4b34& fn_71003327a4(UnkQualifiedName_7100fc4b34* self, const CStrId* name);

struct UnkTypedValue {
    const base::reflection::CType* type;
    void* value;
};
void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);

struct UnkSetFlagDef_b0200 : UnkIdSource {
    u64 field_0x8;
    UnkQualifiedName name;
};
extern UnkSetFlagDef_b0200 g_unk_71727c9968;

static inline UnkSubId_7100fc4b34 SubId(UnkSetFlagDef_b0200& def, const CStrId& name) {
    if (!def.name.str || !def.name.str[0])
        fn_7100ae9d7c(&def.name, &def, g_unk_7101d470e0);
    return fn_71003327a4(&def.name, &name);
}

void fn_7100e9cbac(void* world, const CStrId* current, CStrId* id, bool flag) {
    UnkTypedValue v;
    UnkGameManager* manager = g_unk_7101d47310;
    bool value = flag;
    CBlackboard* blackboard = &manager->field_0x968;
    CStrId section_id = fn_7100dfbda4(manager, *current);
    UnkSubId_7100fc4b34 key = SubId(g_unk_71727c9968, *id);
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section_id)) {
        if (!blackboard->field_0xe8) {
            v.type = fn_710000d228();
            v.value = &value;
            fn_7100332c80(s, (UnkPropertyName*)&key.id, &v);
        }
    }
}

CStrId fn_7100de9c30(UnkGameManager*);

struct UnkNamedId_b0200 : UnkIdSource {
    u64 field_0x8;
    UnkQualifiedId qid;
};
extern UnkNamedId_b0200 g_unk_71727ca210;

struct UnkXformMatrix_b0200 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
};

struct UnkXformHolder_b0200 {
    u8 pad_0x0[0x10];
    UnkXformMatrix_b0200* matrix;  // 0x10
};

struct UnkUsableInfo_b0200 {
    u8 pad_0x0[0x30];
};

struct UnkUsable_b0200 : base::reflection::UnkReflObject {
    u8 pad_0x8[0x10];
    UnkUsableInfo_b0200* info;  // 0x18
    u8 pad_0x20[0xc0];
    UnkXformHolder_b0200* xform;  // 0xe0
};

struct UnkSelf_b0200;
void fn_7100e9af80(UnkSelf_b0200* self, UnkUsableInfo_b0200* info);

u32 fn_7100e9be04(UnkSelf_b0200* self, base::reflection::UnkReflObject* object) {
    if (object && fn_710009d50c(object->vf_0x0(), reinterpret_cast<const base::reflection::CType*>(fn_7100a9bcec()))) {
        UnkUsable_b0200* usable = static_cast<UnkUsable_b0200*>(object);
        UnkTypedValue v;
        UnkGameManager* manager = g_unk_7101d47310;
        CBlackboard* blackboard = &manager->field_0x968;
        CStrId section_id = fn_7100de9c30(manager);
        if (g_unk_71727ca210.qid.id.data == &g_unk_7101d47108->empty)
            fn_7100ae9a10(&g_unk_71727ca210.qid, &g_unk_71727ca210, g_unk_7101d470e0);
        UnkXformMatrix_b0200* m = usable->xform->matrix;
        f32 x = m->x;
        f32 y = m->y;
        f32 z = m->z;
        base::math::CVector3D position;
        position.x = x;
        position.y = y;
        position.z = z;
        if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section_id)) {
            if (!blackboard->field_0xe8) {
                v.type = fn_71000246e4();
                v.value = &position;
                fn_7100332c80(s, (UnkPropertyName*)&g_unk_71727ca210.qid.id, &v);
            }
        }
        fn_7100e9af80(self, (UnkUsableInfo_b0200*)((u8*)usable->info + 0x30));
    }
    return 0;
}

struct UnkIdValuesArray;

struct UnkIdValues_b0200 {
    base::global::UnkStringData* id;
    u32 field_0x8;
    u32 field_0xc;

    // The id is copied bitwise and retained; the values are stored through plain pointers (the original
    // reloads the array's count after them).
    UnkIdValues_b0200(const UnkIdValues_b0200& other) {
        base::global::UnkStringData* d = other.id;
        __builtin_memcpy(&id, &d, sizeof(d));
        if (&g_unk_7101d47108->empty != d)
            ++d->ref_count;
        u32* values = &field_0x8;
        values[0] = other.field_0x8;
        values[1] = other.field_0xc;
    }
    ~UnkIdValues_b0200() {
        if (base::global::UnkStringPool* pool = g_unk_7101d47108) {
            if (id && id != &pool->empty) {
                --id->ref_count;
                if (id->ref_count == 0) {
                    fn_71000803a4(pool, id);
                    pool = g_unk_7101d47108;
                }
            }
            id = &pool->empty;
        }
    }
};

using UnkIdValuesVector_b0200 = base::UnkVector<UnkIdValues_b0200>;

static inline void Reserve(UnkIdValuesVector_b0200* self, u32 capacity) {
    fn_7100b48af4(reinterpret_cast<UnkIdValuesArray*>(self), capacity);
}

// Copy assignment: the elements destroyed, then the other's appended.
UnkIdValuesVector_b0200* fn_7100eafb70(UnkIdValuesVector_b0200* self, const UnkIdValuesVector_b0200* other) {
    if (self != other) {
        for (u64 i = 0; i < self->count; ++i)
            self->data[i].~UnkIdValues_b0200();
        self->count = 0;
        Reserve(self, other->capacity);
        for (const UnkIdValues_b0200 *it = other->data, *end = it + other->count; it != end; ++it) {
            u32 n = self->count;
            u32 new_capacity = self->capacity;
            if (new_capacity <= n && self->step != 0)
                new_capacity = (n / self->step + 1) * self->step;
            if ((s32)n >= (s32)new_capacity)
                for (;;) {
                }
            Reserve(self, new_capacity);
            new (&self->data[self->count]) UnkIdValues_b0200(*it);
            ++self->count;
        }
    }
    return self;
}

struct UnkDoorRegistry_b61;

struct UnkNamedDef_b0200c : UnkIdSource {
    u64 field_0x8;
    UnkQualifiedName name;
};
// As src/game/CMinimapManager/CMinimapManager_ReadFlag_b43.cpp declares it.
struct UnkNamedDef_b43a3;
extern CStrId g_unk_71727cc990;

static inline UnkSubId_7100fc4b34 SubId(UnkNamedDef_b0200c& def, const CStrId& name) {
    if (!def.name.str || !def.name.str[0])
        fn_7100ae9d7c(&def.name, &def, g_unk_7101d470e0);
    return fn_71003327a4(&def.name, &name);
}

struct UnkMinimapTunable_b0200 {
    u8 pad_0x0[0x110];
    CStrId name;  // 0x110
    u8 pad_0x118[0xd63d6a - 0x118];
    bool flag;  // 0xd63d6a
};

void fn_7100e9128c(UnkMinimapTunable_b0200* self) {
    UnkTypedValue v;
    if (self->flag)
        return;
    UnkDoorRegistry_b61* reg = g_unk_71726bee10;
    if (!reg) {
        reg = static_cast<UnkDoorRegistry_b61*>(fn_71000839a0(4, 0x3f));
        fn_7100ddb6dc(reg);
        g_unk_71726bee10 = reg;
    }
    if (!fn_7100ddb938(reg))
        return;
    self->flag = true;
    UnkGameManager* manager = g_unk_7101d47310;
    CBlackboard* blackboard = &manager->field_0x968;
    UnkSubId_7100fc4b34 key = SubId(reinterpret_cast<UnkNamedDef_b0200c&>(g_unk_71727c9f88), self->name);
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, g_unk_71727cc990)) {
        if (!blackboard->field_0xe8) {
            v.type = fn_710000d228();
            v.value = &self->flag;
            fn_7100332c80(s, (UnkPropertyName*)&key.id, &v);
        }
    }
}

// A string id and two words; moved with the id taken bitwise and the old one left empty, released in
// two steps.
struct UnkIdWords_b0200c {
    UnkStringData* data;
    u32 field_0x8;
    u32 field_0xc;

    UnkIdWords_b0200c(UnkIdWords_b0200c&& other) {
        data = other.data;
        UnkStringData* empty = &g_unk_7101d47108->empty;
        __builtin_memcpy(&other.data, &empty, sizeof(empty));
        field_0x8 = other.field_0x8;
        field_0xc = other.field_0xc;
    }
    ~UnkIdWords_b0200c() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

static inline void Grow(base::UnkVector<UnkIdWords_b0200c>* self, u32 capacity) {
    fn_7100b48af4(reinterpret_cast<UnkIdValuesArray*>(self), capacity);
}

// Puts `value` in at `index`, shifting the entries after it up; appending hangs when the array cannot grow.
void fn_7100eafeb8(base::UnkVector<UnkIdWords_b0200c>* self, UnkIdWords_b0200c* value, u32 index) {
    u32 cap = self->capacity;
    u32 n = self->count;
    if (n == index) {
        if (cap <= n && self->step != 0)
            cap = (index / self->step + 1) * self->step;
        if ((s32)cap <= (s32)index)
            for (;;) {
            }
        Grow(self, cap);
        new (&self->data[self->count]) UnkIdWords_b0200c(static_cast<UnkIdWords_b0200c&&>(*value));
    } else {
        if (cap <= n && self->step != 0)
            cap = (n / self->step + 1) * self->step;
        Grow(self, cap);
        for (u32 i = self->count - 1; i >= index; --i) {
            UnkIdWords_b0200c* q = &self->data[i];
            new (q + 1) UnkIdWords_b0200c(static_cast<UnkIdWords_b0200c&&>(*q));
            self->data[i].~UnkIdWords_b0200c();
            if (i == 0)
                break;
        }
        new (&self->data[index]) UnkIdWords_b0200c(static_cast<UnkIdWords_b0200c&&>(*value));
    }
    ++self->count;
}

struct UnkMinimapOwner;
void fn_7100e8f6bc(UnkMinimapOwner* self);
struct UnkMinimapSetter_b0200;
void fn_7100e8fb5c(UnkMinimapSetter_b0200* self, bool a, bool b);

struct UnkMapMenuRef_b0200 {
    UnkMapMenuHolder* target;  // 0x0
    u8 pad_0x8[8];
    u8* toggle_owner;  // 0x10
};

struct UnkMinimapSetter_b0200 {
    u8 pad_0x0[0x108];
    u32 mode;  // 0x108
    u8 pad_0x10c[0xd63d90 - 0x10c];
    u8 timer_0xd63d90[0xb0];
    u8 timer_0xd63e40[0x100];
    UnkMapMenuRef_b0200 menu;  // 0xd63f40
};

void fn_7100e8f9c0(UnkMinimapSetter_b0200* self, bool flag) {
    UnkMapMenuRef_b0200* menu = &self->menu;
    if (!fn_7100deabe4(g_unk_7101d47310) && fn_7100deabc4(g_unk_7101d47310)) {
        self->mode = fn_710002fb70(self->timer_0xd63e40) ? 2 : 1;
        bool on = flag;
        fn_7100219d4c(g_unk_7101d47300, on, 6);
        fn_7100219cb0(g_unk_7101d47300, on, 6);
        bool timer_running = fn_710002fb70(self->timer_0xd63e40);
        u8* owner = menu->toggle_owner;
        bool visible;
        if (timer_running) {
            if (UnkMapMenuHolder* holder = menu->target) {
                if (holder->field_0x10)
                    visible = !fn_7100b41e5c(reinterpret_cast<UnkMapMenuController*>(holder->field_0x10));
                else
                    visible = true;
            } else {
                visible = true;
            }
        } else {
            visible = !fn_710002fb70(self->timer_0xd63d90) & flag;
        }
        fn_710020ac34(reinterpret_cast<UnkToggle*>(owner + 0x18), visible);
        fn_7100e8fb5c(self, false, true);
        fn_7100e8f6bc(reinterpret_cast<UnkMinimapOwner*>(self));
    } else {
        self->mode = 0;
        f32 params[5] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
        fn_7100219294(g_unk_7101d47300, 0x12194b8295e98a84, params, 0);
        fn_7100219d4c(g_unk_7101d47300, false, 6);
        fn_7100219cb0(g_unk_7101d47300, false, 6);
        fn_710020ac34(reinterpret_cast<UnkToggle*>(menu->toggle_owner + 0x18), false);
    }
}

struct UnkEntry50_b0200b {
    u8 data[0x50];
};
void fn_7100ea9eb8(UnkEntry50_b0200b* entry);

struct UnkGroup90_b0200 {
    UnkEntry50_b0200b* entries;  // 0x0
    void* keys;                  // 0x8
    u32 count;                   // 0x10
    u32 capacity;                // 0x14
    u32 step;                    // 0x18
    u32 flags;                   // 0x1c
    base::UnkAllocator* allocator;  // 0x20
    u8 pad_0x28[0x10];
    UnkEntry50_b0200b tail;  // 0x38
};
static_assert(sizeof(UnkGroup90_b0200) == 0x88, "layout");

struct UnkGroupSlot_b0200 {
    union {
        UnkGroup90_b0200 group;
        u8 raw[0x90];
    };
};

struct UnkGroupTable_b0200 {
    UnkGroupSlot_b0200* groups;  // 0x0
    CStrId* ids;                 // 0x8
    u32 count;                   // 0x10
};

void fn_7100eb3648(UnkGroupTable_b0200* self) {
    if (self->ids && self->count)
        for (u64 i = 0; i < self->count; ++i)
            self->ids[i].Reset();
    UnkGroupSlot_b0200* groups = self->groups;
    if (groups && self->count) {
        for (u64 i = 0; i < self->count; ++i) {
            UnkGroup90_b0200* g = &groups[i].group;
            fn_7100ea9eb8(&g->tail);
            if (g->entries && g->count) {
                fn_7100ea9eb8(&g->entries[0]);
                for (u64 j = 1; j < g->count; ++j)
                    fn_7100ea9eb8(&g->entries[j]);
            }
            g->count = 0;
            if (g->keys)
                g->allocator->vf_0x48(g->keys);
            if (g->entries)
                g->allocator->vf_0x48(g->entries);
            if (i + 1 < self->count)
                groups = self->groups;
        }
    }
    self->count = 0;
}
