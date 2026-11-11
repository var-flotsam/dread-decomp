// From unit 53 of tools/tu_map.py (see src/base/UnkCurveModifiers.cpp): a timed curve's value at the
// fraction `u` of its run, shaped by its modifier letters (fn_7100076020) when it has any.
#include "base/UnkTimedCurve.h"
#include "game/UnkTU53.h"
#include "types.h"

f32 fn_7100076450(UnkTimedCurve* curve, f32 u) {
    if (curve->modifiers && *curve->modifiers) {
        f32 v = fn_7100076020(u, curve->modifiers);
        return curve->start + v * curve->delta;
    }
    return curve->start + u * curve->delta;
}
