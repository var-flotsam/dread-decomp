// CGunComponent unit (tools/tu_map.py unit 1304): creates a secondary gun by its reflected class name,
// sets its ids, handles and settings, and adds it to the gun component's list for the gun's slot.
// Names and layouts are placeholders.
#include <new>

#include "base/UnkCloneHelpers.h"
#include "base/animation/CBlendSpaceResource.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkStrIdHandle.h"
#include "game/UnkTU83.h"
#include "types.h"

using base::global::CRntVector;
using base::global::CStrId;

void fn_7100ae986c(UnkStrIdHandle* self, const CStrId& id);

// A handle built in place from an id by fn_7100ae986c, released when it goes out of scope.
class UnkStrIdHandleTemp_l83b83c {
public:
    explicit UnkStrIdHandleTemp_l83b83c(const CStrId& id) { fn_7100ae986c(get(), id); }
    ~UnkStrIdHandleTemp_l83b83c() { get()->id.~CStrId(); }
    UnkStrIdHandle* get() { return reinterpret_cast<UnkStrIdHandle*>(storage); }

private:
    alignas(UnkStrIdHandle) char storage[sizeof(UnkStrIdHandle)];
};

// Sets a handle field to the handle for `id`.
inline void SetHandle_l83b83c(UnkStrIdHandle& dst, const CStrId& id) {
    UnkStrIdHandleTemp_l83b83c handle(id);
    dst.id = handle.get()->id;
    dst.field_0x8 = handle.get()->field_0x8;
}

// A reflected type as the factory reads it: the object's size and its in-place constructor.
struct UnkGunType_l83b83c : base::reflection::UnkReflObject {
    u8 pad_0x8[0x10];
    u64 size;  // 0x18
    u8 pad_0x20[0x8];
    void (*construct)(void* object);  // 0x28
};

// A secondary gun (CSecondaryGun or a class derived from it).
struct UnkSecondaryGun_l83b83c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual u32 vf_0xe8();  // the gun's slot
    virtual void vf_0xf0(s32 value);
    virtual void vf_0xf8();
    virtual void vf_0x100(s32 value);
    virtual void vf_0x108();
    virtual void vf_0x110();
    virtual void vf_0x118();
    virtual void vf_0x120();
    virtual void vf_0x128();
    virtual void vf_0x130();
    virtual void vf_0x138();
    virtual void vf_0x140();
    virtual void vf_0x148();
    virtual void vf_0x150();
    virtual void vf_0x158();
    virtual void vf_0x160();
    virtual void vf_0x168();
    virtual void vf_0x170();
    virtual void vf_0x178();
    virtual void vf_0x180();
    virtual void vf_0x188();
    virtual void vf_0x190();
    virtual void vf_0x198();
    virtual void vf_0x1a0();
    virtual void vf_0x1a8();
    virtual void vf_0x1b0();
    virtual void vf_0x1b8(const CStrId& id);

    u8 pad_0x8[0x8];
    CStrId field_0x10;
    void* field_0x18;
    CStrId field_0x20;
    UnkStrIdHandle field_0x28;
    u8 pad_0x38[0x28];
    u64 field_0x60;
    u64 field_0x68;
    CStrId field_0x70;
    u8 pad_0x78[0x1880 - 0x78];
    UnkStrIdHandle field_0x1880;
    UnkStrIdHandle field_0x1890;
};
static_assert(__builtin_offsetof(UnkSecondaryGun_l83b83c, field_0x70) == 0x70, "layout");
static_assert(__builtin_offsetof(UnkSecondaryGun_l83b83c, field_0x1890) == 0x1890, "layout");

void fn_71008359f4(UnkSecondaryGun_l83b83c* gun, f32 value);

using UnkGunList_l83b83c = CRntVector<UnkSecondaryGun_l83b83c*>;
using UnkGunSlots_l83b83c = CRntVector<UnkGunList_l83b83c>;

// Grows the slot lists to n (fn_7100841b0c, a copy of base::refl_ResizeSubVecs).
struct UnkClone_fn_7100841b0c;
extern template void base::refl_ResizeSubVecs<UnkClone_fn_7100841b0c>(
    CRntVector<base::UnkCloneSubVec<UnkClone_fn_7100841b0c>>* v, u32 n);

inline void GrowSlots_l83b83c(UnkGunSlots_l83b83c& slots, u32 n) {
    base::refl_ResizeSubVecs(reinterpret_cast<CRntVector<base::UnkCloneSubVec<UnkClone_fn_7100841b0c>>*>(&slots), n);
}

// Resizes the slot lists to n, freeing the lists dropped.
inline void ResizeSlots_l83b83c(UnkGunSlots_l83b83c& slots, u32 n) {
    if (slots.count < n) {
        GrowSlots_l83b83c(slots, n);
    } else if (slots.count > n) {
        for (u32 i = n; i < slots.count; ++i)
            slots.data[i].Free();
        slots.count = n;
    }
}

// The list at `index`, growing the slot lists to it first (to the next multiple of the step).
inline UnkGunList_l83b83c& SlotAt_l83b83c(UnkGunSlots_l83b83c& slots, u32 index) {
    if (slots.field_0x14 && slots.count <= index) {
        u32 cap = slots.capacity;
        if (cap <= index && slots.step != 0) {
            u32 grown = (index / slots.step + 1) * slots.step;
            if (cap < grown) {
                UnkGunList_l83b83c* p = static_cast<UnkGunList_l83b83c*>(
                    slots.allocator->vf_0x40((u64)grown * sizeof(UnkGunList_l83b83c), 8, slots.flags));
                if (slots.data) {
                    UnkGunList_l83b83c* src = slots.data;
                    UnkGunList_l83b83c* dst = p;
                    for (u32 i = 0; i < slots.count; ++i, ++dst, ++src)
                        new (dst) UnkGunList_l83b83c(static_cast<UnkGunList_l83b83c&&>(*src));
                    slots.allocator->vf_0x48(slots.data);
                }
                slots.data = p;
                slots.capacity = grown;
            }
        }
        GrowSlots_l83b83c(slots, index + 1);
    }
    return slots.data[index];
}

// Appends a gun to a slot's list, growing it as CRntVector::PushBack does; the count is read again
// only once the new array and capacity are stored.
inline void Append_l83b83c(UnkGunList_l83b83c& list, UnkSecondaryGun_l83b83c* gun) {
    u32 n = list.count;
    u32 old_capacity = list.capacity;
    u32 new_capacity = old_capacity;
    if (old_capacity <= n && list.step != 0)
        new_capacity = (n / list.step + 1) * list.step;
    if ((s32)n >= (s32)new_capacity)
        for (;;) {
        }
    UnkSecondaryGun_l83b83c** p;
    if (old_capacity < new_capacity) {
        p = static_cast<UnkSecondaryGun_l83b83c**>(
            list.allocator->vf_0x40((u64)new_capacity * sizeof(*p), 8, list.flags));
        if (list.data) {
            __builtin_memcpy(p, list.data, (u64)list.count * sizeof(*p));
            list.allocator->vf_0x48(list.data);
        }
        list.data = p;
        list.capacity = new_capacity;
        n = list.count;
    } else {
        p = list.data;
    }
    p[n] = gun;
    ++list.count;
}

struct UnkGunComponent_l83b83c {
    u8 pad_0x0[0x30];
    void* field_0x30;
    u8 pad_0x38[0xf0 - 0x38];
    UnkGunSlots_l83b83c secondary;  // 0xf0
};
static_assert(__builtin_offsetof(UnkGunComponent_l83b83c, secondary) == 0xf0, "layout");

// Looks up a reflected type by its name's 64-bit id.
inline const UnkGunType_l83b83c* FindType_l83b83c(const char* name) {
    UnkVersionRegistry* registry = g_unk_7101d4c8d0;
    u64 id = StringCrc64(name, __builtin_strlen(name));
    return reinterpret_cast<const UnkGunType_l83b83c*>(fn_71000a5118(registry, &id));
}

UnkSecondaryGun_l83b83c* fn_710083b83c(char* self_, const char* a, const char* b, const char* c, f32 value,
                                    const char* d, const char* e, const char* f, const char* g,
                                    const char* h, s32 i, s32 j) {
    UnkGunComponent_l83b83c* self = reinterpret_cast<UnkGunComponent_l83b83c*>(self_);

    const UnkGunType_l83b83c* type = FindType_l83b83c(b);
    if (!type)
        return nullptr;
    if (!fn_710009d50c(type->vf_0x0(), fn_7100096100()))
        return nullptr;
    if (!fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(type), fn_710085c56c()))
        return nullptr;
    if (!type->construct)
        return nullptr;
    void* object = GetDefaultAllocator()->vf_0x40(type->size, 8, 5);
    if (!object)
        return nullptr;
    type->construct(object);
    UnkSecondaryGun_l83b83c* gun = static_cast<UnkSecondaryGun_l83b83c*>(object);

    {
        CStrId name(a, true);
        gun->field_0x20 = name;
    }
    gun->field_0x18 = self->field_0x30;
    {
        CStrId name(c, true);
        gun->field_0x10 = name;
    }
    fn_71008359f4(gun, value);
    SetHandle_l83b83c(gun->field_0x28, CStrId(d, true));
    SetHandle_l83b83c(gun->field_0x1880, CStrId(e, true));
    SetHandle_l83b83c(gun->field_0x1890, CStrId(f, true));
    gun->vf_0x1b8(CStrId(g, true));
    {
        CStrId name(h, true);
        gun->field_0x70 = name;
    }
    gun->vf_0xf0(i);
    gun->vf_0x100(j);
    gun->field_0x60 = StringCrc64("WaveBeamChargeBurst", 19);
    gun->field_0x68 = StringCrc64("WaveBeamBurstChargeBeamCharging", 31);

    if (self->secondary.count <= gun->vf_0xe8())
        ResizeSlots_l83b83c(self->secondary, gun->vf_0xe8() + 1);
    Append_l83b83c(SlotAt_l83b83c(self->secondary, gun->vf_0xe8()), gun);
    return gun;
}
