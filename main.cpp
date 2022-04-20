#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <iostream>
#include <fstream>

int pagina;
using namespace std;

void readEventsFile(){
    ifstream ip("events.csv");

    if(!ip.is_open()){
        printf("\n\n\nErro ao obter eventos dispon�veis\n\n\n");
    }else{
        string code, name, local, hours;

        printf("Codigo\t\t\tNome\t\t\tLocal\t\t\tCarga Horaria\n");

        while(ip.good()){
            getline(ip, code, ',');
            getline(ip, name, ',');
            getline(ip, local, ',');
            getline(ip, hours, '\n');
            printf("%s\t\t\t%s\t\t\t%s\t\t\t%s\n", code.c_str(), name.c_str(), local.c_str(), hours.c_str());

        };
    }

}


void paginacao(){
    printf("\nDigite o valor referente a Tela: \n1 ==== Visualizar Eventos\n2 ==== Adquirir Ingresso\n3 ==== Sair\n\n\nTela Escolhida: ");
    scanf("%d", &pagina);

    switch(pagina){
    case 1:
        readEventsFile();
        break;
    case 2:
        printf("\nAdquirir Ingresso!\n");
        break;
    case 3:
        break;
    default:
        printf("\nP�gina n�o existente!\n");
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
