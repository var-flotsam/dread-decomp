// CMultiModelUpdaterComponent's unit (tools/tu_map.py unit 1426): the property handler. One key is
// accepted and ignored; another reads a model's name and path (and a third string, unused) from the
// property's stream and adds the model (fn_710093852c); other keys go to the base handler
// (fn_7101171fcc). Names and layouts are placeholders.
#include "../../game_s7/UnkPropS7.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntFile.h"
#include "base/global/CStrId.h"
#include "game/CMultiModelUpdaterComponent.h"
#include "types.h"

using namespace unk_prop;
using base::global::CFilePathStrId;
using base::global::CRntFile;
using base::global::UnkStrIdInline;

struct UnkMultiModelUpdater_l93881c;

u32 fn_7101171fcc(UnkMultiModelUpdater_l93881c* self, UnkProp* p);
// As src/game/CMultiModelUpdaterComponent/CMultiModelUpdaterComponent_AddModel_l93852c.cpp defines it.
struct UnkMultiModel_l93852c;

u32 CMultiModelUpdaterComponent_ReadProperty(UnkMultiModelUpdater_l93881c* self, UnkProp* p) {
    if (Is(p, Key(0x1668)))
        return true;
    if (!Is(p, Key(0x690)))
        return fn_7101171fcc(self, p);
    CRntFile* file = reinterpret_cast<CRntFile*>(p->stream);
    const char* name;
    CRntFile_ReadCString(file, &name);
    const char* path = nullptr;
    if (file->position < file->size)
        CRntFile_ReadCString(file, &path);
    const char* unused = nullptr;
    if (file->position < file->size)
        CRntFile_ReadCString(file, &unused);
    CStrId name_id(name, true);
    CFilePathStrId path_id(UnkStrIdInline(), path, true);
    CMultiModelUpdaterComponent_AddModel(reinterpret_cast<UnkMultiModel_l93852c*>(self), &name_id, &path_id);
    return true;
}
