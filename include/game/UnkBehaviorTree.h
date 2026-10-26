#pragma once

// Behaviour-tree objects as the hand-written sources see them. tools/tu_map.py puts fn_71003944e8
// in the unit it names after behaviortree::CBaseCondition (the base constructor of a tree node:
// it sets the node's vtable and its first fields) and fn_71003695b8 in the unit of
// CBehaviorTreeAIComponent (it runs the component's update steps, the last through its vtable).
// The names and the layouts are placeholders.

#include "types.h"

// A behaviour-tree node: the vtable and the fields the base constructor sets (up to +0x38); the
// concrete nodes derive from it and add their own members from +0x38.
struct UnkBehaviorTreeNode {
    void** vtable;
    u8 unk_0x8[0x30];
};
static_assert(sizeof(UnkBehaviorTreeNode) == 0x38, "UnkBehaviorTreeNode placeholder layout");

// The behaviour-tree AI component: the owner link at +0x30 is the only member the callers share
// (each types it for itself); the components derive from it and add their own members from +0x38.
struct UnkBehaviorTreeAI {
    u8 unk_0x0[0x30];
    void* field_0x30;
};

// Runs the component's update steps (the unit of CBehaviorTreeAIComponent): fn_71010fe740, then
// fn_7100417068 on the word at +0x146b8 and the address +0x1260, then slot 0x718 of its vtable. Called
// first by the reset functions of the components (fn_71003ad278, fn_7100403a18).
void fn_71003695b8(UnkBehaviorTreeAI* self);

// The base constructor of a tree node (the unit behaviortree::CBaseCondition): sets the vtable and
// the fields at +0x28, +0x2c and +0x30. Called first by the constructors of the concrete nodes
// (fn_7100398128, fn_71003a3e64, fn_710039d420).
void fn_71003944e8(UnkBehaviorTreeNode* self);
