#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DEBUG 1
#define DEBUG_VALUES 0
#define OUT_FILE_NAME "tempo.dat"

void allocVectors(int*** vect, int*** larger, int v_start, int v_end, int v_step);

int** freeVectors(int*** vect, int*** larger, int v_start, int v_end, int v_step);

int main (int argc, char* argv[]) {

	if (argc < 2) {
		printf("Uso: ./maiores <I> <F> <S>\nI = tamanho inicial do vetor\nF = tamanho final do vetor\nS = passo entre os tamanhos\n");
		return 1;
	}
 
	int start = atoi(argv[1]);
	int end = atoi(argv[2]);
	int step = atoi(argv[3]);

	if(start < 1) {
		printf("Atencao: <I> deve ser maior do que 0!\n");
		printf("Uso: ./maiores <I> <F> <P>\nI = tamanho inicial do vetor\nF = tamanho final do vetor\nS = passo entre os tamanhos");
		return 2;
	}

	if(end < start) {
		printf("Atencao: <F> deve ser maior do que <I>!\n");
		printf("Uso: ./maiores <I> <F> <P>\nI = tamanho inicial do vetor\nF = tamanho final do vetor\nS = passo entre os tamanhos");
		return 3;
	}

	if(step < 1) {
		printf("Atencao: <P> deve ser maior do que 0!\n");
		printf("Uso: ./maiores <I> <F> <S>\nI = tamanho inicial do vetor\nF = tamanho final do vetor\nS = passo entre os tamanhos");
		return 4;
	}

	int amount = 1 + ((end - start) / step);

	int** vector = NULL;
	int** larger = NULL;

	FILE* output_file = NULL;
	output_file = fopen(OUT_FILE_NAME, "w+");

	allocVectors(&vector, &larger, start, end, step);
	if(DEBUG) printf("\n");

	clock_t c1, c2;

	for(int i = 0; i < amount; i++) {

		c1 = clock();

		int cur_size = start + (i * step);
		if(DEBUG) printf("(CALC) Vetor atual: %d (Tamanho = %d)\n", i, cur_size);
		for(int j = 0; j < cur_size; j++) {

			int val = vector[i][j];
			for(int k = 0; k < cur_size; k++) {			

				if(vector[i][k] > vector[i][j])
					larger[i][j]++;
					
				
			}
			if(DEBUG && DEBUG_VALUES && j < cur_size) printf("Valor atual: %d - Maiores: %d\n", val, larger[i][j]);
		}
		
		c2 = clock();
		double time_to_calculate = (double) (c2 - c1) / CLOCKS_PER_SEC;
		if(DEBUG) printf("Tempo necessario para calcular: %0.6lf\n\n", time_to_calculate);

		fprintf(output_file, "Tamanho: %d => Tempo: %lf\n", cur_size, time_to_calculate);

	}

	fclose(output_file);

	freeVectors(&vector, &larger, start, end, step);
}

void allocVectors (int*** vect, int*** larger, int v_start, int v_end, int v_step) {

	int amount = 1 + ((v_end - v_start) / v_step);

	(*vect) = (int**) malloc(amount * sizeof(int*));
	(*larger) = (int**) malloc(amount * sizeof(int*));

	for(int i = 0; i < amount; i++) {

		int cur_size = v_start + (i * v_step);

		(*vect)[i] = (int*) malloc(cur_size * sizeof(int));
		(*larger)[i] = (int*) malloc(cur_size * sizeof(int));

		if(DEBUG) printf("(ALLOC) Vetor atual: %d (Tamanhp = %d)", i, cur_size);
		if(DEBUG_VALUES) printf("- Valores: ");

		for(int j = 0; j < cur_size; j++){
			
			(*vect)[i][j] = rand() % (int) ((v_end / 2.0) + 1);
			if(DEBUG && DEBUG_VALUES) printf("%d ",(*vect)[i][j]);
		}
		if(DEBUG) printf("\n");

	}

}

int** freeVectors(int*** vect, int*** larger, int v_start, int v_end, int v_step) {

	int amount = 1 + ((v_end - v_start) / v_step);

	for(int i = 0; i < amount; i++) {
		free((*vect)[i]);
		free((*larger)[i]);
	}
	free(*vect);
	free(*larger);

}
