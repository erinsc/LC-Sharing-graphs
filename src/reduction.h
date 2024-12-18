#pragma once
#include "nodes.h"

void beta_reduce(Node *app);
void fanout_propagate(Node *app);
void fanin_propagate(Node *fanin);
void fan_interact(Node *fanin);
void arrow_propagate(Node *arrow);
long apply_reductions(Node *node);
