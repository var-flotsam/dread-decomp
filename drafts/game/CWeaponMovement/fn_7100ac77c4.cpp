// CWeaponMovement unit (tools/tu_map.py unit 1624): drops the shots whose source's owner still has a
// target (fn_7100286ad4) from the array of 0x18-byte shots at +0x450, keeping the others in order. Names and layouts are placeholders.
#include <algorithm>
#include <new>

#include "base/UnkRefHandle.h"
#include "base/global/CRntVector.h"
#include "game/UnkTU357.h"
#include "../../game_s7/UnkIter.h"
#include "types.h"

struct UnkGunShots_ovb0070;
void fn_710085c338(UnkGunShots_ovb0070* shots, u32 n);

// What a shot refers to: the owner whose target is looked up.
struct UnkShotSource_lac77c4 : base::UnkRefCounted {
    void* owner;  // 0x10
};

// The counted reference a shot holds; assigned through the shared handle assignment.
struct UnkShotRef_lac77c4 {
    base::UnkRefHandle handle;

    UnkShotRef_lac77c4() { handle.ptr = nullptr; }
    UnkShotRef_lac77c4& operator=(const UnkShotRef_lac77c4& other) {
        fn_710031d208(&handle, &other.handle);
        return *this;
    }
    ~UnkShotRef_lac77c4() {
        if (base::UnkRefCounted* p = handle.ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
    }
    UnkShotSource_lac77c4* Get() const { return static_cast<UnkShotSource_lac77c4*>(handle.ptr); }
};

struct UnkWeaponShot_lac77c4 {
    UnkShotRef_lac77c4 ref;
    s32 field_0x8;
    s32 field_0xc;
    f32 field_0x10;

    UnkWeaponShot_lac77c4() : field_0x8(-0x7fffffff - 1), field_0xc(-0x7fffffff - 1), field_0x10(3.40282347e+38f) {}
};
static_assert(sizeof(UnkWeaponShot_lac77c4) == 0x18, "UnkWeaponShot_lac77c4 layout");

using UnkWeaponShots_lac77c4 = base::global::CRntVector<UnkWeaponShot_lac77c4>;

struct UnkWeaponMovement_lac77c4 {
    u8 pad_0x0[0x450];
    UnkWeaponShots_lac77c4 shots;  // 0x450
};

// Grows the array with default shots or destroys the shots past `n`.
static inline void Resize(UnkWeaponShots_lac77c4* shots, u32 n) {
    if (shots->count < n) {
        fn_710085c338(reinterpret_cast<UnkGunShots_ovb0070*>(shots), n);
        for (u32 i = shots->count; i < n; ++i)
            new (&shots->data[i]) UnkWeaponShot_lac77c4();
        shots->count = n;
    } else if (shots->count > n) {
        for (u64 i = n; i < shots->count; ++i)
            shots->data[i].~UnkWeaponShot_lac77c4();
        shots->count = n;
    }
}

void fn_7100ac77c4(UnkWeaponMovement_lac77c4* self) {
    UnkWeaponShots_lac77c4* shots = &self->shots;
    UnkIter<UnkWeaponShot_lac77c4> begin{shots->data};
    UnkIter<UnkWeaponShot_lac77c4> end{shots->data + shots->count};
    UnkIter<UnkWeaponShot_lac77c4> kept = std::remove_if(begin, end, [](const UnkWeaponShot_lac77c4& shot) {
        UnkShotSource_lac77c4* source = shot.ref.Get();
        return source && source->owner && fn_7100286ad4(source->owner);
    });
    Resize(shots, kept.p - shots->data);
}
