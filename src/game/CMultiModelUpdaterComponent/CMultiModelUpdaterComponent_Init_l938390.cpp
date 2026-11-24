// CMultiModelUpdaterComponent's unit (tools/tu_map.py unit 1426): after the base initialisation, when
// the character class (+0x10) is a CCharClassMultiModelUpdaterComponent, adds each model of its
// name -> path dictionary (+0x80) through fn_710093852c. Names and layouts are placeholders.
#include "base/UnkCollectionIter.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntSmallDictionary.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CMultiModelUpdaterComponent.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;
using base::reflection::UnkReflObject;

using UnkModelPaths_l938390 = base::global::CRntSmallDictionary<CStrId, CFilePathStrId>;

// An iterator over a small dictionary: it walks the keys and the values together and hands out the
// current pair through an iterator value that refers to them.
struct UnkModelPathIter_l938390 {
    base::UnkIterValue<CFilePathStrId, CStrId> current;

    UnkModelPathIter_l938390(CStrId* key, CFilePathStrId* value) { current.SetRef(key, value); }

    void operator++() {
        ++current.ref.index;
        ++current.ref.element;
    }
    bool operator!=(const CStrId* end) const { return current.ref.index != end; }
};

struct UnkCharClassMultiModel_l938390 : UnkReflObject {
    u8 pad_0x8[0x78];
    UnkModelPaths_l938390 models;  // 0x80
};
static_assert(__builtin_offsetof(UnkCharClassMultiModel_l938390, models) == 0x80, "layout");

struct UnkMultiModelUpdater_l938390 {
    u8 pad_0x0[0x10];
    UnkReflObject* char_class;  // 0x10
};

bool fn_71011715d0(UnkMultiModelUpdater_l938390* self);
// As src/game/CMultiModelUpdaterComponent/CMultiModelUpdaterComponent_AddModel_l93852c.cpp defines it.
struct UnkMultiModel_l93852c;

bool fn_7100938390(UnkMultiModelUpdater_l938390* self) {
    if (!fn_71011715d0(self))
        return false;
    UnkReflObject* object = self->char_class;
    if (object && fn_710009d50c(object->vf_0x0(), fn_71007934a4())) {
        UnkModelPaths_l938390& models = static_cast<UnkCharClassMultiModel_l938390*>(object)->models;
        for (UnkModelPathIter_l938390 it(models.keys, models.values); it != &models.keys[models.count]; ++it)
            CMultiModelUpdaterComponent_AddModel(reinterpret_cast<UnkMultiModel_l93852c*>(self), &it.current.Index(),
                          &it.current.Element());
    }
    return true;
}
