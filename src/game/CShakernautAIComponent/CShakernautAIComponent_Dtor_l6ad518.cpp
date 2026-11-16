// CShakernautAIComponent unit (tools/tu_map.py unit 1047): the destructor. The two vectors of counted
// references at +0x14840 and +0x14818, the two attacks at +0x14738 and +0x146d0 (each with its listener),
// then the base AI component's destructor (fn_7100661914). Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/CAbilityGhostAura.h"
#include "game/CRobotAIComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

extern char g_unk_7101a62f60[];  // CShakernautAIComponent's vtable
extern void* g_unk_7101a62a50[];  // the first attack's vtable
extern void* g_unk_7101a62b48[];  // the second attack's vtable

void fn_71003606c0(void*);

// A counted reference released on destruction.
struct UnkShakernautRef_l6ad518 {
    UnkRefObject* p;
    ~UnkShakernautRef_l6ad518() {
        if (UnkRefObject* r = p) {
            if (--r->ref_count == 0)
                r->vf_0x10();
        }
    }
};

struct UnkShakernautEntry_l6ad518 {
    u8 pad_0x0[0x18];
    UnkShakernautRef_l6ad518 ref;  // 0x18
};

// The counted link a listener holds; the object it is registered with is at +0x10.
struct UnkShakernautLink_l6ad518 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Release();  // 0x10
    u32 ref_count;
    void* owner;
};

// A listener that unregisters from its link's owner (fn_710117b3f0) on destruction.
struct UnkShakernautListener_l6ad518 {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkShakernautLink_l6ad518* link;

    ~UnkShakernautListener_l6ad518() {
        vtable = &g_unk_7101a22d48[2];
        if (UnkShakernautLink_l6ad518* l = link) {
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

// The attack base: its destructor is fn_71003606c0.
struct UnkShakernautAttackBase_l6ad518 {
    void** vtable;
    ~UnkShakernautAttackBase_l6ad518() { fn_71003606c0(this); }
};

struct UnkShakernautAttackA_l6ad518 : UnkShakernautAttackBase_l6ad518 {
    u8 pad_0x8[0x30];
    UnkShakernautListener_l6ad518 listener;  // 0x38
    u8 pad_0x58[0x10];
    ~UnkShakernautAttackA_l6ad518() { vtable = &g_unk_7101a62a50[2]; }
};

struct UnkShakernautAttackB_l6ad518 : UnkShakernautAttackBase_l6ad518 {
    u8 pad_0x8[0x30];
    UnkShakernautListener_l6ad518 listener;  // 0x38
    u8 pad_0x58[0x10];
    ~UnkShakernautAttackB_l6ad518() { vtable = &g_unk_7101a62b48[2]; }
};

struct UnkShakernautAIComponent_l6ad518 {
    void** vtable;
    u8 pad_0x8[0x146d0 - 0x8];
    UnkShakernautAttackA_l6ad518 attack_a;  // 0x146d0
    UnkShakernautAttackB_l6ad518 attack_b;  // 0x14738
    u8 pad_0x147a0[0x14818 - 0x147a0];
    base::global::CRntVector<UnkShakernautEntry_l6ad518> entries_a;  // 0x14818
    base::global::CRntVector<UnkShakernautEntry_l6ad518> entries_b;  // 0x14840
};
static_assert(__builtin_offsetof(UnkShakernautAIComponent_l6ad518, attack_b) == 0x14738, "layout");
static_assert(__builtin_offsetof(UnkShakernautAIComponent_l6ad518, entries_b) == 0x14840, "layout");

void fn_71006ad518(UnkShakernautAIComponent_l6ad518* self) {
    self->vtable = reinterpret_cast<void**>(g_unk_7101a62f60 + 0x10);
    self->entries_b.Free();
    self->entries_a.Free();
    self->attack_b.~UnkShakernautAttackB_l6ad518();
    self->attack_a.~UnkShakernautAttackA_l6ad518();
    fn_7100661914(reinterpret_cast<UnkAutsharpAIBase_b47*>(self));
}
