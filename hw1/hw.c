#ifndef HEADER_FILES
#define HEADER_FILES

#include "action_interface.h"

#endif

void load_test_data (const char *filename, struct state *s) {
  char buffer[100];
  int bank = 0;
  FILE* file = fopen(filename, "r");

  while (fgets(buffer, 100, file)) {
    if (bank == 0) {
      s->left->missionaries = atoi(strtok(buffer, ","));
      s->left->cannibals = atoi(strtok(NULL, ","));
      s->left->boat = atoi(strtok(NULL, ","));

      bank++;
    } else if (bank == 1) {
      s->right->missionaries = atoi(strtok(buffer, ","));
      s->right->cannibals = atoi(strtok(NULL, ","));
      s->right->boat = atoi(strtok(NULL, ","));

      break;
    }
  }
}

void bfs (struct monitor *m, int counter) {
  struct bank *boat_bank, *far_bank;
  int queue_size = m->queue_size;
  struct successor *new_queue;

  new_queue = malloc(sizeof(struct successor) * m->queue_cap);
  m->queue_size = 0;

  for (int i = 0; i < queue_size; i++) {

    if (m->queue[i].s->left->boat == 1) {
      boat_bank = m->queue[i].s->left;
      far_bank = m->queue[i].s->right;
    } else {
      boat_bank = m->queue[i].s->right;
      far_bank = m->queue[i].s->left;
    }

    for (int j = 0; j < 5; j++) {
      if (check_action(j, boat_bank, far_bank)) {
        if (take_action(m, &m->queue[i], j)) {
          counter += 1;

          debug_state(m->queue[i].children[j]->s);

          if (m->queue_size + 1 == m->queue_cap)
            m->queue_cap = increase_queue_size(new_queue, m->queue_cap);

          new_queue[m->queue_size] = *m->queue[i].children[j];
          m->queue_size += 1;

          if (compare_states(m->goal, m->queue[i].children[j]->s)) {
            printf("found solution!\n");
            m->goal_node = m->queue[i].children[j];
            m->counter = counter - 1;
          }
        }
      }
    }
  }

  m->queue = new_queue;

  if (m->queue_size > 0)
    bfs(m, counter);
  else
    return;
}

void dfs (struct monitor *m, struct successor *succ, int counter) {
  struct bank *boat_bank, *far_bank;

  if (succ->s->left->boat == 1) {
    boat_bank = succ->s->left;
    far_bank = succ->s->right;
  } else {
    boat_bank = succ->s->right;
    far_bank = succ->s->left;
  }

  for (int i = 0; i < 5; i++) {
    if (check_action(i, boat_bank, far_bank)) {
      if (take_action(m, succ, i)) {
        counter += 1;
        if (compare_states(m->goal, succ->children[i]->s)) {
          printf("found solution!\n");
          m->goal_node = succ->children[i];
          m->counter = counter - 1;
        }
        dfs(m, succ->children[i], counter);
      }
    }
  }
}

void handle_mode (struct monitor *m, const char *mode) {
  if (strncmp("bfs", mode, strlen(mode)) == 0)
    bfs(m, -1);
  else if (strncmp("dfs", mode, strlen(mode)) == 0)
    dfs(m, m->tree_head, -1);
  else if (strncmp("iddfs", mode, strlen(mode)) == 0)
    printf("iddfs called \n");
  else if (strncmp("astar", mode, strlen(mode)) == 0)
    printf("astar called \n");
  else
    printf("unknown mode requested \n");
}

int main (int argc, char const *argv[]) {
  struct monitor *m = malloc(sizeof(struct monitor));

  init_monitor(m);

  if (argc == 5) {
    load_test_data(argv[1], m->start);
    load_test_data(argv[2], m->goal);

    m->states[0] = *m->start;
    m->states_size += 1;

    m->tree_head->s = m->start;

    printf("queue_size: %d\n", m->queue_size);
    m->queue[m->queue_size] = *m->tree_head;
    m->queue_size += 1;
  } else {
    printf(KRED "Invalid number of arguments. %d provided and 4 required." RESET "\n\n"
      "Structure:\n"
      "\t`./hw <initial state file> <goal state file> <mode> <output file>` \n"
      , argc - 1);

    return 0;
  }

  handle_mode(m, argv[3]);

  if (m->counter == 0) {
    printf("no solution found\n");
  } else {
    print_successor_path(m->tree_head, m->goal_node);
    printf("completed search via %s number of expansions: %d\n", argv[3], m->counter);
  }

  return 0;
}
