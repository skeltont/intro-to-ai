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

#define INIT_STATES_SIZE 50

struct monitor {
  struct successor *tree_head;
  struct successor *goal_node;
  struct state *start;
  struct state *goal;
  int counter;
  int states_size;
  struct state *states;
};

struct successor {
  struct state *s;
  struct successor *parent;
  struct successor *children[6];
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
  if (curr != root) {
    print_successor_path(root, curr->parent);
  }
  debug_state(curr->s);
  return;
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
  new_monitor->states_size = 0;
  new_monitor->states = malloc(sizeof(struct state) * INIT_STATES_SIZE);
}

void increase_monitor_states (struct monitor *m) {
  struct state *states;
  const int states_size = m->states_size * 2;
  states = malloc(sizeof(struct state) * states_size);
  memset(states, 0, sizeof(struct state) * states_size);

  for (int i = 0; i < m->states_size; i++) {
    states[i] = m->states[i];
  }

  free(m->states);

  m->states_size = states_size;
  m->states = states;
}

struct state *check_monitor_states(struct monitor *m, struct state *s) {
  for (int i = 0; i < m->states_size; i++) {
    if (compare_states(&m->states[i], s)) {
      return &m->states[i];
    }
  }

  return NULL;
}

// void print_successor_path (struct monitor *m) {
//   struct successor *curr = m->goal_node;
//
//   // while (curr != m->tree_head) {
//   //   debug_state(curr->s);
//   //   curr = curr->parent;
//   // }
// }

void debug_monitor (struct monitor *m) {
  printf(KYEL
    "Current State: \n"
    "Left:\tm:%d\tc:%d\tb:%d\n"
    "Right:\tm:%d\tc:%d\tb:%d\n" RESET "\n",
    m->start->left->missionaries, m->start->left->cannibals, m->start->left->boat,
    m->start->right->missionaries, m->start->right->cannibals, m->start->right->boat
  );

  printf(KGRN
    "Goal State: \n"
    "Left:\tm:%d\tc:%d\tb:%d\n"
    "Right:\tm:%d\tc:%d\tb:%d\n" RESET "\n",
    m->goal->left->missionaries, m->goal->left->cannibals, m->goal->left->boat,
    m->goal->right->missionaries, m->goal->right->cannibals, m->goal->right->boat
  );

  printf("Counter: %d \n", m->counter);
}
