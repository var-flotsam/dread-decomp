// game::logic::collision::CPolygonCollectionShape (unit 2515 of tools/tu_map.py, the name from
// reflection): reads the shape from a file. Two readers of the shape's other parts run first
// (fn_7100d6c288, fn_7100d62ec0), then a flag byte says whether a bounding-box tree (+0xf0,
// read by fn_710010ddc4) follows; last the shape refreshes itself (virtual slot 0x80). Returns
// whether the tree read succeeded (true without one). Names and layouts are placeholders.
#include "base/global/CRntFile.h"
#include "types.h"

using base::global::CRntFile;

struct UnkNavMeshTree;
bool fn_710010ddc4(UnkNavMeshTree* self, base::global::CRntFile* file);

struct UnkPolygonCollectionShape;
bool fn_7100d6c288(UnkPolygonCollectionShape* self, CRntFile* file);
void fn_7100d62ec0(UnkPolygonCollectionShape* self, CRntFile* file);

struct UnkPolygonCollectionShape {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    u8 unk_0x8[0xe0];
    bool has_tree;
    u8 pad_0xe9[0x7];
    u8 tree[0x8];
};

bool fn_7100d62e3c(UnkPolygonCollectionShape* self, CRntFile* file) {
    fn_7100d6c288(self, file);
    fn_7100d62ec0(self, file);
    self->has_tree = true;
    fn_710007c714(file, &self->has_tree, 1, 1);
    bool ok = true;
    if (self->has_tree)
        ok = reinterpret_cast<bool (*)(UnkNavMeshTree*, CRntFile*)>(fn_710010ddc4)(
            reinterpret_cast<UnkNavMeshTree*>(self->tree), file);
    self->vf_0x80();
    return ok;
}
