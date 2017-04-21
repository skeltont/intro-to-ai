# Missionaries and Cannibals
[assignment description](http://classes.engr.oregonstate.edu/eecs/spring2017/cs331/assignments/programming1/programming1.html)

* Build
  * `make`
* Run
  * `make run s=1 m=dfs o=output.txt` or
  * `./hw test_data/start1.txt test_data/goal1.txt dfs output.txt`
* Clean
  * `make clean`


# Results
_Results are presented as < Path Length / # Nodes expanded >_

| Algorithm | Data Set #1 | Data Set #2 | Data Set #3 |
| --------- | :---------: | :--------:  | :--------:  |
| DFS       | 11/13       | 43/54       | 557/558     |
| BFS       | 11/14       | 33/95       | 337/7318    |
| IDDFS     | 11/13       | 41/54       | 555/557     |
| A*        | 11/14       | 33/59       | 337/959     |
