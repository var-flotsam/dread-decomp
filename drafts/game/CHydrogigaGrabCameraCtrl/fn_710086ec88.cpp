// CHydrogigaGrabCameraCtrl (unit 1324): places the camera beside the player from how far they are from the
// grab anchor: offset, direction and field of view are blended between a near, middle and far setting.
// Names and layouts are placeholders.
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "types.h"

struct UnkGrabVec_l86ec88 {
    f32 x, y, z;
};

struct UnkGrabCamSetting_l86ec88 {
    UnkGrabVec_l86ec88 offset;
    UnkGrabVec_l86ec88 dir;
    f32 fov;
};

struct UnkGrabBody_l86ec88 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
};

struct UnkGrabLink_l86ec88 {
    u8 pad_0x0[0x10];
    UnkGrabBody_l86ec88* body;  // 0x10
};

struct UnkGrabCamera_l86ec88 {
    u8 pad_0x0[0x68];
    UnkGrabVec_l86ec88 pos;  // 0x68
    UnkGrabVec_l86ec88 dir;  // 0x74
    u8 pad_0x80[0xc];
    f32 fov;  // 0x8c
    u8 pad_0x90[0x20];
    UnkGrabLink_l86ec88* owner;  // 0xb0
    u8 pad_0xb8[0x148];
    UnkGrabLink_l86ec88* anchor;  // 0x200
};

static_assert(__builtin_offsetof(UnkGrabCamera_l86ec88, owner) == 0xb0, "");
static_assert(__builtin_offsetof(UnkGrabCamera_l86ec88, anchor) == 0x200, "");

__attribute__((visibility("hidden"))) extern f32 g_unk_7172729734;  // far distance
__attribute__((visibility("hidden"))) extern f32 g_unk_7172729738;  // middle distance
__attribute__((visibility("hidden"))) extern f32 g_unk_717272973c;  // near distance

static const UnkGrabCamSetting_l86ec88 kNear = {
    {308.06104f, -26.206055f, 1121.632f}, {-0.45369115f, 0.12968695f, -0.8815586f}, 45.0f};
static const UnkGrabCamSetting_l86ec88 kMiddle = {
    {218.88086f, -88.08301f, 939.16626f}, {-0.521219f, 0.1887451f, -0.8321694f}, 50.0f};
static const UnkGrabCamSetting_l86ec88 kFar = {
    {-57.135742f, -5.2841797f, 841.5446f}, {-0.43376616f, 0.15678945f, -0.8871663f}, 77.0f};

static inline f32 Blend(f32 a, f32 b, f32 t) {
    return t * b + (1.0f - t) * a;
}

static inline UnkGrabVec_l86ec88 Blend(const UnkGrabVec_l86ec88& a, const UnkGrabVec_l86ec88& b, f32 t) {
    UnkGrabVec_l86ec88 out;
    out.x = Blend(a.x, b.x, t);
    out.y = Blend(a.y, b.y, t);
    out.z = Blend(a.z, b.z, t);
    return out;
}

static inline UnkGrabCamSetting_l86ec88 Blend(const UnkGrabCamSetting_l86ec88& a,
                                              const UnkGrabCamSetting_l86ec88& b, f32 t) {
    if (t >= 1.0f)
        return b;
    UnkGrabCamSetting_l86ec88 out;
    out.offset = Blend(a.offset, b.offset, t);
    out.dir = Blend(a.dir, b.dir, t);
    out.fov = t < 0.0f ? a.fov : Blend(a.fov, b.fov, t);
    return out;
}

void fn_710086ec88(UnkGrabCamera_l86ec88* self) {
    if (self->owner == nullptr || self->owner->body == nullptr)
        return;
    UnkGrabBody_l86ec88* player = (UnkGrabBody_l86ec88*)fn_7100dec788(g_unk_7101d47310);
    if (player == nullptr)
        return;
    if (self->anchor == nullptr || self->anchor->body == nullptr)
        return;

    f32 anchor_x = self->anchor->body->x;
    f32 near = g_unk_717272973c;
    f32 distance = __builtin_fabsf(player->x - anchor_x);
    f32 clamped = near;
    if (!(distance <= near))
        clamped = distance >= g_unk_7172729734 ? g_unk_7172729734 : distance;
    bool right = player->x > anchor_x;
    f32 side = right ? clamped : -clamped;
    f32 x = anchor_x + side;
    f32 sign = right ? 1.0f : -1.0f;
    f32 y = player->y;
    f32 z = player->z;

    UnkGrabCamSetting_l86ec88 setting = kMiddle;
    f32 middle = g_unk_7172729738;
    if (clamped < middle)
        setting = Blend(kNear, kMiddle, (clamped - near) / (middle - near));
    else if (clamped > middle)
        setting = Blend(kMiddle, kFar, (clamped - middle) / (g_unk_7172729734 - middle));

    self->fov = setting.fov;
    self->pos.x = x + sign * setting.offset.x;
    self->pos.y = y + setting.offset.y;
    self->pos.z = z + setting.offset.z;
    self->dir.x = sign * setting.dir.x;
    self->dir.y = setting.dir.y;
    self->dir.z = setting.dir.z;
}
