// From unit 54 of tools/tu_map.py (the transform math of src/game/UnkXformAngles.cpp): the rotation
// angles of the matrix fn_71000769f0 builds from three vectors and the constant g_unk_7101647790,
// through fn_7100076acc, which reads only the matrix part of a transform. Names and parameter roles are placeholders.
#include "game/UnkXformNode.h"

extern char g_unk_7101647790[];
void fn_71000769f0(UnkMatrix44* out, const void* a, const void* b, const void* c, const void* d);
UnkVec3f fn_7100076acc(const UnkXform* x);  // as src/game/UnkCollisionContacts.cpp declares it

UnkVec3f fn_710007a8d0(const void* a, const void* b, const void* c) {
    UnkMatrix44 m;
    fn_71000769f0(&m, a, b, c, g_unk_7101647790);
    return fn_7100076acc(reinterpret_cast<const UnkXform*>(&m));
}
