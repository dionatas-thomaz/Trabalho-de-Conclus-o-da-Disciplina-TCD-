#include <stdio.h>
#include <string.h>

#define MAX_EVENTOS 100
#define FILENAME "eventos.txt"

// Definições das estruturas Bebida e Evento
typedef struct
{
    char nome[50];
    float teor_alcoolico;
    float valor_unitario;
} Bebida;

typedef struct
{
    int id;
    char nome[100];
    char local[100];
    char data[11];
    float custo_organizacao;
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

// Funções de salvar e carregar eventos, e outras operações
void salvar_eventos()
{
    FILE *file = fopen(FILENAME, "w");
    if (file == NULL)
    {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return;
    }

    for (int i = 0; i < total_eventos; i++)
    {
        Evento e = eventos[i];
        fprintf(file, "%d\n", e.id);
        fprintf(file, "%s\n", e.nome);
        fprintf(file, "%s\n", e.local);
        fprintf(file, "%s\n", e.data);
        fprintf(file, "%.2f\n", e.custo_organizacao);
        fprintf(file, "%d\n", e.max_ingressos);
        fprintf(file, "%.2f\n", e.valor_entrada_basica);
        fprintf(file, "%.2f\n", e.valor_entrada_normal);
        fprintf(file, "%.2f\n", e.valor_entrada_especial);
        fprintf(file, "%d\n", e.ingressos_vendidos_basica);
        fprintf(file, "%d\n", e.ingressos_vendidos_normal);
        fprintf(file, "%d\n", e.ingressos_vendidos_especial);
        fprintf(file, "%d\n", e.open_bar);

        if (e.open_bar)
        {
            for (int j = 0; j < 3; j++)
            {
                fprintf(file, "%s\n", e.bebidas[j].nome);
                fprintf(file, "%.2f\n", e.bebidas[j].teor_alcoolico);
                fprintf(file, "%.2f\n", e.bebidas[j].valor_unitario);
            }
        }
    }

    fclose(file);
    printf("Dados salvos com sucesso!\n");
}
void carregar_eventos()
{
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL)
    {
        printf("Nenhum dado salvo encontrado.\n");
        return;
    }

    while (fscanf(file, "%d\n", &eventos[total_eventos].id) != EOF)
    {
        Evento *e = &eventos[total_eventos];

        fgets(e->nome, sizeof(e->nome), file);
        e->nome[strcspn(e->nome, "\n")] = 0; // Remove newline character

        fgets(e->local, sizeof(e->local), file);
        e->local[strcspn(e->local, "\n")] = 0;

        fgets(e->data, sizeof(e->data), file);
        e->data[strcspn(e->data, "\n")] = 0;

        fscanf(file, "%f\n", &e->custo_organizacao);
        fscanf(file, "%d\n", &e->max_ingressos);
        fscanf(file, "%f\n", &e->valor_entrada_basica);
        fscanf(file, "%f\n", &e->valor_entrada_normal);
        fscanf(file, "%f\n", &e->valor_entrada_especial);
        fscanf(file, "%d\n", &e->ingressos_vendidos_basica);
        fscanf(file, "%d\n", &e->ingressos_vendidos_normal);
        fscanf(file, "%d\n", &e->ingressos_vendidos_especial);
        fscanf(file, "%d\n", &e->open_bar);

        if (e->open_bar)
        {
            for (int j = 0; j < 3; j++)
            {
                fgets(e->bebidas[j].nome, sizeof(e->bebidas[j].nome), file);
                e->bebidas[j].nome[strcspn(e->bebidas[j].nome, "\n")] = 0;
                fscanf(file, "%f\n", &e->bebidas[j].teor_alcoolico);
                fscanf(file, "%f\n", &e->bebidas[j].valor_unitario);
            }
        }

        total_eventos++;
    }

    fclose(file);
    printf("Dados carregados\n");
}
void cadastrar_evento()
{
    Evento e;
    printf("Digite o identificador do evento (0 para finalizar): ");
    scanf("%d", &e.id);
    if (e.id == 0)
        return;

    printf("Nome do evento: ");
    scanf(" %[^\n]", e.nome);

    printf("Local: ");
    scanf(" %[^\n]", e.local);

    printf("Data (DD/MM/YYYY): ");
    scanf(" %[^\n]", e.data);

    printf("Custo de organizacao: ");
    scanf("%f", &e.custo_organizacao);

    printf("Quantidade maxima de ingressos: ");
    scanf("%d", &e.max_ingressos);

    e.valor_entrada_basica = e.custo_organizacao / e.max_ingressos;

    printf("Evento e open bar? (1 para sim, 0 para nao): ");
    scanf("%d", &e.open_bar);

    if (e.open_bar)
    {
        for (int i = 0; i < 3; i++)
        {
            printf("Nome da bebida %d: ", i + 1);
            scanf(" %[^\n]", e.bebidas[i].nome);
            fflush(stdin);
            printf("Teor alcoolico da bebida %d: ", i + 1);
            scanf("%f", &e.bebidas[i].teor_alcoolico);
            fflush(stdin);
            printf("Valor unitario da bebida %d: ", i + 1);
            scanf("%f", &e.bebidas[i].valor_unitario);
            fflush(stdin);
            e.valor_entrada_basica += 1.75 * e.bebidas[i].valor_unitario;
            setbuf(stdin, NULL);
        }
    }

    e.valor_entrada_normal = e.valor_entrada_basica * 1.10;
    e.valor_entrada_especial = e.valor_entrada_normal * 1.15;

    e.ingressos_vendidos_basica = 0;
    e.ingressos_vendidos_normal = 0;
    e.ingressos_vendidos_especial = 0;

    eventos[total_eventos++] = e;
    printf("Evento cadastrado com sucesso!\n");

    salvar_eventos();
}
void pesquisar_evento()
{
    int id;
    printf("Digite o identificador do evento: ");
    scanf("%d", &id);

    for (int i = 0; i < total_eventos; i++)
    {
        if (eventos[i].id == id)
        {
            printf("Nome: %s\n", eventos[i].nome);
            printf("Local: %s\n", eventos[i].local);
            printf("Data: %s\n", eventos[i].data);
            printf("Custo de organizacao: %.2f\n", eventos[i].custo_organizacao);
            printf("Capacidade: %d\n", eventos[i].max_ingressos);
            printf("Valor entrada basica: %.2f\n", eventos[i].valor_entrada_basica);
            printf("Valor entrada normal: %.2f\n", eventos[i].valor_entrada_normal);
            printf("Valor entrada especial: %.2f\n", eventos[i].valor_entrada_especial);
            if (eventos[i].open_bar)
            {
                printf("Evento e open bar\n");
                for (int j = 0; j < 3; j++)
                {
                    printf("Bebida %d: %s, Teor alcoolico: %.2f, Valor unitario: %.2f\n",
                           j + 1, eventos[i].bebidas[j].nome, eventos[i].bebidas[j].teor_alcoolico, eventos[i].bebidas[j].valor_unitario);
                }
            }
            return;
        }
    }
    printf("Evento nao encontrado.\n");
}
void ingressos_vendidos() {
    int id, tipo, quantidade;
    printf("Digite o identificador do evento: ");
    scanf("%d", &id);
    
    for (int i = 0; i < total_eventos; i++) {
        if (eventos[i].id == id) {
            printf("Tipo de ingresso vendido (1 - Básica, 2 - Normal, 3 - Especial): ");
            scanf("%d", &tipo);
            printf("Quantidade vendida: ");
            scanf("%d", &quantidade);
            
            if (tipo == 1) eventos[i].ingressos_vendidos_basica += quantidade;
            else if (tipo == 2) eventos[i].ingressos_vendidos_normal += quantidade;
            else if (tipo == 3) eventos[i].ingressos_vendidos_especial += quantidade;
            
            printf("Ingressos atualizados com sucesso!\n");
            return;
        }
    }
    printf("Evento não encontrado.\n");
}
void exibir_resultado_evento() {
    int id;
    printf("Digite o identificador do evento: ");
    scanf("%d", &id);
    
    for (int i = 0; i < total_eventos; i++) {
        if (eventos[i].id == id) {
            float total_vendas = eventos[i].ingressos_vendidos_basica * eventos[i].valor_entrada_basica +
                                 eventos[i].ingressos_vendidos_normal * eventos[i].valor_entrada_normal +
                                 eventos[i].ingressos_vendidos_especial * eventos[i].valor_entrada_especial;
            float resultado = total_vendas - eventos[i].custo_organizacao;
            printf("Resultado do evento %s: %s\n", eventos[i].nome, resultado >= 0 ? "Lucro" : "Prejuízo");
            return;
        }
    }
    printf("Evento não encontrado.\n");
}
void exibir_resultado_geral() {
    float saldo_total = 0;
    
    for (int i = 0; i < total_eventos; i++) {
        float total_vendas = eventos[i].ingressos_vendidos_basica * eventos[i].valor_entrada_basica +
                             eventos[i].ingressos_vendidos_normal * eventos[i].valor_entrada_normal +
                             eventos[i].ingressos_vendidos_especial * eventos[i].valor_entrada_especial;
        float resultado = total_vendas - eventos[i].custo_organizacao;
        saldo_total += resultado;
    }
    
    printf("Saldo total do promoter: %.2f\n", saldo_total);
}

int main()
{
    int opcao;

    // Carregar eventos 
    carregar_eventos();

    do
    {
        printf("1  Cadastro de eventos\n");
        printf("2  Pesquisar eventos\n");
        printf("3  Ingressos vendidos\n");
        printf("4  resultado por evento\n");
        printf("5  Exibir resultado geral\n");
        printf("6  Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            cadastrar_evento();
            break;
        case 2:
            pesquisar_evento();
            break;
        case 3:
            ingressos_vendidos();
            break;
        case 4:
            exibir_resultado_evento();
            break;
        case 5:
            exibir_resultado_geral();
            break;
        case 6:
            printf("Saindo do programa...\n");
            break;
        default:
            printf("Opção inválida. Tente novamente.\n");
        }
    } while (opcao != 6);

    return 0;
}
