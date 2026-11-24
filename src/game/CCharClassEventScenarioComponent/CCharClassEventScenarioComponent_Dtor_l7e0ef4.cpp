// CCharClassEventScenarioComponent unit: the destructor of the event scenario component. The collider goes
// back to the collision manager, the arrays and ids are released last first, the four callback
// registrations leave their sets, then the base component's destructor runs. Names and layouts are placeholders.
#include "base/UnkDeferredSet.h"
#include "base/UnkDestructors.h"
#include "base/UnkStrIdTable.h"
#include "base/global/CRntVector.h"
#include "game/CCharClassEventScenarioComponent.h"
#include "game/UnkComponent.h"
#include "game/UnkTU2506.h"
#include "game/UnkTU365.h"
#include "game/behaviortree/CEmmyPatrolAction.h"
#include "types.h"

using base::UnkStrIdMember;

struct UnkManager_7100863a8c;
struct UnkCollisionObject;
struct UnkScenarioIdList_ovb0097;
void CCharClassEventScenarioComponent_ResizeStrIdList(UnkScenarioIdList_ovb0097* self, u32 n);

// A small list of string ids (0x24 bytes) that is emptied when it goes away.
struct UnkIdList_l7e0ef4 {
    u8 pad_0x0[0x24];
    ~UnkIdList_l7e0ef4() { CCharClassEventScenarioComponent_ResizeStrIdList(reinterpret_cast<UnkScenarioIdList_ovb0097*>(this), 0); }
};

// A scenario entry (0x28 bytes): its list of id lists.
struct UnkEntry_l7e0ef4 {
    base::global::CRntVector<UnkIdList_l7e0ef4> lists;
    ~UnkEntry_l7e0ef4() { lists.Free(); }
};

struct UnkItem_l7e0ef4 {
    virtual ~UnkItem_l7e0ef4();
    u8 pad_0x8[0x38];
};

// A registration in a set that it leaves when destroyed.
struct UnkRegA_l7e0ef4 {
    void* set;
    void* object;
    void* callback;
    ~UnkRegA_l7e0ef4() {
        if (set) {
            fn_71004f3ea4(static_cast<base::UnkDeferredSet<void>*>(set), this);
            set = nullptr;
        }
        object = nullptr;
        callback = nullptr;
    }
};
struct UnkRegB_l7e0ef4 {
    void* set;
    void* object;
    void* callback;
    ~UnkRegB_l7e0ef4() {
        if (set) {
            fn_7100514b7c(set, this);
            set = nullptr;
        }
        object = nullptr;
        callback = nullptr;
    }
};

struct UnkEventScenario_l7e0ef4 : UnkBaseComponent {
    UnkRegA_l7e0ef4 reg_0x40;
    UnkRegA_l7e0ef4 reg_0x58;
    UnkRegB_l7e0ef4 reg_0x70;
    UnkRegB_l7e0ef4 reg_0x88;
    u8 pad_0xa0[0x8];
    UnkStrIdMember id_0xa8;
    UnkOwnedStrIdTable table_0xb0;
    UnkStrIdMember id_0xd8;
    UnkStrIdMember id_0xe0;
    UnkStrIdMember id_0xe8;
    UnkStrIdMember id_0xf0;
    UnkCollisionObject* collider;  // 0xf8
    void* shape;                   // 0x100
    base::global::CRntVector<UnkItem_l7e0ef4> items;     // 0x108
    base::global::CRntVector<UnkEntry_l7e0ef4> entries;  // 0x130
    base::global::CRntVector<u32> randoms;               // 0x158
    base::global::CRntVector<u64> timers;                // 0x180
    base::global::CRntVector<void*> field_0x1a8;
};
static_assert(__builtin_offsetof(UnkEventScenario_l7e0ef4, reg_0x88) == 0x88, "layout");
static_assert(__builtin_offsetof(UnkEventScenario_l7e0ef4, table_0xb0) == 0xb0, "layout");
static_assert(__builtin_offsetof(UnkEventScenario_l7e0ef4, collider) == 0xf8, "layout");
static_assert(__builtin_offsetof(UnkEventScenario_l7e0ef4, items) == 0x108, "layout");
static_assert(__builtin_offsetof(UnkEventScenario_l7e0ef4, field_0x1a8) == 0x1a8, "layout");

static inline UnkManager_7100863a8c* CollisionManager() {
    UnkManager_7100863a8c* m = g_unk_71726a4788;
    if (!m) {
        m = static_cast<UnkManager_7100863a8c*>(fn_71000839a0(0x6b0, 0x3f));
        fn_7100d46804(m);
        g_unk_71726a4788 = m;
    }
    return m;
}

void fn_71007e0ef4(UnkEventScenario_l7e0ef4* self) {
    self->vtable = g_unk_7101a73408 + 2;
    if (self->collider) {
        fn_7100d46c60(CollisionManager(), self->collider);
        self->collider = nullptr;
    }
    self->field_0x1a8.Free();
    self->timers.Free();
    self->randoms.Free();
    self->entries.Free();
    self->items.Free();
    self->id_0xf0.~UnkStrIdMember();
    self->id_0xe8.~UnkStrIdMember();
    self->id_0xe0.~UnkStrIdMember();
    self->id_0xd8.~UnkStrIdMember();
    self->table_0xb0.~UnkOwnedStrIdTable();
    self->id_0xa8.~UnkStrIdMember();
    self->reg_0x88.~UnkRegB_l7e0ef4();
    self->reg_0x70.~UnkRegB_l7e0ef4();
    self->reg_0x58.~UnkRegA_l7e0ef4();
    self->reg_0x40.~UnkRegA_l7e0ef4();
    fn_71010c4194(self);
}
