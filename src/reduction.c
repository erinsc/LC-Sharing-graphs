
#include <stdio.h>
#include "nodes.h"
#include "reduction.h"
#include "translation.h"

void beta_reduce(Node *app) {
    Node *abs = app->body;
    
    relink(abs->side, abs, app->side);
    relink(app->side, app, abs->side);
    
    relink(app->root, app, abs->body);
    relink(abs->body, abs, app->root);
    
    free(app);
    free(abs);
}
void fanout_propagate(Node *app) {
    Node *fanout = app->body;
    Node *fanin = malloc(sizeof(Node));
    fanin->type = FANIN;
    fanin->level = fanout->level + 1;// NOTE: this +1 is completely unfounded
    Node *app2 = malloc(sizeof(Node));
    app2->type = APP;
    
    // Link external
    relink(fanout->body, fanout, app);
    relink(fanout->side, fanout, app2);
    relink(app->root, app, fanout);
    relink(app->side, app, fanin);
    // Link to external
    app->body = fanout->body;
    app2->body = fanout->side;
    fanout->root = app->root;
    fanin->body = app->side;
    // Link apps to fanin
    fanin->root = app;
    fanin->side = app2;
    app->side = fanin;
    app2->side = fanin;
    // Link apps to fanout
    fanout->body = app;
    fanout->side = app2;
    app->root = fanout;
    app2->root = fanout;
}
void fanin_propagate(Node *fanin) {
    Node *abst = fanin->body;
    Node *fanout = malloc(sizeof(Node));
    fanout->type = FANOUT;
    fanout->level = fanin->level + 1; // NOTE: this +1 is completely unfounded
    Node *abst2 = malloc(sizeof(Node));
    abst2->type = ABST;
    
    // Link external
    relink(fanin->side, fanin, abst2);
    relink(fanin->root, fanin, abst);
    relink(abst->side, abst, fanout);
    relink(abst->body, abst, fanin);
    // Link to external
    abst->root = fanin->root;
    abst2->root = fanin->side;
    fanout->root = abst->side;
    fanin->body = abst->body;
    // Link absts to fanin
    fanin->root = abst;
    fanin->side = abst2;
    abst->body = fanin;
    abst2->body = fanin;
    // Link absts to fanout
    fanout->body = abst;
    fanout->side = abst2;
    abst->side = fanout;
    abst2->side = fanout;
}
void fan_interact(Node *fanin) {
    Node *fanout = fanin->body;
    // If fans match
    if (fanout->level == fanin->level) {
        relink(fanin->root, fanin, fanout->body);
        relink(fanin->side, fanin, fanout->side);
        relink(fanout->body, fanout, fanin->root);
        relink(fanout->side, fanout, fanin->side);
        
        free(fanin);
        free(fanout);
        
        return;
    }
    // Fans dont match
    Node *fanin2 = malloc(sizeof(Node));
    fanin2->type = FANIN;
    fanin2->level = fanin->level;
    Node *fanout2 = malloc(sizeof(Node));
    fanout2->type = FANOUT;
    fanout2->level = fanout->level;
    
    // Link external
    relink(fanin->root, fanin, fanout);
    relink(fanin->side, fanin, fanout2);
    relink(fanout->body, fanout, fanin);
    relink(fanout->side, fanout, fanin2);
    // Link to external
    fanin->body = fanout->body;
    fanin2->body = fanout->side;
    fanout->root = fanin->root;
    fanout2->root = fanin->side;
    // Link fanouts to fanin
    fanout->body = fanin;
    fanout2->body = fanin;
    fanin->root = fanout;
    fanin->side = fanout2;
    // Link fanouts to fanin2
    fanout->side = fanin2;
    fanout2->side = fanin2;
    fanin2->root = fanout;
    fanin2->side = fanout2;
}
void arrow_propagate(Node *arrow) {
    Node *term = arrow->body;
    
    if (term->type == FANIN)
        return;
    if (term->type == ARROW) {
        term->scale += arrow->scale;
        
        relink(arrow->root, arrow, term);
        term->root = arrow->root;
        
        free(arrow);
        return;
    }
    
    // If term->type == ABST, APP, or FANOUT
    
    Node *arrow2 = malloc(sizeof(Node));
    arrow2->type = ARROW;
    
    term->level += arrow->scale;
    
    // Link external
    relink(arrow->root, arrow, term);
    relink(term->side, term, arrow2);
    relink(term->body, term, arrow);
    
    // Link to external
    term->root = arrow->root;
    arrow->body = term->body;
    if (term->type == ABST) {
        arrow2->root = term->side;
    }
    else {
        arrow2->body = term->side;
    }
    // Link together
    term->body = arrow;
    arrow->root = term;
    
    // Link to arrow2
    term->side = arrow2;
    if (term->type == ABST) {
        arrow2->body = term;
        arrow2->scale = -arrow->scale;
    }
    else {
        arrow2->root = term;
        arrow2->scale = arrow->scale;
    }
    return;
}

// TODO: implement fan swap

long apply_reductions(Node *next) {
    Node *root = next;
    
    to_string(root, 0);
    
    Node *node = NULL;
    long counter = 0;
    while (next == NULL || next->type != ABST || next->side != node) {
        ++counter;
        node = next;
        
        switch(node->type) {
        case ROOT:
            next = node->body;
            break;
        case APP:
            if (node->body->type == ABST) {
                next = node->root;
                printf("Beta Reduce ");
                printf("###############################\n");
                beta_reduce(node);
                print_graph(root, 0);
            }
            else if (node->body->type == FANOUT) {
                next = node->root;
                printf("Fanout Propagate ");
                printf("###############################\n");
                fanout_propagate(node);
                print_graph(root, 0);
            }
            else {
                next = node->body;
            //  next = node->side;
            }
            break;
        case ABST:
            next = node->body;
            break;
        case ARROW:
            if (node->body->type == FANIN) {
                next = node->body;
            }
            else if (node->body->type == ABST && node->body->side == node) {
                return counter;
            }
            else {
                next = node->root;
                printf("Arrow Propagate ");
                printf("###############################\n");
                arrow_propagate(node);
                print_graph(root, 0);
            }
            break;
        case FANIN:
            if (node->body->type == FANOUT) {
                next = node->root;
                printf("Fan Interact ");
                printf("###############################\n");
                fan_interact(node);
                print_graph(root, 0);
            }
            else if (node->body->type == ABST) {
                next = node->root;
                printf("Fanin Propagate ");
                printf("###############################\n");
                fanin_propagate(node);
                print_graph(root, 0);
            }
            else {
                next = node->body;
            }
            break;
        case FANOUT:
            next = node->body;
        //  next = node->side;
        }
    }
    return counter-1;
}
