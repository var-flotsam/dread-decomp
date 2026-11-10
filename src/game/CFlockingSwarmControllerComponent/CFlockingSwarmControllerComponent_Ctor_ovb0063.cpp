// CFlockingSwarmControllerComponent unit (tools/tu_map.py unit 1158): the controller's constructor
// (fn_7100737d58) and destructor (fn_7100737f34). After the base (fn_71007477f8 / fn_71007479e4) it
// holds a path (+0x2c8, the logic path's vtable and its array of 0x20-byte polymorphic points, as in
// game_s2/UnkItem50.h), five flag bits all set at +0x338 (the function-local masks of
// CFlockingSwarmControllerComponent_Reset_A3_b72.cpp), the two lists of 12-byte entries the reset
// resizes (+0x340, +0x368), a value-initialised block with an array at +0x3b8 and the collider count
// at +0x3e0. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game_s2/UnkItem50.h"
#include "types.h"

extern void* g_unk_7101a6d6f8[];  // the controller's vtable

struct UnkSwarmEntry_ovb0063 {
    u32 v[3];
    UnkSwarmEntry_ovb0063(const UnkSwarmEntry_ovb0063& o) {
        __builtin_memcpy(&v[0], &o.v[0], 4);
        __builtin_memcpy(&v[1], &o.v[1], 4);
        __builtin_memcpy(&v[2], &o.v[2], 4);
    }
};

struct UnkSwarmPathNode_ovb0063 {
    virtual void vf_0x0();
    virtual ~UnkSwarmPathNode_ovb0063();
    f32 field_0x8[6];
};

struct UnkSwarmPath_ovb0063 {
    void** vtable;
    base::UnkVector<UnkSwarmPathNode_ovb0063> nodes;
    bool dirty;
    f32 bounds[6];

    UnkSwarmPath_ovb0063() : vtable(g_unk_7101cac938 + 2), nodes(0xf), dirty(true), bounds() {}

    ~UnkSwarmPath_ovb0063() {
        vtable = g_unk_7101cac938 + 2;
        nodes.Free();
    }
};
static_assert(sizeof(UnkSwarmPath_ovb0063) == 0x50, "UnkSwarmPath_ovb0063 layout");

struct UnkSwarmWord_ovb0063 {
    u32 v;
};

struct UnkSwarmFlags_ovb0063 {
    u32 bits;
    UnkSwarmFlags_ovb0063() : bits(0) {}
    explicit UnkSwarmFlags_ovb0063(u32 b) : bits(b) {}
};

inline const UnkSwarmFlags_ovb0063& SwarmFlagsNone_ovb0063() {
    static UnkSwarmFlags_ovb0063 none;
    return none;
}

inline const UnkSwarmFlags_ovb0063& SwarmFlagsAll_ovb0063() {
    static UnkSwarmFlags_ovb0063 all(~SwarmFlagsNone_ovb0063().bits & 0x1f);
    return all;
}

// Two arrays of entries (the reset resizes both to the member count).
struct UnkSwarmLists_ovb0063 {
    base::UnkVector<UnkSwarmEntry_ovb0063> a{0xf};
    base::UnkVector<UnkSwarmEntry_ovb0063> b{0xf};
};

// Value-initialised as a whole (zeroed first), then its array set up.
struct UnkSwarmWordList_ovb0063 {
    u64 field_0x0;
    u64 field_0x8;
    u64 field_0x10;
    base::UnkVector<UnkSwarmWord_ovb0063> words{0xf};
};

struct UnkSwarmCtrl_ovb0063 {
    void** vtable;
    u8 pad_0x8[0x298];
    u64 field_0x2a0;
    u64 field_0x2a8;
    u64 field_0x2b0;
    u64 field_0x2b8;
    u32 field_0x2c0;
    UnkSwarmPath_ovb0063 path;  // 0x2c8
    s32 field_0x318;
    bool field_0x31c;
    u64 field_0x320;
    u64 field_0x328;
    u64 field_0x330;
    UnkSwarmFlags_ovb0063 flags;  // 0x338
    UnkSwarmLists_ovb0063 lists;  // 0x340
    u32 field_0x390;
    bool field_0x394;
    bool field_0x395;
    u32 field_0x398;
    u32 field_0x39c;
    UnkSwarmWordList_ovb0063 words;  // 0x3a0
    u32 collider_count;  // 0x3e0
};
static_assert(__builtin_offsetof(UnkSwarmCtrl_ovb0063, collider_count) == 0x3e0, "layout");

struct UnkRedenkiSwarm_b46a2;  // the controller as src/game/CRedenkiSwarmControllerComponent/ names it
struct UnkDaivoLists_b0012;    // the controller as src/game/CDaivoSwarmControllerComponent/ names it

void fn_71007479e4(UnkSwarmCtrl_ovb0063* self);  // the base destructor
void fn_71007477f8(UnkSwarmCtrl_ovb0063* self);  // the base constructor

void fn_7100737d58(UnkRedenkiSwarm_b46a2* object) {
    auto* self = reinterpret_cast<UnkSwarmCtrl_ovb0063*>(object);
    fn_71007477f8(self);
    self->vtable = g_unk_7101a6d6f8 + 2;
    self->field_0x2a0 = 0;
    self->field_0x2a8 = 0;
    self->field_0x2b0 = 0;
    self->field_0x2b8 = 0;
    self->field_0x2c0 = 0;
    new (&self->path) UnkSwarmPath_ovb0063();
    self->field_0x318 = 0;
    self->field_0x31c = false;
    self->field_0x320 = 0;
    self->field_0x328 = 0;
    self->field_0x330 = 0;
    self->flags = SwarmFlagsAll_ovb0063();
    new (&self->lists) UnkSwarmLists_ovb0063;
    self->field_0x390 = 0;
    self->field_0x394 = true;
    self->field_0x395 = false;
    self->field_0x398 = 0xffff;
    self->field_0x39c = 0xffff;
    new (&self->words) UnkSwarmWordList_ovb0063();
    self->collider_count = 0;
}

void fn_7100737f34(UnkDaivoLists_b0012* object) {
    auto* self = reinterpret_cast<UnkSwarmCtrl_ovb0063*>(object);
    self->vtable = g_unk_7101a6d6f8 + 2;
    self->collider_count = 0;
    self->words.words.Free();
    self->lists.b.Free();
    self->lists.a.Free();
    self->path.~UnkSwarmPath_ovb0063();
    fn_71007479e4(self);
}
