// CCameraComponent's unit (tools/tu_map.py unit 2691): the component's update. Counts down its
// lifetime, drops the cameras at the front of its queue that have finished (applying their final
// view), blends the field of view towards the current camera's, updates it and its modifiers and
// hands the resulting view to the scene object. Names and layouts are placeholders.
#include "base/UnkAtExit.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CActorComponent.h"
#include "game/CCameraComponent.h"
#include "game/CCameraCtrl.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU131.h"
#include "game/UnkTU39.h"
#include "types.h"

using base::global::CStrId;

struct UnkUnit282Widget;
void fn_7100205f98(UnkUnit282Widget* self, f32 x, f32 y, f32 z, void* key);
void fn_7100145388(void* scene, void* value);

struct UnkCamVec3_l0e3af0 {
    u32 x, y, z;
};

// The camera's view: a placement, an orientation, a position and three settings.
struct UnkCamView_l0e3af0 {
    UnkCamVec3_l0e3af0 placement;    // +0x0, fn_7100145388 / fn_7100205f98
    UnkCamVec3_l0e3af0 orientation;  // +0xc, fn_71001453c4
    UnkCamVec3_l0e3af0 position;     // +0x18
    f32 fov;                         // +0x24
    u32 field_0x28;
    f32 field_0x2c;
};

struct UnkCamSceneSub_l0e3af0 {
    u8 pad_0x0[0x90];
    u32 field_0x90;
    u8 pad_0x94[0x88];
    UnkCamVec3_l0e3af0 position;  // +0x11c
    u8 pad_0x128[0x156];
    bool locked;  // +0x27e
};
static_assert(__builtin_offsetof(UnkCamSceneSub_l0e3af0, position) == 0x11c, "layout");
static_assert(__builtin_offsetof(UnkCamSceneSub_l0e3af0, locked) == 0x27e, "layout");

void fn_71001bab1c(UnkCamSceneSub_l0e3af0* sub, f32 value);

struct UnkCamScene_l0e3af0 {
    u8 pad_0x0[0x20];
    u8 xform[0xfb];
    bool dirty;  // +0x11b
    u8 pad_0x11c[0x14];
    UnkCamSceneSub_l0e3af0* sub;  // +0x130

    void SetField90(u32 v) {
        sub->field_0x90 = v;
        dirty = true;
    }
    void SetScale(f32 v) {
        fn_71001bab1c(sub, v);
        dirty = true;
    }
};
static_assert(__builtin_offsetof(UnkCamScene_l0e3af0, sub) == 0x130, "layout");

struct UnkCamera_l0e3af0 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void Finish();  // vf_0x38
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void Update(f32 dt);  // vf_0x58
    virtual bool IsDone();        // vf_0x60
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void Remove();  // vf_0x78
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual f32 GetFov();  // vf_0xb8

    u8 pad_0x8[0x10];
    void* field_0x18;
    u8 pad_0x20[0x18];
    UnkCamView_l0e3af0 view;  // +0x38
    u8 pad_0x68[0x5c];
    f32 fov;  // +0xc4
};
static_assert(__builtin_offsetof(UnkCamera_l0e3af0, fov) == 0xc4, "layout");

struct UnkCamModifier_l0e3af0 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void Apply(f32 dt, UnkCamScene_l0e3af0* scene, UnkCamVec3_l0e3af0* placement,
                       UnkCamVec3_l0e3af0* orientation, f32* fov, u32* field_0x28);  // vf_0x30
    virtual bool IsDone();  // vf_0x38
    virtual void vf_0x40();
    virtual void Destroy();  // vf_0x48
};

template <class T>
struct UnkCamNode_l0e3af0 {
    T* value;
    UnkCamNode_l0e3af0* next;
};

template <class T>
struct UnkCamList_l0e3af0 {
    typedef UnkCamNode_l0e3af0<T> Node;
    void* vtable;
    u32 count;
    Node* head;
    Node* tail;
    base::IAllocator* allocator;

    // Unlinks and frees `node`; returns the node after it, or null when it is not in the list.
    Node* Erase(Node* node) {
        Node* prev = nullptr;
        for (Node* n = head; n;) {
            Node* next = n->next;
            if (n == node) {
                if (prev) {
                    prev->next = next;
                    if (tail == node)
                        tail = prev;
                } else {
                    head = next;
                }
                if (--count == 0) {
                    head = nullptr;
                    tail = nullptr;
                }
                allocator->vf_0x48(node);
                return next;
            }
            prev = n;
            n = next;
        }
        return nullptr;
    }
};

struct UnkCamState_l0e3af0 {
    u8 pad_0x0[0x10];
    bool active;
};

struct UnkCamStates_l0e3af0 {
    UnkCamState_l0e3af0** values;
    CStrId* keys;
    u32 count;

    UnkCamState_l0e3af0** Find(const CStrId& key) {
        u32 i = 0;
        for (; i < count; ++i) {
            if (keys[i].data == key.data)
                return &values[i];
        }
        return &values[count];
    }
};

struct UnkCameraComponent_l0e3af0 {
    u8 pad_0x0[0x40];
    UnkCamScene_l0e3af0* scene;
    u8 pad_0x48[0x8];
    UnkCamList_l0e3af0<UnkCamera_l0e3af0> cameras;  // +0x50
    u8 pad_0x78[0x8];
    UnkCamStates_l0e3af0 states;  // +0x80
    u8 pad_0x98[0x10];
    UnkCamList_l0e3af0<UnkCamModifier_l0e3af0> modifiers;  // +0xa8
    u8 pad_0xd0[0x20];
    u8 slot_0xf0[0xb8];
    u8 slot_0x1a8[0xb0];
    f32 default_fov;  // +0x258
    f32 fov;          // +0x25c
    u8 pad_0x260[0x4];
    f32 fov_speed;     // +0x264
    f32 override_fov;  // +0x268
    f32 override_time;  // +0x26c
    bool check_state;   // +0x270
    u8 pad_0x271[0x9af];
    bool scaled;     // +0xc20
    f32 lifetime;    // +0xc24
};
static_assert(__builtin_offsetof(UnkCameraComponent_l0e3af0, modifiers) == 0xa8, "layout");
static_assert(__builtin_offsetof(UnkCameraComponent_l0e3af0, default_fov) == 0x258, "layout");
static_assert(__builtin_offsetof(UnkCameraComponent_l0e3af0, scaled) == 0xc20, "layout");

// The id table's slot at +0x1080 (the header has no member for it yet).
inline const CStrId& CameraStateId_l0e3af0() {
    static UnkGameManagerIdTable s_table asm("g_unk_7101d552d0");
    return *reinterpret_cast<const CStrId*>(reinterpret_cast<const u8*>(&s_table) + 0x1080);
}

// The game manager's time scale at +0x6a4 (the header has no member for it yet).
static inline f32 TimeScale_l0e3af0(const UnkGameManager* mgr) {
    return *reinterpret_cast<const f32*>(reinterpret_cast<const u8*>(mgr) + 0x6a4);
}

void fn_71010e3af0(UnkCameraComponent_l0e3af0* self, f32 dt) {
    if (!fn_710024fbc8(self))
        return;
    if (self->lifetime > 0.0f) {
        self->lifetime -= dt;
        if (self->lifetime <= 0.0f) {
            fn_710024fa0c(reinterpret_cast<UnkStruct_710024fa0c*>(self), false);
            return;
        }
    }
    if (self->scaled)
        dt /= TimeScale_l0e3af0(g_unk_7101d47310);
    if (!self->scene)
        return;

    if (self->check_state) {
        UnkCamState_l0e3af0** it = self->states.Find(CameraStateId_l0e3af0());
        if (it != &self->states.values[self->states.count] && (*it)->active)
            fn_71010e41e0(reinterpret_cast<UnkCamComp2_ovb5*>(self),
                          reinterpret_cast<UnkCamEntry_ovb5*>(*it), false);
    }

    // Drop the finished cameras at the front of the queue, applying the view of each.
    typedef UnkCamList_l0e3af0<UnkCamera_l0e3af0>::Node CamNode;
    CamNode* node = self->cameras.head;
    if (!node)
        return;
    UnkCamera_l0e3af0* camera;
    bool dropped = false;
    for (;;) {
        camera = node->value;
        if (camera) {
            if (!camera->IsDone()) {
                if (dropped)
                    fn_71010ed948(reinterpret_cast<UnkCam_l450950*>(camera), self->scene, false);
                break;
            }
            if (camera->field_0x18) {
                UnkCamVec3_l0e3af0 placement = camera->view.placement;
                UnkCamVec3_l0e3af0 orientation = camera->view.orientation;
                UnkCamScene_l0e3af0* scene = self->scene;
                if (!scene->sub->locked) {
                    UnkCamVec3_l0e3af0 position = camera->view.position;
                    u32 field_0x28 = camera->view.field_0x28;
                    f32 scale = camera->view.field_0x2c;
                    fn_7100145388(scene, &placement);
                    fn_71001453c4(reinterpret_cast<UnkSceneObj_71010e2278*>(self->scene),
                                  reinterpret_cast<UnkArg2_71010e2278*>(&orientation));
                    scale = scale > 1.0f ? scale : 1.0f;
                    self->scene->sub->position = position;
                    self->scene->SetField90(field_0x28);
                    self->scene->SetScale(scale);
                }
                camera->Finish();
            }
            node->value->Remove();
            node = self->cameras.Erase(node);
            dropped = true;
        } else {
            node = self->cameras.Erase(node);
        }
        if (!node) {
            if (!camera)
                return;
            break;
        }
    }

    // Blend the field of view towards the camera's.
    self->override_time -= dt;
    f32 target;
    if (camera->fov > 0.0f) {
        target = camera->fov;
    } else if (self->override_time > 0.0f) {
        target = self->override_fov;
    } else if (camera->GetFov() > 0.0f) {
        target = camera->GetFov();
    } else {
        target = self->default_fov;
        self->override_time = 0.0f;
    }
    if (self->fov_speed >= 0.0f) {
        f32 t = dt * self->fov_speed;
        f32 k = t > 0.0f ? (!(t >= 1.0f) ? t : 1.0f) : 0.0f;
        target = self->fov + k * (target - self->fov);
    }
    self->fov = target;

    // Update the front camera until it stays at the front.
    for (;;) {
        camera->Update(dt);
        CamNode* head = self->cameras.head;
        UnkCamera_l0e3af0* front = head ? head->value : nullptr;
        if (camera == front)
            break;
        camera = head->value;
    }

    UnkCamVec3_l0e3af0 placement = camera->view.placement;
    UnkCamVec3_l0e3af0 orientation = camera->view.orientation;
    UnkCamVec3_l0e3af0 position = camera->view.position;
    f32 fov = camera->view.fov;
    u32 field_0x28 = camera->view.field_0x28;
    f32 scale = camera->view.field_0x2c;
    if (self->modifiers.count != 0) {
        typedef UnkCamList_l0e3af0<UnkCamModifier_l0e3af0>::Node ModNode;
        for (ModNode* m = self->modifiers.head; m;) {
            UnkCamModifier_l0e3af0* modifier = m->value;
            modifier->Apply(dt, self->scene, &placement, &orientation, &fov, &field_0x28);
            if (modifier->IsDone()) {
                m = self->modifiers.Erase(m);
                modifier->Destroy();
            } else {
                m = m->next;
            }
        }
    }

    UnkCamScene_l0e3af0* scene = self->scene;
    if (scene->sub->locked)
        return;
    f32 y = fn_7100030dec(reinterpret_cast<UnkFloatList_b46a1*>(self->slot_0xf0));
    f32 z = fn_7100030dec(reinterpret_cast<UnkFloatList_b46a1*>(self->slot_0x1a8));
    fn_7100205f98(reinterpret_cast<UnkUnit282Widget*>(scene->xform), fov, y, z, &placement);
    fn_71001453c4(reinterpret_cast<UnkSceneObj_71010e2278*>(self->scene),
                  reinterpret_cast<UnkArg2_71010e2278*>(&orientation));
    scale = scale > 1.0f ? scale : 1.0f;
    self->scene->sub->position = position;
    self->scene->SetField90(field_0x28);
    self->scene->SetScale(scale);
}
