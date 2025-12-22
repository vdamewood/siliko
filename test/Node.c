#include <criterion/criterion.h>
#include <SilikoCore/SyntaxTree.h>
#include <SilikoCore/FunctionCaller.h>

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

Test(SyntaxTreeTests, TwoPlusThreeIsFive)
{
    SilikoSyntaxTreeNode *left_node = SilikoSyntaxTreeNewFromInteger(2);
    cr_assert(SilikoSyntaxTreeGetType(left_node) == SILIKO_AST_LEAF);

    SilikoSyntaxTreeNode *right_node = SilikoSyntaxTreeNewFromInteger(3);
    cr_assert(SilikoSyntaxTreeGetType(right_node) == SILIKO_AST_LEAF);

    SilikoSyntaxTreeNode *branch = SilikoSyntaxTreeNewBranch("add");
    cr_assert(SilikoSyntaxTreeGetType(branch) == SILIKO_AST_BRANCH);

    SilikoSyntaxTreePushRight(branch, left_node);
    cr_assert(SilikoSyntaxTreeFetchChild(branch, 0) == left_node);

    SilikoSyntaxTreePushRight(branch, right_node);
    cr_assert(SilikoSyntaxTreeFetchChild(branch, 0) == left_node);
    cr_assert(SilikoSyntaxTreeFetchChild(branch, 1) == right_node);

    SilikoFunctionCaller *engine = SilikoFunctionCallerNew();
    SilikoFunctionCallerInstallOperators(engine);

    SilikoValue *test_value = SilikoSyntaxTreeEvaluate(branch, engine);
    cr_assert(SilikoValueGetStatus(test_value) == SilikoValueInteger,
        "Evalutes to the wrong type. Should be: %d  Is: %d",
            SilikoValueInteger,
            SilikoValueGetStatus(test_value));
    cr_assert(SilikoValueToInteger(test_value) == 5);
    SilikoValueDelete(test_value);
}
