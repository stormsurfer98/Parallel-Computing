//Name: Shrey Gupta
//Period: 4

#include <stdio.h>
#include <math.h>

int main() {
	//initialize variables
	FILE* fin = fopen("huffman.txt" , "r");
	char ch;
	int n = 0;
	int huffman = 0;
	int ascii = 0;
	char input[1000000];
	char tree[1000000];
	int frequency[256] = {0};

	//fill arrays
	for(int i=0; i<1000000; i++) {
		input[i] = '*';
		tree[i] = '*';
	}

	//read textfile
	while(1) {
		int numbytes = fread(&ch, sizeof(char), 1, fin);
		if(numbytes == 0) break;
		input[n] = ch;
		n++;
	}
	fclose(fin);

	//create tree
	n = 0;
	for(int i=0; i<62; i++) {
		int position = 1;
		char code = input[n];
		n++;
		while(1) {
			char curr = input[n];
			n++;
			if(curr == '\n') break;
			else if(curr == '0') position = position * 2;
			else if(curr == '1') position = position * 2 + 1;
		}
		tree[position] = code;
	}

	//decode message
	while(1) {
		int position = 1;
		char curr = input[n];
		if(curr == '*') break;
		while(1) {
			char curr = input[n];
			n++;
			huffman++;
			if(curr == '0') position = position * 2;
			else if(curr == '1') position = position * 2 + 1;
			if(tree[position] != '*') break;
		}
		ascii += 8;
		frequency[(int)tree[position]] += 1;
		printf("%c", tree[position]);
	}

	//calculate Shannon entropy
	double shannon = 0.0;
	for(int i=0; i<256; i++) {
		if(frequency[i] > 0) shannon -= ((double)frequency[i]) * log(((double)frequency[i])/((double)(ascii/8)))/log(2);
	}

	//print statistics
	printf("\n\n");
	printf("ASCII bits: %d\n", ascii);
	printf("Huffman bits: %d\n", huffman);
	printf("Shannon entropy (average minimum bits): %G\n", shannon);
	printf("ASCII/Huffman ratio: %G\n", ((double)ascii-(double)huffman)/(double)ascii);

	//exit status
	return 0;
}