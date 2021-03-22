# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <string.h>

// Max size of static strings (without the final \0).
# define STR_BUFFER 32
// Use debug options.
# define DEBUG 0

// Struct to store a flower.
typedef struct {
    float sepal_length;
    float sepal_width;
    float petal_length;
    float petal_width;
    char name[STR_BUFFER + 1];

    float distance;

} Flower;

// Header
unsigned read_csv_file(FILE* file, Flower** flowers_read);
void print_flower_sheet (Flower* target, unsigned amount);
char* analyse_flower(Flower target, Flower* training_cases, unsigned training_cases_amount, unsigned k);
float get_distance(Flower a, Flower b);
void insertion_sort(Flower* vet, unsigned v_size);

int main (int argc, char* argv[]) {

    // Store the traning file name (STR_BUFFER + 1 is used in order to store the final \0).
    char training_file_name[STR_BUFFER + 1];
    // Store the test file name (STR_BUFFER + 1 is used in order to store the final \0).
    char test_file_name[STR_BUFFER + 1];
    // Store the neighbor amount.
    unsigned k;

    // Receives the input.
    scanf(" %s %s %u", training_file_name, test_file_name, &k);

    // Attempts to open the training file.
    FILE* training_file = NULL;
    training_file = fopen(training_file_name, "r");
    // Gives an error if file can't be opened.
    if(training_file == NULL) {
        printf("Unable to open training file (%s)!\n", training_file_name);
        return 1;
    }

    // Store the amount of flowers in the training file.
    unsigned training_flower_amount = 0;
    // Store the flowers in the training file.
    Flower* training_flowers = NULL;
    // Reads training flowers from file.
    training_flower_amount = read_csv_file(training_file, &training_flowers);
    // Gives an error if no flower is read.
    if(training_flower_amount == 0) {
        fclose(training_file);
        printf("No training flowers read!\n");
        return 1;
    }
    // Close the training file.
    fclose(training_file);


    // Attempts to open the test file.
    FILE* test_file = NULL;
    test_file = fopen(test_file_name, "r");
    // Gives an error if file can't be opened.
    if(test_file == NULL) {
        printf("Unable to open test file (%s)!\n", test_file_name);
        return 1;
    }

    // Store the amount of flowers in the training file.
    unsigned test_flower_amount = 0;
    // Store the flowers in the training file.
    Flower* test_flowers = NULL;
    // Reads training flowers from file.
    test_flower_amount = read_csv_file(test_file, &test_flowers);
    // Gives an error if no flower is read.
    if(test_flower_amount == 0) {
        fclose(training_file);
        printf("No test flowers read!\n");
        return 1;
    }
    // Close the training file.
    fclose(test_file);

    // If on debug mode prints the flowers read as a sheet.
    if(DEBUG != 0) {

        printf("> DEBUG\n");

        printf("Training Flowers:\n");
        print_flower_sheet(training_flowers, training_flower_amount);
        putchar('\n');

        printf("Test Flowers:\n");
        print_flower_sheet(test_flowers, test_flower_amount);
        putchar('\n');

        printf("DEBUG <\n");

    }

    // Verify if k is valid.
    if(k < 1 || k > training_flower_amount) {
        printf("k is invalid\n");
        return 0;
    }
    
    // Store the amount of times the algorithm got the awnser right.
    unsigned success_amount = 0;

    // Store the current flower to be analysed.
    Flower cur_target;
    // Store the current anwser.
    char* cur_anwser;

    // Tries to discover the species name of each of the training flowers.
    for(int i = 0; i < test_flower_amount; i++) {

        cur_target = test_flowers[i];
        cur_anwser = analyse_flower(cur_target, training_flowers, training_flower_amount, k);    

        // Detects if the algorithm guessed the flower species right.
        if(strcmp(cur_anwser, cur_target.name) == 0)
            success_amount++;

        // Print the guessed flower species and the real flower species.
        printf("%s %s\n", cur_anwser, cur_target.name);

    }

    // Print the success rate.
    printf("%.4f\n", (float)success_amount / (float)test_flower_amount);

    // Free the memory allocated to the flowers.
    free(training_flowers);
    free(test_flowers);

    return 0;
}

// Reads a CSV file, copies the flowers read to the given pointer and returns the 
// amount of flowers read.
unsigned read_csv_file(FILE* file, Flower** flowers_read) {

    // Store the current flower being read.
    Flower temp;
    // Store the amount of flowers read.
    unsigned flower_amount = 0;

    // Ignores the first line of the CSV file.
    char temp_char = '#';
    while(temp_char != '\n' && temp_char != '\r' && temp_char != '\0' && temp_char != EOF) {
        temp_char = fgetc(file);
    }

    // Read the entire file.
    while (fscanf(file, "%f,%f,%f,%f,\"%[^\"]\" ", 
    &temp.sepal_length, &temp.sepal_width, &temp.petal_length, &temp.petal_width, temp.name) 
    != EOF) {

        // Allocate memory for the new flower.
        flower_amount++;
        (*flowers_read) = realloc((*flowers_read), flower_amount * sizeof(Flower));
        // Gives an error if unable to allocate more memory.
        if((*flowers_read) == NULL) {
            printf("Unable to allocate memory for training flowers)!\n");
            return 0;
        }

        // Copy the flower to it's final destination.
        (*flowers_read)[flower_amount - 1] = temp;

    }

    // Return the amount of flowers read.
    return flower_amount;

}

// Analyse a flower and tries to guess it's species based on the training cases.
char* analyse_flower(Flower target, Flower* training_cases, unsigned training_cases_amount, unsigned k) {

    // Get the "distance".
    for(int i = 0; i < training_cases_amount; i++)
        training_cases[i].distance = get_distance(target, training_cases[i]);

    // Sort the vector.
    insertion_sort(training_cases, training_cases_amount);

    // Print the sorted vector if debug is on.
    if(DEBUG != 0) {
        printf("> DEBUG:\n");
        printf("Sorted vector:\n");
        for(int i = 0; i < training_cases_amount; i++) {
            printf("%d - %f (%s), ", i, training_cases[i].distance, training_cases[i].name);
        }
        putchar('\n');
        printf("DEBUG <\n");
    }

    // Used to store the type of the most similar flowers.
    // 0 = setosa
    // 1 = versicolor
    // 2 = virginica
    unsigned name_count[3] = { 0, 0, 0};

    // Count how many times each species of flower appeared between the k most similar ones.
    for(int i = 0; i < k; i++) {

        if(strcmp( training_cases[i].name,"setosa\0") == 0)
            name_count[0] ++;
        else if(strcmp( training_cases[i].name,"versicolor\0") == 0)
            name_count[1] ++;
        else
            name_count[2] ++;

    }

    // Print the most similar flowers if debug is on.
    if(DEBUG != 0) {
        printf("> DEBUG:\n");
        printf("Setosa: %u\n", name_count[0]);
        printf("Versicolor: %u\n", name_count[1]);
        printf("Virginica: %u\n", name_count[2]);        
        printf("DEBUG <\n");
    }

    // Get the type of flower that appears the most.
    int max = 0;
    for(int i = 0; i < 3; i++){
        if(name_count[i] > name_count[max])
            max = i;
    }

    // Return the name of the species that appears the most.
    if(max == 0)
        return "setosa\0";
    if(max == 1)
        return "versicolor\0";
    // else
    return "virginica\0";

}

// Print a vector of flowers like a spritesheet (Used only when debug is on).
void print_flower_sheet (Flower* target, unsigned amount) {

    for(int i = 0; i < amount; i++) {
        printf("%d - %.4f\t%.4f\t%.4f\t%.4f\t%s\n", i, 
        target[i].sepal_length, target[i].sepal_width, target[i].petal_length,
        target[i].petal_width, target[i].name);
    }

}

// Gets the "distance" between 2 flowers.
float get_distance(Flower a, Flower b) {

    float sqr_sum = 0;

    sqr_sum += pow(a.sepal_length - b.sepal_length, 2);
    sqr_sum += pow(a.sepal_width - b.sepal_width, 2);
    sqr_sum += pow(a.petal_length - b.petal_length, 2);
    sqr_sum += pow(a.petal_width - b.petal_width, 2);

    return sqrt(sqr_sum);

}

// Sort a vector.
void insertion_sort(Flower* vet, unsigned v_size) {

    int i = 1;

    while (i < v_size) {
        Flower key = vet[i];
        int j = i - 1;
        while(j >= 0 && key.distance < vet[j].distance) {
            vet[j+1] = vet[j];
            j--;
        }
        vet[j+1] = key;
        i++;
    }

}