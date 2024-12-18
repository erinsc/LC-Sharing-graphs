#include "nodes.h"

void relink(Node *base, Node *from, Node *to) {
    if (base->body == from) {
        base->body = to;
        return;
    }
    if (base->type == ROOT)
        return;
    if (base->root == from) {
        base->root = to;
        return;
    }
    if (base->type == ARROW)
        return;
    if (base->side == from) {
        base->side = to;
    }
}
