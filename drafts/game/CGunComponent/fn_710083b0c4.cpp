// CGunComponent unit (tools/tu_map.py unit 1304): creates a weapon of the class named `class_name`,
// which must be a reflected class deriving from CPrimaryGun with an in-place constructor, fills in its
// ids, names and settings, and appends it to the primary group its index names, growing the groups as
// needed. Returns the weapon, or null. Names and layouts are placeholders.
#include <new>

#include "base/animation/CBlendSpaceResource.h"
#include "base/global/CName.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTU83.h"
#include "types.h"

using base::global::CName;
using base::global::CRntVector;
using base::global::CStrId;

// An id and the object a registry has for it.
struct UnkStrIdHandle {
    CStrId id;
    void* field_0x8;
};
void fn_7100ae986c(UnkStrIdHandle* self, const CStrId& id);

// What a reflected class gives: the object's size and its in-place constructor.
struct UnkGunTypeView_l83b0c4 {
    u8 pad_0x0[0x18];
    u64 size;
    u8 pad_0x20[0x8];
    void (*construct)(void* object);
};

// A primary gun. Only the slots this function calls are named.
struct UnkGunWeapon_l83b0c4 {
    virtual void vf_0x0(); virtual void vf_0x8(); virtual void vf_0x10(); virtual void vf_0x18(); virtual void vf_0x20(); virtual void vf_0x28();
    virtual void vf_0x30(); virtual void vf_0x38(); virtual void vf_0x40(); virtual void vf_0x48(); virtual void vf_0x50(); virtual void vf_0x58();
    virtual void vf_0x60(); virtual void vf_0x68(); virtual void vf_0x70(); virtual void vf_0x78(); virtual void vf_0x80(); virtual void vf_0x88();
    virtual void vf_0x90(); virtual void vf_0x98(); virtual void vf_0xa0(); virtual void vf_0xa8(); virtual void vf_0xb0(); virtual void vf_0xb8();
    virtual void vf_0xc0(); virtual void vf_0xc8(); virtual void vf_0xd0(); virtual void vf_0xd8(); virtual void vf_0xe0();
    virtual u32 GroupIndex();         // 0xe8
    virtual void SetField0xf0(u32 value);   // 0xf0
    virtual void vf_0xf8();
    virtual void SetField0x100(u32 value);  // 0x100
    virtual void vf_0x108(); virtual void vf_0x110(); virtual void vf_0x118(); virtual void vf_0x120(); virtual void vf_0x128(); virtual void vf_0x130();
    virtual void vf_0x138(); virtual void vf_0x140(); virtual void vf_0x148(); virtual void vf_0x150(); virtual void vf_0x158(); virtual void vf_0x160();
    virtual void vf_0x168(); virtual void vf_0x170(); virtual void vf_0x178(); virtual void vf_0x180(); virtual void vf_0x188(); virtual void vf_0x190();
    virtual void vf_0x198(); virtual void vf_0x1a0(); virtual void vf_0x1a8(); virtual void vf_0x1b0();
    virtual void SetField0x1b8(const CStrId& id);  // 0x1b8

    u8 pad_0x8[0x8];
    CStrId field_0x10;
    void* owner;     // 0x18
    CStrId id;       // 0x20
    UnkStrIdHandle handle;  // 0x28
    u8 pad_0x38[0x28];
    CName field_0x60;
    CName field_0x68;
    CStrId field_0x70;

    void SetId(const CStrId& value) { id = value; }
    void SetField0x10(const CStrId& value) { field_0x10 = value; }
    void SetField0x70(const CStrId& value) { field_0x70 = value; }
};
static_assert(__builtin_offsetof(UnkGunWeapon_l83b0c4, field_0x70) == 0x70, "layout");

f32 fn_71008359f4(UnkGunWeapon_l83b0c4* weapon, f32 value);

// A group of weapons; destroying it frees its array.
struct UnkGunWeaponGroup_l83b0c4 : CRntVector<UnkGunWeapon_l83b0c4*> {
    UnkGunWeaponGroup_l83b0c4(UnkGunWeaponGroup_l83b0c4&& other)
        : CRntVector<UnkGunWeapon_l83b0c4*>(static_cast<CRntVector<UnkGunWeapon_l83b0c4*>&&>(other)) {}
    ~UnkGunWeaponGroup_l83b0c4() { Free(); }
};

using UnkGunGroups_l83b0c4 = CRntVector<UnkGunWeaponGroup_l83b0c4>;

// Resizes the groups to `n`, appending empty ones (out of line).
void fn_71008419cc(UnkGunGroups_l83b0c4* groups, u32 n);

struct UnkGunComponent_l83b0c4 {
    u8 pad_0x0[0x30];
    void* actor;  // 0x30
    u8 pad_0x38[0x90];
    UnkGunGroups_l83b0c4 primary;  // 0xc8
};
static_assert(__builtin_offsetof(UnkGunComponent_l83b0c4, primary) == 0xc8, "layout");

// Resizes the groups to `n`, appending empty groups or destroying the ones past it.
static inline void Resize(UnkGunGroups_l83b0c4& groups, u32 n) {
    if (groups.count < n)
        fn_71008419cc(&groups, n);
    else if (groups.count > n)
        groups.Truncate(n);
}

// The group at `index`, growing the groups to it first when they may grow.
static inline UnkGunWeaponGroup_l83b0c4& GroupAt(UnkGunGroups_l83b0c4& groups, u32 index) {
    if (groups.field_0x14 && groups.count <= index) {
        u32 cap = groups.capacity;
        if (cap <= index && groups.step != 0) {
            u32 grown = (index / groups.step + 1) * groups.step;
            if (cap < grown) {
                auto* p = static_cast<UnkGunWeaponGroup_l83b0c4*>(
                    groups.allocator->vf_0x40((u64)grown * sizeof(UnkGunWeaponGroup_l83b0c4), 8, groups.flags));
                if (groups.data) {
                    UnkGunWeaponGroup_l83b0c4* src = groups.data;
                    UnkGunWeaponGroup_l83b0c4* dst = p;
                    for (u32 i = 0; i < groups.count; ++i, ++dst, ++src) {
                        new (dst) UnkGunWeaponGroup_l83b0c4(static_cast<UnkGunWeaponGroup_l83b0c4&&>(*src));
                        src->~UnkGunWeaponGroup_l83b0c4();
                    }
                    groups.allocator->vf_0x48(groups.data);
                }
                groups.data = p;
                groups.capacity = grown;
            }
        }
        fn_71008419cc(&groups, index + 1);
    }
    return groups.data[index];
}

UnkGunWeapon_l83b0c4* fn_710083b0c4(UnkGunComponent_l83b0c4* self, const char* name, const char* class_name,
                                   const char* field_0x10, f32 value, const char* handle_name,
                                   const char* field_0x1b8, const char* field_0x60, const char* field_0x68,
                                   const char* field_0x70, const char* unused, u32 field_0xf0, u32 field_0x100) {
    UnkVersionEntry* entry;
    {
        UnkVersionRegistry* registry = g_unk_7101d4c8d0;
        CName type_name(class_name);
        entry = fn_71000a5118(registry, &type_name.hash);
    }
    if (!entry)
        return nullptr;
    if (!fn_710009d50c(reinterpret_cast<base::reflection::UnkReflObject*>(entry)->vf_0x0(), fn_7100096100()))
        return nullptr;
    if (!fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(entry), fn_71008570a4()))
        return nullptr;
    const auto* type = reinterpret_cast<const UnkGunTypeView_l83b0c4*>(entry);
    if (!type->construct)
        return nullptr;
    void* object = GetDefaultAllocator()->vf_0x40(type->size, 8, 5);
    if (!object)
        return nullptr;
    type->construct(object);
    auto* weapon = static_cast<UnkGunWeapon_l83b0c4*>(object);

    weapon->SetId(CStrId(name, true));
    weapon->owner = self->actor;
    weapon->SetField0x10(CStrId(field_0x10, true));
    fn_71008359f4(weapon, value);
    {
        CStrId id(handle_name, true);
        // fn_7100ae986c builds the handle in place; it is destroyed by hand.
        alignas(UnkStrIdHandle) char storage[sizeof(UnkStrIdHandle)];
        UnkStrIdHandle& handle = *reinterpret_cast<UnkStrIdHandle*>(storage);
        fn_7100ae986c(&handle, id);
        weapon->handle = handle;
        handle.~UnkStrIdHandle();
    }
    weapon->SetField0x1b8(CStrId(field_0x1b8, true));
    weapon->field_0x60 = CName(field_0x60);
    weapon->field_0x68 = CName(field_0x68);
    weapon->SetField0x70(CStrId(field_0x70, true));
    weapon->SetField0xf0(field_0xf0);
    weapon->SetField0x100(field_0x100);

    if (self->primary.count <= weapon->GroupIndex())
        Resize(self->primary, weapon->GroupIndex() + 1);
    GroupAt(self->primary, weapon->GroupIndex()).PushBack(weapon);
    return weapon;
}
