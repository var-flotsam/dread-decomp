// CPlayerInfo unit (tools/tu_map.py unit 2592): the pending gravity-suit update and the timed entries.
// fn_7100f0d7b0 runs fn_7100f0c248 now, or (while the lazily created manager g_unk_7101d47360 holds
// fn_7100eef674) records it as pending at +0xd4 with an optional position; fn_7100f0e100 refreshes the
// gravity-suit flag at +0x8e when the message's id is ITEM_GRAVITY_SUIT and tells fn_7100f0be58 about a
// change; fn_7100f0c988 counts down the timed entries (map at +0x320, 0x30-byte values) and erases the
// finished empty ones; fn_7100f0dd3c stops the effect behind the handle at +0x260 and frees +0x138;
// fn_7100f0bc50 resets everything. Names and layouts are placeholders.
#include "base/UnkKeyedStrIds.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/CCharClassLiquidPoolComponent.h"
#include "game/CPlayerInfo.h"
#include "game/UnkTU2577.h"
#include "game/UnkTU39.h"
#include "game/UnkTU50.h"
#include "game/logic/collision/CPolygonCollectionShape.h"
#include "game_s2/UnkWeakHandle.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "types.h"

struct UnkOwner;
struct UnkActorTarget;
struct UnkInventory;
struct UnkPlayer;
UnkOwner* fn_7100defcd4(UnkGameManager* manager);
void fn_710117662c(UnkActorTarget* target, u32 a, u32 b, u32 c, u32 d, f32 value);
void fn_7101178e08(UnkActorTarget* target, u32 a, u32 b, u32 c, f32 value);
UnkPlayer* fn_7100dec788(UnkGameManager*);
bool fn_71011428bc(UnkInventory* inventory, const UnkQualifiedId& item);
extern void* g_unk_7101d47360;
void fn_7100ee1f34(void*);

struct UnkVec3_ovb0071 {
    f32 x, y, z;
};

struct UnkPendingPos_ovb0071 {
    UnkVec3_ovb0071 pos;
    bool has;  // 0xc
};

struct UnkHandleTarget_ovb0071 {
    void* target;
    u64 serial;
};

struct UnkHandle {
    UnkHandleTarget_ovb0071* target;
    u64 serial;

    void* Get() const {
        if (target && target->serial == serial && serial != ~0ull)
            return target->target;
        return nullptr;
    }
};

struct UnkTimedHead_ovb0071 {
    u8 counter;
    f32 time;
};

struct UnkTimedEntry_ovb0071 {
    UnkTimedHead_ovb0071 head;
    UnkKeyedStrIds properties;  // 0x8
};

struct UnkTimedMap_ovb0071 {
    UnkTimedEntry_ovb0071* values;
    base::global::CStrId* ids;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
};

struct UnkTileOutlines;

#pragma pack(push, 4)
struct UnkFlagEntry_ovb0071 {
    UnkWeakHandle* handle;
    u32 value;
    bool flag;
    bool field_0xd;
    u8 pad_0xe[2];
};
#pragma pack(pop)

struct UnkPlayerInfo_ovb0071 {
    u8 pad_0x0[0x88];
    u32 field_0x88;
    bool field_0x8c;
    bool field_0x8d;
    bool field_0x8e;
    u8 pad_0x8f;
    u32 count;  // 0x90
    UnkFlagEntry_ovb0071 entries[4];  // 0x94
    u8 pending;  // 0xd4
    u8 pad_0xd5[3];
    UnkPendingPos_ovb0071 pending_pos;  // 0xd8
    f32 pending_time;  // 0xe8
    bool field_0xec;
    bool field_0xed;
    u8 pad_0xee[2];
    u32 field_0xf0;
    bool field_0xf4;
    u8 pad_0xf5[3];
    f32 field_0xf8;
    u8 pad_0xfc[8];
    bool field_0x104;
    u8 pad_0x105[3];
    f32 field_0x108;
    u8 pad_0x10c[0x2c];
    UnkTileOutlines* field_0x138;
    u8 pad_0x140[0xa8];
    void* field_0x1e8;
    u8 pad_0x1f0[0x70];
    UnkHandle field_0x260;
    u8 pad_0x270[0xb0];
    UnkTimedMap_ovb0071 timed;  // 0x320
    u8 timer[0x10];  // 0x348
    u8 pad_0x358[0xa0];
    bool field_0x3f8;
};
static_assert(__builtin_offsetof(UnkPlayerInfo_ovb0071, timer) == 0x348, "layout");

struct UnkPlayerInfoMsg_ovb0071 {
    base::global::CStrId id;
    u32 field_0x8;
    bool field_0xc;
};

extern UnkHandle g_unk_7101d586c0;
UnkTimedEntry_ovb0071* fn_7100f0ca9c(UnkTimedMap_ovb0071* self, UnkTimedEntry_ovb0071* it);
void fn_7100840f50(void* target, void** slot);
void fn_7100841270(void* target, void** slot);
void fn_7100f0c248(UnkPlayerInfo_ovb0071* self, const UnkVec3_ovb0071* pos, f32 time);
void fn_7100f0c988(UnkPlayerInfo_ovb0071* self, f32 dt);
void fn_7100f0d88c(UnkPlayerInfo_ovb0071* self);
void fn_7100f0be58(UnkPlayerInfo_ovb0071* self, bool flag);

static inline void* Manager_ovb0071() {
    void* manager = g_unk_7101d47360;
    if (!manager) {
        manager = fn_71000839a0(0xae8, 0x3f);
        fn_7100ee1f34(manager);
        g_unk_7101d47360 = manager;
    }
    return manager;
}

static inline const UnkQualifiedId& GetQualifiedId_ovb0071(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

bool fn_7100f0e100(UnkPlayerInfo_ovb0071* self, UnkPlayerInfoMsg_ovb0071 msg) {
    const UnkQualifiedId& item = GetQualifiedId_ovb0071(g_unk_717275a458);
    if (msg.id.data == item.id.data) {
        char* player = reinterpret_cast<char*>(fn_7100dec788(g_unk_7101d47310));
        UnkInventory* inventory = *reinterpret_cast<UnkInventory**>(player + 0x270);
        bool old = self->field_0x8e;
        bool has = fn_71011428bc(inventory, GetQualifiedId_ovb0071(g_unk_717275a458));
        self->field_0x8e = has;
        if (old != has && (self->count || self->pending))
            fn_7100f0be58(self, !has);
    }
    return false;
}

u32 fn_7100f0d7b0(UnkPlayerInfo_ovb0071* self, void* source, const UnkVec3_ovb0071* pos) {
    fn_7100f0d88c(self);
    if (self->count == 0) {
        if (fn_7100eef674(Manager_ovb0071())) {
            self->pending = 2;
            self->pending_pos.has = pos != nullptr;
            self->pending_time = fn_7100902abc(source);
            if (self->pending_pos.has) {
                self->pending_pos.pos.x = pos->x;
                self->pending_pos.pos.y = pos->y;
                self->pending_pos.pos.z = pos->z;
            }
        } else {
            fn_7100f0c248(self, pos, fn_7100902abc(source));
        }
    }
    return self->count;
}

void fn_7100f0c988(UnkPlayerInfo_ovb0071* self, f32 dt) {
    UnkTimedMap_ovb0071& map = self->timed;
    for (UnkTimedEntry_ovb0071* it = map.values; map.values + map.count != it;) {
        UnkTimedEntry_ovb0071* next = it + 1;
        if (it->head.counter && !fn_7100eef674(Manager_ovb0071())) {
            if (--it->head.counter == 0 && it->properties.count == 0)
                next = fn_7100f0ca9c(&map, it);
        } else {
            it->head.time += dt;
        }
        it = next;
    }
}

void fn_7100f0dd3c(UnkPlayerInfo_ovb0071* self) {
    self->field_0x104 = false;
    if (self->field_0x260.Get() != g_unk_7101d586c0.Get())
        fn_7100072328(self->field_0x260.Get(), 0.2f, 0.0f);
    if (self->field_0x138) {
        fn_7100d633ec(self->field_0x138);
        self->field_0x138 = nullptr;
    }
}

void fn_7100f0bc50(UnkPlayerInfo_ovb0071* self) {
    self->field_0xf4 = false;
    self->field_0xf8 = 0.0f;
    self->field_0x104 = false;
    self->field_0x88 = 0;
    self->field_0x8c = false;
    self->field_0x8d = false;
    for (u64 i = 0; i < self->count; ++i) {
        if (UnkWeakHandle* handle = self->entries[i].handle) {
            if (--handle->ref_count == 0)
                handle->vf_0x10();
        }
    }
    self->count = 0;
    self->pending = 0;
    self->pending_time = 0.0f;
    self->pending_pos = UnkPendingPos_ovb0071();
    UnkTimedMap_ovb0071& map = self->timed;
    if (map.ids && map.count) {
        for (u64 i = 0; i < map.count; ++i)
            map.ids[i].Reset();
    }
    if (map.values && map.count) {
        for (u64 i = 0; i < map.count; ++i)
            fn_7100f0e85c(&map.values[i]);
    }
    map.count = 0;
    self->field_0x8e = false;
    if (char* player = reinterpret_cast<char*>(fn_7100defcd4(g_unk_7101d47310)))
        fn_7101178e08(*reinterpret_cast<UnkActorTarget**>(player + 0x220), 1, 1, 1, -1.0f);
    char* entry = *reinterpret_cast<char**>(reinterpret_cast<char*>(g_unk_7101d47310) + 0x29a0);
    if (entry) {
        if (char* owner = *reinterpret_cast<char**>(entry + 0x10))
            fn_7101178e08(*reinterpret_cast<UnkActorTarget**>(owner + 0x220), 1, 1, 1, -1.0f);
    }
    self->field_0xec = true;
    self->field_0xed = false;
    self->field_0xf0 = 0;
    if (self->field_0x1e8) {
        fn_7100840f50(self->field_0x1e8, &self->field_0x1e8);
        self->field_0x1e8 = nullptr;
    }
    self->field_0x1e8 = reinterpret_cast<char*>(g_unk_7101d47310) + 0x1ba8;
    fn_7100841270(self->field_0x1e8, &self->field_0x1e8);
    self->field_0x3f8 = false;
}
