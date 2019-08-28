#include "graph_view.h"
#include <poincare/serialization_helper.h>
#include <assert.h>
#include <cmath>

using namespace Shared;

namespace Graph {

GraphView::GraphView(CartesianFunctionStore * functionStore, InteractiveCurveViewRange * graphRange,
  CurveViewCursor * cursor, BannerView * bannerView, View * cursorView) :
  FunctionGraphView(graphRange, cursor, bannerView, cursorView),
  m_functionStore(functionStore),
  m_tangent(false)
{
}

void GraphView::reload() {
  if (m_tangent) {
    KDRect dirtyZone(KDRect(0, 0, bounds().width(), bounds().height()-m_bannerView->bounds().height()));
    markRectAsDirty(dirtyZone);
  }
  return FunctionGraphView::reload();
}

void GraphView::drawRect(KDContext * ctx, KDRect rect) const {
  FunctionGraphView::drawRect(ctx, rect);
  for (int i = 0; i < m_functionStore->numberOfActiveFunctions(); i++) {
    Ion::Storage::Record record = m_functionStore->activeRecordAtIndex(i);
    ExpiringPointer<CartesianFunction> f = m_functionStore->modelForRecord(record);;

    switch (f->plotType()) {
    case Shared::CartesianFunction::PlotType::Cartesian:
      drawCartesianCurve(ctx, rect, [](float t, void * model, void * context) {
            CartesianFunction * f = (CartesianFunction *)model;
            Poincare::Context * c = (Poincare::Context *)context;
            return f->evaluateAtParameter(t, c).y();
          }, f.operator->(), context(), f->color(), record == m_selectedRecord, m_highlightedStart, m_highlightedEnd);
      /* Draw tangent */
      if (m_tangent && record == m_selectedRecord) {
        float tangentParameter[2];
        tangentParameter[0] = f->approximateDerivative(m_curveViewCursor->x(), context());
        tangentParameter[1] = -tangentParameter[0]*m_curveViewCursor->x()+f->evaluateAtParameter(m_curveViewCursor->x(), context()).y();
        drawCartesianCurve(ctx, rect, [](float t, void * model, void * context) {
              float * tangent = (float *)model;
              return tangent[0]*t+tangent[1];
            }, tangentParameter, nullptr, Palette::GreyVeryDark);
      }
      break;
    case Shared::CartesianFunction::PlotType::Polar:
      drawCurve(ctx, rect, 0.0f, 396.0f, 36.0f, [](float t, void * model, void * context) { //TODO LEA RUBEN use the models's tMin, tMax
            CartesianFunction * f = (CartesianFunction *)model;
            Poincare::Context * c = (Poincare::Context *)context;
            return f->evaluateAtParameter(t, c).y() * std::cos(t);
          }, [](float t, void * model, void * context) {
            CartesianFunction * f = (CartesianFunction *)model;
            Poincare::Context * c = (Poincare::Context *)context;
            return f->evaluateAtParameter(t, c).y() * std::sin(t);
          }, f.operator->(), context(), false, f->color());
      break;
    case Shared::CartesianFunction::PlotType::Parametric:
      drawCurve(ctx, rect, 0.0f, 396.0f, 36.0f, [](float t, void * model, void * context) { //TODO LEA RUBEN use the models's tMin, tMax
            CartesianFunction * f = (CartesianFunction *)model;
            Poincare::Context * c = (Poincare::Context *)context;
            if (f->isCircularlyDefined(c)) {
              return NAN;
            }
            constexpr int bufferSize = CodePoint::MaxCodePointCharLength + 1;
            char unknownX[bufferSize];
            Poincare::SerializationHelper::CodePoint(unknownX, bufferSize, UCodePointUnknownX);
            Poincare::VariableContext variableContext(unknownX, c);
            variableContext.setApproximationForVariable(t);
            Poincare::Expression e = f->expressionReduced(c);
            assert(
                e.type() == Poincare::ExpressionNode::Type::Matrix &&
                static_cast<Poincare::Matrix&>(e).numberOfRows() == 2 &&
                static_cast<Poincare::Matrix&>(e).numberOfColumns() == 1
                );
            Poincare::Preferences * preferences = Poincare::Preferences::sharedPreferences();
            Poincare::Preferences::ComplexFormat complexFormat = Poincare::Expression::UpdatedComplexFormatWithExpressionInput(preferences->complexFormat(), e, c);
            return e.childAtIndex(0).approximateToScalar<float>(&variableContext, complexFormat, preferences->angleUnit());
          }, [](float t, void * model, void * context) {
            CartesianFunction * f = (CartesianFunction *)model;
            Poincare::Context * c = (Poincare::Context *)context;
            if (f->isCircularlyDefined(c)) {
              return NAN;
            }
            constexpr int bufferSize = CodePoint::MaxCodePointCharLength + 1;
            char unknownX[bufferSize];
            Poincare::SerializationHelper::CodePoint(unknownX, bufferSize, UCodePointUnknownX);
            Poincare::VariableContext variableContext(unknownX, c);
            variableContext.setApproximationForVariable(t);
            Poincare::Expression e = f->expressionReduced(c);
            assert(
                e.type() == Poincare::ExpressionNode::Type::Matrix &&
                static_cast<Poincare::Matrix&>(e).numberOfRows() == 2 &&
                static_cast<Poincare::Matrix&>(e).numberOfColumns() == 1
                );
            Poincare::Preferences * preferences = Poincare::Preferences::sharedPreferences();
            Poincare::Preferences::ComplexFormat complexFormat = Poincare::Expression::UpdatedComplexFormatWithExpressionInput(preferences->complexFormat(), e, c);
            return e.childAtIndex(1).approximateToScalar<float>(&variableContext, complexFormat, preferences->angleUnit());
          }, f.operator->(), context(), false, f->color());
      break;
    default:
      assert(false);
    }
  }
}

}
