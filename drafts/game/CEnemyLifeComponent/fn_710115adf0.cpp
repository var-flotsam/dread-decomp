// CEnemyLifeComponent's unit (tools/tu_map.py unit 2714): rebuilds the enemy's life bar, a scene object
// with a black backing quad and a coloured fill quad whose width and colour follow the life left.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/math/CVector4D.h"
#include "game/UnkActor.h"
#include "game/UnkRenderManager.h"
#include "game/UnkSceneObject.h"
#include "game/UnkTU140.h"
#include "game/UnkTU252.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::math::CVector4D;
using base::math::UnkVector4D;

// Restarts a scene object with a mode, a count and three lists (not defined by hand).
void fn_7100148128(UnkSceneObject* self, u32 mode, u32 count, const UnkSceneSpan& positions,
                   const UnkSceneSpan& colors, const UnkSceneSpan& indices);
void fn_71001494e4(UnkSceneObject* self, bool flag);

struct UnkPoint_l15adf0 {
    f32 x;
    f32 y;
    f32 z;
};

// Up to four points with their count.
struct UnkQuad_l15adf0 {
    u32 count;
    UnkVec3f points[4];

    UnkQuad_l15adf0() : count(0) {}
    void Add(const UnkVec3f& p) { points[count++] = p; }
    UnkSceneSpan Span() { return UnkSceneSpan{points, count}; }
};

struct UnkAIComponent_l15adf0 {
    virtual void vf_0x0();
};

struct UnkLifeBarSettings_l15adf0 {
    u8 pad_0x0[0x139];
    bool show_bar;            // 0x139
    u8 pad_0x13a[0x2];
    UnkPoint_l15adf0 offset;  // 0x13c
};

struct UnkEnemyLife_l15adf0 {
    u8 pad_0x0[0x30];
    UnkActor* actor;          // 0x30
    u8 pad_0x38[0x18];
    f32 max_life;             // 0x50
    f32 life;                 // 0x54
    u8 pad_0x58[0x208];
    UnkLifeBarSettings_l15adf0* settings;  // 0x260
    u8 pad_0x268[0x8];
    UnkSceneObject* bar;      // 0x270

    f32 LifeFraction() const { return max_life > 1.1920929e-07f ? life / max_life : 0.0f; }
};
static_assert(__builtin_offsetof(UnkEnemyLife_l15adf0, bar) == 0x270, "UnkEnemyLife_l15adf0 layout");

static inline UnkPoint_l15adf0 Position(const UnkActor* actor) {
    return UnkPoint_l15adf0{actor->field_0x44, actor->field_0x54, actor->field_0x64};
}

static inline UnkVec3f operator+(const UnkPoint_l15adf0& a, const UnkPoint_l15adf0& b) {
    return UnkVec3f(a.x + b.x, a.y + b.y, a.z + b.z);
}

// Creates a scene object in the render pool and puts it on `layer`.
static inline UnkSceneObject* CreateOnLayer(u32 layer) {
    UnkRenderManager* manager = g_unk_7101d47300;
    UnkSceneObject* created = nullptr;
    fn_7100149930(manager->field_0x218, nullptr, &created, &g_unk_7101d470e0);
    fn_7100219ae0(manager, created ? reinterpret_cast<UnkSceneNode*>(reinterpret_cast<u8*>(created) + 8) : nullptr, layer);
    return created;
}

typedef bool (*AIQuery_l15adf0)(UnkAIComponent_l15adf0*);

void fn_710115adf0(UnkEnemyLife_l15adf0* self) {
    if (self->bar) {
        fn_71002199d8(g_unk_7101d47300, reinterpret_cast<UnkItemBase_71011a91c4*>(reinterpret_cast<u8*>(self->bar) + 8), 0);
        self->bar = nullptr;
    }
    if (!self->settings->show_bar)
        return;
    UnkAIComponent_l15adf0* ai = static_cast<UnkAIComponent_l15adf0*>(self->actor->field_0x240);
    if (ai && !reinterpret_cast<AIQuery_l15adf0>((*reinterpret_cast<void***>(ai))[0x5c8 / 8])(ai))
        return;

    UnkSceneObject* bar = CreateOnLayer(0);
    self->bar = bar;

    UnkQuad_l15adf0 back;
    back.Add(UnkVec3f(-50.0f, 6.0f, 0.0f));
    back.Add(UnkVec3f(50.0f, 6.0f, 0.0f));
    back.Add(UnkVec3f(-50.0f, -6.0f, 0.0f));
    back.Add(UnkVec3f(50.0f, -6.0f, 0.0f));
    UnkVector4D black(0.0f, 0.0f, 0.0f, 1.0f);
    fn_7100148128(bar, 6, 2, back.Span(), UnkSceneSpan{&black, 1}, UnkSceneSpan{nullptr, 0});

    UnkVector4D color(0.0f, 1.0f, 0.0f, 1.0f);
    f32 fraction = self->LifeFraction();
    if (fraction <= 0.33f) {
        UnkVector4D red(1.0f, 0.0f, 0.0f, 1.0f);
        fn_710007b518(&color, &red);
    } else if (fraction <= 0.66f) {
        UnkVector4D orange(1.0f, 0.5f, 0.0f, 1.0f);
        fn_710007b518(&color, &orange);
    }

    UnkQuad_l15adf0 fill;
    fill.Add(UnkVec3f(-48.5f, 4.5f, 0.0f));
    fill.Add(UnkVec3f(self->LifeFraction() * 97.0f + -48.5f, 4.5f, 0.0f));
    fill.Add(UnkVec3f(-48.5f, -4.5f, 0.0f));
    fill.Add(UnkVec3f(self->LifeFraction() * 97.0f + -48.5f, -4.5f, 0.0f));
    fn_7100148128(self->bar, 6, 2, fill.Span(), UnkSceneSpan{&color, 1}, UnkSceneSpan{nullptr, 0});

    UnkVec3f pos = Position(self->actor) + self->settings->offset;
    fn_7100149460(self->bar, &pos);
    fn_71001beee4(self->bar->field_0x130, true);
    fn_71001494e4(self->bar, false);
}
