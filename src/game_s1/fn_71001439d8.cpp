// Copies the translation and rotation angles of an UnkXform (include/game/UnkXformNode.h) into a
// transform node held at a fixed offset, through that node type's translation and angle setters.
#include "game/UnkXformNode.h"

void fn_710020d4b8(UnkXformNode* node, const UnkVec3f* v);  // translation
void fn_710020d5b8(UnkXformNode* node, const UnkVec3f* v);  // angles
void fn_7100206194(UnkXformNode* node, const UnkVec3f* v);  // translation
void fn_71002063e8(UnkXformNode* node, const UnkVec3f* v);  // angles
void fn_7100208d58(UnkXformNode* node, const UnkVec3f* v);  // translation
void fn_7100208e24(UnkXformNode* node, const UnkVec3f* v);  // angles
void fn_71001fb294(UnkXformNode* node, const UnkVec3f* v);  // translation
void fn_71001fb360(UnkXformNode* node, const UnkVec3f* v);  // angles

typedef void (*UnkVec3Setter)(UnkXformNode* node, const UnkVec3f* v);

template <UnkVec3Setter SetTranslation, UnkVec3Setter SetAngle>
static inline void CopyPose(char* node, UnkXform* x) {
    UnkXformNode* n = reinterpret_cast<UnkXformNode*>(node);
    {
        UnkVec3f t = x->GetTranslation();
        SetTranslation(n, &t);
    }
    {
        UnkVec3f a = x->GetAngle();
        SetAngle(n, &a);
    }
}

void fn_71001439d8(char* self, UnkXform* x) {
    CopyPose<fn_710020d4b8, fn_710020d5b8>(self + 0x20, x);
}

void fn_710014555c(char* self, UnkXform* x) {
    CopyPose<fn_7100206194, fn_71002063e8>(self + 0x20, x);
}

void fn_71001474fc(char* self, UnkXform* x) {
    CopyPose<fn_7100208d58, fn_7100208e24>(self + 0x28, x);
}

void fn_7100147694(char* self, UnkXform* x) {
    CopyPose<fn_7100208d58, fn_7100208e24>(self + 0x8, x);
}

void fn_7100149690(char* self, UnkXform* x) {
    CopyPose<fn_71001fb294, fn_71001fb360>(self + 0x20, x);
}
