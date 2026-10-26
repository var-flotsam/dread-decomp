#pragma once

// Behaviour-tree objects as the hand-written sources see them. tools/tu_map.py puts fn_71003944e8
// in the unit it names after behaviortree::CBaseCondition (the base constructor of a tree node:
// it sets the node's vtable and its first fields) and fn_71003695b8 in the unit of
// CBehaviorTreeAIComponent (it runs the component's update steps, the last through its vtable).
// The names and the layouts are placeholders.

#include "types.h"

struct UnkBehaviorTreeNode;
struct UnkBehaviorTreeAI;
void fn_71003695b8(UnkBehaviorTreeAI* self);
void fn_71003944e8(UnkBehaviorTreeNode* self);
