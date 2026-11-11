// GUI::CModel3D (unit 2753 of tools/tu_map.py, the name from reflection): the model's life cycle.
// - fn_71011f3cfc is the destructor body (before the display object's, fn_71011ccfb4).
// - fn_71011f3e0c sets the defaults and takes the "Model3DCustomCC" character class (fn_7100e051b4).
// - fn_71011f4590 creates the main entity from a character class looked up by id (fn_71011938bc);
//   fn_71011f46a0 creates it in the mode set at +0x198 (0: by class id, 1: from the character class
//   at +0x218 with the model path at +0x1b0), then mode 3, and resets and shows it.
// - fn_71011f4350 is the update: the display object's, the entity's two steps, the spin around +0x88,
//   and after `idle_delay` seconds the idle event sent to the entity's animation (when it is in the
//   state the id table names at +0x1d78).
// Names and layouts are placeholders.
#include "../../../game_s2/UnkSpitSend.h"
#include "base/UnkCountedResource.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkTU2747.h"
#include "game/UnkTU341.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

extern char g_unk_7101cb6608[];  // CModel3D's vtable
void fn_71011cdfa8(UnkGuiDisplayObject* self, f32 dt);
struct UnkActorDefFactory;
extern UnkActorDefFactory* g_unk_71726a0220;
struct UnkRemovedValue_b60;
void fn_7100debc28(UnkGameManager*, UnkRemovedValue_b60*);
struct UnkDropTarget;
extern UnkDropTarget* g_unk_7101d47318;
struct UnkTreeNode;
struct UnkModelUpdater_ovb0032;
void fn_7101171a08(UnkModelUpdater_ovb0032*, const void*, const CRntString*);
struct UnkStruct_710117af10;
struct UnkCls_710117af10;

// A counted reference to a resource whose destructor reads the pointer once (as in
// src/game/CSpawnPointComponent/CSpawnPointComponent_b52.cpp).
struct UnkSpawnDefRef_b52 {
    UnkCountedResource* ptr;

    UnkSpawnDefRef_b52& operator=(const UnkSpawnDefRef_b52& other) {
        UnkCountedResource* p = other.ptr;
        if (ptr != p) {
            if (p)
                ++p->ref_count;
            UnkCountedResource* old = ptr;
            if (old && --old->ref_count == 0)
                fn_710001737c(old);
            ptr = p;
        }
        return *this;
    }
    ~UnkSpawnDefRef_b52() {
        UnkCountedResource* p = ptr;
        if (p && --p->ref_count == 0)
            fn_710001737c(p);
    }
};
UnkSpawnDefRef_b52 fn_71011938bc(UnkActorDefFactory* factory, const base::global::CStrId* type, const base::global::CFilePathStrId* path);

// The character class with this name.
UnkSpawnDefRef_b52 fn_7100e051b4(UnkActorDefFactory* factory, const CStrId* name);
// Applies a character class to the model.
void fn_71011f3f88(UnkCountedResource* char_class);
// The entity the game manager creates under `name` from a character class.
void* fn_7100dea8d0(UnkGameManager* manager, const char* name, void* char_class, bool flag);
// Steps of the entity's set-up and update.
void fn_71002324f0(void* entity);
void fn_71002325f8(void* entity);
void fn_7100232938(void* entity);
void fn_7100d7b068(void* entity, u32 index, f32 dt);
void fn_7100d7b1c0(void* entity, u32 index, f32 dt);

// The scene's model manager: vf_0x38 is told before a model creates its entity.
struct UnkModelScene_ovb0109 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38(u32 a, u32 b);
};

struct UnkReflObject_ovb0109 {
    virtual const base::reflection::CClass* GetClass();
};

template <const base::reflection::CClass* (*Type)()>
static inline void* AsKind(void* o) {
    return o && fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(
                                  static_cast<UnkReflObject_ovb0109*>(o)->GetClass()),
                              reinterpret_cast<const base::reflection::CType*>(Type()))
               ? o
               : nullptr;
}

// The id table (fn_7100031894 builds it; as src/game/CAbilitySonar/CAbilitySonar_Pulse_ovb0085.cpp).
struct UnkGameManagerIdTable {
    u8 pad_0x0[0x1d78];
    UnkCls_710117af10* field_0x1d78;
    u8 pad_0x1d80[0x2578 - 0x1d80];
    UnkGameManagerIdTable() asm("fn_7100031894");
    ~UnkGameManagerIdTable() asm("fn_71000407f0");
};
inline UnkGameManagerIdTable& GameManagerIdTable() {
    static UnkGameManagerIdTable s_table;
    return s_table;
}

struct UnkModelUpdaterView_ovb0109 {
    u8 pad_0x0[0x40];
    s32 frame;  // 0x40
};

struct UnkModelEntity_ovb0109 {
    u8 pad_0x0[0x220];
    UnkStruct_710117af10* animation;       // 0x220
    UnkModelUpdaterView_ovb0109* updater;  // 0x228
};

struct UnkGuiModel3D_ovb0109 {
    char* vtable;
    u8 base_0x8[0x80];  // the display object (game/UnkGuiDisplayObject.h)
    f32 angle;          // 0x88
    u8 base_0x8c[0xfd];
    bool field_0x189;
    u8 pad_0x18a[0x2];
    f32 spin_speed;   // 0x18c
    f32 field_0x190;  // 100
    bool field_0x194;
    u8 pad_0x195[0x3];
    u32 mode;  // 0x198: 3 once the entity is made
    u8 pad_0x19c[0x4];
    CStrId char_class_name;  // 0x1a0
    u8 pad_0x1a8[0x8];
    CRntString model_path;  // 0x1b0
    CRntString field_0x1e0;
    u8 pad_0x210[0x8];
    UnkSpawnDefRef_b52 char_class;  // 0x218
    f32 field_0x220;
    f32 field_0x224;
    f32 field_0x228;
    f32 field_0x22c;
    f32 field_0x230;
    f32 field_0x234;
    f32 field_0x238;
    f32 field_0x23c;
    u64 field_0x240;
    u32 field_0x248;
    f32 field_0x24c[3];
    bool field_0x258;
    u8 pad_0x259[0x7];
    UnkModelEntity_ovb0109* entity;  // 0x260
    CStrId idle_event;               // 0x268
    f32 idle_delay;                  // 0x270
    f32 idle_time;                   // 0x274
};
static_assert(__builtin_offsetof(UnkGuiModel3D_ovb0109, field_0x189) == 0x189, "UnkGuiModel3D_ovb0109 layout");
static_assert(__builtin_offsetof(UnkGuiModel3D_ovb0109, char_class) == 0x218, "UnkGuiModel3D_ovb0109 layout");
static_assert(__builtin_offsetof(UnkGuiModel3D_ovb0109, field_0x24c) == 0x24c, "UnkGuiModel3D_ovb0109 layout");
static_assert(__builtin_offsetof(UnkGuiModel3D_ovb0109, idle_time) == 0x274, "UnkGuiModel3D_ovb0109 layout");

static inline UnkGuiDisplayObject* Display(UnkGuiModel3D_ovb0109* self) {
    return reinterpret_cast<UnkGuiDisplayObject*>(self);
}

static inline u64 Hash(const CStrId& id) {
    return *reinterpret_cast<const u64*>(reinterpret_cast<const char*>(id.data) + 0x30);
}

void fn_71011f3cfc(UnkGuiModel3D_ovb0109* self) {
    self->vtable = g_unk_7101cb6608 + 0x10;
    self->idle_event.Reset();
    if (UnkCountedResource* p = self->char_class.ptr) {
        if (--p->ref_count == 0)
            fn_710001737c(p);
    }
    fn_710007ecd4(&self->field_0x1e0);
    fn_710007ecd4(&self->model_path);
    self->char_class_name.Reset();
    fn_71011ccfb4(reinterpret_cast<char*>(self));
}

void fn_71011f3e0c(UnkGuiModel3D_ovb0109* self) {
    fn_71011cd4b4(self);
    self->mode = 3;
    self->spin_speed = 0.0f;
    self->field_0x190 = 100.0f;
    self->field_0x189 = true;
    self->field_0x194 = false;
    self->field_0x240 = 0;
    __builtin_memset(&self->field_0x248, 0, 8);
    __builtin_memset(&self->field_0x24c[1], 0, 8);
    self->field_0x258 = false;
    self->field_0x22c = -3.4028235e38f;
    self->field_0x230 = 3.4028235e38f;
    self->field_0x224 = -1.0f;
    self->field_0x228 = 0.0f;
    self->field_0x220 = -1.0f;
    self->field_0x234 = -1.0f;
    self->field_0x238 = -1.0f;
    self->field_0x23c = -10000.0f;
    UnkActorDefFactory* factory = g_unk_71726a0220;
    self->char_class = fn_7100e051b4(factory, &static_cast<const CStrId&>(CStrId("Model3DCustomCC", true)));
    fn_71011f3f88(self->char_class.ptr);
}

void fn_71011f4350(UnkGuiModel3D_ovb0109* self, f32 dt) {
    fn_71011cdfa8(Display(self), dt);
    UnkGuiObjectState* state = fn_71011cde14(Display(self));
    if (!state->field_0x3c || state->frame == 0x7fffffff || !self->entity)
        return;
    fn_7100d7b068(self->entity, 0, dt);
    fn_7100d7b1c0(self->entity, 0, dt);
    f32 speed = self->spin_speed;
    if (__builtin_fabsf(speed) > 1.1920929e-07f)
        self->angle = speed * dt + self->angle;
    if (!(self->idle_delay > 0.0f) || self->idle_event.data == &g_unk_7101d47108->empty)
        return;
    UnkStruct_710117af10* animation = self->entity->animation;
    if (!animation || !fn_710117af10(animation, &GameManagerIdTable().field_0x1d78, false))
        return;
    f32 t = self->idle_time + dt;
    self->idle_time = t;
    if (!(t >= self->idle_delay))
        return;
    UnkSpitSend(animation, Hash(self->idle_event), 0xf4061ff25a2163d6);
    self->idle_time = 0.0f;
}

void* fn_71011f4590(UnkGuiModel3D_ovb0109* self, const CStrId* type, const CRntString& name) {
    fn_71011cde14(Display(self));
    UnkSpawnDefRef_b52 def = fn_71011938bc(g_unk_71726a0220, type, &g_unk_7101d470e8);
    if (!def.ptr)
        return nullptr;
    void* entity = AsKind<fn_7100d70c20>(
        fn_7100dea8d0(g_unk_7101d47310, name.data, AsKind<fn_71011abbd4>(def.ptr), false));
    fn_71002325f8(self->entity);
    fn_710023272c(self->entity);
    return entity;
}

void fn_71011f46a0(UnkGuiModel3D_ovb0109* self) {
    if (self->entity)
        fn_7100debc28(g_unk_7101d47310, reinterpret_cast<UnkRemovedValue_b60*>(self->entity));
    reinterpret_cast<UnkModelScene_ovb0109*>(g_unk_7101d47318)->vf_0x38(0, 0);
    UnkGuiObjectState* state = fn_71011cde14(Display(self));
    if (state->frame == 0x7fffffff)
        return;
    switch (self->mode) {
    case 0:
        self->entity = static_cast<UnkModelEntity_ovb0109*>(
            fn_71011f4590(self, &self->char_class_name, CRntString("Model3DMainEntity")));
        break;
    case 1:
        if (self->model_path.data && self->model_path.data[0]) {
            self->entity = static_cast<UnkModelEntity_ovb0109*>(AsKind<fn_7100d70c20>(
                fn_7100dea8d0(g_unk_7101d47310, "Model3DMainEntity", self->char_class.ptr, false)));
            fn_71002324f0(self->entity);
            UnkModelUpdaterView_ovb0109* updater = self->entity->updater;
            updater->frame = state->frame;
            fn_7101171a08(reinterpret_cast<UnkModelUpdater_ovb0032*>(updater), &self->model_path,
                          &g_unk_7101d58d10);
            fn_71002325f8(self->entity);
            fn_710023272c(self->entity);
        }
        break;
    }
    self->mode = 3;
    if (!self->entity)
        return;
    fn_7100232938(self->entity);
    fn_710023298c(reinterpret_cast<UnkTreeNode*>(self->entity));
    self->field_0x24c[0] = 0.0f;
    self->field_0x24c[1] = 0.0f;
    self->field_0x24c[2] = 0.0f;
    self->field_0x258 = true;
    if (!Display(self)->field_0x184) {
        Display(self)->field_0x184 = true;
        Display(self)->vf_0xa0();
    }
}
