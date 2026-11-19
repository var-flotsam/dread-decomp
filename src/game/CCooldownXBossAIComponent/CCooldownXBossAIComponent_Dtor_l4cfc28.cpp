// CCooldownXBossAIComponent's unit (tools/tu_map.py unit 754): the destructor. Its members go last first:
// the damage state, the callback lists, the track listener, the navigation link, the records by id, the
// three parts with their own listeners and the two out-of-line members, then the boss base destructor
// (fn_71003ac388). Names and layouts are placeholders.
#include "game/CCooldownXBossAttack.h"
#include "base/UnkCloneHelpers.h"
#include "base/UnkDestructors.h"
#include "base/global/CRntSmallDictionary.h"
#include "base/global/CStrId.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAttack.h"
#include "game/CBossAIComponent.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/CCooldownXBossAIComponent.h"
#include "game/UnkTU1370.h"
#include "game/UnkTU341.h"
#include "game/logic/collision/CPolygonCollectionShape.h"
#include "game/navmesh/SNavMeshPath.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

extern void* g_unk_7101a3ed58[];  // the component's vtable

struct UnkCooldownXRecord_ovb0088;
void fn_71004cb4cc(void* self);
struct UnkLavaDropsAttack_ovb0111;
void fn_71004c86cc(UnkLavaDropsAttack_ovb0111*);

struct UnkCooldownXCallbacks_l4cfc28;
template <>
struct base::UnkCallbackTraits<UnkCooldownXCallbacks_l4cfc28> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a33fa0); }
    static char* base_vtable() { return reinterpret_cast<char*>(g_unk_7101a20998); }
};
using UnkCooldownXCallbackList_l4cfc28 = base::UnkCallbackListShort<UnkCooldownXCallbacks_l4cfc28>;

// The counted link a listener holds; the object it is registered with is at +0x10.
struct UnkCooldownXLink_l4cfc28 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Release();  // 0x10
    u32 ref_count;
    void* owner;
};

// A listener that unregisters from its link's owner (fn_710117b3f0) on destruction.
struct UnkCooldownXListener_l4cfc28 {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkCooldownXLink_l4cfc28* link;

    ~UnkCooldownXListener_l4cfc28() {
        vtable = &g_unk_7101a22d48[2];
        if (UnkCooldownXLink_l4cfc28* l = link) {
            bool held = true;
            if (void* owner = l->owner) {
                fn_710117b3f0(static_cast<UnkWeakTarget*>(owner), this);
                l = link;
                held = l != nullptr;
            }
            if (held && --l->ref_count == 0)
                l->Release();
        }
    }
};

// The base of the three parts: destroyed by fn_71003606c0.
struct UnkCooldownXPartBase_l4cfc28 {
    void** vtable;
    ~UnkCooldownXPartBase_l4cfc28() { fn_71003606c0(this); }
};

// The part at +0x149a8.
struct UnkCooldownXPartA_l4cfc28 : UnkCooldownXPartBase_l4cfc28 {
    u8 pad_0x8[0x68];
    UnkCooldownXListener_l4cfc28 listener;  // 0x70
    u8 pad_0x90[0x10];
    u8 shape[0x138];  // 0xa0: destroyed by fn_7100d62bd8
    u32 field_0x1d8;

    ~UnkCooldownXPartA_l4cfc28() {
        vtable = g_unk_7101a3e5b8 + 2;
        field_0x1d8 = 0;
        fn_7100d62bd8(shape);
    }
};
static_assert(__builtin_offsetof(UnkCooldownXPartA_l4cfc28, field_0x1d8) == 0x1d8, "layout");

// The part at +0x15480.
struct UnkCooldownXPartB_l4cfc28 : UnkCooldownXPartBase_l4cfc28 {
    u8 pad_0x8[0x68];
    UnkCooldownXListener_l4cfc28 listener;  // 0x70

    ~UnkCooldownXPartB_l4cfc28() { vtable = g_unk_7101a3e8b8 + 2; }
};

// The part at +0x155a8.
struct UnkCooldownXPartC_l4cfc28 : UnkCooldownXPartBase_l4cfc28 {
    u8 pad_0x8[0x70];
    UnkCooldownXListener_l4cfc28 listener;  // 0x78

    ~UnkCooldownXPartC_l4cfc28() { vtable = g_unk_7101a3e4b0 + 2; }
};

// A record kept by id (0x40 bytes), destroyed out of line.
struct UnkCooldownXRecordEntry_l4cfc28 {
    u8 data[0x40];
    ~UnkCooldownXRecordEntry_l4cfc28() { fn_71004d0b00(reinterpret_cast<UnkCooldownXRecord_ovb0088*>(this)); }
};

// The records by id, owning both arrays.
struct UnkCooldownXRecords_l4cfc28 : base::global::CRntSmallDictionary<CStrId, UnkCooldownXRecordEntry_l4cfc28> {
    ~UnkCooldownXRecords_l4cfc28() {
        Clear();
        if (keys)
            allocator->vf_0x48(keys);
        if (values)
            allocator->vf_0x48(values);
    }
};

// The boss's place in the navigation list, and the two pointers that go with it.
struct UnkCooldownXNavLink_l4cfc28 {
    UnkLinkList* list;
    void* field_0x8;
    void* field_0x10;

    ~UnkCooldownXNavLink_l4cfc28() {
        if (list) {
            fn_71002aeb9c(list, &list);
            list = nullptr;
        }
        field_0x10 = nullptr;
        field_0x8 = nullptr;
    }
};

struct UnkCooldownXBossAIComponentD_l4cfc28 {
    void** vtable;
    u8 pad_0x8[0x149a0];
    UnkCooldownXPartA_l4cfc28 part_a;  // 0x149a8
    u8 pad_0x14b88[0x5a8];
    u8 field_0x15130[0x350];
    UnkCooldownXPartB_l4cfc28 part_b;  // 0x15480
    u8 field_0x15510[0x98];
    UnkCooldownXPartC_l4cfc28 part_c;  // 0x155a8
    u8 pad_0x15640[0x18];
    UnkCooldownXRecords_l4cfc28 records;           // 0x15658
    u8 pad_0x15680[0x10];
    UnkCooldownXCallbackList_l4cfc28 callbacks_e;  // 0x15690
    UnkCooldownXCallbackList_l4cfc28 callbacks_d;  // 0x156d0
    UnkCooldownXCallbackList_l4cfc28 callbacks_c;  // 0x15710
    u8 pad_0x15750[0x8];
    UnkCooldownXNavLink_l4cfc28 nav;               // 0x15758
    UnkCooldownXListener_l4cfc28 listener;         // 0x15770
    UnkCooldownXCallbackList_l4cfc28 callbacks_b;  // 0x15790
    UnkCooldownXCallbackList_l4cfc28 callbacks_a;  // 0x157d0
    u8 pad_0x15810[0x28];
    u8 damage[0x10];  // 0x15838: destroyed by fn_71008c1558
};
static_assert(__builtin_offsetof(UnkCooldownXBossAIComponentD_l4cfc28, field_0x15130) == 0x15130, "layout");
static_assert(__builtin_offsetof(UnkCooldownXBossAIComponentD_l4cfc28, part_b) == 0x15480, "layout");
static_assert(__builtin_offsetof(UnkCooldownXBossAIComponentD_l4cfc28, part_c) == 0x155a8, "layout");
static_assert(__builtin_offsetof(UnkCooldownXBossAIComponentD_l4cfc28, records) == 0x15658, "layout");
static_assert(__builtin_offsetof(UnkCooldownXBossAIComponentD_l4cfc28, callbacks_e) == 0x15690, "layout");
static_assert(__builtin_offsetof(UnkCooldownXBossAIComponentD_l4cfc28, nav) == 0x15758, "layout");
static_assert(__builtin_offsetof(UnkCooldownXBossAIComponentD_l4cfc28, callbacks_a) == 0x157d0, "layout");
static_assert(__builtin_offsetof(UnkCooldownXBossAIComponentD_l4cfc28, damage) == 0x15838, "layout");

void fn_71004cfc28(UnkCooldownXBossAIComponentD_l4cfc28* self) {
    self->vtable = g_unk_7101a3ed58 + 2;
    fn_71008c1558(reinterpret_cast<UnkBase_71003cc474*>(self->damage));
    self->callbacks_a.~UnkCooldownXCallbackList_l4cfc28();
    self->callbacks_b.~UnkCooldownXCallbackList_l4cfc28();
    self->listener.~UnkCooldownXListener_l4cfc28();
    self->nav.~UnkCooldownXNavLink_l4cfc28();
    self->callbacks_c.~UnkCooldownXCallbackList_l4cfc28();
    self->callbacks_d.~UnkCooldownXCallbackList_l4cfc28();
    self->callbacks_e.~UnkCooldownXCallbackList_l4cfc28();
    self->records.~UnkCooldownXRecords_l4cfc28();
    self->part_c.~UnkCooldownXPartC_l4cfc28();
    fn_71004cb4cc(self->field_0x15510);
    self->part_b.~UnkCooldownXPartB_l4cfc28();
    fn_71004c86cc(reinterpret_cast<UnkLavaDropsAttack_ovb0111*>(self->field_0x15130));
    self->part_a.~UnkCooldownXPartA_l4cfc28();
    fn_71003ac388(reinterpret_cast<UnkBossAI_l3ac388*>(self));
}
