// CSpawnGroupComponent unit (tools/tu_map.py unit 2703; the name from reflection): the group's
// holders of lazily resolved handles, its counters stored in the blackboard, its spawned actors'
// weak handles, its update and its notifications. Each section below says what its functions do.
// Names and layouts are placeholders.
#include <new>

#include "base/UnkErasedVector.h"
#include "base/UnkRefHandle.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CAISmartObjectComponent.h"
#include "game/CActorComponent.h"
#include "game/CBatalloonAIComponent.h"
#include "game/CBlackboard.h"
#include "game/CComponent.h"
#include "game/CPickableComponent.h"
#include "game/CScenario.h"
#include "game/CSpawnGroupComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2704.h"
#include "game/UnkTU341.h"
#include "game/UnkTU604.h"
#include "game/UnkTUProperty.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::reflection::UnkReflValue;
using base::global::CStrId;

// CSpawnGroupComponent unit (tools/tu_map.py): reads the object behind the lazily resolved handle at
// +0x38 of the holder (source +0x8, state +0x30; 0x40 bytes), the shape of fn_7100407018
// (src/game/CCentralUnitAIComponent/CCentralUnitAIComponent_Value_b49.cpp), as a reflected value of
// the type fn_71011201e4 describes, holding a counted reference to it; fn_710111f280 passes the objects of
// the component's holders (+0x68, count +0x70) to fn_710112bec4 while the flag at +0x139 is set, then
// clears it. Names and layouts are
// placeholders.


struct UnkEnumType;
UnkEnumType* fn_71011201e4();

struct UnkCountedSpawn_ovb0065 : base::UnkRefCounted {
    void* field_0x10;
};

struct UnkRef {
    UnkCountedSpawn_ovb0065* ptr;
    ~UnkRef() {
        if (ptr && --ptr->ref_count == 0)
            delete ptr;
    }
};

// The counted reference fn_71003df480 returns (as src/game/CSpawnPointComponent/CSpawnPointComponent_c3_1.cpp).
struct UnkRefObj_c3 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();  // destroys the object
    s32 refs;
};

struct UnkRef_c3 {
    UnkRefObj_c3* p;
    ~UnkRef_c3() {
        if (p && --p->refs == 0)
            p->vf_0x10();
    }
};

base::UnkRefHandle* fn_71006eb8cc(base::UnkRefHandle* self, const base::UnkRefHandle* other);

struct UnkSpawnHolder_ovb0065 {
    u8 pad_0x0[0x8];
    u8 source[0x28];
    u32 state;
    u32 pad_0x34;
    base::UnkRefHandle handle;
};

// Resolves the handle from its source on first use.
static inline void Resolve(UnkSpawnHolder_ovb0065* holder) {
    if (holder->state == 1) {
        {
            UnkRef_c3 fresh = fn_71003df480(holder->source);
            fn_71006eb8cc(&holder->handle, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
        }
        holder->state = 2;
    }
}

UnkReflValue fn_710112008c(UnkSpawnHolder_ovb0065* holder) {
    Resolve(holder);
    UnkRef ref;
    ref.ptr = nullptr;
    if (auto* p = static_cast<UnkCountedSpawn_ovb0065*>(holder->handle.ptr)) {
        ++p->ref_count;
        ref.ptr = p;
    }
    auto* type = reinterpret_cast<const base::reflection::CType*>(fn_71011201e4());
    UnkReflValue value;
    value.kind = 2;
    value.field_0x0 = nullptr;
    value.type = nullptr;
    fn_71000a18d4(&value, type, &ref, 0);
    return value;
}

void fn_710112bec4(void* object, void* arg);

// The group's owner (+0x18).
struct UnkSpawnGroupOwner_ovb0065 {
    u8 pad_0x0[0x1a8];
    void* field_0x1a8;
    u8 pad_0x1b0[2];
    bool field_0x1b2;
};

struct UnkSpawnGroupHolders_ovb0065 {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkSpawnGroupOwner_ovb0065* owner;  // 0x18
    u8 pad_0x20[0x48];
    UnkSpawnHolder_ovb0065* holders;  // 0x68
    u32 count;                        // 0x70
    u8 pad_0x74[0xc5];
    bool pending;  // 0x139
};

void fn_710111f280(UnkSpawnGroupHolders_ovb0065* self, void* arg) {
    if (!self->pending)
        return;
    for (UnkSpawnHolder_ovb0065 *it = self->holders, *end = it + self->count; it != end; ++it) {
        Resolve(it);
        auto* p = static_cast<UnkCountedSpawn_ovb0065*>(it->handle.ptr);
        fn_710112bec4(p ? p->field_0x10 : nullptr, arg);
    }
    self->pending = false;
}


struct UnkActivatable_b48;
void fn_710112bec0(void* object, void* arg);

void fn_710111f184(UnkSpawnGroupHolders_ovb0065* self, void* arg) {
    if (!fn_7100deabc4(g_unk_7101d47310))
        return;
    if (!fn_710024fb2c(reinterpret_cast<UnkActivatable_b48*>(self)))
        return;
    for (UnkSpawnHolder_ovb0065 *it = self->holders, *end = it + self->count; it != end; ++it) {
        Resolve(it);
        auto* p = static_cast<UnkCountedSpawn_ovb0065*>(it->handle.ptr);
        fn_710112bec0(p ? p->field_0x10 : nullptr, arg);
    }
    self->pending = true;
}

void fn_7101119e4c(UnkSpawnGroupHolders_ovb0065* self) {
    CBlackboard* blackboard = &g_unk_7101d47310->field_0x968;
    reinterpret_cast<void (*)(UnkSpawnGroupHolders_ovb0065*, CBlackboard*)>(self->vtable[0x140 / 8])(self, blackboard);
    if (!self->owner->field_0x1a8)
        return;
    for (UnkSpawnHolder_ovb0065 *it = self->holders, *end = it + self->count; it != end; ++it) {
        Resolve(it);
        auto* p = static_cast<UnkCountedSpawn_ovb0065*>(it->handle.ptr);
        fn_710112bec0(p ? p->field_0x10 : nullptr, self->owner->field_0x1a8);
    }
    self->pending = true;
}

// The class id the game manager lists the groups under, and the class's type (TU data).
struct UnkSpawnGroupKey;
extern base::reflection::CType g_unk_7172a067c8 __attribute__((visibility("hidden")));

struct UnkSpawnKeyData_ovb0065 {
    u8 pad_0x0[0x30];
    u64 id;  // 0x30
};

struct UnkSpawnKeyed_ovb0065 {
    u8 pad_0x0[0x48];
    UnkSpawnKeyData_ovb0065* key;  // 0x48
};

struct UnkSpawnWeakHandle_ovb0065 {
    u8 pad_0x0[0x10];
    base::reflection::UnkReflObject* target;  // 0x10
};

// Appends to `out` every registered group (the game manager's list under the class id, of the class)
// that has a holder whose object's key id (+0x48, its +0x30) is `id`.
void fn_710111952c(const u64* id, base::UnkVector<UnkSpawnGroupHolders_ovb0065*>* out) {
    UnkGameManagerIdList* list = fn_7100de94cc(g_unk_7101d47310, reinterpret_cast<const u64*>(&g_unk_7172a067c0));
    for (void **it = list->data, **end = it + list->count; it != end; ++it) {
        auto* handle = static_cast<UnkSpawnWeakHandle_ovb0065*>(*it);
        if (!handle)
            continue;
        base::reflection::UnkReflObject* obj = handle->target;
        if (!obj)
            continue;
        const base::reflection::CType* type = obj->vf_0x0();
        fn_7101117478();
        if (!fn_710009d50c(type, &g_unk_7172a067c8))
            continue;
        auto* group = reinterpret_cast<UnkSpawnGroupHolders_ovb0065*>(obj);
        for (UnkSpawnHolder_ovb0065 *h = group->holders, *hend = h + group->count; h != hend; ++h) {
            Resolve(h);
            auto* p = static_cast<UnkCountedSpawn_ovb0065*>(h->handle.ptr);
            UnkSpawnKeyData_ovb0065* key = static_cast<UnkSpawnKeyed_ovb0065*>(p->field_0x10)->key;
            u64 want = *id;
            if (key->id == want) {
                out->PushBack(group);
                break;
            }
        }
    }
}

// CSpawnGroupComponent unit (tools/tu_map.py): a reflected call on the component behind the
// object (+0x8) that fills a temporary list (vtable g_unk_7101a32680, nodes from the default
// allocator with flags 0x13) through fn_710111b230, frees it again and returns an empty value, as
// fn_71011199cc (CSpawnGroupComponent_Collect_b51.cpp) does. Names and layouts are placeholders.


struct UnkCollectList_ovb0065 {
    struct Node {
        u64 value;
        Node* next;
    };
    char* vtable;
    u32 count;
    Node* head;
    Node* tail;
    base::UnkAllocator* allocator;
    u32 flags;

    UnkCollectList_ovb0065() {
        vtable = reinterpret_cast<char*>(g_unk_7101a32680) + 0x10;
        count = 0;
        head = nullptr;
        tail = nullptr;
        allocator = fn_7100083990();
        flags = 0x13;
    }
    ~UnkCollectList_ovb0065() {
        vtable = reinterpret_cast<char*>(g_unk_7101a32680) + 0x10;
        Node* n = head;
        if (!n)
            return;
        base::UnkAllocator* a = allocator;
        while (n) {
            Node* next = n->next;
            a->vf_0x48(n);
            n = next;
        }
    }
};

struct UnkComponent_71011199cc;
struct UnkList_71011199cc;
void fn_710111b230(UnkComponent_71011199cc* self, __attribute__((noescape)) UnkList_71011199cc* out);

struct UnkCollectArgs_ovb0065 {
    u8 pad_0x0[0x8];
    UnkComponent_71011199cc* component;  // 0x8
};

UnkReflValue fn_710111fc58(void*, UnkCollectArgs_ovb0065* args) {
    UnkComponent_71011199cc* component = args->component;
    {
        UnkCollectList_ovb0065 list;
        fn_710111b230(component, reinterpret_cast<UnkList_71011199cc*>(&list));
    }
    UnkReflValue r;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    r.kind = 2;
    r.field_0x14 = 1;
    return r;
}

struct UnkPlayer;

struct UnkSpawnGroupUpdate_ovb0065 {
    u8 pad_0x0[0x18];
    UnkSpawnGroupOwner_ovb0065* owner;  // 0x18
    u8 pad_0x20[0x118];
    bool field_0x138;
    u8 pad_0x139;
    bool field_0x13a;
    u8 pad_0x13b;
    bool field_0x13c;
    u8 pad_0x13d;
    bool field_0x13e;
    bool field_0x13f;
    u8 pad_0x140[0x38];
    f32 field_0x178;
    f32 field_0x17c;
    f32 field_0x180;
    u8 pad_0x184[0xc];
    s32 field_0x190;
    u8 pad_0x194[4];
    s32 field_0x198;
    u8 pad_0x19c[4];
    bool field_0x1a0;
    u8 pad_0x1a1[3];
    s32 field_0x1a4;
    u8 pad_0x1a8[0x10];
    bool field_0x1b8;
};
static_assert(__builtin_offsetof(UnkSpawnGroupUpdate_ovb0065, field_0x1b8) == 0x1b8, "layout");

bool fn_710111ad64(UnkSpawnGroupUpdate_ovb0065* self);
void fn_710111a458(UnkSpawnGroupUpdate_ovb0065* self, bool value);

// Collects the group's entries through fn_710111b230 when the flag at +0x13e is set and neither
// +0x138 nor +0x13c is, then updates the player-inside state (fn_710111a458) while there is a player.
static inline void UpdateInside(UnkSpawnGroupUpdate_ovb0065* self) {
    if (!self->field_0x138 && self->field_0x13e && !self->field_0x13c) {
        UnkCollectList_ovb0065 list;
        fn_710111b230(reinterpret_cast<UnkComponent_71011199cc*>(self), reinterpret_cast<UnkList_71011199cc*>(&list));
    }
    if (fn_7100dec788(g_unk_7101d47310))
        fn_710111a458(self, fn_710111ad64(self));
}

void fn_710111a364(UnkSpawnGroupUpdate_ovb0065* self) {
    if (!self->owner->field_0x1b2)
        return;
    UpdateInside(self);
}

struct UnkParent;
void fn_710111a708(UnkSpawnGroupUpdate_ovb0065* self, bool flag, UnkList_71011199cc* out);

// The update: after the base update (fn_71010c424c) and UpdateInside, while the flags at +0x13a,
// +0x138 and +0x1b8 are set and +0x13f is not, the two timers (+0x178, and +0x17c while the counter
// +0x198 is below the limit +0x190) run on; once +0x17c reaches +0x180 (and unless +0x1a0 is set
// with +0x1a4 above 0) fn_710111a708 is run with a temporary list.
void fn_710111abb8(UnkSpawnGroupUpdate_ovb0065* self, f32 dt) {
    fn_71010c424c(reinterpret_cast<UnkParent*>(self));
    UpdateInside(self);
    if (!self->field_0x13a || !self->field_0x138 || !self->field_0x1b8 || self->field_0x13f)
        return;
    self->field_0x178 += dt;
    if (self->field_0x198 < self->field_0x190)
        self->field_0x17c += dt;
    if (self->field_0x17c < self->field_0x180)
        return;
    if (self->field_0x1a0 && self->field_0x1a4 > 0)
        return;
    UnkCollectList_ovb0065 list;
    fn_710111a708(self, false, reinterpret_cast<UnkList_71011199cc*>(&list));
}

// CSpawnGroupComponent unit (tools/tu_map.py unit 2703): fn_7101119a5c counts one more use in the
// group's counter (+0x19c) and stores it, as an int, in the given blackboard, in the section of the
// current scenario, under its property (the name looked up in the key list at +0x3f8, count +0x400,
// values at +0x3f0), unless the blackboard is locked; the shape of fn_7101119b7c
// (CSpawnGroupComponent_StoreFlag_b49.cpp). Names and layouts are placeholders.


extern UnkProperty g_unk_7172a06608;

// A typed value as the section setter takes it (as in src/game/CDoorLifeComponent.cpp).
struct UnkTypedValue {
    const base::reflection::CType* type;
    void* value;
};
void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);

struct UnkSpawnGroupValue_ovb0065 {
    union {
        CStrId id;
        UnkPropertyName name;
    };
    u64 field_0x8;
};

struct UnkSpawnGroupCount_ovb0065 {
    u8 pad_0x0[0x19c];
    s32 count;  // 0x19c
    u8 pad_0x1a0[0x250];
    UnkSpawnGroupValue_ovb0065* values;  // 0x3f0
    u64* keys;                           // 0x3f8
    u32 key_count;                       // 0x400
};

// Stores the int at `value` under `name` in the blackboard section `section` names (unless the
// blackboard is locked).
static inline void SetSectionInt(CBlackboard* blackboard, const CStrId& section, UnkPropertyName* name, s32* value) {
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = fn_7100014768();
            v.value = value;
            fn_7100332c80(s, name, &v);
        }
    }
}

void fn_7101119a5c(UnkSpawnGroupCount_ovb0065* self, CBlackboard* blackboard) {
    CStrId section_id = fn_71011a2fbc(g_unk_7101d47310->field_0x608);
    u64 index = 0;
    u32 n = self->key_count;
    if (n) {
        u64* k = self->keys;
        u64 key = (u64)g_unk_7172a06608.name.data;
        for (; index != n; ++index) {
            if (k[index] == key)
                break;
        }
    }
    self->count++;
    UnkSpawnGroupValue_ovb0065* v = self->values;
    SetSectionInt(blackboard, section_id, &v[index].name, &self->count);
}

struct UnkSpawnActor_ovb0065 {
    u8 pad_0x0[0x30];
    CStrId id;  // 0x30
    u8 pad_0x38[0x17a];
    bool field_0x1b2;
    u8 pad_0x1b3[0x2d];
    void* script;  // 0x1e0
};

struct UnkSpawnGroupNotify_ovb0065 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkSpawnActor_ovb0065* owner;  // 0x30
    u8 pad_0x38[0x104];
    bool count_spawns;  // 0x13c
    u8 pad_0x13d[0x7c];
    bool call_script;  // 0x1b9
    u8 pad_0x1ba[0x12e];
    const char* script_function;  // 0x2e8
};

void fn_710111bc50(UnkSpawnGroupNotify_ovb0065* self);

// The text of an actor's id (the use count of its string is incremented first).
static inline const char* IdText(UnkSpawnActor_ovb0065* actor) {
    const char* text = base::reflection::refl_RntStringOf(actor->id).data;
    return text ? text : g_unk_710162e2ed;
}

void fn_710111bb74(UnkSpawnGroupNotify_ovb0065* self, const char* event, UnkSpawnActor_ovb0065* spawned) {
    UnkSpawnActor_ovb0065* owner = self->owner;
    if (void* script = owner->script) {
        const char* function = self->script_function;
        if (function && *function && self->call_script)
            fn_71009cde54(script, function, "sss", IdText(owner), event, IdText(spawned));
    }
    if (self->count_spawns)
        fn_7101119a5c(reinterpret_cast<UnkSpawnGroupCount_ovb0065*>(self), &g_unk_7101d47310->field_0x968);
    if (reinterpret_cast<bool (*)(UnkSpawnGroupNotify_ovb0065*)>(self->vtable[0x190 / 8])(self))
        fn_710111bc50(self);
}

bool fn_7100395a30(CBlackboard::CSection* section, const void* name, s32* out);

struct UnkSpawnGroupRead_ovb0065 {
    u8 pad_0x0[0x30];
    UnkSpawnActor_ovb0065* owner;  // 0x30
};

// The counter: the group's own while its owner's flag (+0x1b2) is set, else the one stored under
// its property in the current scenario's section of the blackboard (0 without one).
s32 fn_710111f07c(UnkSpawnGroupCount_ovb0065* self) {
    if (reinterpret_cast<UnkSpawnGroupRead_ovb0065*>(self)->owner->field_0x1b2)
        return self->count;
    s32 value = 0;
    {
        CBlackboard* blackboard = &g_unk_7101d47310->field_0x968;
        CStrId section_id = fn_71011a2fbc(g_unk_7101d47310->field_0x608);
        u64 index = 0;
        u32 n = self->key_count;
        if (n) {
            u64* k = self->keys;
            u64 key = (u64)g_unk_7172a06608.name.data;
            for (; index != n; ++index) {
                if (k[index] == key)
                    break;
            }
        }
        UnkSpawnGroupValue_ovb0065* v = self->values;
        if (CBlackboard::CSection* section = fn_7101047270(blackboard, section_id))
            fn_7100395a30(section, &v[index].name, &value);
    }
    return value;
}

// CSpawnGroupComponent unit (tools/tu_map.py): after the base step (fn_71010c4228), passes the
// owner's id (the object at +0x18, its +0x30) to fn_710111f418 on the list at +0x3e0 and registers
// the component with the game manager under the key g_unk_7172a067c0. Names and layouts are
// placeholders.


struct UnkParent;


struct UnkSpawnOwner_ovb0065 {
    u8 pad_0x0[0x30];
    CStrId id;  // 0x30
};

struct UnkSpawnGroupStart_ovb0065 {
    u8 pad_0x0[0x18];
    UnkSpawnOwner_ovb0065* owner;  // 0x18
    u8 pad_0x20[0x3c0];
    u8 field_0x3e0[0x10];
};

void fn_710111f418(void* list, CStrId id);

void fn_710111f35c(UnkSpawnGroupStart_ovb0065* self) {
    fn_71010c4228(reinterpret_cast<UnkParent*>(self));
    fn_710111f418(self->field_0x3e0, self->owner->id);
    fn_7100de9540(g_unk_7101d47310, reinterpret_cast<const u64*>(&g_unk_7172a067c0),
                  reinterpret_cast<UnkWeakTarget*>(self));
}

// CSpawnGroupComponent unit (tools/tu_map.py): calls a member function of the component on the
// object (+0x10) of every non-empty entry in one of its two handle arrays (+0x40/+0x48 and
// +0x90/+0x98). Names and layouts are placeholders.

struct UnkSpawnEntry_ovb0065 {
    u8 pad_0x0[0x10];
    void* object;  // 0x10
};

struct UnkSpawnEntryArray_ovb0065 {
    UnkSpawnEntry_ovb0065** data;
    u32 count;
};

struct UnkSpawnGroupEach_ovb0065 {
    u8 pad_0x0[0x40];
    UnkSpawnEntryArray_ovb0065 field_0x40;
    u8 pad_0x50[0x40];
    UnkSpawnEntryArray_ovb0065 field_0x90;
};

using UnkSpawnEachFn_ovb0065 = void (UnkSpawnGroupEach_ovb0065::*)(void*);

static inline void ForEach(UnkSpawnGroupEach_ovb0065* self, UnkSpawnEntryArray_ovb0065& array,
                           UnkSpawnEachFn_ovb0065 fn) {
    for (UnkSpawnEntry_ovb0065 **it = array.data, **end = it + array.count; it != end; ++it) {
        UnkSpawnEntry_ovb0065* e = *it;
        if (e && e->object)
            (self->*fn)(e->object);
    }
}

void fn_710111c3d8(UnkSpawnGroupEach_ovb0065* self, UnkSpawnEachFn_ovb0065 fn) {
    ForEach(self, self->field_0x90, fn);
}

void fn_710111c470(UnkSpawnGroupEach_ovb0065* self, UnkSpawnEachFn_ovb0065 fn) {
    ForEach(self, self->field_0x40, fn);
}

// CSpawnGroupComponent unit (tools/tu_map.py): moves a lazily resolved holder (0x40 bytes: vtable,
// the callback vector at +0x8, the state at +0x30, the refcounted handle at +0x38) into a newly
// constructed one: the base part (vtable g_unk_7101a20998) and then the holder's own (vtable
// g_unk_7101cb2880) are set up empty, the handle and the vector are taken over (fn_710023b29c) and the
// other is left empty (fn_7100ad3588). The shape of the callback sets' move
// (src/game_s7/fn_71011141e8.cpp). Names and layouts are placeholders.


extern void* g_unk_7101cb2880[];  // the holder's vtable (reached through the GOT)

struct UnkSpawnHolderBase_ovb0065 {
    char* vtable;
    UnkOwnedErasedVector callbacks;  // 0x8
    u32 state;                       // 0x30

    UnkSpawnHolderBase_ovb0065() : vtable(reinterpret_cast<char*>(g_unk_7101a20998) + 0x10), state(0) {}
};

struct UnkSpawnHolderMoved_ovb0065 : UnkSpawnHolderBase_ovb0065 {
    base::UnkRefHandle handle;  // 0x38

    __attribute__((always_inline)) UnkSpawnHolderMoved_ovb0065(UnkSpawnHolderMoved_ovb0065& other) {
        vtable = reinterpret_cast<char*>(g_unk_7101cb2880) + 0x10;
        handle.ptr = nullptr;
        handle.Set(other.handle.ptr);
        UnkErasedVector* from = &other.callbacks;
        state = other.state;
        fn_710023b29c(&callbacks, from);
        other.state = 0;
        fn_7100ad3588(&other.callbacks);
        if (base::UnkRefCounted* p = other.handle.ptr) {
            if (--p->ref_count == 0)
                delete p;
            other.handle.ptr = nullptr;
        }
    }
};

void fn_710111fd80(UnkSpawnHolderMoved_ovb0065* self, UnkSpawnHolderMoved_ovb0065* other) {
    new (self) UnkSpawnHolderMoved_ovb0065(*other);
}

// CSpawnGroupComponent unit (tools/tu_map.py): fn_710111c56c handles one of the group's spawned
// actors going away. While the group counts spawns (+0x194 above 0) and the actor's +0x1b8 is present
// and not in state 2, the object registered under the actor's id (fn_7100d7ba84; looked up with
// fn_7100deba8c) has its counter (+0x388 of fn_7100360044's result) cleared, or decremented unless the
// game manager's flag (+0x178) is set, and the group's two counters (+0x194, +0x198) drop by one
// without going below 0. The actor's +0x240 is then told (vtable slot 0x170) and the actor handed to
// fn_7100232c68. Names and layouts are placeholders.


struct UnkSpawnActorState_ovb0065 {
    u8 pad_0x0[0x70];
    s32 state;  // 0x70
};

struct UnkSpawnActorListener_ovb0065 {
    void** vtable;
};

struct UnkSpawnedActor_ovb0065 {
    u8 pad_0x0[0x1b8];
    UnkSpawnActorState_ovb0065* field_0x1b8;
    u8 pad_0x1c0[0x80];
    UnkSpawnActorListener_ovb0065* listener;  // 0x240
};

struct UnkSpawnStringData_ovb0065 {
    u8 pad_0x0[0x30];
    u64 id;  // 0x30
};

struct UnkSpawnRegistered_ovb0065 {
    u8 pad_0x0[0x388];
    s32 counter;  // 0x388
};

struct UnkGameManagerFlag_ovb0065 {
    u8 pad_0x0[0x178];
    bool field_0x178;
};

struct UnkSpawnGroupCounters_ovb0065 {
    u8 pad_0x0[0x194];
    s32 field_0x194;
    s32 field_0x198;
};

// Decrements `value` unless that would take it below 0.
static inline void DecrementToZero(s32& value) {
    s32 v = value - 1;
    if (v >= 0)
        value = v;
}

bool fn_710111c56c(UnkSpawnGroupCounters_ovb0065* self, UnkSpawnedActor_ovb0065* actor) {
    if (self->field_0x194 >= 1 && actor->field_0x1b8 && actor->field_0x1b8->state != 2) {
        const CStrId* name = fn_7100d7ba84(actor);
        if (name->data != &g_unk_7101d47108->empty) {
            UnkGameManager* manager = g_unk_7101d47310;
            u64 id;
            alignas(8) u32 flags;
            u32 flags_value = g_unk_71727bebb8;
            id = reinterpret_cast<UnkSpawnStringData_ovb0065*>(name->data)->id;
            flags = flags_value;
            if (void* object = fn_7100deba8c(manager, &id, &flags)) {
                UnkSpawnRegistered_ovb0065* registered = fn_7100360044(object);
                s32 counter = registered->counter;
                if (reinterpret_cast<UnkGameManagerFlag_ovb0065*>(g_unk_7101d47310)->field_0x178) {
                    if (counter)
                        registered->counter = 0;
                } else {
                    registered->counter = counter - 1;
                }
            }
        }
        DecrementToZero(self->field_0x194);
        DecrementToZero(self->field_0x198);
    }
    if (UnkSpawnActorListener_ovb0065* listener = actor->listener)
        reinterpret_cast<void (*)(UnkSpawnActorListener_ovb0065*, UnkSpawnGroupCounters_ovb0065*)>(
            listener->vtable[0x170 / 8])(listener, self);
    fn_7100232c68(actor);
    return true;
}

// CSpawnGroupComponent unit (tools/tu_map.py): fn_710111b96c adds a spawned actor to the group's
// list of weak handles (+0x40; base::UnkVector's layout): the entries that are gone are dropped first
// (fn_710111aeb8), then the actor's weak handle (created on first use) is appended, the array grown
// by fn_710031da18 as CBossCameraCtrl's targets are (src/game/CBossCameraCtrl/CBossCameraCtrl_Targets_ovb0008.cpp).
// Names and layouts are placeholders.

struct UnkHandlePtr_ovb0008;
template <class Tag>
struct UnkSlotArray;
struct UnkTag_710031da18;
void fn_710031da18(UnkSlotArray<UnkTag_710031da18>* v, u32 capacity);

struct UnkSpawnHandlePtr_ovb0065 {
    UnkWeakHandle* p;
};

struct UnkSpawnGroupList_ovb0065 {
    u8 pad_0x0[0x40];
    base::UnkVector<UnkSpawnHandlePtr_ovb0065> spawned;  // 0x40
};


void fn_710111aeb8(UnkSpawnGroupList_ovb0065* self);

// A reference held for the duration of the add.
struct UnkSpawnHandleRef_ovb0065 {
    UnkWeakHandle* p;
    UnkSpawnHandleRef_ovb0065(UnkWeakHandle* h) : p(h) {
        if (p)
            ++p->ref_count;
    }
    ~UnkSpawnHandleRef_ovb0065() {
        if (p && --p->ref_count == 0)
            p->vf_0x10();
    }
};

void fn_710111b96c(UnkSpawnGroupList_ovb0065* self, UnkWeakTarget* target) {
    fn_710111aeb8(self);
    UnkSpawnHandleRef_ovb0065 ref(GetWeakHandle(target));
    base::UnkVector<UnkSpawnHandlePtr_ovb0065>& v = self->spawned;
    u32 n = v.count;
    u32 capacity = v.capacity;
    if (capacity <= n && v.step != 0)
        capacity = (n / v.step + 1) * v.step;
    if ((s32)n >= (s32)capacity)
        for (;;) {
        }
    fn_710031da18(reinterpret_cast<UnkSlotArray<UnkTag_710031da18>*>(&v), capacity);
    // The slot is written bitwise: the original reloads the count after the writes.
    UnkWeakHandle** slot = &v.data[v.count].p;
    __builtin_memset(slot, 0, sizeof(*slot));
    UnkWeakHandle* p = ref.p;
    if (*slot != p) {
        if (p)
            ++p->ref_count;
        if (UnkWeakHandle* old = *slot) {
            if (--old->ref_count == 0)
                old->vf_0x10();
        }
        __builtin_memcpy(slot, &p, sizeof(p));
    }
    ++v.count;
}

// fn_710111b31c: the group's trigger, while it is enabled (+0x138, +0x1b8) and not blocked (+0x13f).
// The owner's script function named at +0x378 is called with the owner's id first (while +0x1b9 is
// set). A group that spawns on its own (+0x13a) restarts its timer (+0x178) and runs fn_710111a708 when
// its count +0x188 is above 0 or its delay (+0x180) is zero; any other collects into `list`, for each
// holder while the group's vtable slot 0x168 allows it, what fn_7101129b08 gives for the holder's object
// when the object's slot 0x168 accepts it.
struct UnkSpawnHolderObject_ovb0065 {
    void** vtable;
};

struct UnkObj_7101129240;
// The entry fn_7101129b08 returns (the declaration above, shared with src/game_s7/fn_7101129240.cpp,
// leaves the result out).
static inline void* EntryOf(UnkSpawnHolderObject_ovb0065* object) {
    return reinterpret_cast<void* (*)(UnkObj_7101129240*, bool)>(&fn_7101129b08)(
        reinterpret_cast<UnkObj_7101129240*>(object), false);
}

struct UnkSpawnGroupTrigger_ovb0065 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkSpawnActor_ovb0065* owner;  // 0x30
    u8 pad_0x38[0x30];
    UnkSpawnHolder_ovb0065* holders;  // 0x68
    u32 count;                        // 0x70
    u8 pad_0x74[0xc4];
    bool enabled;  // 0x138
    u8 pad_0x139;
    bool self_spawning;  // 0x13a
    u8 pad_0x13b[4];
    bool blocked;  // 0x13f
    u8 pad_0x140[0x38];
    f32 timer;  // 0x178
    u8 pad_0x17c[4];
    f32 delay;  // 0x180
    u8 pad_0x184[4];
    s32 field_0x188;
    u8 pad_0x18c[0x2c];
    bool field_0x1b8;
    bool call_script;  // 0x1b9
    u8 pad_0x1ba[0x1be];
    const char* script_function;  // 0x378
};
static_assert(__builtin_offsetof(UnkSpawnGroupTrigger_ovb0065, script_function) == 0x378, "layout");

void fn_710111b31c(UnkSpawnGroupTrigger_ovb0065* self, UnkCollectList_ovb0065* list) {
    if (!self->enabled || !self->field_0x1b8 || self->blocked)
        return;
    UnkSpawnActor_ovb0065* owner = self->owner;
    if (void* script = owner->script) {
        const char* function = self->script_function;
        if (function && *function && self->call_script)
            fn_71009cde54(script, function, "s", IdText(owner));
    }
    if (self->self_spawning) {
        self->timer = 0.0f;
        if (self->field_0x188 > 0 || self->delay < 1.1920929e-7f)
            fn_710111a708(reinterpret_cast<UnkSpawnGroupUpdate_ovb0065*>(self), true,
                          reinterpret_cast<UnkList_71011199cc*>(list));
        return;
    }
    for (UnkSpawnHolder_ovb0065 *it = self->holders, *end = it + self->count; it != end; ++it) {
        if (!reinterpret_cast<bool (*)(UnkSpawnGroupTrigger_ovb0065*)>(self->vtable[0x168 / 8])(self))
            return;
        Resolve(it);
        auto* object = static_cast<UnkSpawnHolderObject_ovb0065*>(
            static_cast<UnkCountedSpawn_ovb0065*>(it->handle.ptr)->field_0x10);
        if (!reinterpret_cast<bool (*)(UnkSpawnHolderObject_ovb0065*, f32)>(object->vtable[0x168 / 8])(object, -1.0f))
            continue;
        void* entry = EntryOf(object);
        if (!entry)
            continue;
        auto* node = static_cast<UnkCollectList_ovb0065::Node*>(list->allocator->vf_0x40(0x10, 8, list->flags));
        if (!node)
            continue;
        node->value = reinterpret_cast<u64>(entry);
        node->next = nullptr;
        if (list->count == 0) {
            list->head = node;
            list->tail = node;
            list->count = 1;
        } else {
            // Linked bitwise: the original reloads the count after these stores.
            __builtin_memcpy(&list->tail->next, &node, sizeof(node));
            __builtin_memset(&node->next, 0, sizeof(node->next));
            list->tail = node;
            ++list->count;
        }
    }
}
