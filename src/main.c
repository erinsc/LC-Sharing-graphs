#include "nodes.h"
#include "translation.h"
#include "reduction.h"

int main(void) {
    Node *start = malloc(sizeof(Node));
    start->type = ROOT;
    Node *arrow1 = malloc(sizeof(Node));
    arrow1->type = ARROW;
    arrow1->scale = 1;
    start->body = arrow1;
    arrow1->root = start;
    Node *app1 = malloc(sizeof(Node));
    app1->type = APP;
    arrow1->body = app1;
    app1->root = arrow1;
    Node *abst1 = malloc(sizeof(Node));
    abst1->type = ABST;
    app1->body = abst1;
    abst1->root = app1;
    Node *app2 = malloc(sizeof(Node));
    app2->type = APP;
    abst1->body = app2;
    app2->root = abst1;
    Node *arrow2 = malloc(sizeof(Node));
    arrow2->type = ARROW;
    arrow2->level = 1;
    arrow2->scale = -1;
    app2->body = arrow2;
    arrow2->root = app2;
    Node *fanin1 = malloc(sizeof(Node));
    fanin1->type = FANIN;
    fanin1->level = 0;
    arrow2->body = fanin1;
    app2->side = fanin1;
    fanin1->side = app2;
    fanin1->root = arrow2;
    fanin1->body = abst1;
    abst1->side = fanin1;
    Node *abst2 = malloc(sizeof(Node));
    abst2->type = ABST;
    abst2->level = 1;
    app1->side = abst2;
    abst2->root = app1;
    Node *arrow3 = malloc(sizeof(Node));
    arrow3->type = ARROW;
    arrow3->level = 1;
    arrow3->scale = -1;
    abst2->body = arrow3;
    abst2->side = arrow3;
    arrow3->root = abst2;
    arrow3->body = abst2;
    
    apply_reductions(start);    
}
