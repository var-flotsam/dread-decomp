// CHecathonPlanktonFXComponent (tools/tu_map.py unit 1271): the per-frame update. Fades each of the ten
// plankton slots' models out or in, releasing a faded-out slot's render entries, then fades the shared
// strip and hands its points and indices to the scene object. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/math/CVector4D.h"
#include "base/math/UnkSegment2D.h"
#include "game/UnkRenderManager.h"
#include "game/UnkSceneObject.h"
#include "game/UnkSlots.h"
#include "game/UnkTU253.h"
#include "game/UnkTU315.h"
#include "game/malloc.h"
#include "types.h"

using base::math::CVector4D;

// As src/game/UnkTU140/UnkTU140_GrowAt_ovb0243.cpp defines it (fn_7100149230's element).
typedef u32 __attribute__((may_alias)) a_u32_9230;

struct UnkRecord16_9230 {
    u32 words[4];

    UnkRecord16_9230() : words{0, 0, 0, 0} {}
    UnkRecord16_9230(UnkRecord16_9230&& o) {
        ((a_u32_9230*)this)[0] = ((const a_u32_9230*)&o)[0];
        ((a_u32_9230*)this)[1] = ((const a_u32_9230*)&o)[1];
        ((a_u32_9230*)this)[2] = ((const a_u32_9230*)&o)[2];
        ((a_u32_9230*)this)[3] = ((const a_u32_9230*)&o)[3];
    }
};

UnkRecord16_9230* fn_7100149230(base::global::CRntVector<UnkRecord16_9230>* self, u32 index);

// As src/game/UnkTU2566/UnkTU2566_Minimap_ovb0201.cpp declares it.
void fn_7100148148(UnkSceneObject* self, u32 layer, const UnkSceneSpan& a, const UnkSceneSpan& b);

// As src/game/CHecathonPlanktonFXComponent/CHecathonPlanktonFXComponent_b44.cpp declares it.
struct UnkPlanktonInstance_b44a2;
UnkPlanktonInstance_b44a2* fn_71001be428(void* model);

// A fixed-capacity array kept inline: its count, then room for N elements. Indexing past the end
// first constructs the elements up to the index.
template <class T, u32 N>
struct UnkPlanktonInlineVector_l80d4a0 {
    u32 count;
    u8 storage[N * sizeof(T)];

    UnkPlanktonInlineVector_l80d4a0() : count(0) {}

    T* items() { return reinterpret_cast<T*>(storage); }

    T& operator[](u32 index) {
        u32 n = index + 1;
        if (n > count) {
            for (u32 i = count; i < n; ++i)
                new (&items()[i]) T();
            count = n;
        }
        return items()[index];
    }
};

struct UnkPlanktonMaterial_l80d4a0 {
    u8 pad_0x0[0x30];
    CVector4D* params;
};

struct UnkPlanktonInstance_l80d4a0 {
    u8 pad_0x0[0x14c];
    CVector4D intensity;  // 0x14c
    u8 pad_0x15c[0x94];
    UnkPlanktonMaterial_l80d4a0* intensityBlock;  // 0x1f0
    u8 pad_0x1f8[0x48];
    CVector4D position;  // 0x240
    u8 pad_0x250[0x90];
    UnkPlanktonMaterial_l80d4a0* positionBlock;  // 0x2e0
};
static_assert(__builtin_offsetof(UnkPlanktonInstance_l80d4a0, positionBlock) == 0x2e0, "layout");

struct UnkPlanktonModel_l80d4a0 {
    u8 pad_0x0[0x138];
    UnkSlots* slots;
};

// A render entry of a slot: its handle in the render manager.
struct UnkPlanktonEntry_l80d4a0 {
    u64 handle;
    u64 field_0x8;
    bool active;

    UnkPlanktonEntry_l80d4a0() : handle(0), field_0x8(0), active(false) {}
};

static void SetWhite(CVector4D& colour) {
    colour.x = 1.0f;
    colour.y = 1.0f;
    colour.z = 1.0f;
    colour.w = 1.0f;
}

// A segment with no extent (field_0x14 is left unset, as in the original).
static UnkSegment2D EmptySegment() {
    UnkSegment2D segment;
    segment.field_0x10 = -1.0f;
    segment.start = {0.0f, 0.0f};
    segment.end = {0.0f, 0.0f};
    return segment;
}

struct UnkPlanktonSlot_l80d4a0 {
    UnkSegment2D segment;
    UnkPlanktonModel_l80d4a0* model;  // 0x18
    u32 field_0x20;
    u32 field_0x24;
    UnkPlanktonInlineVector_l80d4a0<UnkPlanktonEntry_l80d4a0, 16> entries;  // 0x28
    u8 pad_0x1ac[0x18];
    u32 field_0x1c4;
    bool field_0x1c8;
    bool fadingOut;  // 0x1c9
    bool fadingIn;   // 0x1ca

    UnkPlanktonSlot_l80d4a0()
        : segment(EmptySegment()), model(nullptr), field_0x20(0), field_0x24(0), field_0x1c4(0),
          field_0x1c8(false), fadingOut(false), fadingIn(false) {}
};
static_assert(sizeof(UnkPlanktonSlot_l80d4a0) == 0x1d0, "layout");

struct UnkPlanktonOwner_l80d4a0 {
    u8 pad_0x0[0x44];
    f32 x;
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
};

struct CHecathonPlanktonFXComponent_l80d4a0 {
    u8 pad_0x0[0x30];
    UnkPlanktonOwner_l80d4a0* owner;  // 0x30
    u8 pad_0x38[0x18];
    UnkPlanktonInlineVector_l80d4a0<UnkPlanktonSlot_l80d4a0, 10> slots;  // 0x50
    f32 fadeInTime;     // 0x1274
    f32 fadeOutTime;    // 0x1278
    f32 fadeInTimer;    // 0x127c
    f32 fadeOutTimer;   // 0x1280
    f32 stripAlpha;     // 0x1284
    u8 pad_0x1288;
    bool stripActive;   // 0x1289
    bool field_0x128a;
    u8 pad_0x128b;
    f32 stripTime;      // 0x128c
    f32 stripTimer;     // 0x1290
    f32 intensity;      // 0x1294
    void* shapes;       // 0x1298
    u32 shapeCount;     // 0x12a0
    u8 pad_0x12a4[0x1c];
    UnkSceneObject* model;  // 0x12c0
    u32 toggle;         // 0x12c8
};
static_assert(__builtin_offsetof(CHecathonPlanktonFXComponent_l80d4a0, fadeInTime) == 0x1274, "layout");
static_assert(__builtin_offsetof(CHecathonPlanktonFXComponent_l80d4a0, model) == 0x12c0, "layout");

static inline UnkPlanktonInstance_l80d4a0* Instance(UnkSceneObject* model) {
    return reinterpret_cast<UnkPlanktonInstance_l80d4a0*>(fn_71001be428(model->field_0x130));
}

void fn_710080d4a0(CHecathonPlanktonFXComponent_l80d4a0* self, f32 dt) {
    for (u32 i = 0; i < 10; ++i) {
        if (self->slots[i].fadingOut) {
            self->fadeOutTimer -= dt;
            if (!(self->fadeOutTimer <= 0.0f)) {
                CVector4D colour;
                SetWhite(colour);
                fn_71001c287c(self->slots[i].model->slots, 0, 0, &colour, 1);
                colour.w = self->fadeOutTimer / self->fadeOutTime * 0.05f;
                fn_71001c2764(self->slots[i].model->slots, 0, 0, &colour, 1, true);
            } else {
                self->fadeOutTimer = self->fadeOutTime;
                self->slots[i].fadingOut = false;
                UnkSegment2D empty = EmptySegment();
                fn_7100125b44(&self->slots[i].segment, &empty);
                self->slots[i].field_0x1c4 = 0;
                self->slots[i].field_0x1c8 = false;
                self->slots[i].field_0x20 = 0;
                self->slots[i].field_0x24 = 0;
                self->slots[i].field_0x1c4 = 0;
                for (u32 j = 0; j < 16; ++j) {
                    if (self->slots[i].entries[j].active) {
                        self->slots[i].entries[j].active = false;
                        self->slots[i].entries[j].field_0x8 = 0;
                        fn_7100219e58(g_unk_7101d47300, self->slots[i].entries[j].handle, false);
                    }
                }
            }
        } else if (self->slots[i].fadingIn) {
            self->fadeInTimer -= dt;
            if (!(self->fadeInTimer <= 0.0f)) {
                CVector4D colour;
                SetWhite(colour);
                fn_71001c287c(self->slots[i].model->slots, 0, 0, &colour, 1);
                colour.w = (1.0f - self->fadeInTimer / self->fadeInTime) * 0.05f;
                fn_71001c2764(self->slots[i].model->slots, 0, 0, &colour, 1, true);
            } else {
                self->fadeInTimer = self->fadeInTime;
                self->slots[i].fadingIn = false;
                CVector4D colour;
                SetWhite(colour);
                fn_71001c287c(self->slots[i].model->slots, 0, 0, &colour, 1);
                colour.w = 0.05f;
                fn_71001c2764(self->slots[i].model->slots, 0, 0, &colour, 1, true);
            }
        }
    }

    if (!self->stripActive)
        return;
    self->stripTimer -= dt;
    if (!(self->stripTimer <= 0.0f)) {
        u32 n = self->shapeCount;
        if (n == 0 || !self->model)
            return;
        f32 t = self->stripTimer / self->stripTime;
        f32 alpha = t;
        if (t - 0.001f <= 1.0f && t + 0.001f >= 1.0f)
            alpha = 1.0f;
        CVector4D colour = {1.0f, 1.0f - t, 1.0f, alpha};
        self->stripAlpha = alpha;

        base::global::CRntVector<u16> indices(0xf);
        base::global::CRntVector<UnkRecord16_9230> points(0xf);
        indices.Reserve(n);
        points.Reserve(n);
        for (u32 k = 0; k < n; ++k) {
            indices.GrowAt(k) = k;
            fn_710007b518(reinterpret_cast<CVector4D*>(fn_7100149230(&points, k)), &colour);
        }

        fn_71001be430(self->model->field_0x130, 0, false);
        fn_71001be430(self->model->field_0x130, 1, false);
        self->toggle = self->toggle == 0;
        fn_7100148148(self->model, self->toggle, UnkSceneSpan{self->shapes, self->shapeCount},
                      UnkSceneSpan{points.data, points.count});
        fn_7100148178(self->model, self->toggle, UnkSceneSpan{indices.data, indices.count}, true);

        UnkPlanktonInstance_l80d4a0* instance = Instance(self->model);
        CVector4D position = {self->owner->x, self->owner->y, self->owner->z, 1.0f};
        fn_710007b518(&instance->position, &position);
        if (instance->positionBlock)
            *instance->positionBlock->params = position;

        instance = Instance(self->model);
        CVector4D intensity = {self->intensity, 0.0f, 0.0f, 1.0f};
        fn_710007b518(&instance->intensity, &intensity);
        if (instance->intensityBlock)
            *instance->intensityBlock->params = intensity;

        fn_71001be430(self->model->field_0x130, self->toggle, true);
        fn_7100149490(self->model, true);
        points.Free();
        indices.Free();
    } else if (self->model) {
        fn_71001be430(self->model->field_0x130, 0, false);
        fn_71001be430(self->model->field_0x130, 1, false);
        fn_7100149490(self->model, false);
        self->stripActive = false;
        self->field_0x128a = false;
        self->shapeCount = 0;
    } else {
        self->stripActive = false;
    }
}
