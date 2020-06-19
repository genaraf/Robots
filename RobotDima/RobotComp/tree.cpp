#include <stdio.h>
#include "tree.h"

void TreeNode::printSubtree(int level) const {
    int i;

    // Skip space for indent
    for (i = 0; i < level; ++i)
        printf("    "); // 4 spaces

    switch (nodeType) {
    case node_type_undefined:
        printf("undefined");
        break;
    case int_const:
        printf("int_const");
        printf(" value=%d", intValue);
        break;
    case double_const:
        printf("double_const");
        printf(" value=%lf", doubleValue);
        break;
    case string_const:
        printf("string_const");
        printf(" value=%s", stringValue.c_str());
        break;
    case name:
        printf("name");
        printf(" %s", nameDef->name.c_str());
        break;
    case array_idx:
        printf("array_idx");
        break;
    case plus:
        printf("plus");
        break;
    case minus:
        printf("minus");
        break;
    case uminus:
        printf("uminus");
        break;
    case mul:
        printf("mul");
        break;
    case div:
        printf("div");
        break;
    case mod:
        printf("mod");
        break;
    case lor:
        printf("lor");
        break;
    case land:
        printf("land");
        break;
    case lnot:
        printf("lnot");
        break;
    case relop:
        printf("relop: ");
        switch (intValue) {
        case RELOP_EQ:
            printf("=="); break;
        case RELOP_NE:
            printf("!="); break;
        case RELOP_GT:
            printf(">"); break;
        case RELOP_GE:
            printf(">="); break;
        case RELOP_LT:
            printf("<"); break;
        case RELOP_LE:
            printf("<="); break;
        default:
            printf("Illegal"); break;
        }
        break;
    default:
        printf("unknown_node_type");
    }
    printf("\n");
    if (left != 0)
        left->printSubtree(level+1);
    if (right != 0)
        right->printSubtree(level+1);
}

void Tree::print() const {
    if (root() != 0)
        root()->printSubtree(0);
}
