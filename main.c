#include <stdbool.h>
#include <stdio.h>   // Standard input and output
#include <string.h>  // String manipulation
#include <ctype.h>   // Character verification and conversion
#include <math.h>    // Math functions (e.g., pow)
#include <stdlib.h>  // Helper functions (e.g., atoi)

#define TAM 100  // Maximum input buffer size

// Function declarations
void base_conversao_digitos(int base, char lista_digitos[]);
bool adicionarLista(int lista[], char lista_numero[], int base_entrada);
long long int ConverterBase10(int lista_entrada[], int base_inicial, char lista_input[]);
void ConverterBaseFinal(long long int valor_base_10, int base_final);

int main(void)
{
    int numeroConvertidoInteiro[TAM]; // Array that stores the converted digits as integers
    int baseEntrada;
    char numeroUsuario[TAM];         // String with the number entered by the user
    int baseDesejada;
    char digitosBaseDesejada[37];    // List of valid characters for the target base
    char digitosBaseEntrada[37];     // List of valid characters for the source base

    // Input of the number to be converted
    printf("\n ------------------------------------------------------------\n");
    printf("| Enter the Number and the Base for conversion \n");
    printf(" ------------------------------------------------------------\n\n");

    printf("  Enter the number to convert: ");
    fgets(numeroUsuario, TAM, stdin);
    numeroUsuario[strcspn(numeroUsuario, "\n")] = 0;  // Removes newline character from the string

    // Input and validation of the source base
    do
    {
        printf("  Enter the base of the number (between 2 and 36): ");
        scanf("%d", &baseEntrada);
        while(getchar() != '\n'); // Clears input buffer
    }
    while (baseEntrada <= 1 || baseEntrada > 36); // Base validation

    base_conversao_digitos(baseEntrada, digitosBaseEntrada); // Fills in the valid digits for the source base

    // Checks if the number's characters are valid for the specified base
    while (!adicionarLista(numeroConvertidoInteiro, numeroUsuario, baseEntrada))
    {
        printf("\n                     ||INVALID NUMBER!||\n");
        printf("\n ------------------------------------------------------------");
        printf("\n|  Digits must be between (( 0 and %c )) \n", digitosBaseEntrada[baseEntrada - 1]);
        printf(" ------------------------------------------------------------\n");

        printf("\n  Enter the number to convert: ");
        fgets(numeroUsuario, TAM, stdin);
        numeroUsuario[strcspn(numeroUsuario, "\n")] = 0;
    }

    // Input and validation of the target base
    printf("\n ------------------------------------------------------------\n");
    printf("| Now enter the base to convert the number to \n");
    printf(" ------------------------------------------------------------\n");

    do
    {
        printf("\n  Enter the target base (between 2 and 36): ");
        scanf("%d", &baseDesejada);
        while(getchar() != '\n');
    }
    while (baseDesejada <= 1 || baseDesejada > 36);

    base_conversao_digitos(baseDesejada, digitosBaseDesejada); // Fills in the valid digits for the target base

    // Converts the number from the original base to base 10
    long long int valor_base_10 = ConverterBase10(numeroConvertidoInteiro, baseEntrada, numeroUsuario);

    // Converts from base 10 to the final base
    printf("\n-------------------------------------------------------------\n");
    printf("| Conversion Result in base %i:  ", baseDesejada);
    ConverterBaseFinal(valor_base_10, baseDesejada);
    printf("\n-------------------------------------------------------------\n");
}

// Generates valid characters for a given base (e.g., 0-9, A-Z)
void base_conversao_digitos(int base, char lista_digitos[])
{
    for (int i = 0; i < base; i++)
    {
        if (i < 10)
        {
            lista_digitos[i] = '0' + i;
        }
        else
        {
            lista_digitos[i] = 'A' + i - 10;
        }
    }
    lista_digitos[base] = '\0'; // Null-terminates the string
}

// Converts each character of the entered number to its numeric value (in base 10) and validates
bool adicionarLista(int lista[], char lista_numero[], int base_entrada)
{
    int len = strlen(lista_numero);
    for (int i = 0; i < len; i++)
    {
        int valor;
        if (isdigit(lista_numero[i]))
        {
            valor = lista_numero[i] - '0';
        }
        else if (isalpha(lista_numero[i]))
        {
            valor = toupper(lista_numero[i]) - 'A' + 10;
        }
        else
        {
            return false; // Invalid characters
        }
        if (valor >= base_entrada)
        {
            return false; // Digit out of range for the base
        }
        lista[i] = valor;
    }
    return true;
}

// Converts a number from any base to base 10
long long int ConverterBase10(int lista_entrada[], int base_inicial, char lista_input[])
{
    long long int valor = 0;
    int potencia = strlen(lista_input);

    for(int i = 0; i < potencia; i++)
    {
        valor = valor + (long long int)lista_entrada[i] * (long long int)pow(base_inicial, potencia - 1 - i);
    }
    return valor;
}

// Recursively converts a number from base 10 to the target base
void ConverterBaseFinal(long long int valor_base_10, int base_final)
{
    long long int valor = -1;

    if(valor_base_10 > 0)
    {
        valor = valor_base_10 % base_final;
        ConverterBaseFinal(valor_base_10 / base_final, base_final); // Recursive call
    }

    if(valor >= 0 && valor < 10)
    {
        printf("%i", (int)valor); // Digits from 0 to 9
    }
    else if(valor >= 10)
    {
        printf("%c", (char)(valor + 'A' - 10)); // Digits from A to Z
    }
}
