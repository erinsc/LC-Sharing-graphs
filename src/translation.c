#include "nodes.h"
#include <stdio.h>

Node* to_graph(char *string) {
    return (Node*) string;
}
char* to_string(Node *next) { 
    return NULL;
}
void print_graph(Node *next, short disp) { 
    Node *node = NULL;
    Node *prev = NULL;
    while (1) {
        prev = node;
        node = next;
        
        //printf("%d ", next == NULL);
        //if (next != NULL) printf("%d ", next->type != ABST);
        //if (next != NULL) printf("%d ", next->side != node);
        
        //printf("%p ", (void *) node);
        //printf("%p ", (void *) node->root);
        //printf("%p ", (void *) node->body);
        //printf("%p ", (void *) node->side);
        
        for (int i = 0;i < disp; ++i) {
            printf("    ");
        }
        
        switch(node->type) {
        case ROOT:
            printf("ROOT %ld\n",node->level);
            next = node->body;
            break;
        case ABST:
            if (node->side == prev) {
                printf("VAR %ld\n",node->level);
                return;
            }
            printf("ABST %ld\n",node->level);
            next = node->body;
            break;
        case APP:
            printf("APP %ld\n",node->level);
            print_graph(node->side, disp + 1);
            next = node->body;
            break;
        case FANIN:
            printf("FANIN %ld\n",node->level);
            if (node->side == prev)
                return;
            next = node->body;
            break;
        case FANOUT:
            printf("FANOUT %ld\n",node->level);
            print_graph(node->side, disp + 1);
            next = node->body;
            break;
        case ARROW:
            printf("ARROW %ld %ld\n",node->level, node->scale);
            next = node->body;
            break;
        }
    }
    return;
}
