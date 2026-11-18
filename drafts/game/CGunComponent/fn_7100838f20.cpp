// CGunComponent unit (tools/tu_map.py unit 1304): the gun component's destructor. It destroys the
// weapons of both group lists, then its members last first: the listener lists, the list set, the id
// and id list, the group lists, the "GUN_INPUT" listener, the two callbacks and the life handle, and
// ends in the component base's destructor. Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/UnkNamedRegistry.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CInputComponent.h"
#include "game/UnkComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CRntVector;
using base::global::CStrId;

extern void* g_unk_7101a77bc0[];
extern void* g_unk_7101a77dc8[];
struct UnkGunCallback_l838f20;
void fn_7100840f50(void* owner, UnkGunCallback_l838f20* callback);
void fn_71008410e0(void* owner, UnkGunCallback_l838f20* callback);

// A vector that frees its array when destroyed.
template <class T>
struct UnkOwnedVector_l838f20 : CRntVector<T> {
    ~UnkOwnedVector_l838f20() { this->Free(); }

    // Destroys the objects the elements point at, then empties the vector.
    void DestroyAll() {
        for (T* it = this->data; it != this->data + this->count; ++it) {
            if (*it) {
                (*it)->Destroy();
                *it = nullptr;
            }
        }
        this->Clear();
    }
};

// A weapon of a group; its slot 0x10 destroys it.
struct UnkGunWeapon_l838f20 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Destroy();  // 0x10
};

using UnkGunWeaponGroup_l838f20 = UnkOwnedVector_l838f20<UnkGunWeapon_l838f20*>;
using UnkGunGroups_l838f20 = CRntVector<UnkGunWeaponGroup_l838f20>;

// A callback registered with an owner (field_0x0) that calls `callback` with `object`.
struct UnkGunCallback_l838f20 {
    void* owner;
    void* object;
    bool (*callback)(void* object, bool value);
};

// The component whose actor's input component (+0x1d8) holds the listener.
struct UnkGunInputOwner_l838f20 {
    u8 pad_0x0[0x1d8];
    void* input;  // 0x1d8
};

// The gun's input listener ("GUN_INPUT").
struct UnkGunInput_l838f20 {
    void** vtable;
    base::UnkNamedEntry name;  // 0x8; a CRntString
    u64 capacity;              // 0x38
    void* entries;             // 0x40; after their count
    UnkBaseComponent* target;  // 0x48
};

// Three pointer lists and a lock count behind a vtable of their own.
struct UnkGunListeners_l838f20 {
    void** vtable;
    CRntVector<void*> added;    // 0x8
    CRntVector<void*> removed;  // 0x30
    CRntVector<void*> items;    // 0x58
    s32 lock;                   // 0x80
};

// Three lists; the first holds the addresses of slots that refer back to the gun.
struct UnkGunListSet_l838f20 {
    CRntVector<void**> slots;
    CRntVector<void*> field_0x28;
    CRntVector<void*> field_0x50;

    void Release() {
        void*** end = slots.data + slots.count;
        for (void*** it = slots.data; it != end; ++it)
            **it = nullptr;
        slots.Clear();
        field_0x28.Clear();
        field_0x50.Clear();
        field_0x50.Free();
        field_0x28.Free();
        slots.Free();
    }
};

struct UnkGunComponent_l838f20 : UnkBaseComponent {
    UnkWeakHandle* life;  // 0x40
    UnkGunCallback_l838f20 field_0x48;
    UnkGunCallback_l838f20 field_0x60;
    UnkGunInput_l838f20 input;      // 0x78
    UnkGunGroups_l838f20 primary;    // 0xc8
    UnkGunGroups_l838f20 secondary;  // 0xf0
    u8 pad_0x118[0x28];
    CRntVector<CStrId> ids;  // 0x140
    CStrId id;               // 0x168
    u8 pad_0x170[0x28];
    UnkGunListSet_l838f20 lists;  // 0x198
    u8 pad_0x210[0x10];
    UnkGunListeners_l838f20 listeners;  // 0x220
};
static_assert(__builtin_offsetof(UnkGunComponent_l838f20, ids) == 0x140, "layout");
static_assert(__builtin_offsetof(UnkGunComponent_l838f20, lists) == 0x198, "layout");
static_assert(__builtin_offsetof(UnkGunComponent_l838f20, listeners) == 0x220, "layout");

// The input component of the component's actor.
static inline void* InputOf(UnkBaseComponent* component) {
    return static_cast<UnkGunInputOwner_l838f20*>(component->field_0x30)->input;
}

// Destroys every weapon of the groups and empties each group.
inline void DestroyWeapons(UnkGunGroups_l838f20* groups) {
    for (UnkGunWeaponGroup_l838f20* group = groups->data; group != groups->data + groups->count; ++group)
        group->DestroyAll();
}

void fn_7100838f20(UnkGunComponent_l838f20* self) {
    self->vtable = g_unk_7101a77bc0 + 2;
    DestroyWeapons(&self->primary);
    DestroyWeapons(&self->secondary);
    self->primary.Clear();
    self->secondary.Clear();

    self->listeners.vtable = g_unk_7101a77dc8 + 2;
    self->listeners.items.Free();
    self->listeners.removed.Free();
    self->listeners.added.Free();
    self->lists.Release();
    self->id.Reset();
    self->ids.Free();
    self->secondary.Free();
    self->primary.Free();

    UnkGunInput_l838f20& input = self->input;
    input.vtable = g_unk_7101a22858 + 2;
    if (input.target && input.target->field_0x30 && InputOf(input.target))
        fn_7100872908(InputOf(input.target), &input);
    input.target = nullptr;
    input.vtable = g_unk_7101a22878 + 2;
    if (input.entries) {
        fn_7100083c30(static_cast<u64*>(input.entries) - 1);
        input.entries = nullptr;
    }
    fn_710007ecd4(reinterpret_cast<base::global::CRntString*>(&input.name));

    if (self->field_0x60.owner) {
        fn_71008410e0(self->field_0x60.owner, &self->field_0x60);
        self->field_0x60.owner = nullptr;
    }
    self->field_0x60.object = nullptr;
    self->field_0x60.callback = nullptr;
    if (self->field_0x48.owner) {
        fn_7100840f50(self->field_0x48.owner, &self->field_0x48);
        self->field_0x48.owner = nullptr;
    }
    self->field_0x48.object = nullptr;
    self->field_0x48.callback = nullptr;

    if (UnkWeakHandle* life = self->life) {
        if (--life->ref_count == 0)
            life->vf_0x10();
    }
    fn_71010c4194(self);
}
