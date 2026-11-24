// CSpawnGroupComponent unit (tools/tu_map.py unit 2703): fn_710111a458 sets whether the player is
// inside the group (+0x1b8). Entering runs the trigger (fn_710111b31c) with a temporary list; leaving
// detaches the group's targets and objects, releases both lists, calls the script's leave function
// and restarts the cooldown. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CPickableComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU341.h"
#include "game/UnkTU604.h"
#include "types.h"

struct UnkCollectList_ovb0065 {
    struct Node {
        u64 value;
        Node* next;
    };
    char* vtable;
    u32 count;
    Node* head;
    Node* tail;
    base::IAllocator* allocator;
    u32 flags;

    UnkCollectList_ovb0065() {
        vtable = reinterpret_cast<char*>(g_unk_7101a32680) + 0x10;
        count = 0;
        head = nullptr;
        tail = nullptr;
        allocator = GetDefaultAllocator();
        flags = 0x13;
    }
    ~UnkCollectList_ovb0065() {
        vtable = reinterpret_cast<char*>(g_unk_7101a32680) + 0x10;
        Node* n = head;
        if (!n)
            return;
        base::IAllocator* a = allocator;
        while (n) {
            Node* next = n->next;
            a->vf_0x48(n);
            n = next;
        }
    }
};

struct UnkSpawnGroupTrigger_ovb0065;
struct UnkSpawnGroupUpdate_ovb0065;
void fn_710111b31c(UnkSpawnGroupTrigger_ovb0065* self, UnkCollectList_ovb0065* list);

// The object a group's target refers to: its controller (+0x1b8).
struct UnkSpawnController_l11a458;
struct UnkSpawnControllerVtbl_l11a458 {
    void* slots[0x30];
    void (*vf_0x180)(UnkSpawnController_l11a458* self, bool a, bool b);
};
struct UnkSpawnController_l11a458 {
    UnkSpawnControllerVtbl_l11a458* vtable;
    u8 pad_0x8[0x68];
    s32 state;  // 0x70
};
struct UnkSpawnObject_l11a458 {
    u8 pad_0x0[0x1b8];
    UnkSpawnController_l11a458* controller;  // 0x1b8
};

// The actor's name (+0x10) and how often it was asked for (+0x44).
struct UnkSpawnActorName_l11a458 {
    u8 pad_0x0[0x10];
    const char* name;  // 0x10
    u8 pad_0x18[0x2c];
    u32 uses;  // 0x44

    const char* Get() {
        ++uses;
        return name ? name : g_unk_710162e2ed;
    }
};
struct UnkSpawnActor_l11a458 {
    u8 pad_0x0[0x30];
    UnkSpawnActorName_l11a458* name;  // 0x30
    u8 pad_0x38[0x1a8];
    void* script;  // 0x1e0
};

struct UnkSpawnGroupInside_l11a458;
struct UnkSpawnGroupInsideVtbl_l11a458 {
    void* slots[0x31];
    void (*vf_0x188)(UnkSpawnGroupInside_l11a458* self, void* target);
};

// A counted reference to an object whose +0x10 is the target.
struct UnkSpawnRef_l11a458 : base::UnkRefCounted {
    void* target;
};

static inline void* Target(const base::UnkRefPtrMember& ref) {
    return ref.ptr ? static_cast<UnkSpawnRef_l11a458*>(ref.ptr)->target : nullptr;
}

// Drops every reference in the list.
static inline void ReleaseAll(base::global::CRntVector<base::UnkRefPtrMember>& refs) {
    for (u64 i = 0; i < refs.count; ++i) {
        if (base::UnkRefCounted* p = refs.data[i].ptr) {
            s32& count = p->ref_count;
            if (--count == 0)
                delete p;
        }
    }
    refs.count = 0;
}

struct UnkSpawnGroupInside_l11a458 {
    UnkSpawnGroupInsideVtbl_l11a458* vtable;
    u8 pad_0x8[0x28];
    UnkSpawnActor_l11a458* actor;  // 0x30
    u8 pad_0x38[0x8];
    base::global::CRntVector<base::UnkRefPtrMember> objects;  // 0x40
    u8 pad_0x68[0x28];
    base::global::CRntVector<base::UnkRefPtrMember> targets;  // 0x90
    u8 pad_0xb8[0xc8];
    f32 cooldown;  // 0x180
    u32 left_at;   // 0x184
    u8 pad_0x188[0x10];
    s32 field_0x198;
    u8 pad_0x19c[0x1c];
    bool inside;          // 0x1b8
    bool call_on_leave;   // 0x1b9
    u8 pad_0x1ba[0x1ee];
    const char* on_leave;  // 0x3a8
};
static_assert(__builtin_offsetof(UnkSpawnGroupInside_l11a458, targets) == 0x90, "layout");
static_assert(__builtin_offsetof(UnkSpawnGroupInside_l11a458, inside) == 0x1b8, "layout");
static_assert(__builtin_offsetof(UnkSpawnGroupInside_l11a458, on_leave) == 0x3a8, "layout");

void fn_710111a458(UnkSpawnGroupUpdate_ovb0065* group, bool inside) {
    auto* self = reinterpret_cast<UnkSpawnGroupInside_l11a458*>(group);
    if (inside == self->inside)
        return;
    self->inside = inside;
    if (inside) {
        UnkCollectList_ovb0065 list;
        fn_710111b31c(reinterpret_cast<UnkSpawnGroupTrigger_ovb0065*>(self), &list);
        return;
    }

    for (base::UnkRefPtrMember *it = self->targets.data, *end = it + self->targets.count; it != end; ++it) {
        if (void* target = Target(*it))
            self->vtable->vf_0x188(self, target);
    }
    for (base::UnkRefPtrMember *it = self->objects.data, *end = it + self->objects.count; it != end; ++it) {
        if (auto* object = static_cast<UnkSpawnObject_l11a458*>(Target(*it))) {
            UnkSpawnController_l11a458* controller = object->controller;
            if (controller && controller->state != 2)
                controller->vtable->vf_0x180(controller, false, false);
            fn_7100232c68(object);
        }
    }
    ReleaseAll(self->targets);
    ReleaseAll(self->objects);
    self->field_0x198 = 0;

    UnkSpawnActor_l11a458* actor = self->actor;
    if (void* script = actor->script) {
        const char* on_leave = self->on_leave;
        if (on_leave && on_leave[0] && self->call_on_leave)
            fn_71009cde54(script, on_leave, "s", actor->name->Get());
    }
    if (self->cooldown > 1.1920929e-7f)
        self->left_at = g_unk_7101d47310->field_0x2444;
}
