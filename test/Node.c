#include <criterion/criterion.h>
#include <SilikoCore/SyntaxTree.h>

Test(SyntaxTreeTests, NewIntZero) {
    SilikoSyntaxTreeNode *Node = SilikoSyntaxTreeNewFromInteger(0);
    cr_assert(SilikoSyntaxTreeGetType(Node) == SILIKO_AST_LEAF);

    struct SilikoValue Value = SilikoSyntaxTreeEvaluate(Node, NULL);
    cr_assert(Value.Status == SILIKO_VAL_INTEGER);
    cr_assert(Value.Integer == 0L);
}

Test(SyntaxTreeTests, NewFloatZero) {
    SilikoSyntaxTreeNode *Node = SilikoSyntaxTreeNewFromFloat(0.0);
    cr_assert(SilikoSyntaxTreeGetType(Node) == SILIKO_AST_LEAF);

    struct SilikoValue Value = SilikoSyntaxTreeEvaluate(Node, NULL);
    cr_assert(Value.Status == SILIKO_VAL_FLOAT);
    cr_assert(Value.Integer == 0.0);
}
