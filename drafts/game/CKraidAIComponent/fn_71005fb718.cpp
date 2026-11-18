// CKraidAIComponent unit: the component's destructor: its list links, listener, callback lists and
// attacks, last first, then the array at +0x149e8 and CBossAIComponent's destructor (fn_71003ac388).
// Names and layouts are placeholders.
#include "../../game_5821e4/CAttackDtor.h"
#include "base/UnkCloneHelpers.h"
#include "base/global/CRntVector.h"
#include "game/CBossAIComponent.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/UnkTU341.h"
#include "game/navmesh/SNavMeshPath.h"
#include "types.h"

extern void* g_unk_7101a54408[];  // CKraidAIComponent's vtable
extern void* g_unk_7101a53d18[];  // the vtables of the three attacks with a listener
extern void* g_unk_7101a53e10[];
extern void* g_unk_7101a54100[];

struct UnkKraidCallbacks_l5fb718;
template <>
struct base::UnkCallbackTraits<UnkKraidCallbacks_l5fb718> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a33fa0); }
    static char* base_vtable() { return reinterpret_cast<char*>(g_unk_7101a20998); }
};
using UnkKraidCallbackList_l5fb718 = base::UnkCallbackListShort<UnkKraidCallbacks_l5fb718>;

struct UnkKraidSetTag_l5fb718;
template <>
struct base::UnkCallbackTraits<UnkKraidSetTag_l5fb718> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a20950); }
    static char* base_vtable() { return reinterpret_cast<char*>(g_unk_7101a20998); }
};
using UnkKraidCallbackSet_l5fb718 = base::UnkCallbackList<UnkKraidSetTag_l5fb718>;

// A link into a list (SNavMeshPath): unlinked and cleared on destruction.
struct UnkListLink_l5fb718 {
    UnkLinkList* list;
    void* field_0x8;
    void* field_0x10;

    ~UnkListLink_l5fb718() {
        if (list) {
            fn_71002aeb9c(list, &list);
            list = nullptr;
        }
        field_0x10 = nullptr;
        field_0x8 = nullptr;
    }
};

// A listener that unregisters from its owner on destruction.
struct UnkKraidListener_l5fb718 : UnkAttackListenerRaw {
    ~UnkKraidListener_l5fb718() { Destroy(); }
};

// CAttack's part, destroyed out of line.
struct UnkAttack_l5fb718 {
    void** vtable;
    u8 pad_0x8[0x30];

    ~UnkAttack_l5fb718() { fn_71003606c0(this); }
};

struct UnkAttack40_l5fb718 : UnkAttack_l5fb718 {
    u8 pad_0x38[0x8];
};

struct UnkAttack50_l5fb718 : UnkAttack_l5fb718 {
    u8 pad_0x38[0x18];
};

// An attack with a listener at +0x30.
struct UnkListeningAttackA_l5fb718 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkAttackListenerRaw listener;  // 0x30
    u8 pad_0x50[0x18];

    ~UnkListeningAttackA_l5fb718() {
        vtable = g_unk_7101a54100 + 2;
        listener.Destroy();
        fn_71003606c0(this);
    }
};

// An attack with a listener at +0x38.
struct UnkListeningAttackB_l5fb718 {
    void** vtable;
    u8 pad_0x8[0x30];
    UnkAttackListenerRaw listener;  // 0x38

    ~UnkListeningAttackB_l5fb718() {
        vtable = g_unk_7101a53e10 + 2;
        listener.Destroy();
        fn_71003606c0(this);
    }
};

// An attack with a listener at +0x48.
struct UnkListeningAttackC_l5fb718 {
    void** vtable;
    u8 pad_0x8[0x40];
    UnkAttackListenerRaw listener;  // 0x48

    ~UnkListeningAttackC_l5fb718() {
        vtable = g_unk_7101a53d18 + 2;
        listener.Destroy();
        fn_71003606c0(this);
    }
};

struct UnkKraidAI_l5fb718 {
    void** vtable;
    u8 pad_0x8[0x149e8 - 0x8];
    base::global::CRntVector<u64> values;              // 0x149e8
    u8 pad_0x14a10[0x8];
    UnkListLink_l5fb718 link_a;                        // 0x14a18
    u8 pad_0x14a30[0x8];
    UnkListeningAttackA_l5fb718 attack_a;              // 0x14a38
    UnkAttack_l5fb718 attack_b;                        // 0x14aa0
    UnkListeningAttackB_l5fb718 attack_c;              // 0x14ad8
    UnkAttack50_l5fb718 attack_d;                      // 0x14b30
    UnkAttack40_l5fb718 attack_e;                      // 0x14b80
    UnkAttack40_l5fb718 attack_f;                      // 0x14bc0
    UnkListeningAttackC_l5fb718 attack_g;              // 0x14c00
    UnkAttack_l5fb718 attack_h;                        // 0x14c68
    UnkKraidCallbackSet_l5fb718 set_a;                 // 0x14ca0
    u8 pad_0x14ce8[0x8];
    UnkKraidCallbackSet_l5fb718 set_b;                 // 0x14cf0
    u8 pad_0x14d38[0x8];
    UnkKraidCallbackList_l5fb718 callbacks_a;          // 0x14d40
    UnkKraidCallbackList_l5fb718 callbacks_b;          // 0x14d80
    UnkKraidCallbackList_l5fb718 callbacks_c;          // 0x14dc0
    u8 pad_0x14e00[0x8];
    UnkKraidListener_l5fb718 listener;                 // 0x14e08
    u8 pad_0x14e28[0x10];
    UnkListLink_l5fb718 link_b;                        // 0x14e38
    UnkListLink_l5fb718 link_c;                        // 0x14e50
};
static_assert(__builtin_offsetof(UnkKraidAI_l5fb718, link_a) == 0x14a18, "layout");
static_assert(__builtin_offsetof(UnkKraidAI_l5fb718, attack_h) == 0x14c68, "layout");
static_assert(__builtin_offsetof(UnkKraidAI_l5fb718, callbacks_a) == 0x14d40, "layout");
static_assert(__builtin_offsetof(UnkKraidAI_l5fb718, listener) == 0x14e08, "layout");
static_assert(__builtin_offsetof(UnkKraidAI_l5fb718, link_c) == 0x14e50, "layout");

void fn_71005fb718(UnkKraidAI_l5fb718* self) {
    self->vtable = g_unk_7101a54408 + 2;
    self->link_c.~UnkListLink_l5fb718();
    self->link_b.~UnkListLink_l5fb718();
    self->listener.~UnkKraidListener_l5fb718();
    self->callbacks_c.~UnkKraidCallbackList_l5fb718();
    self->callbacks_b.~UnkKraidCallbackList_l5fb718();
    self->callbacks_a.~UnkKraidCallbackList_l5fb718();
    self->set_b.~UnkKraidCallbackSet_l5fb718();
    self->set_a.~UnkKraidCallbackSet_l5fb718();
    self->attack_h.~UnkAttack_l5fb718();
    self->attack_g.~UnkListeningAttackC_l5fb718();
    self->attack_f.~UnkAttack40_l5fb718();
    self->attack_e.~UnkAttack40_l5fb718();
    self->attack_d.~UnkAttack50_l5fb718();
    self->attack_c.~UnkListeningAttackB_l5fb718();
    self->attack_b.~UnkAttack_l5fb718();
    self->attack_a.~UnkListeningAttackA_l5fb718();
    self->link_a.~UnkListLink_l5fb718();
    self->values.Free();
    fn_71003ac388(self);
}
