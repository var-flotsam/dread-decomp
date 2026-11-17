// Unit 260 (render passes, no reliable name): sets one render property, chosen by its string-id hash, from
// the value it points to: render-target state, flags kept in globals, and the owner's own settings (the
// setter beside fn_71001e065c's query). Names and layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "base/math/CVector.h"
#include "base/math/CVector4D.h"
#include "base/math/CVectorOps.h"
#include "base/tunable/CTunable.h"
#include "game/UnkTU259.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;
using base::math::CVector4D;
using base::math::UnkVector4D;

struct UnkCountedObject259;

extern base::math::CVector4D g_unk_7101a1e98c __attribute__((visibility("hidden")));
extern base::math::CVector4D g_unk_7101a1e99c __attribute__((visibility("hidden")));
extern base::math::CVector4D g_unk_7101a1e9ac __attribute__((visibility("hidden")));
extern u8 g_unk_7101a1e968 __attribute__((visibility("hidden")));
extern u8 g_unk_7101a1e96c __attribute__((visibility("hidden")));
extern u8 g_unk_7101a1e970 __attribute__((visibility("hidden")));
extern f32 g_unk_7101a1e974 __attribute__((visibility("hidden")));
extern u8 g_unk_716e596024 __attribute__((visibility("hidden")));
extern u8 g_unk_716e596028 __attribute__((visibility("hidden")));
extern u8 g_unk_716e59602c __attribute__((visibility("hidden")));
extern u8 g_unk_716e596030 __attribute__((visibility("hidden")));
extern u8 g_unk_716e596034 __attribute__((visibility("hidden")));
extern bool g_unk_716e596038 __attribute__((visibility("hidden")));
extern f32 g_unk_716e596040 __attribute__((visibility("hidden")));
extern f32 g_unk_716e596044 __attribute__((visibility("hidden")));
extern u32 g_unk_716e596540[4] __attribute__((visibility("hidden")));

void fn_71001d09fc(UnkRenderTargetA1* target, u8 value);
void fn_71001d0a10(UnkRenderTargetA1* target, f32 value);
void fn_71001d0a28(UnkRenderTargetA1* target, f32 value);
void fn_71001d0a40(UnkRenderTargetA1* target, f32 value);
void fn_71001d0a4c(UnkRenderTargetA1* target, f32 value);
void fn_71001d0a88(UnkRenderTargetA1* target, f32 value);
void fn_71001d0acc(void* object, f32 value);
void fn_71001d0ad8(UnkRenderTargetA1* target, f32 value);
void fn_71001d50c4(UnkRenderTargetA1* target, bool enable);
void fn_71001d50d8(UnkRenderTargetA1* target, const UnkVector4D* color);
void fn_71001d50e8(UnkRenderTargetA1* target, const UnkVector4D* color);
void fn_71001d538c(UnkRenderTargetA1* target, const UnkVector4D* a, const UnkVector4D* b, const UnkVector4D* c);
void fn_71001d53ec(UnkRenderTargetA1* target, const UnkVector4D* v);
void fn_71001d53fc(UnkRenderTargetA1* target, const UnkVector4D* v);
void fn_71001d540c(UnkRenderTargetA1* target, bool enable);
void fn_71001d5420(UnkRenderTargetA1* target, bool enable);
void fn_71001d5434(char* self, UnkCountedObject259* const* source);
void fn_710014f48c(void* object);

// A counted reference to a resource.
struct UnkResourceRef_l1dfc74 {
    UnkCountedResource* resource;

    explicit UnkResourceRef_l1dfc74(UnkCountedResource* r) : resource(nullptr) {
        if (r) {
            ++r->ref_count;
            resource = r;
        }
    }
    UnkResourceRef_l1dfc74(const UnkResourceRef_l1dfc74& other) : resource(other.resource) {
        if (resource)
            ++resource->ref_count;
    }
    // The reference as fn_71001d5434 reads it.
    UnkCountedObject259* const* Address() const {
        return reinterpret_cast<UnkCountedObject259* const*>(&resource);
    }
    ~UnkResourceRef_l1dfc74() {
        if (resource && --resource->ref_count == 0)
            fn_710001737c(resource);
    }
};

// fn_71001d5434 takes the reference by value (the address of the caller's copy).
static inline void SetResource(UnkRenderTargetA1* target, UnkResourceRef_l1dfc74 resource) {
    fn_71001d5434(reinterpret_cast<char*>(target), resource.Address());
}

struct UnkResourceValue_l1dfc74 {
    void* object;
    UnkResourceRef_l1dfc74 resource;
};

struct UnkLightValues_l1dfc74 {
    f32 enable;
    CVector4D a;  // 0x4
    CVector3D b;  // 0x14
    CVector2D c;  // 0x20
};

struct UnkColorValue_l1dfc74 {
    f32 enable;
    CVector4D color;  // 0x4
};

struct UnkRectValue_l1dfc74 {
    f32 enable;
    f32 rect[4];  // 0x4, fractions of the display
};

struct UnkFadeValue_l1dfc74 {
    f32 enable;
    f32 value;
};

struct UnkSettingsValue_l1dfc74 {
    f32 unused;
    f32 values[5];    // 0x4
    CVector4D color;  // 0x18
    f32 last;         // 0x28
};

struct UnkRenderTargetFields_l1dfc74 {
    u8 pad_0x0[0x5c];
    f32 field_0x5c;
};

struct UnkRenderSettings_l1dfc74 {
    u8 pad_0x0[0xf0];
    UnkRenderTargetA1* target;  // 0xf0
    u8 pad_0xf8[0x80];
    f32 field_0x178;
    f32 field_0x17c;
    u8 pad_0x180[0x10];
    f32 values_0x190[5];
    CVector4D color_0x1a4;
    f32 field_0x1b4;
    f32 field_0x1b8;
    u8 pad_0x1bc[0x27c];
    f32 fade_0x438;
    u8 fade_0x43c;
    u8 pad_0x43d[0x63];
    CVector4D field_0x4a0;
    u8 pad_0x4b0[0x9cf0];
    u8 field_0xa1a0[8];
};
static_assert(__builtin_offsetof(UnkRenderSettings_l1dfc74, values_0x190) == 0x190, "layout");
static_assert(__builtin_offsetof(UnkRenderSettings_l1dfc74, fade_0x438) == 0x438, "layout");
static_assert(__builtin_offsetof(UnkRenderSettings_l1dfc74, field_0x4a0) == 0x4a0, "layout");
static_assert(__builtin_offsetof(UnkRenderSettings_l1dfc74, field_0xa1a0) == 0xa1a0, "layout");

static inline f32 AsFloat(const void* value) {
    return *static_cast<const f32*>(value);
}

// The vector in the form the render target takes it.
static inline UnkVector4D ToVector4D(const CVector4D& v) {
    return UnkVector4D(v.x, v.y, v.z, v.w);
}

static inline bool AsFlag(const void* value) {
    return AsFloat(value) > 0.5f;
}

void fn_71001dfc74(UnkRenderSettings_l1dfc74* self, s64 id, const void* value) {
    switch (id) {
    case 0x5c4bf62fb5704af4: {
        const f32* v = static_cast<const f32*>(value);
        self->field_0x178 = v[0];
        self->field_0x17c = v[1];
        break;
    }
    case (s64)0x8f2f84ad24e74e64: {
        const UnkLightValues_l1dfc74* v = static_cast<const UnkLightValues_l1dfc74*>(value);
        bool enable = v->enable > 0.5f;
        UnkVector4D a = ToVector4D(v->a);
        UnkVector4D b(v->b.x, v->b.y, v->b.z, 0.0f);
        UnkVector4D c(v->c.x, v->c.y, 0.0f, 0.0f);
        fn_71001d538c(self->target, &a, &b, &c);
        fn_71001d540c(self->target, enable);
        break;
    }
    case (s64)0xf91950417c43bfaa:
        Assign(g_unk_7101a1e9ac, *static_cast<const CVector4D*>(value));
        break;
    case (s64)0xb37573f01782316a:
        Assign(g_unk_7101a1e99c, *static_cast<const CVector4D*>(value));
        break;
    case 0x187a83862f0cf5ee: {
        const UnkFadeValue_l1dfc74* v = static_cast<const UnkFadeValue_l1dfc74*>(value);
        bool done = self->fade_0x43c || v->enable >= 0.5f;
        self->fade_0x43c = done;
        self->fade_0x438 = done ? 0.0f : v->value;
        break;
    }
    case (s64)0x9ad56a86fea3f8d7:
        g_unk_7101a1e974 = AsFloat(value);
        break;
    case 0x090460a8b3084abc: {
        bool on = AsFlag(value);
        g_unk_716e596024 = on;
        f32 scale = on ? 1.0f : 0.8f;
        fn_71001d0ad8(self->target, scale);
        fn_71001d0acc(self->target, scale);
        break;
    }
    case (s64)0xd1a557a87747d6d7:
        g_unk_716e59602c = AsFlag(value);
        break;
    case 0x478f99b643ea9c4c:
        g_unk_7101a1e96c = AsFlag(value);
        break;
    case (s64)0x947df12d002e01d6: {
        const UnkColorValue_l1dfc74* v = static_cast<const UnkColorValue_l1dfc74*>(value);
        bool enable = v->enable >= 0.5f;
        UnkVector4D color = ToVector4D(v->color);
        fn_71001d50c4(self->target, enable);
        fn_71001d50d8(self->target, &color);
        break;
    }
    case (s64)0xfe815708d5f2b892:
        fn_71001d0a40(self->target, AsFloat(value));
        break;
    case (s64)0xc66c8984dda950d6:
        self->field_0x1b8 = AsFloat(value);
        break;
    case 0x38bb3d82ecfaffaa:
        g_unk_716e596034 = AsFlag(value);
        break;
    case (s64)0x9ee0a674e69e19a5:
        g_unk_716e596030 = AsFlag(value);
        break;
    case 0x0b3b14aea1e3e17f:
        g_unk_716e596040 = AsFloat(value);
        break;
    case (s64)0xdaafacbdfd7d600f: {
        const CVector3D* v = static_cast<const CVector3D*>(value);
        UnkVector4D v4(v->x, v->y, v->z, 0.0f);
        fn_71001d53fc(self->target, &v4);
        break;
    }
    case 0x4b93841b1f20eed7:
        self->field_0x4a0 = UnkVector4D(*static_cast<const CVector4D*>(value));
        break;
    case 0x5736f2a79b27ad4e:
        fn_71001d5420(self->target, *static_cast<void* const*>(value) != nullptr);
        break;
    case (s64)0x86fdd4eeb6de9890: {
        const CVector3D* v = static_cast<const CVector3D*>(value);
        UnkVector4D v4(v->x, v->y, v->z, 0.0f);
        fn_71001d53ec(self->target, &v4);
        break;
    }
    case (s64)0xefd7baeacd6c536d:
        g_unk_7101a1e968 = AsFlag(value);
        break;
    case (s64)0xa1fcaf230447d553:
        fn_71001d0a10(self->target, AsFloat(value));
        break;
    case 0x12194b8295e98a84: {
        const UnkRectValue_l1dfc74* v = static_cast<const UnkRectValue_l1dfc74*>(value);
        g_unk_716e596028 = v->enable > 0.5f;
        f32 width = fn_7100132f80();
        f32 height = fn_7100132f90();
        g_unk_716e596540[0] = v->rect[0] * width;
        g_unk_716e596540[1] = v->rect[1] * height;
        g_unk_716e596540[2] = v->rect[2] * width;
        g_unk_716e596540[3] = v->rect[3] * height;
        break;
    }
    case (s64)0x95a2e304fd05cda8:
        Assign(g_unk_7101a1e98c, *static_cast<const CVector4D*>(value));
        break;
    case (s64)0xffa4d6b88c29ee76:
        reinterpret_cast<UnkRenderTargetFields_l1dfc74*>(self->target)->field_0x5c = AsFloat(value);
        break;
    case (s64)0xcc53b066013b1983: {
        bool on = AsFlag(value);
        if (on && !g_unk_716e596038)
            fn_710014f48c(self->field_0xa1a0);
        g_unk_716e596038 = on;
        break;
    }
    case 0x46342169c9265aea: {
        const UnkSettingsValue_l1dfc74* v = static_cast<const UnkSettingsValue_l1dfc74*>(value);
        self->values_0x190[0] = v->values[0];
        self->values_0x190[1] = v->values[1];
        self->values_0x190[2] = v->values[2];
        self->values_0x190[3] = v->values[3];
        self->values_0x190[4] = v->values[4];
        self->color_0x1a4 = UnkVector4D(v->color);
        self->field_0x1b4 = v->last;
        break;
    }
    case (s64)0x90e4f789f8288231:
        g_unk_716e596044 = AsFloat(value);
        break;
    case (s64)0xfb146333273b1b21: {
        bool on = AsFlag(value);
        g_unk_7101a1e970 = on;
        fn_71001d09fc(self->target, on);
        break;
    }
    case (s64)0xbd88e4a69dc0e5c9: {
        UnkResourceValue_l1dfc74 v = *static_cast<const UnkResourceValue_l1dfc74*>(value);
        fn_71001d5420(self->target, v.object != nullptr);
        SetResource(self->target, UnkResourceRef_l1dfc74(v.resource.resource));
        break;
    }
    case 0x3341e9b921761ed5: {
        UnkVector4D color = ToVector4D(*static_cast<const CVector4D*>(value));
        fn_71001d50e8(self->target, &color);
        break;
    }
    case (s64)0x9daa3a4588a370dc:
        fn_71001d0a88(self->target, AsFloat(value));
        break;
    case 0x0a0b60deb29d8a1b:
        fn_71001d0a4c(self->target, AsFloat(value));
        break;
    case (s64)0xd8cb39e0e66e0d64:
        fn_71001d0a28(self->target, AsFloat(value));
        break;
    }
}
