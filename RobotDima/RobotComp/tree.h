#ifndef TREE_H
#define TREE_H

#include <string>
#include <assert.h>
#include "lextypes.h"
#include "nt.h"

using std::string;

class TreeNode {
public:
    TreeNode* left;
    TreeNode* right;
    TreeNode* parent;
    int nodeType;
    int intValue;
    double doubleValue;
    string stringValue;
    int baseType;
    int dimension;
    NameDef* nameDef;

    // Node types
    enum {
        node_type_undefined = (-1),
        int_const = 0,
        double_const,
        string_const,
        name,
        array_idx,
        plus,
        minus,
        mul,
        div,
        mod,
        uminus,
        lor,
        land,
        lnot,
        relop           // Comparing (relation operation)
    };

    TreeNode():
        left(0),
        right(0),
        parent(0),
        nodeType(node_type_undefined),
        intValue(0),
        doubleValue(0),
        stringValue(),
        baseType(TYPE_UNDEFINED),
        dimension(0),
        nameDef()
    {}

    void addLeftSon(TreeNode* t) {
        assert(t != 0);
        left = t;
        t->parent = this;
    }

    void addRightSon(TreeNode* t) {
        assert(t != 0);
        right = t;
        t->parent = this;
    }

    void printSubtree(int level = 0) const;
    void print() const {
        printSubtree(0);
    }
};

class Tree {
public:
    TreeNode header;

    Tree():
        header()
    {}

    TreeNode* root() {
        return header.left;
    }

    const TreeNode* root() const {
        return header.left;
    }

    void print() const;
};

#endif
