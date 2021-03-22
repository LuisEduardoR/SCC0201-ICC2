# include <stdio.h>
# include <stdlib.h>

// Header
unsigned char* createBuffer(unsigned char** _buf, unsigned _size);
void freeBuffer(unsigned char* _buf);

int main (int argc, char* argv[]) {

    // Cria um buffer com 512 bytes para guardar os bytes lidos.
    unsigned char* buffer = NULL;
    createBuffer(&buffer, 512);

    // Gera um erro caso o buffer não possa ser criado.
    if(buffer == NULL) {
        fprintf(stderr, "Failed to create buffer with %ld bytes of memory!\n", 512 * sizeof(unsigned char));
        return -1;
    }

    // Quantas imagens foram encontradas.
    int pictures_found = 0;

    // Guarda o caminho para o arquivo atual.
    FILE* current_file = NULL;
    // Guarda o nome do arquivo atual.
    char file_name[16];

    // Usado para guardar quantos caracteres/bytes foram lidos
    unsigned characters_read;

    do {
        // Tenta ler 512 caracteres/bytes da imagem.
        characters_read = fread(buffer, sizeof(unsigned char), 512, stdin);

        // Impede uma possível execução extra quando o arquivo acaba.
        if(characters_read != 512)
            continue;

        // Se o padrão dos caracteres/bytes lidos for compativel com um .jpg cria um arquivo para escreve-los. Caso um arquivo já esteja 
        // sendo criado, fecha-o e cria um novo.
        if(buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && buffer[3] >= 0xe * 16) {

            pictures_found++; // Aumenta o número de imagens encontradas.

            if(current_file != NULL) // Se já estiver escrevendo um arquivo, fecha-o e inicia um novo.
                fclose(current_file);
            
            // Gera o nome do novo arquivo e o cria.
            sprintf(file_name, "%.3d.jpg", pictures_found - 1);
            current_file = fopen(file_name, "w+b");

        }
        
        // Escreve o buffer encontrado para o arquivo atual caso uma imagem já tenha sido encontrada.
        if(current_file != NULL)
            fwrite(buffer, sizeof(unsigned char), 512, current_file);

    } while (characters_read == 512); // Repete o loop enquanto ainda ouverem bytes a serem lidos.

    if(current_file != NULL) // Fecha um arqquivo que possa ter ficado aberto ao final.
        fclose(current_file);

    // Avisa caso nenhuma imagem tenha sido encontrada.
    if(pictures_found == 0)
        printf("Could not find pictures\n");

    freeBuffer(buffer);

    return 0;
}

// Cria um buffer para guarda os bytes lidos.
unsigned char* createBuffer(unsigned char** _buf, unsigned _size) {

    *_buf = (unsigned char*)malloc(_size * sizeof(unsigned char));

}

// Libera a memória do buffer.
void freeBuffer(unsigned char* _buf) {

    free(_buf);

}