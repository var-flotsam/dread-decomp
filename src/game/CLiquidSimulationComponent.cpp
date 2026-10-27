// CLiquidSimulationComponent's unit (its reflection is src/reflection/CLiquidSimulationComponent.cpp):
// a liquid surface simulated as a row of column heights, stepped at a fixed 1/60 s while its owner
// is in the camera's view.
// Holds fn_7100905e9c (the update). Names and layouts are placeholders.
#include <float.h>
#include <stdlib.h>

#include "base/reflection/CClass.h"
#include "game/UnkGameManager.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkLiquidVec2 {
    f32 x, y;
    UnkLiquidVec2(f32 x_, f32 y_) : x(x_), y(y_) {}
};

// Heights are read and written as words that may alias anything (the original reloads the column
// count after each store).
typedef f32 __attribute__((may_alias)) UnkLiquidHeight;

// A 2D box: minimum then maximum corner.
struct UnkLiquidViewBox {
    f32 minX, minY, maxX, maxY;
};

// The view's bounds (false when there is none).
bool fn_71010e53dc(void* camera, UnkLiquidViewBox* box);

// The owner's "ACTIVATABLE" component (src/reflection_statics/statics_00.cpp instantiates it).
UnkReflObject* fn_7100899610(void* owner);

// The component's placement as the liquid uses it: its size and its position.
struct UnkLiquidVolume : UnkReflObject {
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual void vf_0xe8();
    virtual void vf_0xf0();
    virtual void vf_0xf8();
    virtual void vf_0x100();
    virtual void vf_0x108();
    virtual void vf_0x110();
    virtual void vf_0x118();
    virtual void vf_0x120();
    virtual void vf_0x128();
    virtual void vf_0x130();
    virtual void vf_0x138();
    virtual void vf_0x140();
    virtual void vf_0x148();
    virtual void vf_0x150();
    virtual void vf_0x158();
    virtual void vf_0x160();
    virtual void vf_0x168();
    virtual void vf_0x170();
    virtual UnkVec3f GetSize();      // vf_0x178
    virtual UnkVec3f GetPosition();  // vf_0x180
};

struct CLiquidSimulationComponent {
    u8 unk_0x0[0x30];
    void* owner;  // 0x30
    u8 unk_0x38[0x854 - 0x38];
    UnkLiquidHeight heights[0x200];  // 0x854
    u32 columnCount;     // 0x1054
    u8 unk_0x1058[4];
    f32 amplitude;  // 0x105c
    f32 time;       // 0x1060
};

// One fixed simulation step.
void fn_7100906110(CLiquidSimulationComponent* self, f32 dt);

// Unless the owner is out of view, disturbs a random column (and the one before it) and runs as
// many 1/60 s steps as the accumulated time holds; false when there is no view or it has no bounds.
// NON_MATCHING(minor): the original materialises 0.5 once and keeps it in s9 across the calls (it
// saves d12 as well, so the saved-register slots and the FP register numbers differ), adds the
// column's height and the disturbance with the fadd operands the other way round, and checks the
// second neighbour's index against 0 on one merged path where ours splits it by the first check's
// outcome (same conditions).
bool fn_7100905e9c(CLiquidSimulationComponent* self, f32 dt) {
    void* camera = fn_7100defcec(g_unk_7101d47310);
    UnkLiquidViewBox view;
    view.minX = FLT_MAX;
    view.minY = FLT_MAX;
    view.maxX = -FLT_MAX;
    view.maxY = -FLT_MAX;
    if (!camera)
        return false;
    if (!fn_71010e53dc(camera, &view))
        return false;
    if (UnkLiquidVolume* volume = static_cast<UnkLiquidVolume*>(fn_7100899610(self->owner))) {
        UnkLiquidVec2 size(volume->GetSize().x, volume->GetSize().y);
        UnkLiquidVec2 pos(volume->GetPosition().x, volume->GetPosition().y);
        if (!(pos.y - size.y * 0.5f <= view.maxY && size.x * 0.5f + pos.x >= view.minX &&
              pos.x - size.x * 0.5f <= view.maxX && size.y * 0.5f + pos.y >= view.minY))
            return false;
    }
    s32 r = rand();
    if (self->columnCount != 0) {
        f32 f = (f32)((r & 0x7fff) * (1.0 / 32767.0));
        if (f >= 0.0f && f <= 1.0f) {
            u32 column = (u32)((f32)(self->columnCount - 1) * f);
            f32 a = self->amplitude * 0.5f;
            self->heights[column] += a * 0.5f;
            for (s32 i = (s32)(column - 1); i < (s32)(column + 1); ++i) {
                if (i >= 0 && i <= (s32)(self->columnCount - 1))
                    self->heights[i] = a * 0.3f + self->heights[i];
            }
        }
    }
    f32 t = self->time + dt;
    self->time = t;
    while (t > 0.016f) {
        fn_7100906110(self, 0.016f);
        t = self->time - 0.016f;
        self->time = t;
    }
    return true;
}
