/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_SVGFEConvolveMatrixElement_h
#define mozilla_dom_SVGFEConvolveMatrixElement_h

#include "SVGAnimatedNumberList.h"
#include "SVGBoolean.h"
#include "SVGEnum.h"
#include "SVGFilters.h"
#include "SVGInteger.h"
#include "SVGIntegerPair.h"
#include "nsSVGNumber2.h"
#include "SVGString.h"

nsresult NS_NewSVGFEConvolveMatrixElement(
    nsIContent** aResult, already_AddRefed<mozilla::dom::NodeInfo>&& aNodeInfo);

namespace mozilla {

namespace dom {
class DOMSVGAnimatedNumberList;
class SVGAnimatedBoolean;

typedef SVGFE SVGFEConvolveMatrixElementBase;

class SVGFEConvolveMatrixElement : public SVGFEConvolveMatrixElementBase {
  friend nsresult(::NS_NewSVGFEConvolveMatrixElement(
      nsIContent** aResult,
      already_AddRefed<mozilla::dom::NodeInfo>&& aNodeInfo));

 protected:
  explicit SVGFEConvolveMatrixElement(
      already_AddRefed<mozilla::dom::NodeInfo>&& aNodeInfo)
      : SVGFEConvolveMatrixElementBase(std::move(aNodeInfo)) {}
  virtual JSObject* WrapNode(JSContext* aCx,
                             JS::Handle<JSObject*> aGivenProto) override;

 public:
  virtual FilterPrimitiveDescription GetPrimitiveDescription(
      nsSVGFilterInstance* aInstance, const IntRect& aFilterSubregion,
      const nsTArray<bool>& aInputsAreTainted,
      nsTArray<RefPtr<SourceSurface>>& aInputImages) override;
  virtual bool AttributeAffectsRendering(int32_t aNameSpaceID,
                                         nsAtom* aAttribute) const override;
  virtual SVGString& GetResultImageName() override {
    return mStringAttributes[RESULT];
  }
  virtual void GetSourceImageNames(nsTArray<SVGStringInfo>& aSources) override;

  virtual nsresult Clone(dom::NodeInfo*, nsINode** aResult) const override;

  // WebIDL
  already_AddRefed<SVGAnimatedString> In1();
  already_AddRefed<SVGAnimatedInteger> OrderX();
  already_AddRefed<SVGAnimatedInteger> OrderY();
  already_AddRefed<DOMSVGAnimatedNumberList> KernelMatrix();
  already_AddRefed<SVGAnimatedInteger> TargetX();
  already_AddRefed<SVGAnimatedInteger> TargetY();
  already_AddRefed<SVGAnimatedEnumeration> EdgeMode();
  already_AddRefed<SVGAnimatedBoolean> PreserveAlpha();
  already_AddRefed<SVGAnimatedNumber> Divisor();
  already_AddRefed<SVGAnimatedNumber> Bias();
  already_AddRefed<SVGAnimatedNumber> KernelUnitLengthX();
  already_AddRefed<SVGAnimatedNumber> KernelUnitLengthY();

 protected:
  virtual NumberAttributesInfo GetNumberInfo() override;
  virtual NumberPairAttributesInfo GetNumberPairInfo() override;
  virtual IntegerAttributesInfo GetIntegerInfo() override;
  virtual IntegerPairAttributesInfo GetIntegerPairInfo() override;
  virtual BooleanAttributesInfo GetBooleanInfo() override;
  virtual EnumAttributesInfo GetEnumInfo() override;
  virtual StringAttributesInfo GetStringInfo() override;
  virtual NumberListAttributesInfo GetNumberListInfo() override;

  enum { DIVISOR, BIAS };
  nsSVGNumber2 mNumberAttributes[2];
  static NumberInfo sNumberInfo[2];

  enum { KERNEL_UNIT_LENGTH };
  SVGNumberPair mNumberPairAttributes[1];
  static NumberPairInfo sNumberPairInfo[1];

  enum { TARGET_X, TARGET_Y };
  SVGInteger mIntegerAttributes[2];
  static IntegerInfo sIntegerInfo[2];

  enum { ORDER };
  SVGIntegerPair mIntegerPairAttributes[1];
  static IntegerPairInfo sIntegerPairInfo[1];

  enum { PRESERVEALPHA };
  SVGBoolean mBooleanAttributes[1];
  static BooleanInfo sBooleanInfo[1];

  enum { EDGEMODE };
  SVGEnum mEnumAttributes[1];
  static SVGEnumMapping sEdgeModeMap[];
  static EnumInfo sEnumInfo[1];

  enum { RESULT, IN1 };
  SVGString mStringAttributes[2];
  static StringInfo sStringInfo[2];

  enum { KERNELMATRIX };
  SVGAnimatedNumberList mNumberListAttributes[1];
  static NumberListInfo sNumberListInfo[1];
};

}  // namespace dom
}  // namespace mozilla

#endif  // mozilla_dom_SVGFEConvolveMatrixElement_h
