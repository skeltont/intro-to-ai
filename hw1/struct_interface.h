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
#define BOAT_CAP 2

struct monitor {
  struct successor *tree_head;
  struct successor *goal_node;
  struct state *start;
  struct state *goal;
  bool found_result;
  int result;

  int queue_size;
  int queue_cap;
  struct successor *queue;

  struct priority_queue *p_queue_head;
  int p_queue_size;

  int states_size;
  int states_cap;
  struct state *states;
};

struct priority_queue {
  struct successor *node;
  float weight;
  struct priority_queue *prev;
  struct priority_queue *next;
};

struct successor {
  struct state *s;
  struct successor *parent;
  struct successor *children[5];
  int depth;
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

// void set_banks (struct state *s, struct bank *boat_bank, struct bank *far_bank) {
//   if (s->left->boat == 1) {
//     boat_bank = s->left;
//     far_bank = s->right;
//   } else {
//     boat_bank = s->right;
//     far_bank = s->left;
//   }
// }

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

int print_successor_path(struct successor *root, struct successor *curr, int path_length) {
  if (curr->s != root->s) {
    path_length = print_successor_path(root, curr->parent, ++path_length);
  }
  debug_state(curr->s);
  return path_length;
}

int increase_queue_cap (struct successor *queue, int size) {
  // printf(KYEL "increasing queue size." RESET "\n");
  struct successor *new_queue;
  const int queue_size = size * 2;
  new_queue = malloc(sizeof(struct successor) * queue_size);
  memset(new_queue, 0, sizeof(struct successor) * queue_size);

  for (int i = 0; i < size; i++) {
    new_queue[i] = queue[i];
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
  new_successor->depth = 0;
}

struct successor *create_successor () {
  struct successor *new_successor = malloc(sizeof(struct successor));
  init_successor(new_successor);
  return new_successor;
}

void init_priority_queue(struct priority_queue *p) {
  p->node = create_successor();
  p->weight = 0.0;
  p->prev = NULL;
  p->next = NULL;
}


float calculate_objective_weight (struct state *s) {
  float value;

  value = (s->right->missionaries + s->right->cannibals) / BOAT_CAP;

  return value;
}

struct priority_queue *create_priority_queue() {
  struct priority_queue *new_priority_queue = malloc(sizeof(struct priority_queue));
  init_priority_queue(new_priority_queue);
  return new_priority_queue;
}

void add_to_priority_queue (struct monitor *m, struct successor *node) {
  struct priority_queue *curr, *pq;

  curr = m->p_queue_head;
  pq = create_priority_queue();
  pq->node = node;
  pq->weight = calculate_objective_weight(node->s);

  if (m->queue_size == 0)
    m->p_queue_head = pq;
  else
    while (curr != NULL) {
      if (pq->weight <= curr->weight) {
        pq->next = curr;
        pq->prev = curr->prev;
        curr->prev = pq;

        if (pq->prev == NULL) {
          m->p_queue_head = pq;
        } else
          pq->prev->next = pq;

        break;
      } else if (pq->weight > curr->weight && curr->next == NULL) {
        pq->prev = curr;
        pq->next = curr->next;
        curr->next = pq;

        break;
      }

      curr = curr->next;
    }

  m->p_queue_size += 1;
}

void remove_from_priority_queue (struct monitor *m, struct priority_queue *pq) {
  if (pq->prev != NULL)
    pq->prev->next = pq->next;
  if (pq->next != NULL)
    pq->next->prev = pq->prev;

  // free(pq);

  if (pq->prev == NULL)
    m->p_queue_head = pq->next;

  m->p_queue_size -= 1;
  printf("removed node: %d\n", m->p_queue_size);
}

void init_monitor (struct monitor *new_monitor) {
  new_monitor->tree_head = create_successor();
  new_monitor->start = create_state();
  new_monitor->goal = create_state();

  new_monitor->found_result = false;
  new_monitor->result = 0;

  new_monitor->queue_size = 0;
  new_monitor->queue_cap = INIT_SIZE;
  new_monitor->queue = malloc(sizeof(struct successor) * INIT_SIZE);

  new_monitor->p_queue_head = create_priority_queue();
  new_monitor->p_queue_size = 0;

  new_monitor->states_size = 0;
  new_monitor->states_cap = INIT_SIZE;
  new_monitor->states = malloc(sizeof(struct state) * INIT_SIZE);
}

int increase_states_cap (struct monitor *m) {
  // printf(KYEL "increasing states size" RESET "\n");
  struct state *states;
  const int states_cap = m->states_cap * 2;
  states = malloc(sizeof(struct state) * states_cap);
  memset(states, 0, sizeof(struct state) * states_cap);

  for (int i = 0; i < m->states_size; i++) {
    states[i] = m->states[i];
  }

  // free(m->states);

  m->states = states;
  return states_cap;
}

void clear_states (struct monitor *m) {
  memset(m->states, 0, sizeof(struct state) * m->states_cap);
  m->states_size = 0;
}

struct state *check_monitor_states(struct monitor *m, struct state *s) {
  for (int i = 0; i < m->states_size; i++) {
    if (compare_states(&m->states[i], s)) {
      return &m->states[i];
    }
  }

  return NULL;
}
