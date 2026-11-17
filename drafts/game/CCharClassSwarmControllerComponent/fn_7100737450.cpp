// CCharClassSwarmControllerComponent's unit: sets three path resources from three path texts. An
// empty first or third text takes the second resource instead. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkPathIdsFromText.h"
#include "base/UnkStrIdPathPair.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/UnkTU25.h"
#include "game/UnkTU32.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkStringData;

struct UnkSwarmPaths_l737450 {
    UnkStrIdPathResource field_0x0;
    UnkStrIdPathResource field_0x18;
    UnkStrIdPathResource field_0x30;
};

inline bool IsEmpty(const CRntString* text) {
    return !text->data || !text->data[0];
}

inline void SetPath(UnkStrIdPathResource* resource, const CRntString* text) {
    fn_71000267f8(resource, UnkPathIdsFromText(*text).ids);
}

void fn_7100737450(UnkSwarmPaths_l737450* self, const CRntString* first, const CRntString* second,
                   const CRntString* third) {
    if (!IsEmpty(second))
        SetPath(&self->field_0x18, second);
    if (!IsEmpty(first))
        SetPath(&self->field_0x0, first);
    else
        fn_7100027118(&self->field_0x0, &self->field_0x18);
    if (IsEmpty(third))
        fn_7100027118(&self->field_0x30, &self->field_0x18);
    else
        SetPath(&self->field_0x30, third);
}
