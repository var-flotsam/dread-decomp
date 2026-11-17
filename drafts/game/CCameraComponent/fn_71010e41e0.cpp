// CCameraComponent's unit (tools/tu_map.py unit 2691): makes a camera controller the active one.
// Every registered controller is marked inactive; the controller is (re)inserted into the
// component's stack by priority, the one it displaces is suspended (and dropped when it asks to
// be), and when the change is immediate the scene camera takes the new top's settings. Names and
// layouts are placeholders.
#include "base/UnkSList.h"
#include "game/CCameraComponent.h"
#include "game/CCameraCtrl.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU131.h"
#include "game/UnkTU2656.h"
#include "types.h"

struct UnkCam_b0258i;
struct UnkScreenFadeState_b48a3;
bool fn_7101053a1c(UnkScreenFadeState_b48a3* self, f32 t);
void fn_7100145388(void* scene, void* value);
void fn_71001b7d80(UnkCam_b0258i* self, f32 fov);
void fn_71001bab1c(UnkCam_b0258i* self, f32 value);

// A camera controller as the stack sees it.
struct UnkCamCtrl_l0e41e0 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();  // activated
    virtual void vf_0x38();  // suspended
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();  // removed
    virtual void vf_0x80();
    virtual void vf_0x88();  // resumed

    u8 pad_0x8[0x20];
    s32 priority;  // 0x28
    u8 pad_0x2c[0x9];
    bool drop_when_covered;  // 0x35
    u8 pad_0x36[0x2];
    u8 field_0x38[0xc];
    u8 field_0x44[0xc];
    u32 field_0x50[3];
    f32 fov;  // 0x5c
    u32 field_0x60;
    f32 field_0x64;
    u8 pad_0x68[0x30];
    // A flag kept as raw byte storage: its stores alias the component's arrays.
    u8 inactive[1];  // 0x98
};
static_assert(__builtin_offsetof(UnkCamCtrl_l0e41e0, inactive) == 0x98, "layout");

void fn_71010ee048(UnkCamCtrl_l0e41e0* ctrl);

// The render camera the scene camera drives.
struct UnkRenderCam_l0e41e0 {
    u8 pad_0x0[0x90];
    u32 field_0x90;
    u8 pad_0x94[0x88];
    u32 field_0x11c[3];
};

// The scene camera: setting a value marks it changed.
struct UnkSceneCam_l0e41e0 {
    u8 pad_0x0[0x11b];
    u8 changed[1];  // 0x11b, a flag kept as raw byte storage
    u8 pad_0x11c[0x14];
    UnkRenderCam_l0e41e0* render;  // 0x130

    UnkCam_b0258i* Render() { return reinterpret_cast<UnkCam_b0258i*>(render); }
    void SetFov(f32 fov) {
        fn_71001b7d80(Render(), fov);
        changed[0] = true;
    }
    void SetValue(f32 value) {
        fn_71001bab1c(Render(), value);
        changed[0] = true;
    }
};

struct UnkCamStackTag_l0e41e0;
using UnkCamStack_l0e41e0 = base::UnkSList<UnkCamStackTag_l0e41e0>;
using UnkCamNode_l0e41e0 = UnkCamStack_l0e41e0::Node;

// Inserts `ctrl` before `pos` (UnkSList::InsertBefore, out of line).
void fn_71010e7c0c(UnkCamStack_l0e41e0* stack, UnkCamCtrl_l0e41e0* const* ctrl, UnkCamNode_l0e41e0* pos);

struct UnkCamComp_l0e41e0 {
    u8 pad_0x0[0x40];
    UnkSceneCam_l0e41e0* scene;  // 0x40
    u8 pad_0x48[0x8];
    UnkCamStack_l0e41e0 stack;  // 0x50
    UnkCamCtrl_l0e41e0** ctrls;  // 0x80
    u8 pad_0x88[0x8];
    u32 ctrl_count;  // 0x90
};
static_assert(__builtin_offsetof(UnkCamComp_l0e41e0, ctrls) == 0x80, "layout");

static inline UnkCamCtrl_l0e41e0* Ctrl(const UnkCamNode_l0e41e0* node) {
    return reinterpret_cast<UnkCamCtrl_l0e41e0*>(node->value);
}

void fn_71010e41e0(UnkCamComp2_ovb5* self_, UnkCamEntry_ovb5* entry, bool flag) {
    UnkCamComp_l0e41e0* self = reinterpret_cast<UnkCamComp_l0e41e0*>(self_);
    UnkCamCtrl_l0e41e0* ctrl = reinterpret_cast<UnkCamCtrl_l0e41e0*>(entry);
    bool now = flag || fn_7101053a0c(g_unk_7101d47310->field_0x1040) ||
               fn_7101053a1c(reinterpret_cast<UnkScreenFadeState_b48a3*>(g_unk_7101d47310->field_0x1040), 0.0f);
    for (UnkCamCtrl_l0e41e0** it = self->ctrls; it != self->ctrls + self->ctrl_count; ++it)
        (*it)->inactive[0] = true;

    UnkCamStack_l0e41e0& stack = self->stack;
    if (stack.head && Ctrl(stack.head) == ctrl) {
        if (!now) {
            fn_71010ee048(ctrl);
            return;
        }
        if (ctrl) {
            ctrl->vf_0x88();
            fn_7100145388(self->scene, ctrl->field_0x38);
            fn_71001453c4(reinterpret_cast<UnkSceneObj_71010e2278*>(self->scene),
                          reinterpret_cast<UnkArg2_71010e2278*>(ctrl->field_0x44));
            self->scene->SetValue(ctrl->field_0x64);
        }
    } else {
        ctrl->inactive[0] = !now;
        UnkCamNode_l0e41e0* node = stack.head;
        for (; node; node = node->next)
            if (Ctrl(node) == ctrl)
                break;
        if (node) {
            if (ctrl)
                ctrl->vf_0x78();
            stack.Remove(node);
        }
        if (stack.count == 0) {
            stack.PushFront(reinterpret_cast<u64>(ctrl));
            ctrl->vf_0x30();
            fn_71010ed948(reinterpret_cast<UnkCam_l450950*>(ctrl), self->scene, true);
            ctrl->vf_0x70();
        } else {
            UnkCamNode_l0e41e0* pos = stack.head;
            UnkCamCtrl_l0e41e0* below = nullptr;
            for (; pos; pos = pos->next) {
                below = Ctrl(pos);
                if (below && below->priority <= ctrl->priority)
                    break;
            }
            if (!pos) {
                ctrl->vf_0x70();
                stack.PushBack(reinterpret_cast<u64>(ctrl));
            } else {
                ctrl->vf_0x30();
                if (pos == stack.head) {
                    below->vf_0x38();
                    ctrl->vf_0x70();
                    fn_71010e7c0c(&stack, &ctrl, pos);
                    fn_71010ed948(reinterpret_cast<UnkCam_l450950*>(ctrl), self->scene, true);
                    if (below->drop_when_covered) {
                        below->vf_0x78();
                        stack.Remove(pos);
                    }
                } else {
                    ctrl->vf_0x70();
                    fn_71010e7c0c(&stack, &ctrl, pos);
                }
            }
        }
    }

    if (now && self->scene && stack.count != 0 && Ctrl(stack.head) == ctrl) {
        fn_7100145388(self->scene, ctrl->field_0x38);
        fn_71001453c4(reinterpret_cast<UnkSceneObj_71010e2278*>(self->scene),
                      reinterpret_cast<UnkArg2_71010e2278*>(ctrl->field_0x44));
        UnkRenderCam_l0e41e0* render = self->scene->render;
        render->field_0x11c[0] = ctrl->field_0x50[0];
        render->field_0x11c[1] = ctrl->field_0x50[1];
        render->field_0x11c[2] = ctrl->field_0x50[2];
        self->scene->SetFov(ctrl->fov);
        self->scene->render->field_0x90 = ctrl->field_0x60;
        self->scene->changed[0] = true;
        self->scene->SetValue(ctrl->field_0x64);
    }
}
