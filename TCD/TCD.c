#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_EVENTOS 100
#define FILENAME "eventos.txt"

typedef struct {
    char nome[50];
    float teor_alcoolico;
    float valor_unitario;
} Bebida;

typedef struct {
    int id;
    char nome[100];
    char local[100];
    char data[11];
    float custo_org;
    int max_ingressos;
    float valor_entrada_basica;
    float valor_entrada_normal;
    float valor_entrada_especial;
    int ingressos_vendidos_basica;
    int ingressos_vendidos_normal;
    int ingressos_vendidos_especial;
    int open_bar;
    Bebida bebidas[3];
} Evento;

Evento eventos[MAX_EVENTOS];
int total_eventos = 0;

int evento_existe(int id) {
    for (int i = 0; i < total_eventos; i++) {
        if (eventos[i].id == id) {
            return 1; 
        }
    }
    return 0; 
}


void carregar_eventos()
{
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL)
    {
        printf("Nenhum dado salvo encontrado.\n");
        return;
    }

    while (1)
    {
        Evento *evento = &eventos[total_eventos];
        if (fscanf(file, "%d\n", &evento->id) == EOF)
            break;

        fgets(evento->nome, sizeof(evento->nome), file);
        evento->nome[strcspn(evento->nome, "\n")] = 0;

        fgets(evento->local, sizeof(evento->local), file);
        evento->local[strcspn(evento->local, "\n")] = 0;

        fgets(evento->data, sizeof(evento->data), file);
        evento->data[strcspn(evento->data, "\n")] = 0;

        fscanf(file, "%f\n", &evento->custo_org);
        fscanf(file, "%d\n", &evento->max_ingressos);
        fscanf(file, "%f\n", &evento->valor_entrada_basica);
        fscanf(file, "%f\n", &evento->valor_entrada_normal);
        fscanf(file, "%f\n", &evento->valor_entrada_especial);
        fscanf(file, "%d\n", &evento->ingressos_vendidos_basica);
        fscanf(file, "%d\n", &evento->ingressos_vendidos_normal);
        fscanf(file, "%d\n", &evento->ingressos_vendidos_especial);
        fscanf(file, "%d\n", &evento->open_bar);

        if (evento->open_bar)
        {
            for (int j = 0; j < 3; j++)
            {
                fgets(evento->bebidas[j].nome, sizeof(evento->bebidas[j].nome), file);
                evento->bebidas[j].nome[strcspn(evento->bebidas[j].nome, "\n")] = 0;
                fscanf(file, "%f\n", &evento->bebidas[j].teor_alcoolico);
                fscanf(file, "%f\n", &evento->bebidas[j].valor_unitario);
            }
        }
        total_eventos++;
    }
    fclose(file);
    printf("Dados carregados\n");
}

int main() {
     int opcao;
    FILE *file = fopen(FILENAME, "r");
    carregar_eventos();
   
    do {
        printf("\n1. Cadastrar evento\n2. Pesquisar evento\n3. Atualizar vendas\n4. Calcular lucro\n0. Sair\nEscolha uma opcao: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            Evento evento;
            printf("Digite o identificador do evento ");
            scanf("%d", &evento.id);
            }
            if (evento_existe(evento.id)) {
            printf("Evento já cadastrado!\n");
            continue;
            }
            printf("Nome do evento: ");
            scanf(" %[^\n]", evento.nome);

            printf("Local: ");
            scanf(" %[^\n]", evento.local);

            printf("Data (DD/MM/YYYY): ");
            scanf(" %[^\n]", evento.data);

            printf("Custo de organizacao: ");
            scanf("%f", &evento.custo_org);

            printf("Quantidade maxima de ingressos: ");
            scanf("%d", &evento.max_ingressos);

            evento.valor_entrada_basica = evento.custo_org / evento.max_ingressos;

            char open_bar_input;
            printf("Evento é open bar s/n? ");
            scanf(" %c", &open_bar_input);

            evento.open_bar = (tolower(open_bar_input) == 's') ? 1 : 0;

            if (evento.open_bar) {
                for (int i = 0; i < 3; i++) {
                    printf("Nome da bebida %d: ", i + 1);
                    scanf(" %[^\n]", evento.bebidas[i].nome);
                    printf("Teor alcoolico da bebida %d: ", i + 1);
                    scanf("%f", &evento.bebidas[i].teor_alcoolico);
                    printf("Valor da bebida %d: ", i + 1);
                    scanf("%f", &evento.bebidas[i].valor_unitario);
                    evento.valor_entrada_basica += 1.75 * evento.bebidas[i].valor_unitario;
                }
            }

            evento.valor_entrada_normal = evento.valor_entrada_basica * 1.10;
            evento.valor_entrada_especial = evento.valor_entrada_normal * 1.15;

            evento.ingressos_vendidos_basica = 0;
            evento.ingressos_vendidos_normal = 0;
            evento.ingressos_vendidos_especial = 0;

            eventos[total_eventos++] = evento;
            printf("Evento cadastrado\n");

            file = fopen(FILENAME, "w");
            if (file != NULL) {
                for (int i = 0; i < total_eventos; i++) {
                    Evento eventoAtual = eventos[i];
                    fprintf(file, "%d\n", eventoAtual.id);
                    fprintf(file, "%s\n", eventoAtual.nome);
                    fprintf(file, "%s\n", eventoAtual.local);
                    fprintf(file, "%s\n", eventoAtual.data);
                    fprintf(file, "%.2f\n", eventoAtual.custo_org);
                    fprintf(file, "%d\n", eventoAtual.max_ingressos);
                    fprintf(file, "%.2f\n", eventoAtual.valor_entrada_basica);
                    fprintf(file, "%.2f\n", eventoAtual.valor_entrada_normal);
                    fprintf(file, "%.2f\n", eventoAtual.valor_entrada_especial);
                    fprintf(file, "%d\n", eventoAtual.ingressos_vendidos_basica);
                    fprintf(file, "%d\n", eventoAtual.ingressos_vendidos_normal);
                    fprintf(file, "%d\n", eventoAtual.ingressos_vendidos_especial);
                    fprintf(file, "%d\n", eventoAtual.open_bar);

                    if (eventoAtual.open_bar) {
                        for (int j = 0; j < 3; j++) {
                            fprintf(file, "%s\n", eventoAtual.bebidas[j].nome);
                            fprintf(file, "%.2f\n", eventoAtual.bebidas[j].teor_alcoolico);
                            fprintf(file, "%.2f\n", eventoAtual.bebidas[j].valor_unitario);
                        }
                    }
                }
                fclose(file);
                printf("Dado salvo\n");
            } else {
                printf("Erro ao abrir o arquivo para escrita.\n");
            }
        }

        else if (opcao == 2) {
            int id;
            printf("Digite o identificador do evento: ");
            scanf("%d", &id);

            int encontrado = 0;
            for (int i = 0; i < total_eventos; i++) {
                if (eventos[i].id == id) {
                    printf("Nome: %s\n", eventos[i].nome);
                    printf("Local: %s\n", eventos[i].local);
                    printf("Data: %s\n", eventos[i].data);
                    printf("Custo de organizacao: %.2f\n", eventos[i].custo_org);
                    printf("Capacidade: %d\n", eventos[i].max_ingressos);
                    printf("Valor entrada basica: %.2f\n", eventos[i].valor_entrada_basica);
                    printf("Valor entrada normal: %.2f\n", eventos[i].valor_entrada_normal);
                    printf("Valor entrada especial: %.2f\n", eventos[i].valor_entrada_especial);
                    if (eventos[i].open_bar) {
                        printf("Evento e open bar\n");
                        for (int j = 0; j < 3; j++) {
                            printf("Bebida %d: %s, Teor alcoolico: %.2f, Valor unitario: %.2f\n",
                                   j + 1, eventos[i].bebidas[j].nome, eventos[i].bebidas[j].teor_alcoolico, eventos[i].bebidas[j].valor_unitario);
                        }
                    }
                    encontrado = 1;
                    break;
                }
            }
            if (!encontrado) {
                printf("Evento nao encontrado.\n");
            }
        }

               else if (opcao == 3) {
            int id;
            printf("Digite o identificador do evento: ");
            scanf("%d", &id);

            int encontrado = 0;
            for (int i = 0; i < total_eventos; i++) {
                if (eventos[i].id == id) {
                    int ingressos_basica, ingressos_normal, ingressos_especial;

                    printf("Ingressos vendidos para entrada basica: ");
                    scanf("%d", &ingressos_basica);

                    printf("Ingressos vendidos para entrada normal: ");
                    scanf("%d", &ingressos_normal);

                    printf("Ingressos vendidos para entrada especial: ");
                    scanf("%d", &ingressos_especial);

                    eventos[i].ingressos_vendidos_basica += ingressos_basica;
                    eventos[i].ingressos_vendidos_normal += ingressos_normal;
                    eventos[i].ingressos_vendidos_especial += ingressos_especial;

                    printf("Vendas atualizadas!\n");
                    encontrado = 1;
                    break;
                }
            }
            if (!encontrado) {
                printf("Evento nao encontrado.\n");
            }
        }

        else if (opcao == 4) {
            int id;
            printf("Digite o identificador do evento: ");
            scanf("%d", &id);

            int encontrado = 0;
            for (int i = 0; i < total_eventos; i++) {
                if (eventos[i].id == id) {
                    float receita = (eventos[i].ingressos_vendidos_basica * eventos[i].valor_entrada_basica) +
                                    (eventos[i].ingressos_vendidos_normal * eventos[i].valor_entrada_normal) +
                                    (eventos[i].ingressos_vendidos_especial * eventos[i].valor_entrada_especial);

                    float lucro = receita - eventos[i].custo_org;

                    printf("Receita total: %.2f\n", receita);
                    printf("Lucro/prejuizo: %.2f\n", lucro);
                    encontrado = 1;
                    break;
                }
            }
            if (!encontrado) {
                printf("Evento nao encontrado.\n");
            }
        }

    } while (opcao != 0);

    printf("fim:/\n");
    return 0;
}

