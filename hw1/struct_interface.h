#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

#define KNRM    "\x1B[0m"
#define KRED    "\x1B[31m"
#define KGRN    "\x1B[32m"
#define KYEL    "\x1B[33m"
#define RESET   "\033[0m"

#define INIT_SIZE 50

struct monitor {
  struct successor *tree_head;
  struct successor *goal_node;
  struct state *start;
  struct state *goal;
  int counter;

  int queue_size;
  int queue_cap;
  struct successor *queue;

  int states_size;
  int states_cap;
  struct state *states;
};

struct successor {
  struct state *s;
  struct successor *parent;
  struct successor *children[5];
};

struct state {
  struct bank *left;
  struct bank *right;
};

struct bank {
  int missionaries;
  int cannibals;
  int boat;
};

void init_bank (struct bank *new_bank) {
    new_bank->missionaries = 0;
    new_bank->cannibals = 0;
    new_bank->boat = 0;
}

struct bank *create_bank () {
    struct bank *new_bank = malloc(sizeof(struct bank));
    init_bank(new_bank);
    return new_bank;
}

void init_state (struct state *new_state) {
  new_state->left = create_bank();
  new_state->right = create_bank();
}

void debug_state (struct state *s) {
  printf(KGRN
    "State: \n"
    "Left:\tm:%d\tc:%d\tb:%d\n"
    "Right:\tm:%d\tc:%d\tb:%d\n" RESET "\n",
    s->left->missionaries, s->left->cannibals, s->left->boat,
    s->right->missionaries, s->right->cannibals, s->right->boat
  );
}

struct state *create_state () {
  struct state *new_state = malloc(sizeof(struct state));
  init_state(new_state);
  return new_state;
}

void copy_state (struct state *source, struct state *target) {
  target->left->missionaries = source->left->missionaries;
  target->left->cannibals = source->left->cannibals;
  target->left->boat = source->left->boat;

  target->right->missionaries = source->right->missionaries;
  target->right->cannibals = source->right->cannibals;
  target->right->boat = source->right->boat;
}

bool compare_states (struct state *s1, struct state *s2) {
  if (s1->left->missionaries != s2->left->missionaries) return false;
  if (s1->left->cannibals != s2->left->cannibals) return false;
  if (s1->left->boat != s2->left->boat) return false;

  if (s1->right->missionaries != s2->right->missionaries) return false;
  if (s1->right->cannibals != s2->right->cannibals) return false;
  if (s1->right->boat != s2->right->boat) return false;

  return true;
}

void print_successor_path(struct successor *root, struct successor *curr) {
  if (curr->s != root->s) {
    print_successor_path(root, curr->parent);
  }
  debug_state(curr->s);
}

int increase_queue_size (struct successor *queue, int size) {
  printf(KYEL "increasing queue size." RESET "\n");
  struct successor *new_queue;
  const int queue_size = size * 2;
  new_queue = malloc(sizeof(struct successor) * queue_size);
  memset(new_queue, 0, sizeof(struct successor) * queue_size);

  for (int i = 0; i < size; i++) {
    new_queue[i] = queue[i];
    debug_state(new_queue[i].s);
  }

  queue = new_queue;

  return queue_size;
}

void init_successor (struct successor *new_successor) {
  new_successor->s = NULL;
  new_successor->parent = NULL;
  for (int i = 0; i < 5; i++) {
    new_successor->children[i] = NULL;
  }
}

struct successor *create_successor () {
  struct successor *new_successor = malloc(sizeof(struct successor));
  init_successor(new_successor);
  return new_successor;
}

void init_monitor (struct monitor *new_monitor) {
  new_monitor->tree_head = create_successor();
  new_monitor->start = create_state();
  new_monitor->goal = create_state();

  new_monitor->counter = 0;

  new_monitor->queue_size = 0;
  new_monitor->queue_cap = INIT_SIZE;
  new_monitor->queue = malloc(sizeof(struct successor) * INIT_SIZE);

  new_monitor->states_size = 0;
  new_monitor->states_cap = INIT_SIZE;
  new_monitor->states = malloc(sizeof(struct state) * INIT_SIZE);
}

int increase_states_size (struct monitor *m) {
  printf(KYEL "increasing states size" RESET "\n");
  struct state *states;
  const int states_cap = m->states_cap * 2;
  states = malloc(sizeof(struct state) * states_cap);
  memset(states, 0, sizeof(struct state) * states_cap);

  for (int i = 0; i < m->states_size; i++) {
    states[i] = m->states[i];
  }

  free(m->states);

  m->states = states;
  return states_cap;
}

struct state *check_monitor_states(struct monitor *m, struct state *s) {
  for (int i = 0; i < m->states_size; i++) {
    if (compare_states(&m->states[i], s)) {
      return &m->states[i];
    }
  }

  return NULL;
}
