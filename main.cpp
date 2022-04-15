#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>
int pagina;

void paginacao(){
    printf("\nDigite o valor referente a Tela: \n1 ==== Visualizar Eventos\n2 ==== Adquirir Ingresso\n3 ==== Sair\n\n\nTela Escolhida: ");
    scanf("%d", &pagina);

    switch(pagina){
    case 1:
        printf("\Visualizar Eventos!\n");
        break;
    case 2:
        printf("\nAdquirir Ingresso!\n");
        break;
    default:
        printf("\nPágina não existente!\n");
    }

    while(pagina != 3){
        paginacao();
    };
}

int main(){
    setlocale(LC_ALL, "Portuguese");
    paginacao();

    printf("\n\n\nObrigado por utilizar nosso sistema!\n\n\n");

    return 0;
}
