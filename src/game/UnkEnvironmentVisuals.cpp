// From unit 2525 of tools/tu_map.py (0x7100d8c56c-0x7100d911d8; the unit describes
// CEnvironmentVisualPresets* and CEnvironmentSoundPresets* to reflection): applying an
// environment preset to the environment's fading render settings (src/game/UnkEnvironmentFade.cpp,
// at +0x28 of the owner). A preset's groups (fog, vertical fog, cube map, ...: the members of
// CEnvironmentData, whose transition types reflection names) each have their values, the time
// each value fades over, and a list of transitions that override those times when the visuals
// change from the named preset. Names and layouts are placeholders.
#include <algorithm>

#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/global/UnkStringPoolBody.h"
#include "base/math/CVector.h"
#include "base/math/CVector4D.h"
#include "base/UnkVector.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringEntry;
using base::math::CVector2D;
using base::math::CVector3D;
using base::math::CVector4D;
using base::math::UnkVector4D;

// The fading render settings (src/game/UnkEnvironmentFade.cpp).
struct UnkEnvironmentFade;
using UnkEnvFadeColor = base::math::UnkVector4D;

// A 2D vector passed by value as a copy in memory (its copy constructor is user-provided).
struct UnkEnvVec2 {
    f32 x;
    f32 y;

    UnkEnvVec2(const UnkEnvVec2& other) {
        x = other.x;
        y = other.y;
    }
};

// STransition (reflection; 0x10 bytes): the preset the transition is from (sPreset). The times
// of each kind follow it.
struct UnkTransition {
    void* field_0x0;
    UnkStringData* preset;  // a CStrId
};

// The preset's hash: the CRC-64 its string id keeps.
static inline s64 PresetKey(const UnkStringData* id) {
    return reinterpret_cast<const UnkStringEntry*>(id)->buffer.key;
}

// A transition with one time (most kinds; 0x18 bytes) or two.
struct UnkTransition1 : UnkTransition {
    f32 time;
};
struct UnkTransition2 : UnkTransition {
    f32 time0;
    f32 time1;
};
// CEnvironmentData::SFogTransition (0x20 bytes) and SVerticalFogTransition (0x28 bytes).
struct UnkFogTransition : UnkTransition {
    f32 time[4];
};
struct UnkVerticalFogTransition : UnkTransition {
    f32 time[6];
};
static_assert(sizeof(UnkTransition1) == 0x18, "UnkTransition1 layout");
static_assert(sizeof(UnkTransition2) == 0x18, "UnkTransition2 layout");
static_assert(sizeof(UnkFogTransition) == 0x20, "UnkFogTransition layout");
static_assert(sizeof(UnkVerticalFogTransition) == 0x28, "UnkVerticalFogTransition layout");

// The transition from the preset whose key is `key`, if the list has one.
template <class T>
static inline const T* FindTransition(const base::UnkVector<T>& transitions, s64 key) {
    const T* end = transitions.data + transitions.count;
    const T* it = transitions.data;
    if (it != end && PresetKey(it->preset) != key) {
        do
            ++it;
        while (it != end && PresetKey(it->preset) != key);
    }
    return end == it ? nullptr : it;
}

// A preset (probably CEnvironmentData): each group's fade times (defaults for its transitions'),
// its values, and its transitions.
struct UnkEnvironmentData {
    // Fog (0x0).
    f32 field_0x0;  // time
    UnkEnvFadeColor field_0x4;
    f32 field_0x14;  // time
    f32 field_0x18;
    f32 field_0x1c;  // time
    UnkEnvVec2 field_0x20;
    UnkVector4D field_0x28;
    f32 field_0x38;  // time
    base::UnkVector<UnkFogTransition> field_0x40;
    // 0x68.
    f32 field_0x68;  // time
    UnkEnvFadeColor field_0x6c;
    base::UnkVector<UnkTransition1> field_0x80;
    // 0xa8: two colours passed by value.
    f32 field_0xa8;  // time
    CVector4D field_0xac;
    CVector4D field_0xbc;
    base::UnkVector<UnkTransition1> field_0xd0;
    // 0xf8.
    f32 field_0xf8;  // time
    UnkEnvFadeColor field_0xfc;
    f32 field_0x10c;  // time
    f32 field_0x110;
    base::UnkVector<UnkTransition2> field_0x118;
    // 0x140.
    f32 field_0x140;  // time
    UnkEnvFadeColor field_0x144;
    UnkEnvFadeColor field_0x154;
    UnkEnvFadeColor field_0x164;
    f32 field_0x174;  // time
    CVector2D field_0x178;
    base::UnkVector<UnkTransition2> field_0x180;
    // 0x1a8.
    f32 field_0x1a8;  // time
    CVector3D field_0x1ac;
    f32 field_0x1b8;  // time
    CVector3D field_0x1bc;
    base::UnkVector<UnkTransition2> field_0x1c8;
    // 0x1f0.
    f32 field_0x1f0;  // time
    CVector3D field_0x1f4;
    base::UnkVector<UnkTransition1> field_0x200;
    // Vertical fog (0x228).
    bool field_0x228;
    UnkEnvFadeColor field_0x22c;
    f32 field_0x23c;  // time
    f32 field_0x240;
    f32 field_0x244;  // time
    CVector2D field_0x248;
    f32 field_0x250;  // time
    UnkEnvFadeColor field_0x254;
    f32 field_0x264;  // time
    f32 field_0x268;
    f32 field_0x26c;  // time
    f32 field_0x270;
    f32 field_0x274;  // time
    base::UnkVector<UnkVerticalFogTransition> field_0x278;
    // Cube map (0x2a0): no transitions are looked up.
    u32 field_0x2a0;
    bool field_0x2a4;
    CFilePathStrId field_0x2a8;  // the texture's path
    base::UnkVector<UnkTransition1> field_0x2b0;
    // 0x2d8.
    f32 field_0x2d8;  // time
    f32 field_0x2dc;
    f32 field_0x2e0;
    base::UnkVector<UnkTransition1> field_0x2e8;
    // 0x310.
    f32 field_0x310;  // time
    f32 field_0x314[5];
    UnkVector4D field_0x328;
    f32 field_0x338;
    bool field_0x33c;
    base::UnkVector<UnkTransition1> field_0x340;
    // 0x368.
    f32 field_0x368;  // time
    f32 field_0x36c[3];
    base::UnkVector<UnkTransition1> field_0x378;
    CStrId field_0x3a0;  // the preset's name
};
static_assert(__builtin_offsetof(UnkEnvironmentData, field_0x40) == 0x40, "UnkEnvironmentData layout");
static_assert(__builtin_offsetof(UnkEnvironmentData, field_0xd0) == 0xd0, "UnkEnvironmentData layout");
static_assert(__builtin_offsetof(UnkEnvironmentData, field_0x180) == 0x180, "UnkEnvironmentData layout");
static_assert(__builtin_offsetof(UnkEnvironmentData, field_0x228) == 0x228, "UnkEnvironmentData layout");
static_assert(__builtin_offsetof(UnkEnvironmentData, field_0x278) == 0x278, "UnkEnvironmentData layout");
static_assert(__builtin_offsetof(UnkEnvironmentData, field_0x2a8) == 0x2a8, "UnkEnvironmentData layout");
static_assert(__builtin_offsetof(UnkEnvironmentData, field_0x340) == 0x340, "UnkEnvironmentData layout");
static_assert(__builtin_offsetof(UnkEnvironmentData, field_0x3a0) == 0x3a0, "UnkEnvironmentData layout");

// The preset's owner: the preset applied last, its name, and the fading settings at +0x28.
struct UnkEnvironmentVisuals {
    u8 pad_0x0[0x18];
    CStrId field_0x18;                     // the name of the preset applied last
    const UnkEnvironmentData* field_0x20;  // that preset

    UnkEnvironmentFade* Fade() { return reinterpret_cast<UnkEnvironmentFade*>(reinterpret_cast<char*>(this) + 0x28); }
};

// The fading settings' setters (unit 2607; src/game/UnkEnvironmentFade.cpp defines the first
// ones). Each starts a channel's fade over `time`, or sets a value at once.
void fn_7100f97f64(f32 time, UnkEnvironmentFade* self, const UnkEnvFadeColor* to);
void fn_7100f9800c(f32 to, f32 time, UnkEnvironmentFade* self);
// The three that take a value as a copy in memory have it before the time (the original makes
// the copy before it reads the time).
void fn_7100f98094(UnkEnvironmentFade* self, UnkEnvVec2 to, f32 time);
void fn_7100f98128(UnkEnvironmentFade* self, UnkVector4D to, f32 time);
void fn_7100f9819c(f32 time, UnkEnvironmentFade* self, const UnkEnvFadeColor* to);
void fn_7100f98244(CVector4D a, CVector4D b, UnkEnvironmentFade* self, f32 time);
void fn_7100f98370(f32 time, UnkEnvironmentFade* self, const UnkEnvFadeColor* to);
void fn_7100f98410(f32 to, f32 time, UnkEnvironmentFade* self);
void fn_7100f984ac(f32 time, UnkEnvironmentFade* self, const UnkEnvFadeColor* a, const UnkEnvFadeColor* b,
                   const UnkEnvFadeColor* c);
void fn_7100f985d8(f32 time, UnkEnvironmentFade* self, const CVector2D* to);
void fn_7100f98d24(f32 time, UnkEnvironmentFade* self, const CVector3D* to);
void fn_7100f98dc4(f32 time, UnkEnvironmentFade* self, const CVector3D* to);
void fn_7100f986c8(f32 time, UnkEnvironmentFade* self, const CVector3D* to);
void fn_7100f987b0(UnkEnvironmentFade* self, bool value);
void fn_7100f9873c(f32 time, UnkEnvironmentFade* self, const UnkEnvFadeColor* to);
void fn_7100f987c4(f32 to, f32 time, UnkEnvironmentFade* self);
void fn_7100f9881c(f32 time, UnkEnvironmentFade* self, const CVector2D* to);
void fn_7100f98884(f32 time, UnkEnvironmentFade* self, const UnkEnvFadeColor* to);
void fn_7100f988f8(f32 to, f32 time, UnkEnvironmentFade* self);
void fn_7100f98948(f32 to, f32 time, UnkEnvironmentFade* self);
void fn_7100f98998(UnkEnvironmentFade* self, bool value);
void fn_7100f989f0(UnkEnvironmentFade* self, const CFilePathStrId& path);
void fn_7100f98e64(f32 to, f32 time, UnkEnvironmentFade* self);
void fn_7100f98ebc(f32 to, f32 time, UnkEnvironmentFade* self);
void fn_7100f98f14(UnkEnvironmentFade* self, bool value);
void fn_7100f98f28(f32 to, f32 time, UnkEnvironmentFade* self);
void fn_7100f98f80(f32 to, f32 time, UnkEnvironmentFade* self);
void fn_7100f98fd8(f32 to, f32 time, UnkEnvironmentFade* self);
void fn_7100f99030(f32 to, f32 time, UnkEnvironmentFade* self);
void fn_7100f99088(f32 to, f32 time, UnkEnvironmentFade* self);
void fn_7100f990e0(UnkEnvironmentFade* self, UnkVector4D to, f32 time);
void fn_7100f9915c(f32 to, f32 time, UnkEnvironmentFade* self);
void fn_7100f991b4(f32 a, f32 b, f32 c, f32 time, UnkEnvironmentFade* self);

// Applies `data`: at once when no preset was applied before or `force` is set, else with the
// times of each group's transition from the previous preset (the group's own times when it has
// none). Then remembers the preset and its name.
// NON_MATCHING(minor): register allocation and scheduling only. The key, the time pointers and
// the found transition sit in other registers; the original computes the two vertical-fog vector
// addresses one call early (two more moves); some argument set-up and address computations are
// ordered differently within their blocks.
void fn_7100d8e398(UnkEnvironmentVisuals* self, const UnkEnvironmentData* data, bool force) {
    if (!data)
        return;
    if (self->field_0x18.data == &g_unk_7101d47108->empty || force) {
        UnkEnvironmentFade* fade = self->Fade();
        fn_7100f97f64(0.0f, fade, &data->field_0x4);
        fn_7100f9800c(data->field_0x18, 0.0f, fade);
        fn_7100f98094(fade, data->field_0x20, 0.0f);
        fn_7100f98128(fade, data->field_0x28, 0.0f);
        fn_7100f9819c(0.0f, fade, &data->field_0x6c);
        fn_7100f98244(data->field_0xac, data->field_0xbc, fade, 0.0f);
        fn_7100f98370(0.0f, fade, &data->field_0xfc);
        fn_7100f98410(data->field_0x110, 0.0f, fade);
        fn_7100f984ac(0.0f, fade, &data->field_0x144, &data->field_0x154, &data->field_0x164);
        fn_7100f985d8(0.0f, fade, &data->field_0x178);
        fn_7100f98d24(0.0f, fade, &data->field_0x1ac);
        fn_7100f98dc4(0.0f, fade, &data->field_0x1bc);
        fn_7100f986c8(0.0f, fade, &data->field_0x1f4);
        fn_7100f987b0(fade, data->field_0x228);
        fn_7100f9873c(0.0f, fade, &data->field_0x22c);
        fn_7100f987c4(data->field_0x240, 0.0f, fade);
        fn_7100f9881c(0.0f, fade, &data->field_0x248);
        fn_7100f988f8(data->field_0x268, 0.0f, fade);
        fn_7100f98948(data->field_0x270, 0.0f, fade);
        fn_7100f98884(0.0f, fade, &data->field_0x254);
        fn_7100f98998(fade, data->field_0x2a4);
        fn_7100f989f0(fade, data->field_0x2a8);
        fn_7100f98e64(data->field_0x2dc, 0.0f, fade);
        fn_7100f98ebc(data->field_0x2e0, 0.0f, fade);
        fn_7100f98f14(fade, data->field_0x33c);
        fn_7100f98f28(data->field_0x314[0], 0.0f, fade);
        fn_7100f98f80(data->field_0x314[1], 0.0f, fade);
        fn_7100f98fd8(data->field_0x314[2], 0.0f, fade);
        fn_7100f99030(data->field_0x314[3], 0.0f, fade);
        fn_7100f99088(data->field_0x314[4], 0.0f, fade);
        fn_7100f990e0(fade, data->field_0x328, 0.0f);
        fn_7100f9915c(data->field_0x338, 0.0f, fade);
        fn_7100f991b4(data->field_0x36c[0], data->field_0x36c[1], data->field_0x36c[2], 0.0f, fade);
    } else {
        s64 key = PresetKey(self->field_0x18.data);

        const UnkFogTransition* fog = FindTransition(data->field_0x40, key);
        UnkEnvironmentFade* fade = self->Fade();
        fn_7100f97f64(fog ? fog->time[0] : data->field_0x0, fade, &data->field_0x4);
        fn_7100f9800c(data->field_0x18, fog ? fog->time[1] : data->field_0x14, fade);
        fn_7100f98094(fade, data->field_0x20, fog ? fog->time[2] : data->field_0x1c);
        fn_7100f98128(fade, data->field_0x28, fog ? fog->time[3] : data->field_0x38);

        const UnkTransition1* t68 = FindTransition(data->field_0x80, key);
        fn_7100f9819c(t68 ? t68->time : data->field_0x68, fade, &data->field_0x6c);

        const UnkTransition1* ta8 = FindTransition(data->field_0xd0, key);
        fn_7100f98244(data->field_0xac, data->field_0xbc, fade, ta8 ? ta8->time : data->field_0xa8);

        const UnkTransition2* tf8 = FindTransition(data->field_0x118, key);
        fn_7100f98370(tf8 ? tf8->time0 : data->field_0xf8, fade, &data->field_0xfc);
        fn_7100f98410(data->field_0x110, tf8 ? tf8->time1 : data->field_0x10c, fade);

        const UnkTransition2* t140 = FindTransition(data->field_0x180, key);
        const CVector2D* v178 = &data->field_0x178;
        fn_7100f984ac(t140 ? t140->time0 : data->field_0x140, fade, &data->field_0x144, &data->field_0x154,
                      &data->field_0x164);
        fn_7100f985d8(t140 ? t140->time1 : data->field_0x174, fade, v178);

        const UnkTransition2* t1a8 = FindTransition(data->field_0x1c8, key);
        const CVector3D* v1bc = &data->field_0x1bc;
        fn_7100f98d24(t1a8 ? t1a8->time0 : data->field_0x1a8, fade, &data->field_0x1ac);
        fn_7100f98dc4(t1a8 ? t1a8->time1 : data->field_0x1b8, fade, v1bc);

        const UnkTransition1* t1f0 = FindTransition(data->field_0x200, key);
        fn_7100f986c8(t1f0 ? t1f0->time : data->field_0x1f0, fade, &data->field_0x1f4);

        fn_7100f987b0(fade, data->field_0x228);
        const UnkVerticalFogTransition* vfog = FindTransition(data->field_0x278, key);
        fn_7100f9873c(vfog ? vfog->time[0] : data->field_0x23c, fade, &data->field_0x22c);
        fn_7100f987c4(data->field_0x240, vfog ? vfog->time[1] : data->field_0x244, fade);
        fn_7100f9881c(vfog ? vfog->time[2] : data->field_0x250, fade, &data->field_0x248);
        fn_7100f98884(vfog ? vfog->time[3] : data->field_0x264, fade, &data->field_0x254);
        fn_7100f988f8(data->field_0x268, vfog ? vfog->time[4] : data->field_0x26c, fade);
        fn_7100f98948(data->field_0x270, vfog ? vfog->time[5] : data->field_0x274, fade);

        fn_7100f98998(fade, data->field_0x2a4);
        fn_7100f989f0(fade, data->field_0x2a8);

        const UnkTransition1* t2d8 = FindTransition(data->field_0x2e8, key);
        const f32& time_2d8 = t2d8 ? t2d8->time : data->field_0x2d8;
        fn_7100f98e64(data->field_0x2dc, time_2d8, fade);
        fn_7100f98ebc(data->field_0x2e0, time_2d8, fade);

        fn_7100f98f14(fade, data->field_0x33c);
        const UnkTransition1* t310 = FindTransition(data->field_0x340, key);
        const f32& time_310 = t310 ? t310->time : data->field_0x310;
        fn_7100f98f28(data->field_0x314[0], time_310, fade);
        fn_7100f98f80(data->field_0x314[1], time_310, fade);
        fn_7100f98fd8(data->field_0x314[2], time_310, fade);
        fn_7100f99030(data->field_0x314[3], time_310, fade);
        fn_7100f99088(data->field_0x314[4], time_310, fade);
        fn_7100f990e0(fade, data->field_0x328, time_310);
        fn_7100f9915c(data->field_0x338, time_310, fade);

        const UnkTransition1* t368 = FindTransition(data->field_0x378, key);
        fn_7100f991b4(data->field_0x36c[0], data->field_0x36c[1], data->field_0x36c[2],
                      t368 ? t368->time : data->field_0x368, fade);
    }
    self->field_0x18 = data->field_0x3a0;
    self->field_0x20 = data;
}
