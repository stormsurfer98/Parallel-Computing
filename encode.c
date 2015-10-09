//Name: Shrey Gupta
//Period: 4

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

int keys[256][16];
int items[256] = {0};

typedef struct Node {
	int symbol;
	int frequency;
	struct Node* left;
	struct Node* right;
} TreeNode;

void traverse(TreeNode* t, int key[16], int depth) {
	if((*t).symbol != -1) {
		items[(*t).symbol] = 1;
		for(int i=0; i<depth; i++) {
			keys[(*t).symbol][i] = key[i];
		}
	} else {
		if((*t).left != NULL) {
			key[depth] = 0;
			traverse((*t).left, key, depth+1);
		}
		if((*t).right != NULL) {
			key[depth] = 1;
			traverse((*t).right, key, depth+1);
		}
	}
}

int main() {
	//initialize variables
	FILE* fin = fopen("phrase.txt" , "r");
	char ch;
	int n = 0;
	char input[10000];
	int frequency[256] = {0};
	TreeNode* nodes[256];
	int count = 0;

	//fill arrays
	for(int i=0; i<10000; i++) {
		input[i] = '*';
	}
	for(int i=0; i<256; i++) {
		for(int j=0; j<16; j++) {
			keys[i][j] = -1;
		}
	}

	//read textfile
	while(1) {
		int numbytes = fread(&ch, sizeof(char), 1, fin);
		if(numbytes == 0) break;
		input[n] = ch;
		frequency[(int)ch] += 1;
		n++;
	}
	fclose(fin);

	//create nodes
	TreeNode* t = NULL;
	for(int i=0; i<256; i++) {
		if(frequency[i] != 0) {
			t = (TreeNode*)malloc(sizeof(TreeNode));
			(*t).symbol = i;
			(*t).frequency = frequency[i];
			(*t).left = NULL;
			(*t).right = NULL;
			int j = 0;
			while(1) {
				if(j == count) {
					nodes[j] = t;
					count += 1;
					break;
				} else if((*nodes[j]).frequency < frequency[i]) {
					j += 1;
				} else {
					for(int k=count; k>j; k--) {
						nodes[k] = nodes[k-1];
					}
					nodes[j] = t;
					count += 1;
					break;
				}
			}
		}
	}

	//create tree
	while(count > 1) {
		t = (TreeNode*)malloc(sizeof(TreeNode));
		(*t).symbol = -1;
		(*t).frequency = (*nodes[0]).frequency + (*nodes[1]).frequency;
		(*t).left = nodes[0];
		(*t).right = nodes[1];
		for(int i=0; i<count-2; i++) {
			nodes[i] = nodes[i+2];
			nodes[i+1] = nodes[i+3];
		}
		count -= 2;
		int j = 0;
		while(1) {
			if(j == count) {
				nodes[j] = t;
				count += 1;
				break;
			} else if((*nodes[j]).frequency < (*t).frequency) {
				j += 1;
			} else {
				for(int k=count; k>j; k--) {
					nodes[k] = nodes[k-1];
				}
				nodes[j] = t;
				count += 1;
				break;
			}
		}
	}

	//print encoded message
	int temp[16] = {0};
	traverse(nodes[0], temp, 0);
	for(int i=0; i<256; i++) {
		if(items[i] != 0) {
			printf("%c", (char)i);
			for(int j=0; j<16; j++) {
				if(keys[i][j] != -1) printf("%d", keys[i][j]);
			}
			printf("\n");
		}
	}
	for(int i=0; i<10000; i++) {
		if(input[i] != '*') {
			for(int j=0; j<16; j++) {
				if(keys[input[i]][j] != -1) printf("%d", keys[input[i]][j]);
			}
		}
	}
	printf("\n");

	//exit status
	return 0;
}