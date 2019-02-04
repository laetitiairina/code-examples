//
//  main.c
//  task6
//
//  Created by Laetitia Britschgi on 29.04.18.
//  Copyright © 2018 Laetitia Britschgi. All rights reserved.
//

#include<stdio.h>
#include<stdlib.h>
#include<time.h>

struct TreapNode {
    int key;
    int priority;
    struct TreapNode *left;
    struct TreapNode *right;
//    additional pointer to parent
    struct TreapNode *parent;
};

struct TreapNode *root;

struct TreapNode* newNode(int key) {
    struct TreapNode* temp = malloc(sizeof(struct TreapNode));
    
    temp->key = key;
    temp->priority = rand()%100;
    temp->left = temp->right = temp->parent = NULL;
    
    return temp;
}

struct TreapNode* search(struct TreapNode* root, int val) {
    while(root != NULL && (root->key) != val) {
        if(root->key > val) {
            root = root->left;
        }
        else {
            root = root->right;
        }
    }
    return root;
}

void inorder(struct TreapNode* root) {
    if (root) {
        inorder(root->left);
        printf("key: %d | priority: %d ", root->key, root->priority);
        if (root->left)
            printf(" | left child: %d", root->left->key);
        if (root->right)
            printf(" | right child: %d", root->right->key);
        printf("\n");
        inorder(root->right);
    }
}

// normal insertion into Binary Search Tree
struct TreapNode* insertBST(struct TreapNode* root, int val) {
    struct TreapNode* parent = NULL;
    struct TreapNode* x = root;
    while(x != NULL) {
        parent = x;
        if(x->key < val) {
            x = x->right;
        }
        else {
            x = x->left;
        }
    }
    struct TreapNode* new = newNode(val);
    new->parent = parent;
    if(parent->key < val) {
        parent->right = new;
    }
    else {
        parent->left = new;
    }
    return new;
}

// in these two rotations p is always the parent node and s the child
struct TreapNode* rightRotation(struct TreapNode* root, struct TreapNode* p) {
    struct TreapNode* s = p->left;
    p->left = s->right;
    s->parent = p->parent;
    if(s->right != NULL) {
        s->right->parent = p;
    }
    if(p->parent == NULL) {
        root = s;
    }
    else {
        if(p == p->parent->right) {
            p->parent->right = s;
        }
        else {
            p->parent->left = s;
        }
    }
    s->right = p;
    p->parent = s;
    return root;
}

struct TreapNode* leftRotation(struct TreapNode* root, struct TreapNode* p) {
    struct TreapNode* s = p->right;
    p->right = s->left;
    s->parent = p->parent;
    if(s->left != NULL) {
        s->left->parent = p;
    }
    if(p->parent == NULL) {
        root = s;
    }
    else {
        if(p == p->parent->right) {
            p->parent->right = s;
        }
        else {
            p->parent->left = s;
        }
    }
    s->left = p;
    p->parent = s;
    return root;
}

// checks if rotation is needed and further on up the tree until all nodes are in the right order (for inserting)
struct TreapNode* rotationRecursiveUpwards(struct TreapNode* t, struct TreapNode* parent, struct TreapNode* root) {
//    if t is the right child of parent and the parents priority is lower
    if(parent->right == t && parent->priority < t->priority) {
//        rotate the parent and set t=p
        root = leftRotation(root, parent);
//        if t isn't the root, go on
        if(t->parent != NULL) {
        return rotationRecursiveUpwards(t, t->parent, root);
        }
    }
//    if t is the left child of parent and the parents priority is lower
    else if(parent->left == t && parent->priority < t->priority) {
//        rotate the parent and set t=p
        root = rightRotation(root, parent);
//        if t isn't the root, go on
        if(t->parent != NULL) {
        return rotationRecursiveUpwards(t, t->parent, root);
        }
    }
    return root;
}

struct TreapNode* insert(struct TreapNode* root, int val) {
//    if the tree is empty
    if(root == NULL) {
        struct TreapNode* new = newNode(val);
        root = new;
        return root;
    }
    else {
//    first: a normal BST insertion
        struct TreapNode* t = insertBST(root, val);
        struct TreapNode* parent = t->parent;
//        check priority with parent and rotate if needed
        root = rotationRecursiveUpwards(t, parent, root);
        return root;
    }
}

// checks that the priorities are in the right order downwards (for deletion)
struct TreapNode* rotationRecursiveDownwards(struct TreapNode* root, struct TreapNode* parent) {
//    find child with larger priority
    if(parent->left != NULL && parent->left->priority > parent->priority && parent->left->priority > parent->right->priority) {
        root = rightRotation(root, parent);
//    now after the rotation parent is in the lower position, so we can reuse it to go down in the tree
        return rotationRecursiveDownwards(root, parent);
    }
    else if(parent->right != NULL && parent->right->priority > parent->priority){
        root = leftRotation(root, parent);
//    now after the rotation parent is in the lower position, so we can reuse it to go down in the tree
        return rotationRecursiveDownwards(root, parent);
    }
    return root;
}

struct TreapNode* deleteNode(struct TreapNode* root, int val) {
//    first a normal BST deletion
    struct TreapNode* u = search(root, val);
    struct TreapNode* v = u->parent;
    //    if u has no children
    if(u->right == NULL && u->left == NULL) {
        if(v->right == u){
            v->right = NULL;
        }
        else {
            v->left = NULL;
        }
    }
    //    less than 2 children - will be correct regarding priority
    else if(u->right == NULL) {
        u->left->parent = v; // updating parent pointer
        if(v == NULL) {
            root = u ->left;
        }
        else if(v->left == u) {
            v->left = u->left;
        }
        else {
            v->right = u->left;
        }
    }
    else if(u->left == NULL) {
        u->right->parent = v; // updating parent pointer
        if(v == NULL) {
            root = u->right;
        }
        else if(v->left == u) {
            v->left = u->right;
        }
        else {
            v->right = u->right;
        }
    }
    //    two children (q one step behind p)
    //    priority may be off
    else {
        struct TreapNode* p = u->left;
        struct TreapNode* q = p;
        //        finds element furthest to the right in x's left subtree
        while(p->right != NULL) {
            q = p;
            p = p->right;
        }
        p->parent = v; // updating parent pointer
        if(v == NULL) {
            root = p;
        }
        else if(v->left == u) {
            v->left = p;
        }
        else {
            v->right = p;
        }
        p->right = u->right;
        u->right->parent = p; // updating parent pointer
        if(q != p) {
            q->right = p->left;
            p->left->parent = q; // updating parent pointer
            p->left = u->left;
            u->left->parent = p; // updating parent pointer
        }
//        second priority updating
        root = rotationRecursiveDownwards(root, p);
    }
    free(u);
    return root;
}

void main() {
    srand(time(NULL));
    
    root = NULL;
    
    root = insert(root, 50);
    root = insert(root, 30);
    root = insert(root, 20);
    root = insert(root, 40);
    root = insert(root, 70);
    root = insert(root, 60);
    root = insert(root, 80);
    
    printf("Inorder traversal of the given tree \n");
    inorder(root);
    
    printf("\nDelete 20\n");
    root = deleteNode(root, 20);
    printf("Inorder traversal of the modified tree \n");
    inorder(root);

    printf("\nDelete 30\n");
    root = deleteNode(root, 30);
    printf("Inorder traversal of the modified tree \n");
    inorder(root);

    printf("\nDelete 50\n");
    root = deleteNode(root, 50);
    printf("Inorder traversal of the modified tree \n");
    inorder(root);
    
    struct TreapNode *res = search(root, 50);

    if(res == NULL){
        printf("\n50 Not Found\n");
    } else {
        printf("\n50 Found\n");
    }
}

