//MEMGRIND.C
//sed96, sp1106

#include <ctype.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "mymalloc.h"

struct timeval begin, end;

//malloc 1 byte 3000 times, then free the 3000 1 byte pointers one by one
float a(){
	
	gettimeofday(&begin, NULL);   //start clock 
	
	char ** p[3000];	//keep track of pointers to free
	int i=0;
	int k;
	for(k = 0; k < 3000; k++){
		p[i] = malloc(sizeof(char));		//malloc 1 byte
		i++;
	}
	for(k = i-1; k>=0; k--){
		free(p[k]);		//free all bytes one by one
	}
	
	//get end time
	gettimeofday(&end, NULL); //stop clock
	return ((end.tv_sec - begin.tv_sec)*1000000 +end.tv_usec) - begin.tv_usec;
}

//malloc 1 byte and immefiatley free it 3000 times in a row
float b(){
	
	gettimeofday(&begin, NULL);   //start clock 
	
	
	int i;
	for(i =0; i < 3000; i++){
		char * c = malloc(1);	//malloc byte
		free(c);	//free byte just malloced
	}
	
	//get end time
	gettimeofday(&end, NULL); //stop clock
	return ((end.tv_sec - begin.tv_sec)*1000000 +end.tv_usec) - begin.tv_usec;
	
}

//randomly choose between a 1 byte malloc() or free() 6000 times 
float c(){
	gettimeofday(&begin, NULL);   //start clock 
	
	srand(time(NULL));
	int m = 0, memLoc = 0; 
	char ** p[3000];	//keep track of pointers to free
	while(m < 3000){
		int k = rand()%2 ;			//random number 0 or 1 to choose action 
		if(k != 0){
			memLoc++;
			m++;
			p[memLoc]= malloc(sizeof(char));		//malloc 1 byte
		}
		else {	
			if(memLoc > 0){
				free(p[memLoc]); //free pointer
				memLoc--;
			}
		}
	}
	
	while(memLoc > 0){
		free(p[memLoc]);		//free remaining pointers
		memLoc--;
	}
	
	gettimeofday(&end, NULL); //stop clock
	return ((end.tv_sec - begin.tv_sec)*1000000 +end.tv_usec) - begin.tv_usec;
	
}

//randomly choose between a random size malloc() or free() 6000 times 
float d(){
	gettimeofday(&begin, NULL);   //start clock 
	srand(time(NULL));
	int m = 0, memLoc = 0; 
	char **p[3000]; 	//keep track of pointers to free
		while(m < 3000){
			int k = rand()%2 ;   //random number 0 or 1 to choose action
			if(k != 0){
				memLoc++;
				m++;
				int s = rand()%(3000)+1;	//random size 1-3000
				p[memLoc]= malloc(s);		//malloc random sized byte
			}
			else{	
				if(memLoc > 0){
					free(p[memLoc]);	//free pointers
					memLoc--;
				}
				
			}
		}	
		while(memLoc > 0){
			free(p[memLoc]);	//free remaining pointers
			memLoc--;
		}
	gettimeofday(&end, NULL); //stop clock
	return ((end.tv_sec - begin.tv_sec)*1000000 +end.tv_usec) - begin.tv_usec;
	
}

//randomly malloc() 1 byte or random sized byte 3000 times or free()
float e(){
	
	gettimeofday(&begin, NULL);   //start clock 
	int k;
	int m=0, memLoc=0;
	char **p[300]; //keep track of pointers to free
	for(m=0; m < 300; m++){
		k= rand()%(4)+1;	//random number 1 - 4 to choose action
		if(k == 1){
			memLoc++;
			p[memLoc] = malloc(sizeof(char));		//malloc 1 byte
			continue;
		}else if(k == 2){
			memLoc++;
			int s = rand()%(300)+1;	//random size 1-3000
			p[memLoc]= malloc(s);		//malloc random sized byte			
			continue;
		}else{
			if (memLoc>0){
				free(p[memLoc]);	//free pointers
				memLoc--;
				m--;
			}
		}
	}
	
	while(memLoc > 0){
		free(p[memLoc]);		//free remaining pointers
		memLoc--;
	}
	
	gettimeofday(&end, NULL); //stop clock
	return ((end.tv_sec - begin.tv_sec)*1000000 +end.tv_usec) - begin.tv_usec;
	
}

//malloc 1 byte a random number of times then free all bytes, then malloc 3000 - random number of bytes and free them 
float f(){
	struct timeval begin, end;
	gettimeofday(&begin, NULL);   //start clock 
	
	srand(time(NULL));
	char **p[300];  //keep track of pointers to free
	int i=0, k;
		int r = rand()%(300)+1;   //random  size 500 to 1000 bytes 
		
		//for r number of mallocs 
		for(k = 0; k < r; k++){
			p[i] = malloc(sizeof(char));
				i++;
		}
		for(k = i-1; k>=0; k--){  //free pointers
			free(p[k]);
		}
		
		//for 3000-r number of mallocs
		i=0;
		for(k = 0; k < 300-r; k++){
			p[i] = malloc(sizeof(char));
				i++;
		}
		for(k = i-1; k>=0; k--){  //free pointers
			free(p[k]);
		}
		
	gettimeofday(&end, NULL); //stop clock
	return ((end.tv_sec - begin.tv_sec)*1000000 +end.tv_usec) - begin.tv_usec;
	
}




int main(){
	
	printf("MEMGRIND AVG RUNTIME A thru F:\n");
	
	int i = 0; 
	double avgA, avgB, avgC, avgD, avgE, avgF;
	
	//TEST A
	avgA = 0; 
	printf("TESTCASE A \n");
	for(i = 0; i < 100; i++){
		printf("RUN %d: \n", i);
		avgA += a();
	}
	
	
	//TEST B
	avgB = 0;
	printf("TESTCASE B \n");
	for(i = 0; i < 100; i++){
		avgB += b();
	}
	
	
	//TEST C
	avgC = 0;
	printf("TESTCASE C \n");
	for(i = 0; i < 100; i++){
		avgC += c();
	}
	
	//TEST D
	avgD = 0;
	printf("TESTCASE D \n");
	for(i = 0; i < 100; i++){
		printf("RUN %d: \n", i);
		avgD += d();
	}
	
		
	//TEST E
	avgE = 0;
	printf("TESTCASE E \n");
	for(i = 0; i < 100; i++){
		avgE += e();
	}
	
	
	//TEST F
	avgF = 0;
	printf("TESTCASE F \n");
	for(i = 0; i < 100; i++){
		avgF += f();
	}
	
	
	printf( "Test A: %g microseconds\n", avgA/100);
	printf( "Test B: %g microseconds\n", avgB/100);
	printf( "Test C: %g microseconds\n", avgC/100);
	printf( "Test D: %g microseconds\n", avgD/100);
	printf( "Test E: %g microseconds\n", avgE/100);
	printf( "Test F: %g microseconds\n", avgF/100);
	
	printf("END TEST\n\n");
	return 0;
	
}

