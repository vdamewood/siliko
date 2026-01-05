#include <criterion/criterion.h>
#include <SilikoCore/Node.h>
#include <SilikoCore/Engine.h>
#include <SilikoCore/Evaluate.h>

Test(SyntaxTreeTests, NewIntZero)
{
    SilikoNode *Node = SilikoNodeNewFromInteger(0);
    cr_assert(SilikoNodeGetStatus(Node) == SilikoNodeLeaf);

    SilikoValue *test_value = SilikoEvaluateNode(Node, NULL);
    cr_assert(SilikoValueGetStatus(test_value) == SilikoValueInteger);
    cr_assert(SilikoValueGetInteger(test_value) == 0LL);
    SilikoValueDelete(test_value);
}

Test(SyntaxTreeTests, NewFloatZero)
{
    SilikoNode *Node = SilikoNodeNewFromReal(0.0);
    cr_assert(SilikoNodeGetStatus(Node) == SilikoNodeLeaf);

    SilikoValue *test_value = SilikoEvaluateNode(Node, NULL);
    cr_assert(SilikoValueGetStatus(test_value) == SilikoValueReal);
    cr_assert(SilikoValueGetInteger(test_value) == 0.0);
    SilikoValueDelete(test_value);
}

Test(SyntaxTreeTests, TwoPlusThreeIsFive)
{
    SilikoNode *left_node = SilikoNodeNewFromInteger(2);
    cr_assert(SilikoNodeGetStatus(left_node) == SilikoNodeLeaf);

    SilikoNode *right_node = SilikoNodeNewFromInteger(3);
    cr_assert(SilikoNodeGetStatus(right_node) == SilikoNodeLeaf);

    SilikoNode *branch = SilikoNodeNewBranch("add");
    cr_assert(SilikoNodeGetStatus(branch) == SilikoNodeBranch);

    SilikoNodePushRight(branch, left_node);
    cr_assert(SilikoNodeFetchChild(branch, 0) == left_node);

    SilikoNodePushRight(branch, right_node);
    cr_assert(SilikoNodeFetchChild(branch, 0) == left_node);
    cr_assert(SilikoNodeFetchChild(branch, 1) == right_node);

    SilikoEngine *engine = SilikoEngineNew();
    SilikoInstallOperators(engine);

    SilikoValue *test_value = SilikoEvaluateNode(branch, engine);
    cr_assert(SilikoValueGetStatus(test_value) == SilikoValueInteger,
        "Evalutes to the wrong type. Should be: %d  Is: %d",
            SilikoValueInteger,
            SilikoValueGetStatus(test_value));
    cr_assert(SilikoValueGetInteger(test_value) == 5);
    SilikoValueDelete(test_value);
}
