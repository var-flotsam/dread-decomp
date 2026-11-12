// CSubAreaManager::CTunableSubAreaManager unit (tools/tu_map.py unit 2577): fn_7100ee4704: handles a sub-area request that names an entry of the current sub-area: with the set of changes it makes non-empty the manager switches to it (fn_7100ee6884); otherwise the objects of that name are told the ids (fn_7100eee61c). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/CChangeStageNavMeshItemComponent.h"
#include "game/UnkTU2577.h"
#include "game/UnkTU2611.h"
#include "types.h"

using base::global::CStrId;

struct CSubAreaManager;
struct UnkPoppedRequest;
struct UnkSubareaNamedList;
struct UnkSubareaNamedEntry;
struct UnkSubareaIdLists;
struct UnkSubArea;

// The six id lists a sub-area change fills; only the counts are read here.
struct UnkIdList {
    void* data;
    u32 count;
    u8 pad_0xc[0x1c];
};
struct UnkIdSet {
    UnkIdList field_0x0;
    UnkIdList field_0x28;
    UnkIdList field_0x50;
    UnkIdList field_0x78;
    UnkIdList field_0xa0;
    UnkIdList field_0xc8;

    bool IsEmpty() const {
        return field_0x28.count == 0 && field_0x0.count == 0 && field_0x50.count == 0 &&
               field_0x78.count == 0 && field_0xc8.count == 0 && field_0xa0.count == 0;
    }
};

struct UnkSubAreaRecord_0x188;
void fn_7100faa3d8(UnkSubAreaRecord_0x188* self);
struct UnkSubAreaRecord_0x188 {
    u8 data[0x188];
    ~UnkSubAreaRecord_0x188() { fn_7100faa3d8(this); }
};

struct UnkItem_b190 {
    u8 pad_0x0[0x10];
    CStrId id;  // 0x10
};
struct UnkNamedObject_b190 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    CStrId name;  // 0x8
    UnkItem_b190** items;  // 0x10
    u32 count;  // 0x18
};
struct UnkNamedObjectPtr_b190 {
    UnkNamedObject_b190* ptr;
};

struct UnkSubAreaMgr_b190 {
    u8 pad_0x0[0x110];
    u32 field_0x110;
    u8 pad_0x114[0x44];
    UnkIdSet field_0x158;
    u8 pad_0x248[0x1a8];
    UnkSubArea* field_0x3f0;
    UnkSubArea* field_0x3f8;
    UnkSubareaNamedEntry* field_0x400;
    bool field_0x408;
    bool field_0x409;
    u8 pad_0x40a[0x426];
    base::UnkVector<UnkNamedObjectPtr_b190> field_0x830;
};
static_assert(__builtin_offsetof(UnkSubAreaMgr_b190, field_0x3f0) == 0x3f0, "layout");
static_assert(__builtin_offsetof(UnkSubAreaMgr_b190, field_0x830) == 0x830, "layout");

struct UnkSubAreaEntity_b190 {
    u8 pad_0x0[0x10];
    void* field_0x10;
};
struct UnkSubAreaEntityList_b190 {
    UnkSubAreaEntity_b190** data;
    u32 count;
};

bool fn_7100fad994(UnkSubareaIdLists* self, const CStrId* id);
void fn_7100faa9ec(UnkSubAreaRecord_0x188* self, UnkSubArea* area);
void fn_7100fad0bc(UnkSubAreaRecord_0x188* self, const CStrId* id, bool flag, bool other);
void fn_7100ee6884(CSubAreaManager* self);
UnkNamedObjectPtr_b190* fn_7100eee97c(base::UnkVector<UnkNamedObjectPtr_b190>* v, u32 index);

void fn_7100ee4704(CSubAreaManager* self_, const UnkPoppedRequest* request_, bool a, bool b) {
    UnkSubAreaMgr_b190* self = reinterpret_cast<UnkSubAreaMgr_b190*>(self_);
    const CStrId* request = reinterpret_cast<const CStrId*>(request_);
    if (self->field_0x3f0) {
        UnkSubareaNamedEntry* entry = fn_7100fab838(reinterpret_cast<UnkSubareaNamedList*>(self->field_0x3f0), request);
        if (entry && self->field_0x110 == 0 &&
            fn_7100fad994(reinterpret_cast<UnkSubareaIdLists*>(self->field_0x3f0), request)) {
            UnkSubAreaRecord_0x188 record;
            fn_7100faa9ec(&record, self->field_0x3f0);
            fn_7100fad0bc(&record, request, a, false);
            fn_7100ee8044(self_, entry, record.data + 0x38, &self->field_0x158);
            if (!self->field_0x158.IsEmpty()) {
                self->field_0x3f8 = self->field_0x3f0;
                self->field_0x400 = entry;
                self->field_0x408 = a;
                self->field_0x409 = b;
                fn_7100ee6884(self_);
                return;
            }
        }
    }
    for (u32 i = 0; i < self->field_0x830.count; ++i) {
        UnkNamedObject_b190* object = fn_7100eee97c(&self->field_0x830, i)->ptr;
        if (object->name.data == request->data) {
            for (u64 j = 0; j < object->count; ++j)
                fn_7100eee61c(self, &object->items[j]->id, const_cast<CStrId*>(request), a, b);
            break;
        }
    }
    UnkSubAreaEntityList_b190* list = fn_71009ba31c();
    for (UnkSubAreaEntity_b190** it = list->data, **end = it + list->count; it != end; ++it)
        fn_71009b9ee8(*it ? (*it)->field_0x10 : nullptr);
}
