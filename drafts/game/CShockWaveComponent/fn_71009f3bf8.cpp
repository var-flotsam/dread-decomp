// CShockWaveComponent's unit (tools/tu_map.py unit 1505): the copy assignment of the component's 0x210-byte
// member (fn_71009f3bf8; game/CShockWaveComponent.h declares it as a free function): member by member, the trivial runs copied as blocks, the rest through their own
// assignments. Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/math/CVector4D.h"
#include "game/CCentralUnitCannonBeamMovementComponent.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

// The 0x100-byte damage description, assigned out of line.
struct UnkShockWaveDamage_l9f3bf8 {
    u8 raw[0x100];
    UnkShockWaveDamage_l9f3bf8& operator=(const UnkShockWaveDamage_l9f3bf8& other) {
        fn_7100400364(reinterpret_cast<UnkWeaponDamage_ovb0054*>(this),
                      reinterpret_cast<const UnkWeaponDamage_ovb0054*>(&other));
        return *this;
    }
};

struct UnkShockWaveString_l9f3bf8 {
    base::global::CRntString str;
    UnkShockWaveString_l9f3bf8& operator=(const UnkShockWaveString_l9f3bf8& other) {
        CRntString_Assign(&str, &other.str);
        return *this;
    }
};

struct UnkShockWaveHandle_l9f3bf8 {
    base::UnkRefHandle handle;
    UnkShockWaveHandle_l9f3bf8& operator=(const UnkShockWaveHandle_l9f3bf8& other) {
        fn_710031d208(&handle, &other.handle);
        return *this;
    }
};

struct UnkShockWaveColor_l9f3bf8 {
    base::math::CVector4D value;
    UnkShockWaveColor_l9f3bf8& operator=(const UnkShockWaveColor_l9f3bf8& other) {
        fn_710007b518(&value, &other.value);
        return *this;
    }
};

struct UnkShockWaveVec3_l9f3bf8 {
    f32 x, y, z;
    UnkShockWaveVec3_l9f3bf8& operator=(const UnkShockWaveVec3_l9f3bf8& other) {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }
};

struct UnkShockWaveVec4_l9f3bf8 {
    f32 x, y, z, w;
    UnkShockWaveVec4_l9f3bf8& operator=(const UnkShockWaveVec4_l9f3bf8& other) {
        x = other.x;
        y = other.y;
        z = other.z;
        w = other.w;
        return *this;
    }
};

struct UnkShockWaveData_l9f3bf8 {
    virtual ~UnkShockWaveData_l9f3bf8();
    UnkShockWaveDamage_l9f3bf8 field_0x8;
    u64 field_0x108;
    bool field_0x110;
    UnkShockWaveVec3_l9f3bf8 field_0x114;
    u64 field_0x120;
    u64 field_0x128;
    u64 field_0x130;
    bool field_0x138;
    CStrId field_0x140;
    bool field_0x148;
    bool field_0x149;
    CFilePathStrId field_0x150;
    UnkShockWaveString_l9f3bf8 field_0x158;
    UnkShockWaveHandle_l9f3bf8 field_0x188;
    bool field_0x190;
    UnkShockWaveColor_l9f3bf8 field_0x194;
    f32 field_0x1a4;
    f32 field_0x1a8;
    CStrId field_0x1b0;
    u64 field_0x1b8;
    u64 field_0x1c0;
    u64 field_0x1c8;
    u64 field_0x1d0;
    CFilePathStrId field_0x1d8;
    UnkShockWaveVec3_l9f3bf8 field_0x1e0;
    UnkShockWaveVec4_l9f3bf8 field_0x1ec;
    f32 field_0x1fc;
    f32 field_0x200;
    f32 field_0x204;
    f32 field_0x208;

    UnkShockWaveData_l9f3bf8& operator=(const UnkShockWaveData_l9f3bf8& other) asm("fn_71009f3bf8");
};
static_assert(__builtin_offsetof(UnkShockWaveData_l9f3bf8, field_0x158) == 0x158, "layout");
static_assert(__builtin_offsetof(UnkShockWaveData_l9f3bf8, field_0x1a4) == 0x1a4, "layout");
static_assert(__builtin_offsetof(UnkShockWaveData_l9f3bf8, field_0x1fc) == 0x1fc, "layout");
static_assert(sizeof(UnkShockWaveData_l9f3bf8) == 0x210, "layout");

UnkShockWaveData_l9f3bf8& UnkShockWaveData_l9f3bf8::operator=(const UnkShockWaveData_l9f3bf8& other) = default;
