//Name: Shrey Gupta
//Period: 4

#include <stdio.h>
#include <math.h>

int main() {
	//initialize variables
	FILE* fin = fopen("phrase.txt" , "r");
	char ch
		int n = 0;
	char input[10000];
	int frequency[256] = {0};

	//fill arrays
	for(int i=0; i<10000; i++) {
		input[i] = '*';
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

	//TODO: create tree

	//exit status
	return 0;
}