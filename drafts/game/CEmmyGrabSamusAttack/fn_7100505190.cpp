// CEmmyGrabSamusAttack (unit 800): a grab attack's constructor. After CAttack's part it sets up the
// property list, the listener, the thirteen numbered property entries and the defaults, then
// creates its two effects (hidden, with two of their parts switched off). Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "base/global/UnkTwoStepStrId.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAttack.h"
#include "game/UnkRenderManager.h"
#include "game/UnkSceneObject.h"
#include "game/UnkTU140.h"
#include "game/UnkTU252.h"
#include "types.h"

extern void* g_unk_7101a477e0[];  // the class's vtable
extern void* g_unk_7101a47970[];  // the entries' base vtable
extern void* g_unk_7101a47918[];  // the entries' vtable
extern void* g_unk_7101a478d8[];  // the property list's vtable

// A property entry: its number and its reflected value.
struct UnkGrabEntryBase_505190 {
    void** vtable;
    u64 field_0x8;
    u32 index;
    base::reflection::UnkReflValue value;  // 0x18

    UnkGrabEntryBase_505190(u32 index) : vtable(g_unk_7101a47970 + 2), field_0x8(0), index(index) {
        value.kind = 2;
        value.field_0x14 = 1;
        value.field_0x0 = nullptr;
        value.type = nullptr;
    }
};

struct UnkGrabEntry_505190 : UnkGrabEntryBase_505190 {
    u64 block[8];  // 0x30

    UnkGrabEntry_505190(u32 index) : UnkGrabEntryBase_505190(index) {
        vtable = g_unk_7101a47918 + 2;
        for (int i = 0; i < 8; i++)
            block[i] = 0;
    }
};
static_assert(sizeof(UnkGrabEntry_505190) == 0x70, "layout");

struct UnkGrabList_505190 {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
    u32 count;

    UnkGrabList_505190() : vtable(g_unk_7101a478d8 + 2), field_0x8(0), field_0x10(0), field_0x18(0), count(0) {}
};

struct UnkGrabListener_505190 {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    void* owner;

    UnkGrabListener_505190() : vtable(g_unk_7101a22d48 + 2), field_0x8(0), field_0x10(0), owner(nullptr) {}
};

// CAttack's part, built out of line, then given the class's vtable.
struct UnkGrabAttackBase_505190 {
    void** vtable;
    u8 pad_0x8[0x20];

    explicit UnkGrabAttackBase_505190(void** class_vtable) {
        fn_710036069c(this);
        vtable = class_vtable + 2;
    }
};

struct UnkGrabAttack_505190 : UnkGrabAttackBase_505190 {
    UnkGrabList_505190 list;  // 0x28
    u8 pad_0x50[0x7c];
    u32 field_0xcc = 14;
    bool field_0xd0 = false;
    bool field_0xd1 = false;
    u64 field_0xd8 = 0;
    UnkGrabListener_505190 listener;  // 0xe0
    UnkGrabEntry_505190 entries[13]{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};  // 0x100
    bool field_0x6b0 = false;
    bool field_0x6b1 = false;
    bool field_0x6b2 = false;
    u32 field_0x6b4 = 0;
    f32 field_0x6b8 = -1.0f;
    f32 field_0x6bc = -1.0f;
    f32 field_0x6c0 = -1.0f;
    f32 field_0x6c4 = -1.0f;
    f32 field_0x6c8 = -1.0f;
    f32 field_0x6cc = -1.0f;
    f32 field_0x6d0 = -1.0f;
    f32 field_0x6d4 = 1.0f;
    u64 field_0x6d8 = 0;
    u64 field_0x6e0 = 0;
    u64 field_0x6e8 = 0;
    UnkSceneObject* effect_a = nullptr;  // 0x6f0
    UnkSceneObject* effect_b = nullptr;  // 0x6f8
    u64 field_0x700[13] = {};
    f32 field_0x768 = 1.0f;
    f32 field_0x76c = 0.0f;
    f32 field_0x770 = 0.0f;
    f32 field_0x774 = 0.0f;
    f32 field_0x778 = 0.0f;
    f32 field_0x77c = 0.0f;
    f32 field_0x780 = 0.0f;
    f32 field_0x784 = 0.0f;
    u8 pad_0x788[8];
    u8 field_0x790 = 0;
    u64 field_0x798 = 0;
    UnkEmptyStrId name;  // 0x7a0
    u64 field_0x7a8 = 0;
    bool field_0x7b0 = false;
    bool field_0x7b1 = false;
    bool field_0x7b2 = false;
    u32 field_0x7b4;
    u32 field_0x7b8;

    UnkGrabAttack_505190();  // fn_7100505190

    // Creates an effect in the render manager's pool and registers it, hidden, with its first
    // two parts switched off.
    void CreateEffect(UnkSceneObject*& effect) {
        UnkRenderManager* manager = g_unk_7101d47300;
        UnkSceneObject* object = nullptr;
        fn_7100149930(manager->field_0x218, nullptr, &object, &g_unk_7101d470e0);
        fn_7100219ae0(manager, object ? reinterpret_cast<UnkSceneNode*>(reinterpret_cast<char*>(object) + 8) : nullptr,
                      0);
        effect = object;
        fn_7100149490(object, false);
        fn_71001beee4(effect->field_0x130, true);
        fn_71001be430(effect->field_0x130, 0, false);
        fn_71001be430(effect->field_0x130, 1, false);
    }
};
static_assert(__builtin_offsetof(UnkGrabAttack_505190, field_0xcc) == 0xcc, "layout");
static_assert(__builtin_offsetof(UnkGrabAttack_505190, entries) == 0x100, "layout");
static_assert(__builtin_offsetof(UnkGrabAttack_505190, effect_a) == 0x6f0, "layout");
static_assert(__builtin_offsetof(UnkGrabAttack_505190, name) == 0x7a0, "layout");
static_assert(__builtin_offsetof(UnkGrabAttack_505190, field_0x7b4) == 0x7b4, "layout");

UnkGrabAttack_505190::UnkGrabAttack_505190() : UnkGrabAttackBase_505190(g_unk_7101a477e0) {
    CreateEffect(effect_b);
    CreateEffect(effect_a);
    field_0x7b4 = 0;
    field_0x7b8 = 0;
}
