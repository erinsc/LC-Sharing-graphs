#pragma once
#include <stdlib.h>

typedef enum { ROOT, ABST, APP, FANIN, FANOUT, ARROW } Type;


typedef struct Node Node;

struct Node {
    Type type;
    Node *root;
    Node *body;
    union {
        Node *side;
        long scale;
    };
    size_t level;
};

void relink(Node *base, Node *from, Node *to);
