// Property setters of a minimap settings object: each stores a new value when the reflected
// type says it differs from the current one, then tells the global notifier (created on first
// use) which property of the object changed.
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkReflAttributes.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector3D;
using base::reflection::UnkReflTypeOf;
using base::reflection::UnkTunableRegistry;

extern UnkTunableRegistry* g_unk_7101d47350;
void fn_710013c3c8(void* notifier, void* object, const CStrId* property);

struct UnkMinimapSettings {
    u8 unk_0x0[0x18];
    u8 field_0x18[0xc];  // what the notifier is told about
    CVector3D field_0x24;
    CVector3D field_0x30;
    CVector3D field_0x3c;
    CVector3D field_0x48;
    CVector3D field_0x54;
    CVector3D field_0x60;
    CVector3D field_0x6c;
    CVector3D field_0x78;
    u8 unk_0x84[0x210];
    f32 field_0x294;
    f32 field_0x298;
    f32 field_0x29c;
    f32 field_0x2a0;
    f32 field_0x2a4;
    f32 field_0x2a8;
    f32 field_0x2ac;
    f32 field_0x2b0;
    f32 field_0x2b4;
    bool field_0x2b8;
};

// Vectors are stored component by component.
static inline void Assign(f32& field, const f32& value) { field = value; }
static inline void Assign(bool& field, const bool& value) { field = value; }
static inline void Assign(CVector3D& field, const CVector3D& value) {
    field.x = value.x;
    field.y = value.y;
    field.z = value.z;
}

template <class T, T UnkMinimapSettings::*Field>
__attribute__((always_inline)) static inline void SetProperty(UnkMinimapSettings* self, const T* value,
                                                              const char* name) {
    if (UnkReflTypeOf<T>::Get()->m_fn_0x58(&(self->*Field), value))
        return;
    Assign(self->*Field, *value);
    UnkTunableRegistry* notifier = g_unk_7101d47350;
    if (!notifier) {
        notifier = static_cast<UnkTunableRegistry*>(fn_71000839a0(0x58, 0x3f));
        fn_710013ac14(notifier);
        g_unk_7101d47350 = notifier;
    }
    void* object = self->field_0x18;
    CStrId id(name, true);
    fn_710013c3c8(notifier, object, &id);
}

#define SETTER(addr, T, field, name)                                                          \
    void fn_##addr(UnkMinimapSettings* self, const T* value) {                                 \
        SetProperty<T, &UnkMinimapSettings::field>(self, value, name);                         \
    }

SETTER(7100eaa7a8, CVector3D, field_0x3c, "vOccluderColor")
SETTER(7100eaa93c, CVector3D, field_0x48, "vDefaultBorderColor")
SETTER(7100eaaad0, CVector3D, field_0x54, "vUnexploredBorderColor")
SETTER(7100eaac64, CVector3D, field_0x60, "vNormalRoomColor")
SETTER(7100eaadf8, CVector3D, field_0x6c, "vSpecialRoomColor")
SETTER(7100eaaf8c, CVector3D, field_0x78, "vTransportRoomColor")
SETTER(7100eab120, CVector3D, field_0x24, "vDefaultColor")
SETTER(7100eab2b4, CVector3D, field_0x30, "vCooldownColor")
SETTER(7100eab448, f32, field_0x294, "fMinimapViewAlpha")
SETTER(7100eab5d0, f32, field_0x298, "fMinimapViewZoom")
SETTER(7100eab758, f32, field_0x29c, "fAreaMapSnapVelMult")
SETTER(7100eab8e0, f32, field_0x2a0, "fAreaMapMoveVel")
SETTER(7100eaba68, f32, field_0x2a4, "fAreaMapFastMoveVel")
SETTER(7100eabbf0, f32, field_0x2a8, "fAreaMapZoomMin")
SETTER(7100eabd78, f32, field_0x2ac, "fAreaMapZoomMax")
SETTER(7100eabf00, f32, field_0x2b0, "fAreaMapZoomDefault")
SETTER(7100eac088, f32, field_0x2b4, "fAreaMapZoomVel")
SETTER(7100eac210, bool, field_0x2b8, "bAutoFadeEnabled")
