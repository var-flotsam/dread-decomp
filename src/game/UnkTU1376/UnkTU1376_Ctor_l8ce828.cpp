// CDoorLifeComponent's unit (tools/tu_map.py unit 1376; the name is a hint): the door's constructor. After the
// life component base constructor it builds two callback lists, the state machine and its three states, the
// door's colours (copied from two global sets), bounds, the vector groups (fn_71008cec30), three bound
// callbacks, two timers and a list. Names and layouts are placeholders.
#include <new>

#include "base/UnkErasedVector.h"
#include "base/color/CColor4F.h"
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/CItemLifeComponent.h"
#include "game/UnkStruct_71727313a4.h"
#include "game/UnkStruct_71727313d4.h"
#include "game/UnkTU1376.h"
#include "game/UnkTU341.h"
#include "game/UnkTU39.h"
#include "types.h"

using base::color::CColor4F;
using base::global::CRntVector;
using base::math::CVector2D;
using base::math::CVector3D;

extern const CVector2D g_unk_7101647778;

extern void* g_unk_7101a7e840[];
extern void* g_unk_7101a7ebb8[];
extern void* g_unk_7101a7eb08[];

struct UnkDoorVecSet_b0251;
void fn_71008cec30(UnkDoorVecSet_b0251* self);
void fn_71008dbbdc(void* list);
struct UnkDoorShieldLinks;
bool fn_71008dcf50(UnkDoorShieldLinks* self);
bool fn_71008dcf38(void* self);
bool fn_71008dd010(void* self);

// A callback list: an empty array of callables under the base vtable, then the derived one.
struct UnkDoorCallbacksBase_l8ce828 {
    void** vtable;
    UnkOwnedErasedVector callables;
    u32 field_0x30;

    UnkDoorCallbacksBase_l8ce828() : vtable(&g_unk_7101a20998[2]), field_0x30(0) {}
};

struct UnkDoorCallbacks_l8ce828 : UnkDoorCallbacksBase_l8ce828 {
    void* owner;

    UnkDoorCallbacks_l8ce828() {
        vtable = &g_unk_7101a33fa0[2];
        owner = nullptr;
    }
};
static_assert(sizeof(UnkDoorCallbacks_l8ce828) == 0x40, "UnkDoorCallbacks_l8ce828 layout");

// The door's state machine: no state yet, none pending.
struct UnkDoorStates_l8ce828 {
    void** vtable;
    u64 field_0x8;
    void* current;
    void* previous;
    u32 count;
    u32 states[0x20];
    s32 pending;
    bool field_0xa8;
    bool field_0xa9;

    UnkDoorStates_l8ce828()
        : vtable(&g_unk_7101a7ebb8[2]), field_0x8(0), current(nullptr), previous(nullptr), count(0),
          pending(0x7fffffff), field_0xa8(false), field_0xa9(false) {}
};
static_assert(sizeof(UnkDoorStates_l8ce828) == 0xb0, "UnkDoorStates_l8ce828 layout");

struct UnkDoorState_l8ce828 {
    void** vtable;
    u64 field_0x8;
    s32 id;
    u64 field_0x18;
    u64 field_0x20;
    u32 field_0x28;
    u32 field_0x2c;
    u64 field_0x30[8];

    explicit UnkDoorState_l8ce828(s32 id_)
        : field_0x8(0), id(id_), field_0x18(0), field_0x20(0), field_0x28(2), field_0x2c(1) {
        vtable = &g_unk_7101a7eb08[2];
        for (u64& v : field_0x30)
            v = 0;
    }
};
static_assert(sizeof(UnkDoorState_l8ce828) == 0x70, "UnkDoorState_l8ce828 layout");

// An empty 2D box: the minimum at the largest value, the maximum at the smallest.
struct UnkDoorBox_l8ce828 {
    CVector2D min;
    CVector2D max;

    UnkDoorBox_l8ce828() {
        min.x = 3.4028235e38f;
        min.y = 3.4028235e38f;
        max.x = -3.4028235e38f;
        max.y = -3.4028235e38f;
    }
};

// A function bound to an object.
struct UnkDoorBound_l8ce828 {
    u64 field_0x0;
    void* object;
    void* function;

    UnkDoorBound_l8ce828(void* object_, void* function_) : field_0x0(0), object(object_), function(function_) {}
};

struct UnkDoorLife_l8ce828 {
    void** vtable;
    u8 pad_0x8[0x380];
    UnkDoorCallbacks_l8ce828 field_0x388;
    UnkDoorCallbacks_l8ce828 field_0x3c8;
    u8 pad_0x408[0x8];
    f32 field_0x410;
    f32 field_0x414;
    u32 field_0x418;
    bool field_0x41c;
    u8 pad_0x41d[0x2];
    bool field_0x41f;
    UnkDoorStates_l8ce828 states;  // 0x420
    UnkDoorState_l8ce828 state1;   // 0x4d0
    UnkDoorState_l8ce828 state2;   // 0x540
    UnkDoorState_l8ce828 state3;   // 0x5b0
    f32 field_0x620;
    f32 field_0x624;
    bool field_0x628;
    u32 field_0x62c;
    u32 field_0x630;
    bool field_0x634;
    f32 field_0x638;
    u32 field_0x63c[8];
    u8 pad_0x65c[0x4];
    u8 field_0x660[0x38];
    CVector3D position;  // 0x698
    u8 field_0x6a4[0xb];
    u64 field_0x6b0;
    CColor4F field_0x6b8[3];
    CColor4F field_0x6e8[3];
    CColor4F field_0x718;
    CRntVector<void*> field_0x728;
    u32 field_0x750;
    u8 pad_0x754[0x40];
    UnkDoorBox_l8ce828 field_0x794;
    UnkDoorBox_l8ce828 field_0x7a4;
    CVector2D field_0x7b4;
    CVector2D field_0x7bc;
    bool field_0x7c4;
    u8 pad_0x7c5[0x3];
    u8 field_0x7c8[0x200];
    UnkDoorBound_l8ce828 field_0x9c8;
    UnkDoorBound_l8ce828 field_0x9e0;
    void* field_0x9f8;
    u64 field_0xa00;
    u64 field_0xa08;
    s32 field_0xa10;
    u32 field_0xa14;
    u8 field_0xa18[0xb0];
    u64 field_0xac8;
    s64 field_0xad0;
    u64 field_0xad8;
    s64 field_0xae0;
    u64 field_0xae8;
    bool field_0xaf0;
    u8 pad_0xaf1[0x7];
    u8 field_0xaf8[0xb0];
    UnkDoorBound_l8ce828 field_0xba8;
    UnkDoorBound_l8ce828 field_0xbc0;
    CRntVector<void*> field_0xbd8;
};
static_assert(__builtin_offsetof(UnkDoorLife_l8ce828, states) == 0x420, "layout");
static_assert(__builtin_offsetof(UnkDoorLife_l8ce828, field_0x620) == 0x620, "layout");
static_assert(__builtin_offsetof(UnkDoorLife_l8ce828, position) == 0x698, "layout");
static_assert(__builtin_offsetof(UnkDoorLife_l8ce828, field_0x728) == 0x728, "layout");
static_assert(__builtin_offsetof(UnkDoorLife_l8ce828, field_0x7c8) == 0x7c8, "layout");
static_assert(__builtin_offsetof(UnkDoorLife_l8ce828, field_0xa18) == 0xa18, "layout");
static_assert(__builtin_offsetof(UnkDoorLife_l8ce828, field_0xbd8) == 0xbd8, "layout");

void fn_71008ce828(UnkDoorLife_l8ce828* self) {
    fn_71008e04e8(reinterpret_cast<UnkLifeComponent_A5_b71*>(self));
    self->vtable = &g_unk_7101a7e840[2];
    new (&self->field_0x388) UnkDoorCallbacks_l8ce828;
    new (&self->field_0x3c8) UnkDoorCallbacks_l8ce828;
    self->field_0x410 = 3.0f;
    self->field_0x414 = 2000.0f;
    self->field_0x418 = 0;
    self->field_0x41c = true;
    self->field_0x41f = false;
    new (&self->states) UnkDoorStates_l8ce828;
    new (&self->state1) UnkDoorState_l8ce828(1);
    new (&self->state2) UnkDoorState_l8ce828(2);
    new (&self->state3) UnkDoorState_l8ce828(3);
    self->field_0x620 = 0.0f;
    self->field_0x624 = -1.0f;
    self->field_0x628 = false;
    self->field_0x62c = 0;
    self->field_0x630 = 0;
    self->field_0x634 = false;
    self->field_0x638 = 0.65f;
    for (u32& v : self->field_0x63c)
        v = 0;
    fn_71008dbbdc(self->field_0x660);
    new (&self->position) CVector3D();
    new (&self->field_0x6a4) u8[0xb]();
    self->field_0x6b0 = 0;
    // The twelve floats are three colours.
    const CColor4F* defaults = reinterpret_cast<const CColor4F*>(&g_unk_71727313d4);
    for (u32 i = 0; i < 3; ++i)
        new (&self->field_0x6b8[i]) CColor4F(defaults[i]);
    for (u32 i = 0; i < 3; ++i)
        new (&self->field_0x6e8[i]) CColor4F(g_unk_71727313a4.colours[i]);
    new (&self->field_0x718) CColor4F(1.0f, 1.0f, 1.0f, 1.0f);
    new (&self->field_0x728) CRntVector<void*>(0xf);
    self->field_0x750 = 0;
    new (&self->field_0x794) UnkDoorBox_l8ce828;
    new (&self->field_0x7a4) UnkDoorBox_l8ce828;
    self->field_0x7b4.x = g_unk_7101647778.x;
    self->field_0x7b4.y = g_unk_7101647778.y;
    self->field_0x7bc.x = g_unk_7101647778.x;
    self->field_0x7bc.y = g_unk_7101647778.y;
    self->field_0x7c4 = false;
    fn_71008cec30(reinterpret_cast<UnkDoorVecSet_b0251*>(self->field_0x7c8));
    new (&self->field_0x9c8) UnkDoorBound_l8ce828(self, reinterpret_cast<void*>(&fn_71008dcf38));
    new (&self->field_0x9e0) UnkDoorBound_l8ce828(self, reinterpret_cast<void*>(&fn_71008dcf50));
    self->field_0x9f8 = nullptr;
    self->field_0xa00 = 0;
    self->field_0xa08 = 0;
    self->field_0xa10 = -1;
    self->field_0xa14 = 0;
    fn_710002f4e4(self->field_0xa18);
    self->field_0xac8 = 0;
    self->field_0xad0 = -1;
    self->field_0xad8 = 0;
    self->field_0xae0 = -1;
    self->field_0xae8 = 0;
    self->field_0xaf0 = false;
    fn_710002f4e4(self->field_0xaf8);
    new (&self->field_0xba8) UnkDoorBound_l8ce828(self, reinterpret_cast<void*>(&fn_71008dd010));
    new (&self->field_0xbc0) UnkDoorBound_l8ce828(self, reinterpret_cast<void*>(&fn_71008dd010));
    new (&self->field_0xbd8) CRntVector<void*>(5);
}
