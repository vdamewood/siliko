#include <stdlib.h>
#include <stdio.h>

#include "XvcEvaluate.h"
#include "cleanup.h"


void debug_report(const char * function, void * address)
{
	if (!address) {
		printf("[NULL]:      %s\n", function);
	}
	else {
		printf("%p: %s\n", address, function);
	}
}

struct TreeList {
	struct TreeList * next;
	tree * tree;
};

static struct TreeList * DanglingTrees = NULL;

void print_tree_list();

void xvcalc_clear_dangling_trees();
void XvcMemoryCleanup()
{
	//debug_report(__FUNCTION__, NULL);
	xvcalc_clear_dangling_trees();
}

void print_tree_list()
{
	struct TreeList * CurrentNode;
	CurrentNode = DanglingTrees;
	while (CurrentNode) {
		printf("%p	Listing!\n", CurrentNode->tree);
		CurrentNode = CurrentNode->next;
	}
}

void xvcalc_cache_dangling_tree(tree * in)
{
	//debug_report(__FUNCTION__, in);
	struct TreeList * NewNode;
	struct TreeList * CurrentNode;
	NewNode = malloc(sizeof(struct TreeList));
	NewNode->tree = in;
	NewNode->next = NULL;
	//printf("NewNode: %p t:%p n:%p\n", NewNode, NewNode->tree, NewNode->next);

	if (!DanglingTrees) {
		DanglingTrees = NewNode;
		//printf("!DT so DT becomes: %p\n", DanglingTrees);
	}
	else {
		CurrentNode = DanglingTrees;
		while (CurrentNode->next) {
			CurrentNode = CurrentNode->next;
		}
		CurrentNode->next = NewNode;
		//printf("Appending %p to %p.\n", NewNode, CurrentNode);
	}
	//print_tree_list();
}

void xvcalc_release_dangling_tree(tree * in)
{
	//debug_report(__FUNCTION__, in);
	struct TreeList ** ParentPointer;
	struct TreeList * CurrentNode;

	if (!DanglingTrees) {
		//printf("!Dangling Tree, so not removing anything.\n");
		return;
	}

	CurrentNode = DanglingTrees;
	ParentPointer = &DanglingTrees;
	
	//printf("ParentPointer(%p), CurrentNode(%p)\n", ParentPointer, DanglingTrees);
	while (CurrentNode) {
		if (CurrentNode->tree == in) {
			//printf("Found %p in %p. Deleting!\n", in, CurrentNode);
			*ParentPointer = CurrentNode->next;
			free(CurrentNode);
			CurrentNode = *ParentPointer;
		}
		else {
			ParentPointer = &(CurrentNode->next);
			CurrentNode = *ParentPointer;
		}
	}
}

void xvcalc_clear_dangling_trees()
{
	//debug_report(__FUNCTION__, NULL);
	struct TreeList * CurrentNode;
	struct TreeList * OldNode;

	CurrentNode = DanglingTrees;
	DanglingTrees = NULL;
	
	while (CurrentNode) {
		//printf("Deleting: %p t:%p n:%p\n", CurrentNode, CurrentNode->tree, CurrentNode->next);
		XvcTreeDelete(CurrentNode->tree);
		OldNode = CurrentNode;
		CurrentNode = CurrentNode->next;
		free(OldNode);
	}
}

void xvcalc_cache_dangling_arglist(arglist * in)
{
	//debug_report(__FUNCTION__, in);
}

void xvcalc_release_dangling_arglist(arglist * in)
{
	//debug_report(__FUNCTION__, in);
}

void xvcalc_clear_dangling_arglists()
{
	//debug_report(__FUNCTION__, NULL);
}

void xvcalc_cache_dangling_id(char * in)
{
	//debug_report(__FUNCTION__, in);
}

void xvcalc_release_dangling_id(char * in)
{
	//debug_report(__FUNCTION__, in);
}

void xvcalc_clear_dangling_ids()
{
	//debug_report(__FUNCTION__, NULL);
}
