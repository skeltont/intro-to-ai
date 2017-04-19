default: all

all:
	gcc -std=c99 -Wall -pthread hw.c -lpthread -o hw
clean:
	rm -f hw
tar:
	tar cvjf cs331_assignment_1_skeltont.tar.bz2 hw.c makefile
test1dfs:
	./hw test_data/start1.txt test_data/goal1.txt dfs output.txt
test1bfs:
	./hw test_data/start1.txt test_data/goal1.txt bfs output.txt
test2dfs:
	./hw test_data/start2.txt test_data/goal2.txt dfs output.txt
test2bfs:
	./hw test_data/start2.txt test_data/goal2.txt bfs output.txt
test3dfs:
	./hw test_data/start3.txt test_data/goal3.txt dfs output.txt
test3bfs:
	./hw test_data/start3.txt test_data/goal3.txt bfs output.txt
