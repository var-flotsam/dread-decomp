#include "types.h"

struct UnkUnit110Vec2 {
    f32 x;
    f32 y;
    UnkUnit110Vec2() {}
    UnkUnit110Vec2(f32 x_, f32 y_) : x(x_), y(y_) {}
    UnkUnit110Vec2(const UnkUnit110Vec2& o) : x(o.x), y(o.y) {}
};

struct UnkUnit110Segment {
    UnkUnit110Vec2 start;
    UnkUnit110Vec2 end;
};

UnkUnit110Vec2 fn_71001261e8(const UnkUnit110Segment* seg, const UnkUnit110Vec2* p) {
    f32 dx = seg->end.x - seg->start.x;
    f32 dy = seg->end.y - seg->start.y;
    f32 lenSq = dx * dx + dy * dy;
    f32 t = (dx * (p->x - seg->start.x) + dy * (p->y - seg->start.y)) / lenSq;
    return UnkUnit110Vec2(seg->start.x + dx * t, seg->start.y + dy * t);
}
