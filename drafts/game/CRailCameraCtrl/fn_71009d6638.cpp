// CRailCameraCtrl unit (tools/tu_map.py unit 1497): the per-frame step. Finds where the player is on the
// rail, picks the cameras of the two rail nodes around that point, updates them and blends their poses by
// the position between the nodes. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CCameraComponent.h"
#include "game/CCameraCtrl.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU365.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;

struct CSynchronizedBlockCameraCtrl;
void fn_71007642d4(CSynchronizedBlockCameraCtrl* self, f32 t, bool flag);

struct UnkBreakablePart;
struct UnkBreakableTarget {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

// A camera's position and the point it looks at.
struct UnkCamPose_l9d6638 {
    UnkVec3f pos;
    UnkVec3f target;
};

// What a rail node's entity says about its camera.
struct UnkRailCamDef_l9d6638 {
    u8 pad_0x0[0x30];
    u8 field_0x30[0x18];  // what the camera is pointed at (vtable slot 0x100)
    CStrId kind;  // 0x48
    u8 pad_0x50[4];
    UnkCamPose_l9d6638 pose;  // 0x54
};
static_assert(__builtin_offsetof(UnkRailCamDef_l9d6638, pose) == 0x54, "layout");

UnkRailCamDef_l9d6638* fn_71003aff78(void* entity);

struct UnkRailCam_l9d6638 {
    void** vtable;
    u8 pad_0x8[0x10];
    void* field_0x18;
    u8 pad_0x20[0x18];
    UnkCamPose_l9d6638 pose;  // 0x38

    void Reset(bool on, f32 time) {
        reinterpret_cast<void (*)(UnkRailCam_l9d6638*, bool, f32)>(vtable[0x48 / 8])(this, on, time);
    }
    void Update(f32 dt) { reinterpret_cast<void (*)(UnkRailCam_l9d6638*, f32)>(vtable[0x58 / 8])(this, dt); }
    void Blend(f32 time) { reinterpret_cast<void (*)(UnkRailCam_l9d6638*, f32)>(vtable[0xd0 / 8])(this, time); }
    void Aim(void* at) { reinterpret_cast<void (*)(UnkRailCam_l9d6638*, void*)>(vtable[0x100 / 8])(this, at); }
};
static_assert(__builtin_offsetof(UnkRailCam_l9d6638, pose) == 0x38, "layout");

struct UnkRailNode_l9d6638 {
    u8 pad_0x0[0x18];
    u8 field_0x18[8];  // the node's entity link
};

struct UnkRail_l9d6638 {
    void** vtable;

    s32 Count() { return reinterpret_cast<s32 (*)(UnkRail_l9d6638*)>(vtable[0x28 / 8])(this); }
    UnkRailNode_l9d6638* Node(s32 index) {
        return reinterpret_cast<UnkRailNode_l9d6638* (*)(UnkRail_l9d6638*, s32)>(vtable[0x30 / 8])(this, index);
    }
};

void fn_7100c03dd0(UnkRail_l9d6638* rail, const UnkVec3f* pos, s32* index, f32* t, UnkVec3f* dir, void* extra);

struct UnkRailHolder_l9d6638 {
    u8 pad_0x0[0x10];
    UnkRail_l9d6638* rail;  // 0x10
};

struct UnkCamHolder_l9d6638 {
    u8 pad_0x0[0x10];
    void* cameras;  // 0x10
};

struct UnkRailPlayer_l9d6638 {
    u8 pad_0x0[0x38];
    f32 transform[3][4];  // 0x38
};

struct UnkRailCameraCtrl_l9d6638 {
    u8 pad_0x0[0x18];
    void* owner;  // 0x18
    u8 pad_0x20[0x14];
    bool active;  // 0x34
    u8 pad_0x35[3];
    UnkCamPose_l9d6638 output;  // 0x38
    u8 pad_0x50[0x18];
    UnkCamPose_l9d6638 pose;  // 0x68
    u8 pad_0x80[0x30];
    UnkCamHolder_l9d6638* cameras;  // 0xb0
    u8 pad_0xb8[0x1e8];
    UnkRailHolder_l9d6638* rail;  // 0x2a0
    s32 segment;  // 0x2a8
    f32 blend;  // 0x2ac
    u8 pad_0x2b0[0x10];
    UnkRailCam_l9d6638* current;  // 0x2c0
    UnkCamPose_l9d6638 pose_a;  // 0x2c8
    UnkCamPose_l9d6638 pose_b;  // 0x2e0
};
static_assert(__builtin_offsetof(UnkRailCameraCtrl_l9d6638, rail) == 0x2a0, "layout");
static_assert(__builtin_offsetof(UnkRailCameraCtrl_l9d6638, pose_b) == 0x2e0, "layout");

s32 fn_71009d6d2c(UnkRailCameraCtrl_l9d6638* self, UnkVec3f* dir, s32 index, f32 t, f32 dt);
UnkCamPose_l9d6638 fn_71009d77cc(const UnkCamPose_l9d6638* a, const UnkCamPose_l9d6638* b, f32 t);

static inline UnkRailCamDef_l9d6638* DefOf(UnkRailNode_l9d6638* node) {
    UnkBreakableTarget* target = *fn_71002a76a4(reinterpret_cast<UnkBreakablePart*>(node->field_0x18));
    return fn_71003aff78(target ? target->field_0x10 : nullptr);
}

static inline UnkRailCam_l9d6638* CameraOf(UnkRailCameraCtrl_l9d6638* self, UnkRailCamDef_l9d6638* def) {
    void* cameras = self->cameras ? self->cameras->cameras : nullptr;
    return reinterpret_cast<UnkRailCam_l9d6638*>(fn_71010e389c(cameras, &def->kind));
}

static inline void Attach(UnkRailCameraCtrl_l9d6638* self, UnkRailCam_l9d6638* camera) {
    if (!camera->field_0x18)
        fn_71010ed948(reinterpret_cast<UnkCam_l450950*>(camera), self->owner, true);
}

// The id table's kind after the fixed camera kind (+0x17e0): a camera that also takes the target.
static inline const CStrId& AimedKind() {
    return *reinterpret_cast<const CStrId*>(GameManagerIdTable().pad_0x17e8);
}

// A fixed camera only runs (or restarts on a reset).
static inline void Run(UnkRailCam_l9d6638* camera, f32 dt, bool reset) {
    if (reset) {
        camera->Reset(true, 0.0f);
        camera->Blend(-1.0f);
    } else if (dt > 0.0f) {
        camera->Update(dt);
    }
}

// Other cameras take the node's position (and its target, for the aimed kind).
static inline void Place(UnkRailCam_l9d6638* camera, UnkRailCamDef_l9d6638* def) {
    camera->pose.pos = def->pose.pos;
    if (def->kind == AimedKind())
        camera->pose.target = def->pose.target;
    camera->Update(0.0f);
}

// The id table's fixed camera kind.
static inline bool IsFixed(const UnkRailCamDef_l9d6638* def) {
    return def->kind == GameManagerIdTable().field_0x17e0;
}

void fn_71009d6638(UnkRailCameraCtrl_l9d6638* self, f32 dt, bool reset) {
    fn_71007642d4(reinterpret_cast<CSynchronizedBlockCameraCtrl*>(self), dt, reset);
    auto* player = reinterpret_cast<UnkRailPlayer_l9d6638*>(fn_7100dec788(g_unk_7101d47310));
    if (!player || !self->active) {
        if (reset)
            self->output = self->pose;
        return;
    }
    UnkRail_l9d6638* rail = self->rail->rail;
    s32 index = 0;
    f32 t = 0.0f;
    UnkVec3f dir(0.0f, 0.0f, 0.0f);
    UnkVec3f pos(player->transform[0][3], player->transform[1][3], player->transform[2][3]);
    fn_7100c03dd0(rail, &pos, &index, &t, &dir, nullptr);
    s32 count = rail->Count();
    if (count > 3) {
        if (self->segment == 0 && index == count - 2) {
            self->segment = index;
            self->blend = t;
        } else if (self->segment == count - 2 && index == 0) {
            self->segment = 0;
            self->blend = t;
        }
    }
    index = fn_71009d6d2c(self, &dir, index, t, dt);
    UnkRailNode_l9d6638* node_a = rail->Node(index);
    UnkRailNode_l9d6638* node_b = rail->Node(index + 1);
    self->current = nullptr;
    if (node_a && node_b) {
        UnkRailCamDef_l9d6638* def_a = DefOf(node_a);
        UnkRailCamDef_l9d6638* def_b = DefOf(node_b);
        UnkRailCam_l9d6638* cam_a = CameraOf(self, def_a);
        UnkRailCam_l9d6638* cam_b = CameraOf(self, def_b);
        if (cam_a && cam_b) {
            if (self->segment != index) {
                Attach(self, cam_a);
                cam_a->Aim(def_a->field_0x30);
                Attach(self, cam_b);
                if (def_a != def_b)
                    cam_b->Aim(def_b->field_0x30);
            }
            self->segment = index;
            if (def_a != def_b && cam_a == cam_b)
                cam_a->Aim(def_a->field_0x30);

            if (IsFixed(def_a))
                Run(cam_a, dt, reset);
            else
                Place(cam_a, def_a);
            self->pose_a = cam_a->pose;
            if (def_a == def_b || self->blend <= 0.0f) {
                self->pose = self->pose_a;
                self->current = cam_a;
                if (reset)
                    self->output = self->pose;
                return;
            }
            if (cam_a == cam_b)
                cam_a->Aim(def_b->field_0x30);
            if (IsFixed(def_b))
                Run(cam_b, dt, reset);
            else
                Place(cam_b, def_b);
            self->pose_b = cam_b->pose;
            if (self->blend >= 1.0f) {
                self->pose = self->pose_b;
                self->current = cam_b;
                if (reset)
                    self->output = self->pose;
                return;
            }
            if (cam_a == cam_b)
                self->current = cam_a;
        }
    }
    self->pose = fn_71009d77cc(&self->pose_a, &self->pose_b, self->blend);
    if (reset)
        self->output = self->pose;
}
