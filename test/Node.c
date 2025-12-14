#include <criterion/criterion.h>
#include <SilikoCore/SyntaxTree.h>

Test(SyntaxTreeTests, NewIntZero)
{
    SilikoSyntaxTreeNode *Node = SilikoSyntaxTreeNewFromInteger(0);
    cr_assert(SilikoSyntaxTreeGetType(Node) == SILIKO_AST_LEAF);

    SilikoValue *test_value = SilikoSyntaxTreeEvaluate(Node, NULL);
    cr_assert(SilikoValueGetStatus(test_value) == SilikoValueInteger);
    cr_assert(SilikoValueToInteger(test_value) == 0LL);
    SilikoValueDelete(test_value);
}

Test(SyntaxTreeTests, NewFloatZero)
{
    SilikoSyntaxTreeNode *Node = SilikoSyntaxTreeNewFromFloat(0.0);
    cr_assert(SilikoSyntaxTreeGetType(Node) == SILIKO_AST_LEAF);

    SilikoValue *test_value = SilikoSyntaxTreeEvaluate(Node, NULL);
    cr_assert(SilikoValueGetStatus(test_value) == SilikoValueReal);
    cr_assert(SilikoValueToInteger(test_value) == 0.0);
    SilikoValueDelete(test_value);
}
