#pragma once

// The GUI's frames: an array of 0x48-byte records at +0x40 of the object g_unk_717271e8f8 points to,
// each with its key at +0x8 and scale and offset words from +0x24. The helpers next to the GUI
// manager (src/game/GUI/CGUIManager/fn_71011b7f48.cpp) find a frame by key (it must exist: the
// search does not stop) and scale a 2D value by it. Names and layouts are placeholders.

#include "types.h"

// A 2D value the scale helpers hand back in memory (copied member by member).
struct UnkLabelVec2 {
    f32 x;
    f32 y;
    UnkLabelVec2(f32 x_, f32 y_) : x(x_), y(y_) {}
    UnkLabelVec2(const UnkLabelVec2& other) : x(other.x), y(other.y) {}
};

// The value scaled by the frame `key` names, in five ways (y only by +0x38 or +0x34; both by +0x28
// and +0x30; and both offset by a half and scaled twice, fn_71011b810c and fn_71011b8160 alike).
UnkLabelVec2 fn_71011b7f48(s32 key, const UnkLabelVec2& v);
UnkLabelVec2 fn_71011b7f88(s32 key, const UnkLabelVec2& v);
UnkLabelVec2 fn_71011b8078(s32 key, const UnkLabelVec2& v);
UnkLabelVec2 fn_71011b810c(s32 key, const UnkLabelVec2& v);
UnkLabelVec2 fn_71011b8160(s32 key, const UnkLabelVec2& v);

// The same over a given root (game/UnkGuiRoot.h): the frame `key` names or null (fn_71011b7a4c), and a
// size scaled by one of two root-wide scales and the frame's (fn_71011b7e7c, fn_71011b7fc8).
struct UnkGuiRoot;
struct UnkGuiFrame;
namespace base::math {
struct CVector2D;
}
UnkGuiFrame* fn_71011b7a4c(UnkGuiRoot* root, s32 key);
UnkLabelVec2 fn_71011b7e7c(UnkGuiRoot* root, s32 key, const base::math::CVector2D* v);
UnkLabelVec2 fn_71011b7fc8(UnkGuiRoot* root, s32 key, const base::math::CVector2D* v);
