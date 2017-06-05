#ifndef _UTIL_H_
#define _UTIL_H_

TreeNode * newStmtNode(StmtKind kind);
TreeNode * newExpNode(ExpKind kind);
void printTree(TreeNode * tree);
void setindexzero(Index *index);

#endif