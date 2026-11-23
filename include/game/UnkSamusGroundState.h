#pragma once

// Samus's ground movement state, as the sources that take it see it: CSamusGroundMovementState::CParams' unit
// (tools/tu_map.py unit 1462) sends ground requests on it (fn_710099db00), and the floor-slide state's unit
// (unit 1459) passes its own state to that function, so both read the same object through this one layout.
// Only the members those sources touch are spelled out. Names and layout are placeholders.
#include "base/UnkMatrix44.h"
#include "types.h"

struct UnkAnimationComponent;

// The state machine the state belongs to.
struct UnkSamusGroundMachine {
    u8 pad_0x0[0xa4];
    u32 pending;  // 0xa4; 15: no state change pending
};

// The owning actor.
struct UnkSamusGroundOwner {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
    u8 pad_0x78[0x220 - 0x78];
    UnkAnimationComponent* animation;  // 0x220
};
static_assert(__builtin_offsetof(UnkSamusGroundOwner, animation) == 0x220, "layout");

// Samus's movement, as the ground state reaches it.
struct UnkSamusGroundMovement {
    u8 pad_0x0[0x7c8];
    f32 field_0x7c8;
    u8 pad_0x7cc[0x7e4 - 0x7cc];
    bool field_0x7e4;
};

// The state's parameters.
struct UnkSamusGroundParams {
    u8 pad_0x0[0xd0];
    f32 field_0xd0;  // facing speed while turning
    f32 field_0xd4;  // facing speed with the movement's +0x7e4 flag
};

struct UnkSamusGroundState {
    virtual void vf_0();
    virtual void vf_1();
    virtual void vf_2();
    virtual void vf_3();
    virtual void vf_4();
    virtual void vf_5();
    virtual void vf_6();
    virtual void vf_7();
    virtual void vf_8();
    virtual void vf_9();
    virtual void vf_10();
    virtual void vf_11();
    virtual void vf_12();
    virtual void vf_13();
    virtual void vf_14();
    virtual void vf_15();
    virtual void vf_16();
    virtual void vf_17();
    virtual void vf_18();
    virtual void vf_19();
    virtual void vf_20();
    virtual void vf_21();
    virtual void vf_22();
    virtual void vf_23();
    virtual void vf_24();
    virtual void vf_25();
    virtual void vf_26();
    virtual void vf_27();
    virtual void vf_28();
    virtual void vf_29();
    virtual void vf_30();
    virtual void vf_31();
    virtual void vf_32();
    virtual void vf_33();
    virtual void vf_34();
    virtual void vf_35();
    virtual void vf_36();
    virtual void vf_37();
    virtual bool vf_38(bool a, bool b, void* c);

    UnkSamusGroundMachine* machine;  // 0x8
    u8 pad_0x10[0x30 - 0x10];
    UnkSamusGroundOwner* owner;        // 0x30
    UnkSamusGroundMovement* movement;  // 0x38
    UnkSamusGroundParams* params;      // 0x40
    u8 pad_0x48[0x198 - 0x48];
    s32 state;  // 0x198
};
static_assert(__builtin_offsetof(UnkSamusGroundState, owner) == 0x30, "layout");
static_assert(__builtin_offsetof(UnkSamusGroundState, params) == 0x40, "layout");
static_assert(__builtin_offsetof(UnkSamusGroundState, state) == 0x198, "layout");
