default: all

all:
	gcc -std=c99 -Wall -pthread hw.c -lpthread -o hw
clean:
	rm -f hw
tar:
	tar cvjf cs331_assignment_1_skeltont.tar.bz2 hw.c makefile
test1dfs:
	./hw start1.txt goal1.txt dfs output.txt
test1bfs:
	./hw start1.txt goal1.txt bfs output.txt
test2dfs:
	./hw start2.txt goal2.txt dfs output.txt
test2bfs:
	./hw start2.txt goal2.txt bfs output.txt
test3dfs:
	./hw start3.txt goal3.txt dfs output.txt
test3bfs:
	./hw start3.txt goal3.txt bfs output.txt
