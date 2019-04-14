
CC = g++
CFLAG = -g -c

bplustree : BPlusTreeNode.o BPlusTree.o main.o
	$(CC) -o bplustree BPlusTreeNode.o BPlusTree.o main.o

main.o : main.cpp
	$(CC) $(CFLAG) main.cpp

BPlusTree.o : BPlusTree.cpp
	$(CC) $(CFLAG) BPlusTree.cpp

BPlusTreeNode.o : BPlusTreeNode.cpp
	$(CC) $(CFLAG) BPlusTreeNode.cpp

clean: 
	rm -f *.o
	rm -f bplustree