#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <gmp.h>

#define TAM 100
#define MAX_BASES 10

// Protótipos das funções
void base_conversao_digitos(int base, char lista_digitos[]);
bool adicionarLista(int lista[], char lista_numero[], int base_entrada);
void ConverterBase10(mpz_t resultado, int lista_entrada[], int base_inicial, char lista_input[]);
void ConverterBaseFinal(mpz_t valor_base_10, int base_final, char resultado[]);

int main(void) {
    int numeroConvertidoInteiro[TAM] = {0};
    int baseEntrada;
    char numeroUsuario[TAM];
    char digitosBaseEntrada[37];
    mpz_t valor_base_10;
    int basesDesejadas[MAX_BASES];
    int quantidade_bases;
    char resultados[MAX_BASES][TAM];

    mpz_init(valor_base_10);

    printf("\n ------------------------------------------------------------\n");
    printf("| Conversor de Bases Numéricas (2 a 36) com Múltiplas Saídas |\n");
    printf(" ------------------------------------------------------------\n\n");

    // Etapa 1: Ler número e base de entrada
    printf("  Digite o número que deseja converter: ");
    fgets(numeroUsuario, TAM, stdin);
    numeroUsuario[strcspn(numeroUsuario, "\n")] = '\0';

    if (strlen(numeroUsuario) == 0) {
        printf("Erro: Nenhum número foi digitado.\n");
        mpz_clear(valor_base_10);
        return 1;
    }

    do {
        printf("  Digite a base do número (2 a 36): ");
        scanf("%d", &baseEntrada);
        while(getchar() != '\n');
    } while (baseEntrada <= 1 || baseEntrada > 36);

    base_conversao_digitos(baseEntrada, digitosBaseEntrada);

    while (!adicionarLista(numeroConvertidoInteiro, numeroUsuario, baseEntrada)) {
        printf("\n                     ||NÚMERO INVÁLIDO!||\n");
        printf("\n ------------------------------------------------------------");
        printf("\n|  Os dígitos devem estar entre (( 0 e %c )) \n", digitosBaseEntrada[baseEntrada - 1]);
        printf(" ------------------------------------------------------------\n");

        printf("\n  Digite o número que deseja converter: ");
        fgets(numeroUsuario, TAM, stdin);
        numeroUsuario[strcspn(numeroUsuario, "\n")] = '\0';

        if (strlen(numeroUsuario) == 0) {
            printf("Erro: Nenhum número foi digitado.\n");
            mpz_clear(valor_base_10);
            return 1;
        }
    }

    // Etapa 2: Ler múltiplas bases de saída
    printf("\n ------------------------------------------------------------\n");
    printf("| Informe a quantidade de bases para conversão (1-%d)        |\n", MAX_BASES);
    printf(" ------------------------------------------------------------\n");

    do {
        printf("\n  Quantidade de bases para conversão (1-%d): ", MAX_BASES);
        scanf("%d", &quantidade_bases);
        while(getchar() != '\n');
    } while (quantidade_bases < 1 || quantidade_bases > MAX_BASES);

    for (int i = 0; i < quantidade_bases; i++) {
        do {
            printf("  Informe agora a %dª base (2-36): ", i+1);
            scanf("%d", &basesDesejadas[i]);
            while(getchar() != '\n');
        } while (basesDesejadas[i] <= 1 || basesDesejadas[i] > 36);
    }

    // Conversão para base 10
    ConverterBase10(valor_base_10, numeroConvertidoInteiro, baseEntrada, numeroUsuario);

    // Conversão para cada base desejada e armazenamento na matriz
    for (int i = 0; i < quantidade_bases; i++) {
        ConverterBaseFinal(valor_base_10, basesDesejadas[i], resultados[i]);
    }

    // Exibir resultados
    printf("\n-------------------------------------------------------------\n");
    printf("| Resultados das Conversões:                                |\n");
    printf("-------------------------------------------------------------\n");
    for (int i = 0; i < quantidade_bases; i++) {
        printf("| Base %2d: %-50s |\n", basesDesejadas[i], resultados[i]);
    }
    printf("-------------------------------------------------------------\n");

    mpz_clear(valor_base_10);
    return 0;
}

void base_conversao_digitos(int base, char lista_digitos[]) {
    for (int i = 0; i < base; i++) {
        lista_digitos[i] = (i < 10) ? '0' + i : 'A' + i - 10;
    }
    lista_digitos[base] = '\0';
}

bool adicionarLista(int lista[], char lista_numero[], int base_entrada) {
    int len = strlen(lista_numero);
    for (int i = 0; i < len; i++) {
        int valor;
        if (isdigit(lista_numero[i])) {
            valor = lista_numero[i] - '0';
        } else if (isalpha(lista_numero[i])) {
            valor = toupper(lista_numero[i]) - 'A' + 10;
        } else {
            return false;
        }
        if (valor >= base_entrada) return false;
        lista[i] = valor;
    }
    return true;
}

void ConverterBase10(mpz_t resultado, int lista_entrada[], int base_inicial, char lista_input[]) {
    mpz_set_ui(resultado, 0);
    mpz_t base, potencia, temp;
    mpz_inits(base, potencia, temp, NULL);

    int len = strlen(lista_input);
    mpz_set_ui(base, base_inicial);

    for (int i = 0; i < len; i++) {
        int expoente = len - 1 - i;
        mpz_ui_pow_ui(potencia, base_inicial, expoente);
        mpz_mul_ui(temp, potencia, lista_entrada[i]);
        mpz_add(resultado, resultado, temp);
    }

    mpz_clears(base, potencia, temp, NULL);
}

void ConverterBaseFinal(mpz_t valor_base_10, int base_final, char resultado[]) {
    mpz_t valor_temp;
    mpz_init_set(valor_temp, valor_base_10);
    
    int i = 0;
    
    if (mpz_cmp_ui(valor_temp, 0) == 0) {
        resultado[0] = '0';
        resultado[1] = '\0';
        return;
    }

    while (mpz_cmp_ui(valor_temp, 0) > 0) {
        mpz_t resto;
        mpz_init(resto);
        mpz_tdiv_qr_ui(valor_temp, resto, valor_temp, base_final);
        unsigned long digito = mpz_get_ui(resto);
        
        if (digito < 10) {
            resultado[i++] = '0' + digito;
        } else {
            resultado[i++] = 'A' + digito - 10;
        }
        
        mpz_clear(resto);
    }
    
    resultado[i] = '\0';
    
    // Inverter a string
    int inicio = 0, fim = i - 1;
    while (inicio < fim) {
        char temp = resultado[inicio];
        resultado[inicio] = resultado[fim];
        resultado[fim] = temp;
        inicio++;
        fim--;
    }
    
    mpz_clear(valor_temp);
}
