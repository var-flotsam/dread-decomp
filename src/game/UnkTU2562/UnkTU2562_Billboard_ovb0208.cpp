// Dropped-item manager (CLogicDroppedItemManager's unit, 2562, tools/tu_map.py; its reflected class is
// CLogicDroppedItemManager::CTunableDroppableBillboard): the billboard tunable's constructor and
// destructors, the per-entry copy, the manager's frame update and the pickup effects. Names and layouts
// are placeholders.
#include "base/UnkVector.h"
#include "base/color/CColor4F.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/input/CRumbleManager/CTunableRumbleManager.h"
#include "base/math/UnkZeroVector3.h"
#include "base/snd/UnkSound.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkModelManager.h"
#include "game/UnkTU101.h"
#include "game/UnkTU1102.h"
#include "game/UnkTU2560.h"
#include "game/UnkTUProperty.h"
#include "game/UnkXformNode.h"
#include "game/sound/CSoundManager.h"
#include "types.h"

using base::global::CStrId;

// ---- The tunable ----

extern void* g_unk_7101aa9cc8[];
void fn_710007e6c4(void* self);

struct UnkDropTunable_ovb0208 : UnkTunable {
    u8 pad_0x8[0x90];
    base::global::CRntString n0, n1, n2, n3, n4, n5, n6, n7, n8;  // 0x98
    u8 pad_0x248[0x10];
};
static_assert(sizeof(UnkDropTunable_ovb0208) == 0x258, "size");

static inline void Destroy(UnkDropTunable_ovb0208* self) {
    self->vtable = g_unk_7101aa9cc8 + 2;
    fn_710007ecd4(&self->n8);
    fn_710007ecd4(&self->n7);
    fn_710007ecd4(&self->n6);
    fn_710007ecd4(&self->n5);
    fn_710007ecd4(&self->n4);
    fn_710007ecd4(&self->n3);
    fn_710007ecd4(&self->n2);
    fn_710007ecd4(&self->n1);
    fn_710007ecd4(&self->n0);
    fn_7100075104(self);
}

void fn_7100e20ba0(UnkDropTunable_ovb0208* self) { Destroy(self); }

void fn_7100e2f208(UnkDropTunable_ovb0208* self) {
    Destroy(self);
    __builtin_operator_delete(self);
}

void fn_7100e2f6e0(UnkDropTunable_ovb0208* self) {
    __builtin_memset(self, 0, sizeof(*self));
    fn_7100136ecc(self);
    self->vtable = g_unk_7101aa9cc8 + 2;
    fn_710007e6c4(&self->n0);
    fn_710007e6c4(&self->n1);
    fn_710007e6c4(&self->n2);
    fn_710007e6c4(&self->n3);
    fn_710007e6c4(&self->n4);
    fn_710007e6c4(&self->n5);
    fn_710007e6c4(&self->n6);
    fn_710007e6c4(&self->n7);
    fn_710007e6c4(&self->n8);
    reinterpret_cast<void (*)(UnkDropTunable_ovb0208*)>(self->vtable[3])(self);
}


// ---- The entry copy ----


struct UnkDropEntry_ovb0208 {
    CStrId id0;                  // 0x0
    CStrId id1;                  // 0x8
    base::color::CColor4F c0;    // 0x10
    base::color::CColor4F c1;    // 0x20
    u64 pair[2];                 // 0x30
    u8 flag;                     // 0x40
    u8 pad_0x41[7];

    __attribute__((always_inline)) UnkDropEntry_ovb0208* Assign(const UnkDropEntry_ovb0208& o) {
        id0 = o.id0;
        id1 = o.id1;
        fn_7100030728(&c0, &o.c0);
        fn_7100030728(&c1, &o.c1);
        u8 f = o.flag;
        u64 p0 = o.pair[0], p1 = o.pair[1];
        flag = f;
        pair[0] = p0;
        pair[1] = p1;
        return this;
    }
};
static_assert(sizeof(UnkDropEntry_ovb0208) == 0x48, "size");


__attribute__((noinline)) UnkDropEntry_ovb0208* fn_7100e2f28c(UnkDropEntry_ovb0208* self, const UnkDropEntry_ovb0208* other) {
    return self->Assign(*other);
}

struct UnkDropEntrySet_ovb0208 {
    u32 field_0x0;
    u32 pad_0x4;
    UnkDropEntry_ovb0208 entries[15];  // 0x8
};
static_assert(sizeof(UnkDropEntrySet_ovb0208) == 0x440, "size");

UnkDropEntrySet_ovb0208* fn_7100e2c3f4(UnkDropEntrySet_ovb0208* self, const UnkDropEntrySet_ovb0208* other) {
    self->field_0x0 = other->field_0x0;
    for (int i = 0; i < 15; ++i)
        fn_7100e2f28c(&self->entries[i], &other->entries[i]);
    return self;
}

// ---- The manager ----

struct UnkDropSlot_ovb0208 {
    u8 pad_0x0[0x7c];
    f32 x;  // 0x7c
    f32 y;  // 0x80
    f32 z;  // 0x84
    u8 pad_0x88[0x60];
    u64 handle;  // 0xe8
};
static_assert(sizeof(UnkDropSlot_ovb0208) == 0xf0, "slot size");

// An item's record (0x78 bytes): its offset position, a counter and a changed flag.
struct UnkDropItem_ovb0208 {
    f32 x, y, z;  // 0x0
    u32 count;    // 0xc
    u8 pad_0x10[0x18];
    u8 flag;  // 0x28
    u8 pad_0x29[0x4f];
};
static_assert(sizeof(UnkDropItem_ovb0208) == 0x78, "item size");

// The items, with the handle of the slot each belongs to.
struct UnkDropItemMap_ovb0208 {
    UnkDropItem_ovb0208* items;  // 0x0
    u64* handles;                // 0x8
    u32 count;                   // 0x10
    u32 capacity;                // 0x14
    u32 field_0x18;
    u32 field_0x1c;
    void* allocator;  // 0x20

    UnkDropItem_ovb0208* Find(u64 handle) {
        for (u32 i = 0; i < count; ++i)
            if (handles[i] == handle)
                return &items[i];
        return &items[count];
    }
};

void fn_7100f05fb4(UnkDropSlot_ovb0208* slot, u32 arg);
void fn_7100f05cc8(UnkDropSlot_ovb0208* slot);
void fn_7100f05c18(UnkDropSlot_ovb0208* slot, void* actor, bool flag);

// Each 0xf0-byte element starts the slot 8 bytes in; its active flag is at +0x7d.
struct UnkDropElem_ovb0208 {
    u8 bytes[0xf0];
};

struct UnkSoundRef {
    UnkSound* object;
    s64 serial;
};
struct UnkSoundHandle {
    UnkSoundRef* ref;
    s64 serial;
};
extern const UnkSoundHandle g_unk_7108b6c460;  // the empty handle

struct UnkDropManagerE_ovb0208 {
    UnkDropElem_ovb0208 elems[128];  // 0x0
    u8 pad_0x7800[0x7820 - 0x7800];
    UnkSoundHandle sound;  // 0x7820
    u8 pad_0x7830[0xa01c - 0x7830];
    u32 field_0xa01c;
    u8 pad_0xa020[0xa0c8 - 0xa020];
    UnkDropItemMap_ovb0208 items;  // 0xa0c8
    bool field_0xa0f0;
    u8 pad_0xa0f1[3];
    f32 field_0xa0f4;
};

struct UnkPlayer;
UnkPlayer* fn_7100dec788(UnkGameManager* manager);
void fn_7100e2c5b0(void* self, UnkDropSlot_ovb0208* slot, f32 dt);
struct UnkDroppedItemManager;
void fn_7100e2d39c(UnkDroppedItemManager* self);

void fn_7100e2c28c(void* self_) {
    UnkDropManagerE_ovb0208* self = static_cast<UnkDropManagerE_ovb0208*>(self_);
    self->field_0xa01c = 0;
    for (u64 i = 0; i < 128; ++i) {
        if (!self->elems[i].bytes[0x7d])
            continue;
        fn_7100f05fb4(reinterpret_cast<UnkDropSlot_ovb0208*>(&self->elems[i].bytes[8]), 0);
        u64 handle = *reinterpret_cast<u64*>(&self->elems[i].bytes[0xf0]);
        if (handle == ~0ull)
            continue;
        UnkDropItem_ovb0208* item = self->items.Find(handle);
        if (item == &self->items.items[self->items.count])
            continue;
        const f32* pos = reinterpret_cast<const f32*>(&self->elems[i].bytes[0x84]);
        f32 sx = pos[0], sy = pos[1], sz = pos[2];
        if (--item->count == 0) {
            item->x = g_unk_710164779c.x;
            item->y = g_unk_710164779c.y;
            item->z = g_unk_710164779c.z;
        } else {
            f32 iy = item->y - sy;
            f32 ix = item->x - sx;
            item->y = iy;
            item->x = ix;
            item->z = item->z - sz;
        }
        item->flag = 1;
    }
    self->field_0xa0f4 = 1.0f / 30.0f;
}

// Applies the slot's position to the item registered for its handle.
static inline __attribute__((always_inline)) void ApplySlot(UnkDropManagerE_ovb0208* self, UnkDropSlot_ovb0208* slot) {
    u64 handle = slot->handle;
    if (handle == ~0ull)
        return;
    UnkDropItem_ovb0208* item = self->items.Find(handle);
    if (item == &self->items.items[self->items.count])
        return;
    f32 sx = slot->x, sy = slot->y, sz = slot->z;
    if (--item->count == 0) {
        item->x = g_unk_710164779c.x;
        item->y = g_unk_710164779c.y;
        item->z = g_unk_710164779c.z;
    } else {
        item->y = item->y - sy;
        item->x = item->x - sx;
        item->z = item->z - sz;
    }
    item->flag = 1;
}

void fn_7100e2d67c(void* self, UnkDropSlot_ovb0208* slot) {
    fn_7100f05fb4(slot, 0);
    fn_7100f05cc8(slot);
    ApplySlot(static_cast<UnkDropManagerE_ovb0208*>(self), slot);
}

void fn_7100e2d80c(void* self, UnkDropSlot_ovb0208* slot, void* actor, bool flag) {
    fn_7100f05c18(slot, actor, flag);
    fn_7100f05fb4(slot, 0);
    ApplySlot(static_cast<UnkDropManagerE_ovb0208*>(self), slot);
}

void fn_7100e2c4e4(void* self_, f32 dt) {
    UnkDropManagerE_ovb0208* self = static_cast<UnkDropManagerE_ovb0208*>(self_);
    if (self->field_0xa0f4 > 0.0f) {
        f32 t = self->field_0xa0f4 - dt;
        self->field_0xa0f4 = !(t < 0.0f) ? t : 0.0f;
    }
    if (!self->field_0xa0f0)
        dt = dt / *reinterpret_cast<f32*>(reinterpret_cast<u8*>(g_unk_7101d47310) + 0x6a4);
    for (u32 i = 0; i < 128; ++i)
        if (self->elems[i].bytes[0x7d])
            fn_7100e2c5b0(self, reinterpret_cast<UnkDropSlot_ovb0208*>(&self->elems[i].bytes[8]), dt);
    fn_7100e2d39c(reinterpret_cast<UnkDroppedItemManager*>(self));
}

void fn_7100e2e78c(void* self_, bool flag) {
    UnkDropManagerE_ovb0208* self = static_cast<UnkDropManagerE_ovb0208*>(self_);
    void* player = fn_7100dec788(g_unk_7101d47310);
    if (!player)
        return;
    for (u32 i = 0; i < 128; ++i)
        if (self->elems[i].bytes[0x7d])
            fn_7100e2d80c(self, reinterpret_cast<UnkDropSlot_ovb0208*>(&self->elems[i].bytes[8]), player, flag);
    UnkSoundHandle* handle = &self->sound;
    if (handle->ref && handle->ref->serial == handle->serial && handle->serial != -1 && handle->ref->object) {
        fn_7100105a80(handle->ref->object, 0.2f, 0.0f);
        UnkSound* sound = nullptr;
        if (handle->ref && handle->ref->serial == handle->serial && handle->serial != -1)
            sound = handle->ref->object;
        fn_7100105034(sound);
        *handle = g_unk_7108b6c460;
    }
}

// ---- Pickup effects ----

struct UnkDropAbility_ovb0208 {
    u8 pad_0x0[0xb0];
    struct UnkDropHolder_ovb0208* holder;  // 0xb0
};
struct UnkDropHolder_ovb0208 {
    u8 pad_0x0[0x10];
    void* object;  // 0x10
};
struct UnkDropInfo_ovb0208 {
    u8 pad_0x0[0x10];
    void* field_0x10;
};
struct UnkDropPlayerSub_ovb0208 {
    u8 pad_0x0[0x7e8];
    UnkDropInfo_ovb0208* info;  // 0x7e8
    u8 pad_0x7f0[0x2d08 - 0x7f0];
    u64 field_0x2d08;
};
struct UnkDropPlayer_ovb0208 {
    u8 pad_0x0[0x208];
    UnkDropPlayerSub_ovb0208* sub;  // 0x208
};
struct UnkDropPickup_ovb0208 {
    u8 pad_0x0[0x78];
    f32 amount;  // 0x78
};
struct UnkDropTarget;
extern UnkDropTarget* g_unk_7101d47318;

static inline void* Holder(UnkDropPlayer_ovb0208* player) {
    if (!player)
        return nullptr;
    UnkDropPlayerSub_ovb0208* sub = player->sub;
    if (!sub)
        return nullptr;
    UnkDropInfo_ovb0208* info = sub->info;
    if (!info)
        return nullptr;
    if ((sub->field_0x2d08 == 0) | (info->field_0x10 == nullptr))
        return nullptr;
    UnkDropAbility_ovb0208* ability = static_cast<UnkDropAbility_ovb0208*>(fn_71006eb114(info->field_0x10));
    if (!ability)
        return nullptr;
    if (!ability->holder)
        return nullptr;
    return ability->holder->object;
}

struct UnkInventory;
f32 fn_7101142928(UnkInventory* inventory, const UnkQualifiedId& item);
void fn_7101142a28(UnkInventory* inventory, const UnkQualifiedId& item, bool a, bool b, bool c, f32 amount, f32 extra);

struct UnkDropPlayerInv_ovb0208 {
    u8 pad_0x0[0x270];
    UnkInventory* inventory;  // 0x270
};

static inline const UnkQualifiedId& GetQualifiedId(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

struct UnkDropManager3_ovb0208 {
    u8 pad_0x0[0xa0b0];
    u8 entry[0x18];  // 0xa0b0
};

void fn_7100e2ebf4(void* self_, void* item_, void* arg_, u32 flags) {
    UnkDropManager3_ovb0208* self = static_cast<UnkDropManager3_ovb0208*>(self_);
    UnkDropPickup_ovb0208* item = static_cast<UnkDropPickup_ovb0208*>(item_);
    void* holder = Holder(reinterpret_cast<UnkDropPlayer_ovb0208*>(fn_7100dec788(g_unk_7101d47310)));
    if (holder && fn_71006eb1f0(holder))
        return;
    const UnkQualifiedId& current_id = GetQualifiedId(g_unk_717275a258);
    const UnkQualifiedId& max_id = GetQualifiedId(g_unk_717275a298);
    if (UnkDropPlayerInv_ovb0208* player = reinterpret_cast<UnkDropPlayerInv_ovb0208*>(fn_7100dec788(g_unk_7101d47310))) {
        if (UnkInventory* inventory = player->inventory) {
            f32 total = fn_7101142928(inventory, current_id) + item->amount;
            f32 max = fn_7101142928(inventory, max_id);
            f32 clamped = total >= max ? max : total;
            f32 value = 0.0f;
            if (!(total <= 0.0f))
                value = clamped;
            fn_7101142a28(inventory, current_id, true, true, true, value, 0.0f);
        }
    }
    if (flags & 1) {
        if (UnkDropPlayer_ovb0208* player = reinterpret_cast<UnkDropPlayer_ovb0208*>(fn_7100dec788(g_unk_7101d47310)))
            fn_7101023f04(g_unk_7101d47318, self->entry, player, nullptr, 0);
    }
}

struct UnkGameManagerIdTable {
    u8 pad_0x0[0x2578];
    UnkGameManagerIdTable() asm("fn_7100031894");
    ~UnkGameManagerIdTable() asm("fn_71000407f0");
};
inline UnkGameManagerIdTable& IdTable_ovb0208() {
    static UnkGameManagerIdTable s_table;
    return s_table;
}

struct UnkDropPlayerSub2_ovb0208 {
    u8 pad_0x0[0x2a8];
    void* field_0x2a8;
};
void* fn_71006eb3a8(void* h);
void* fn_7100bab1e8(void* owner, void* key);
void fn_710084f8c4(void* arg);

struct UnkDropManager4_ovb0208 {
    u8 pad_0x0[0xa098];
    u8 entry[0x18];  // 0xa098
};

void fn_7100e2e9b0(void* self_, void* item_, void* arg_, u32 flags) {
    UnkDropManager4_ovb0208* self = static_cast<UnkDropManager4_ovb0208*>(self_);
    UnkDropPickup_ovb0208* item = static_cast<UnkDropPickup_ovb0208*>(item_);
    void* holder = Holder(reinterpret_cast<UnkDropPlayer_ovb0208*>(fn_7100dec788(g_unk_7101d47310)));
    if (holder) {
        if (fn_71006eb3a8(holder))
            return;
        if (fn_71006eb1f0(holder))
            return;
    }
    const UnkQualifiedId& current_id = GetQualifiedId(g_unk_717275a1d8);
    const UnkQualifiedId& max_id = GetQualifiedId(g_unk_717275a218);
    if (UnkDropPlayerInv_ovb0208* player = reinterpret_cast<UnkDropPlayerInv_ovb0208*>(fn_7100dec788(g_unk_7101d47310))) {
        if (UnkInventory* inventory = player->inventory) {
            f32 total = fn_7101142928(inventory, current_id) + item->amount;
            f32 max = fn_7101142928(inventory, max_id);
            f32 clamped = total >= max ? max : total;
            f32 value = 0.0f;
            if (!(total <= 0.0f))
                value = clamped;
            fn_7101142a28(inventory, current_id, true, true, true, value, 0.0f);
        }
    }
    if (flags & 1) {
        if (UnkDropPlayer_ovb0208* player = reinterpret_cast<UnkDropPlayer_ovb0208*>(fn_7100dec788(g_unk_7101d47310)))
            fn_7101023f04(g_unk_7101d47318, self->entry, player, nullptr, 0);
    }
    UnkDropPlayerSub2_ovb0208* player = reinterpret_cast<UnkDropPlayerSub2_ovb0208*>(fn_7100dec788(g_unk_7101d47310));
    void* r = fn_7100bab1e8(player->field_0x2a8, reinterpret_cast<u8*>(&IdTable_ovb0208()) + 0x4c0);
    if (r)
        fn_710084f8c4(r);
}

// ---- Effects ----

struct UnkSwitchObject;
void fn_7100146dac(UnkSwitchObject* self, bool enable);

struct UnkFxObject_ovb0041;
struct UnkFxObjectVtable_ovb0208 {
    void* unk_0x0[2];
    void (*set_position)(UnkFxObject_ovb0041* self, const UnkVec3f* v);  // 0x10
    void (*set_rotation)(UnkFxObject_ovb0041* self, const UnkVec3f* v);  // 0x18
    void (*set_scale)(UnkFxObject_ovb0041* self, const UnkVec3f* v);     // 0x20
    void* unk_0x28[13];
    void (*set_loop)(UnkFxObject_ovb0041* self, bool loop);  // 0x90
    void (*set_speed)(UnkFxObject_ovb0041* self, f32 speed);  // 0x98
};
static inline UnkFxObjectVtable_ovb0208* FxVtable(UnkFxObject_ovb0041* fx) {
    return *reinterpret_cast<UnkFxObjectVtable_ovb0208**>(fx);
}
UnkFxObject_ovb0041* fn_71003b7db0(UnkModelManager* self, const void* path, bool a, bool b);
void fn_71011a9bb4(UnkModelManager* self, UnkFxObject_ovb0041* fx);
extern const u8 g_unk_71727c1370[] __attribute__((visibility("hidden")));  // the effect's path id

static inline UnkModelManager* ModelManager() {
    if (!g_unk_71726b0cd8) {
        UnkModelManager* m = static_cast<UnkModelManager*>(fn_71000839a0(0x1e8, 0x3f));
        fn_71011a870c(m);
        g_unk_71726b0cd8 = m;
    }
    return g_unk_71726b0cd8;
}

struct UnkDropManager5_ovb0208 {
    u8 pad_0x0[0xa0f4];
    f32 field_0xa0f4;
};

void fn_7100e2eda8(UnkDropManager5_ovb0208* self, f32 scale_value, const UnkVec3f* where) {
    if (self->field_0xa0f4 == 0.0f) {
        self->field_0xa0f4 = 1.0f / 30.0f;
        UnkModelManager* manager = ModelManager();
        UnkVec3f position = *where;
        UnkVec3f rotation(0.0f, 0.0f, 0.0f);
        UnkVec3f scale(1.0f, 1.0f, 1.0f);
        UnkFxObject_ovb0041* fx = fn_71003b7db0(manager, g_unk_71727c1370, false, true);
        if (fx) {
            FxVtable(fx)->set_position(fx, &position);
            FxVtable(fx)->set_rotation(fx, &rotation);
            FxVtable(fx)->set_scale(fx, &scale);
            fn_7100146dac(reinterpret_cast<UnkSwitchObject*>(fx), true);
            bool loop = scale_value != 1.0f;
            FxVtable(fx)->set_loop(fx, loop);
            FxVtable(fx)->set_speed(fx, scale_value);
            fn_71011a9bb4(manager, fx);
        }
    }
}

// ---- Requests ----

// The singleton (0xe0 bytes) that plays the requests below.
extern void* g_unk_71726da048;

struct UnkDropRequest_ovb0208 {
    CStrId id;  // 0x0
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    bool field_0x14;
    u8 pad_0x15[3];
    u32 field_0x18;
    u32 field_0x1c;
    f32 field_0x20;
    u8 pad_0x24[4];

    UnkDropRequest_ovb0208() : id(g_unk_710162e2ed, true) {
        __builtin_memset(&field_0x8, 0, 0xd);
        field_0x20 = 1.0f;
    }
};
static_assert(sizeof(UnkDropRequest_ovb0208) == 0x28, "request size");

struct UnkDropPlayer;
struct UnkDropTarget_ovb0208;
void fn_7100e1cd34(void* self, UnkDropTarget_ovb0208* target, UnkDropRequest_ovb0208* request);

static inline void* Singleton() {
    void* p = g_unk_71726da048;
    if (!p) {
        p = fn_71000839a0(0xe0, 0x3f);
        fn_7100e1bca4(p);
        g_unk_71726da048 = p;
    }
    return p;
}

void fn_7100e2c124(f32 value, void* self, UnkDropTarget_ovb0208* target, const CStrId* id, u32 a, u32 b, u32 c, u32 d, u32 e,
                   bool flag) {
    UnkDropRequest_ovb0208 request;
    request.field_0x14 = flag;
    request.field_0x18 = a;
    request.field_0x1c = b;
    request.id = *id;
    request.field_0x8 = c;
    request.field_0xc = d;
    request.field_0x10 = e;
    request.field_0x20 = value;
    fn_7100e1cd34(Singleton(), target, &request);
}
