//Name: Shrey Gupta
//Period: 4

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double myrand() {
   return(rand()%100)/100.0;
}

int main() {
	int size = 30;
	double trials = 1000.0;
	double dp = 0.001;
	srand(time(NULL));

	printf("Probability\t\tAverage Time\n");
	for(double probability=0; probability<=1; probability+=dp) {
		int totalTime = 0;
		for(int i=0; i<trials; i++) {
			//create forest
			int forest[size][size];
			for(int j=0; j<size; j++) {
				for(int k=0; k<size; k++) {
					if(myrand() < probability) {
						forest[j][k] = 1;
					} else {
						forest[j][k] = 0;
					}
				}
			}

			//begin forest fire
			int time = 0;
			int noFire = 1;
			for(int j=0; j<size; j++) {
				if(forest[j][0] == 1) {
					forest[j][0] = -1;
					noFire = 0;
				}
			}

			//run forest fire
			int burning = 1;
			while(burning) {
				burning = 0;
				for(int j=0; j<size; j++) {
					for(int k=0; k<size; k++) {
						if(forest[j][k] == -1) {
							if(j != 0) {
								if(forest[j-1][k] == 1) {
									forest[j-1][k] = -2;
									burning = 1;
								}
							}
							if(j != size-1) {
								if(forest[j+1][k] == 1) {
									forest[j+1][k] = -2;
									burning = 1;
								}
							}
							if(k != 0) {
								if(forest[j][k-1] == 1) {
									forest[j][k-1] = -2;
									burning = 1;
								}
							}
							if(k != size-1) {
								if(forest[j][k+1] == 1) {
									forest[j][k+1] = -2;
									burning = 1;
								}
							}
						}
					}
				}
				for(int j=0; j<size; j++) {
					for(int k=0; k<size; k++) {
						if(forest[j][k] == -2) {
							forest[j][k] = -1;
						}
					}
				}
				time += 1;
			}
			if(noFire) time = 0;
			totalTime += time;
		}
		printf("%G\t\t\t%G\n", probability, (totalTime/trials)/size);
	}

	//exit status
	return 0;
}