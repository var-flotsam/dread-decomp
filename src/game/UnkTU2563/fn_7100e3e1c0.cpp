// Unit 2563 (CMinimapData hint): recomputes the 2D bounds of a mesh's vertices (game/UnkDashMesh.h).
// Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/UnkDashMesh.h"
#include "game/UnkTU2563.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

void fn_7100e3e1c0(UnkDashMesh* mesh) {
    mesh->min_x = 3.4028235e+38f;
    mesh->min_y = 3.4028235e+38f;
    mesh->max_x = -3.4028235e+38f;
    mesh->max_y = -3.4028235e+38f;
    for (UnkMeshVertex *v = mesh->vertices.data, *end = v + mesh->vertices.count; v != end; ++v) {
        CVector2D p;
        fn_710007b430(&p, reinterpret_cast<const CVector3D*>(v));
        f32 lx = mesh->min_x < p.x ? mesh->min_x : p.x;
        f32 ly = mesh->min_y < p.y ? mesh->min_y : p.y;
        mesh->min_x = lx;
        mesh->min_y = ly;
        f32 hx = mesh->max_x > p.x ? mesh->max_x : p.x;
        f32 hy = mesh->max_y > p.y ? mesh->max_y : p.y;
        mesh->max_x = hx;
        mesh->max_y = hy;
    }
}
