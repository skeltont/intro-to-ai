default: all

all:
	gcc -std=c99 -Wall -pthread hw.c -lpthread -o hw
clean:
	rm -f hw
tar:
	tar cvjf cs331_assignment_1_skeltont.tar.bz2 hw.c makefile
run:
	./hw test_data/start${s}.txt test_data/goal${s}.txt ${m} output/${o}
