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

int dfs (struct monitor *m, struct successor *succ, int counter) {
  struct bank *boat_bank, *far_bank;

  if (succ->s->left->boat == 1) {
    boat_bank = succ->s->left;
    far_bank = succ->s->right;
  } else {
    boat_bank = succ->s->right;
    far_bank = succ->s->left;
  }

  for (int i = 0; i < 5; i++) {
    if (check_action(succ->s, i, boat_bank, far_bank)) {
      counter += 1;

      if (take_action(m, succ, i)) {
        if (compare_states(m->goal, succ->children[i]->s)) {
          printf("found solution!\n");
          m->goal_node = succ->children[i];
          m->counter = counter - 1; // don't count the final iteration that gets the goal node
        }
        dfs(m, succ->children[i], counter);
      }
    }
  }
  return 0;
}

void generate_successors (struct monitor *m, const char *mode) {
  if (strncmp("bfs", mode, strlen(mode)) == 0)
    printf("bfs called \n");
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
  } else {
    printf(KRED "Invalid number of arguments. %d provided and 4 required." RESET "\n\n"
      "Structure:\n"
      "\t`./hw <initial state file> <goal state file> <mode> <output file>` \n"
      , argc - 1);

    return 0;
  }

  generate_successors(m, argv[3]);


  if (m->counter == 0) {
    printf("no solution found\n");
  } else {
    printf("completed search via %s number of expansions: %d\n", argv[3], m->counter);
    print_successor_path(m->tree_head, m->goal_node);
  }

  return 0;
}
