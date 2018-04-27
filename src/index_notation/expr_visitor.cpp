#include "taco/index_notation/expr_visitor.h"
#include "taco/index_notation/expr_nodes.h"

namespace taco {

// class ExprVisitorStrict
IndexExprVisitorStrict::~IndexExprVisitorStrict() {
}

void IndexExprVisitorStrict::visit(const IndexExpr& expr) {
  expr.accept(this);
}

// class IndexNotationVisitorStrict
IndexNotationVisitorStrict::~IndexNotationVisitorStrict() {
}

void IndexNotationVisitorStrict::visit(const TensorExpr& expr) {
  expr.accept(this);
}


// class ExprVisitor
IndexNotationVisitor::~IndexNotationVisitor() {
}

void IndexNotationVisitor::visit(const AccessNode* op) {
}

void IndexNotationVisitor::visit(const NegNode* op) {
  visit(static_cast<const UnaryExprNode*>(op));
}

void IndexNotationVisitor::visit(const SqrtNode* op) {
  visit(static_cast<const UnaryExprNode*>(op));
}

void IndexNotationVisitor::visit(const AddNode* op) {
  visit(static_cast<const BinaryExprNode*>(op));
}

void IndexNotationVisitor::visit(const SubNode* op) {
  visit(static_cast<const BinaryExprNode*>(op));
}

void IndexNotationVisitor::visit(const MulNode* op) {
  visit(static_cast<const BinaryExprNode*>(op));
}

void IndexNotationVisitor::visit(const DivNode* op) {
  visit(static_cast<const BinaryExprNode*>(op));
}

void IndexNotationVisitor::visit(const IntImmNode* op) {
  visit(static_cast<const ImmExprNode*>(op));
}

void IndexNotationVisitor::visit(const FloatImmNode* op) {
  visit(static_cast<const ImmExprNode*>(op));
}

void IndexNotationVisitor::visit(const ComplexImmNode* op) {
  visit(static_cast<const ImmExprNode*>(op));
}

void IndexNotationVisitor::visit(const UIntImmNode* op) {
  visit(static_cast<const ImmExprNode*>(op));
}

void IndexNotationVisitor::visit(const ImmExprNode*) {
}

void IndexNotationVisitor::visit(const UnaryExprNode* op) {
  op->a.accept(this);
}

void IndexNotationVisitor::visit(const BinaryExprNode* op) {
  op->a.accept(this);
  op->b.accept(this);
}

void IndexNotationVisitor::visit(const ReductionNode* op) {
  op->a.accept(this);
}

void IndexNotationVisitor::visit(const AssignmentNode* op) {
  taco_not_supported_yet;
}

void IndexNotationVisitor::visit(const ForallNode* op) {
  taco_not_supported_yet;
}

void IndexNotationVisitor::visit(const WhereNode* op) {
  taco_not_supported_yet;
}

}
