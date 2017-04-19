#include "struct_interface.h"

/*
    Action: Send a missionary alone.
    This requires that the bank they are leaving will not become under-populated with
    missionaries because then they'll be eaten.
*/
bool check_action_0 (struct state *s, struct bank *boat_bank, struct bank *far_bank) {
  if (boat_bank->missionaries > 0) {
    if ((boat_bank->missionaries - 1 >= boat_bank->cannibals || boat_bank->missionaries - 1 == 0)
        && far_bank->missionaries + 1 >= far_bank->cannibals) {
      return true;
    }
  }
  return false;
}

/*
    Action: Send two missionaries.
    This requires that the bank they are leaving will not become under-populated with
    missionaries because then they'll be eaten.
*/
bool check_action_1 (struct state *s, struct bank *boat_bank, struct bank *far_bank) {
  if (boat_bank->missionaries > 1) {
    if (boat_bank->missionaries - 2 >= boat_bank->cannibals || boat_bank->missionaries - 2 == 0) {
      return true;
    }
  }
  return false;
}

/*
    Action: Send a cannibal alone.
    This requires that the bank they're arriving at won't become over-populated with
    cannibals, because then they'll eat the missionaries.
*/
bool check_action_2 (struct state *s, struct bank *boat_bank, struct bank *far_bank) {
  if (boat_bank->cannibals > 0) {
    if (far_bank->cannibals + 1 <= far_bank->missionaries || far_bank->missionaries == 0) {
      return true;
    }
  }
  return false;
}

/*
    Action: Send a cannibal and a missionary.
    This is fine because no side can get into imbalance. We just have to make sure
    that we can have the missionaries and cannibals available for taking
*/
bool check_action_3 (struct state *s, struct bank *boat_bank, struct bank *far_bank) {
  if (boat_bank->missionaries > 0 && boat_bank->cannibals > 0 && boat_bank->missionaries == boat_bank->cannibals) {
    return true;
  }
  return false;
}

/*
    Action: Send two cannibals.
    We have to REALLY make sure that they won't put the other bank into imbalance.
*/
bool check_action_4 (struct state *s, struct bank *boat_bank, struct bank *far_bank) {
  if (boat_bank->cannibals > 1) {
    if (far_bank->cannibals + 2 <= far_bank->missionaries || far_bank->missionaries == 0) {
      return true;
    }
  }
  return false;
}

bool check_action (struct state *s, int action, struct bank *boat_bank, struct bank *far_bank) {
  bool result = false;

  switch (action) {
    case 0:
      result = check_action_0(s, boat_bank, far_bank);
      break;
    case 1:
      result = check_action_1(s, boat_bank, far_bank);
      break;
    case 2:
      result = check_action_2(s, boat_bank, far_bank);
      break;
    case 3:
      result = check_action_3(s, boat_bank, far_bank);
      break;
    case 4:
      result = check_action_4(s, boat_bank, far_bank);
      break;
  }

  return result;
}

void take_action_0 (struct bank *boat_bank, struct bank *far_bank) {
  boat_bank->missionaries -= 1;
  boat_bank->boat = 0;
  far_bank->missionaries += 1;
  far_bank->boat = 1;
}

void take_action_1 (struct bank *boat_bank, struct bank *far_bank) {
  boat_bank->missionaries -= 2;
  boat_bank->boat = 0;
  far_bank->missionaries += 2;
  far_bank->boat = 1;
}

void take_action_2 (struct bank *boat_bank, struct bank *far_bank) {
  boat_bank->cannibals -= 1;
  boat_bank->boat = 0;
  far_bank->cannibals += 1;
  far_bank->boat = 1;
}

void take_action_3 (struct bank *boat_bank, struct bank *far_bank) {
  boat_bank->missionaries -= 1;
  boat_bank->cannibals -= 1;
  boat_bank->boat = 0;
  far_bank->missionaries += 1;
  far_bank->cannibals += 1;
  far_bank->boat = 1;
}

void take_action_4 (struct bank *boat_bank, struct bank *far_bank) {
  boat_bank->cannibals -= 2;
  boat_bank->boat = 0;
  far_bank->cannibals += 2;
  far_bank->boat =1;
}

bool take_action (struct monitor *m, struct successor *succ, int action) {
  struct state *s;
  struct bank *boat_bank, *far_bank;
  char *message;

  succ->children[action] = create_successor();
  succ->children[action]->s = create_state();
  copy_state(succ->s, succ->children[action]->s);

  if (succ->children[action]->s->left->boat == 1) {
    boat_bank = succ->children[action]->s->left;
    far_bank = succ->children[action]->s->right;
  } else {
    boat_bank = succ->children[action]->s->right;
    far_bank = succ->children[action]->s->left;
  }

  switch (action) {
    case 0:
      message = "0: sending one missionary";
      take_action_0(boat_bank, far_bank);
      break;
    case 1:
      message = "1: sending two missionaries";
      take_action_1(boat_bank, far_bank);
      break;
    case 2:
      message = "2: sending one cannibal";
      take_action_2(boat_bank, far_bank);
      break;
    case 3:
      message = "3: sending both";
      take_action_3(boat_bank, far_bank);
      break;
    case 4:
      message = "4: sending two cannibals";
      take_action_4(boat_bank, far_bank);
      break;
  }


  s = check_monitor_states(m, succ->children[action]->s);
  if (s == NULL) {
    // printf(KYEL "%s" RESET "\n", message);
    succ->children[action]->parent = succ;
    m->states[m->states_size] = *succ->children[action]->s;
    m->states_size += 1;

    return true;
  } else {
    // printf(KRED "deadend: %s" RESET "\n", message);
    // succ->children[action]->parent = succ;
    succ->children[action]->s = s;

    return false;
  }
}
