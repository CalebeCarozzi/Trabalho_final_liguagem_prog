
//TRABALHO FINAL LINGUAGEM DE PROGRAMAÇÂO - TADS - UFPR 
//ALUNO: CALEBE CAROZZI 
//PERIODO: NOTURO
//SEMESTRE: SEGUNDO

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int dia;
    int mes;
    int ano;
    char categoria[31];
    char descricao[101];
    float valor;
} Despesa;

typedef struct {
    Despesa *itens;
    int qtd;
    int cap;
} ListaDespesas;

int ler_int(const char *mensagem, int min, int max) {
    char linha[64];
    long valor;
    char *fim;
    int ok = 0;

    while (!ok) {
        printf("%s", mensagem);
        if (fgets(linha, sizeof(linha), stdin) == NULL) {
            continue;
        }
        valor = strtol(linha, &fim, 10);
        if (fim == linha || (*fim != '\n' && *fim != '\0')) {
            printf("Entrada invalida. Digite um numero inteiro.\n");
            continue;
        }
        if (valor < min || valor > max) {
            printf("Valor deve estar entre %d e %d.\n", min, max);
            continue;
        }
        ok = 1;
    }

    return (int) valor;
}

float ler_float(const char *mensagem, float min, float max) {
    char linha[64];
    float valor;
    char *fim;
    int ok = 0;

    while (!ok) {
        printf("%s", mensagem);
        if (fgets(linha, sizeof(linha), stdin) == NULL) {
            continue;
        }
        valor = strtof(linha, &fim);
        if (fim == linha || (*fim != '\n' && *fim != '\0')) {
            printf("Entrada invalida. Digite um numero (use ponto, ex: 123.45).\n");
            continue;
        }
        if (valor < min || valor > max) {
            printf("Valor deve estar entre %.2f e %.2f.\n", min, max);
            continue;
        }
        ok = 1;
    }

    return valor;
}

void ler_string(const char *mensagem, char *destino, int tamanho) {
    char linha[256];
    int ok = 0;

    while (!ok) {
        printf("%s", mensagem);
        if (fgets(linha, sizeof(linha), stdin) == NULL) {
            continue;
        }
        linha[strcspn(linha, "\n")] = '\0';
        if (strlen(linha) == 0) {
            printf("Texto nao pode ser vazio.\n");
            continue;
        }
        if ((int)strlen(linha) >= tamanho) {
            printf("Texto muito longo. Maximo de %d caracteres.\n", tamanho - 1);
            continue;
        }
        strcpy(destino, linha);
        ok = 1;
    }
}

void inicializar_lista(ListaDespesas *lista) {
    lista->qtd = 0;
    lista->cap = 10;
    lista->itens = (Despesa *) malloc(lista->cap * sizeof(Despesa));
    if (lista->itens == NULL) {
        printf("Erro de memoria.\n");
        exit(1);
    }
}

void redimensionar_lista(ListaDespesas *lista) {
    int novo_cap = lista->cap * 2;
    Despesa *novo = (Despesa *) realloc(lista->itens, novo_cap * sizeof(Despesa));
    if (novo == NULL) {
        printf("Erro ao redimensionar memoria.\n");
        exit(1);
    }
    lista->itens = novo;
    lista->cap = novo_cap;
}

void adicionar_despesa(ListaDespesas *lista) {
    if (lista->qtd == lista->cap) {
        redimensionar_lista(lista);
    }

    Despesa d;

    d.dia = ler_int("Dia (1-31): ", 1, 31);
    d.mes = ler_int("Mes (1-12): ", 1, 12);
    d.ano = ler_int("Ano (1900-2100): ", 1900, 2100);

    ler_string("Categoria (ex: Alimentacao, Transporte): ", d.categoria, sizeof(d.categoria));
    ler_string("Descricao: ", d.descricao, sizeof(d.descricao));
    d.valor = ler_float("Valor (ex: 123.45): ", 0.01f, 100000000.0f);

    lista->itens[lista->qtd] = d;
    lista->qtd++;

    printf("Despesa adicionada.\n");
}

void listar_todas(const ListaDespesas *lista) {
    if (lista->qtd == 0) {
        printf("Nenhuma despesa cadastrada.\n");
        return;
    }

    int i;
    printf("=== TODAS AS DESPESAS ===\n");
    for (i = 0; i < lista->qtd; i++) {
        Despesa d = lista->itens[i];
        printf("%d) %02d/%02d/%04d - R$ %.2f - %s - %s\n",
               i + 1, d.dia, d.mes, d.ano, d.valor, d.categoria, d.descricao);
    }
}

void listar_por_mes(const ListaDespesas *lista) {
    if (lista->qtd == 0) {
        printf("Nenhuma despesa cadastrada.\n");
        return;
    }

    int mes = ler_int("Mes para filtrar (1-12): ", 1, 12);
    int ano = ler_int("Ano para filtrar (1900-2100): ", 1900, 2100);

    float total = 0.0f;
    int i, encontrou = 0;

    printf("=== DESPESAS DE %02d/%04d ===\n", mes, ano);
    for (i = 0; i < lista->qtd; i++) {
        Despesa d = lista->itens[i];
        if (d.mes == mes && d.ano == ano) {
            printf("%02d/%02d/%04d - R$ %.2f - %s - %s\n",
                   d.dia, d.mes, d.ano, d.valor, d.categoria, d.descricao);
            total += d.valor;
            encontrou = 1;
        }
    }

    if (!encontrou) {
        printf("Nenhuma despesa nesse mes/ano.\n");
    } else {
        printf("Total do periodo: R$ %.2f\n", total);
    }
}

void listar_por_categoria(const ListaDespesas *lista) {
    if (lista->qtd == 0) {
        printf("Nenhuma despesa cadastrada.\n");
        return;
    }

    char categoria[31];
    ler_string("Categoria para filtrar: ", categoria, sizeof(categoria));

    float total = 0.0f;
    int i, encontrou = 0;

    printf("=== DESPESAS NA CATEGORIA %s ===\n", categoria);
    for (i = 0; i < lista->qtd; i++) {
        Despesa d = lista->itens[i];
        if (strcmp(d.categoria, categoria) == 0) {
            printf("%02d/%02d/%04d - R$ %.2f - %s\n",
                   d.dia, d.mes, d.ano, d.valor, d.descricao);
            total += d.valor;
            encontrou = 1;
        }
    }

    if (!encontrou) {
        printf("Nenhuma despesa nessa categoria.\n");
    } else {
        printf("Total da categoria: R$ %.2f\n", total);
    }
}

void total_geral(const ListaDespesas *lista) {
    if (lista->qtd == 0) {
        printf("Nenhuma despesa cadastrada.\n");
        return;
    }

    float total = 0.0f;
    int i;
    for (i = 0; i < lista->qtd; i++) {
        total += lista->itens[i].valor;
    }

    printf("Total geral gasto: R$ %.2f\n", total);
}

void salvar_arquivo(const ListaDespesas *lista) {
    FILE *f = fopen("despesas.dat", "wb");
    if (f == NULL) {
        printf("Erro ao abrir arquivo para escrita.\n");
        return;
    }

    fwrite(&lista->qtd, sizeof(int), 1, f);
    if (lista->qtd > 0) {
        fwrite(lista->itens, sizeof(Despesa), lista->qtd, f);
    }

    fclose(f);
    printf("Dados salvos em despesas.dat\n");
}

void carregar_arquivo(ListaDespesas *lista) {
    FILE *f = fopen("despesas.dat", "rb");
    if (f == NULL) {
        return;
    }

    int qtd;
    if (fread(&qtd, sizeof(int), 1, f) != 1) {
        fclose(f);
        return;
    }

    if (qtd <= 0) {
        fclose(f);
        return;
    }

    if (lista->itens != NULL) {
        free(lista->itens);
    }

    lista->qtd = qtd;
    lista->cap = qtd;
    lista->itens = (Despesa *) malloc(lista->cap * sizeof(Despesa));
    if (lista->itens == NULL) {
        printf("Erro de memoria ao carregar arquivo.\n");
        fclose(f);
        exit(1);
    }

    size_t lidos = fread(lista->itens, sizeof(Despesa), lista->qtd, f);
    if (lidos != (size_t) lista->qtd) {
        printf("Erro ao ler dados do arquivo.\n");
    } else {
        printf("Dados carregados de despesas.dat\n");
    }

    fclose(f);
}

int main() {
    ListaDespesas lista;
    lista.itens = NULL;
    inicializar_lista(&lista);
    carregar_arquivo(&lista);

    int opcao;

    do {
        printf("\n=== MENU DESPESAS ===\n");
        printf("1 - Adicionar despesa\n");
        printf("2 - Listar todas as despesas\n");
        printf("3 - Listar despesas por mes/ano\n");
        printf("4 - Listar despesas por categoria\n");
        printf("5 - Mostrar total geral\n");
        printf("6 - Salvar em arquivo\n");
        printf("0 - Sair\n");
        opcao = ler_int("Opcao: ", 0, 6);

        switch (opcao) {
            case 1:
                adicionar_despesa(&lista);
                break;
            case 2:
                listar_todas(&lista);
                break;
            case 3:
                listar_por_mes(&lista);
                break;
            case 4:
                listar_por_categoria(&lista);
                break;
            case 5:
                total_geral(&lista);
                break;
            case 6:
                salvar_arquivo(&lista);
                break;
            case 0:
                salvar_arquivo(&lista);
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida.\n");
        }

    } while (opcao != 0);

    if (lista.itens != NULL) {
        free(lista.itens);
    }

    return 0;
}
