/*
 * Copyright 2015 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef GrCopySurfaceOp_DEFINED
#define GrCopySurfaceOp_DEFINED

#include "GrOp.h"
#include "GrOpFlushState.h"

class GrCopySurfaceOp final : public GrOp {
public:
    DEFINE_OP_CLASS_ID

    static std::unique_ptr<GrOp> Make(GrSurfaceProxy* dst, GrSurfaceProxy* src,
                                      const SkIRect& srcRect,
                                      const SkIPoint& dstPoint);

    const char* name() const override { return "CopySurface"; }

    void visitProxies(const VisitProxyFunc& func) const override {
        func(fSrc.get());
    }

    SkString dumpInfo() const override {
        SkString string;
        string.append(INHERITED::dumpInfo());
        string.printf("srcProxyID: %d,\n"
                      "srcRect: [ L: %d, T: %d, R: %d, B: %d ], dstPt: [ X: %d, Y: %d ]\n",
                      fSrc.get()->uniqueID().asUInt(),
                      fSrcRect.fLeft, fSrcRect.fTop, fSrcRect.fRight, fSrcRect.fBottom,
                      fDstPoint.fX, fDstPoint.fY);
        return string;
    }

private:
    GrCopySurfaceOp(GrSurfaceProxy* dst, GrSurfaceProxy* src,
                    const SkIRect& srcRect, const SkIPoint& dstPoint)
            : INHERITED(ClassID())
            , fSrc(src)
            , fSrcRect(srcRect)
            , fDstPoint(dstPoint) {
        SkRect bounds =
                SkRect::MakeXYWH(SkIntToScalar(dstPoint.fX), SkIntToScalar(dstPoint.fY),
                                 SkIntToScalar(srcRect.width()), SkIntToScalar(srcRect.height()));
        this->setBounds(bounds, HasAABloat::kNo, IsZeroArea::kNo);
    }

    bool onCombineIfPossible(GrOp* that, const GrCaps& caps) override { return false; }

    void onPrepare(GrOpFlushState*) override {}

    void onExecute(GrOpFlushState* state) override;

    GrPendingIOResource<GrSurfaceProxy, kRead_GrIOType>  fSrc;
    SkIRect                                              fSrcRect;
    SkIPoint                                             fDstPoint;

    typedef GrOp INHERITED;
};

#endif
