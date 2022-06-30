#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max eleitores e candidatos
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferências[i][j] é a j-ésima preferência pelo eleitor i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Os candidatos têm nome, contagem de votos, status eliminado
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array da estrutura dos candidatos
candidate candidates[MAX_CANDIDATES];

// Números de eleitores e candidatos
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // ingonar o primiero parâmetro, pois corresponde ao nome do file.
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Continuar procurando por votos
    for (int i = 0; i < voter_count; i++)
    {

        // Consulta para cada posto
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Registro de voto, a menos que seja inválido
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Continuar mantendo runoffs até que o vencedor exista
    while (true)
    {
        // Calcular os votos dados aos candidatos restantes
        tabulate();

        // Verificar se a eleição foi ganha
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminar os candidatos de último lugar
        int min = find_min();
        bool tie = is_tie(min);

        // Se empate, todos ganham
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminar qualquer pessoa com número minimo de votos
        eliminate(min);

        //  Redefinir a contagem dos votos de volta a zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Registrar preferência se o voto for válido
bool vote(int voter, int rank, string name)
{
    for(int i = 0; i < candidate_count; i++) // passeando pelos candidatos passados na linha de comando.
    {
        if(strcmp(candidates[i].name, name) == 0) // Se o nome que o user está tentando passar corresponder aos da linha de argv
        {
             preferences[voter][rank] = i; // ele vai adicionar sua preferência de eleitora em um rank para i
            return true;
        }
    }
    return false;
}

// Tabular os votos dos candidatos não eliminados
void tabulate(void)
{
    for( int v =0; v < voter_count; v++) // percorre cada voto
    {
        for(int r =0; r < candidate_count; r++)
        {
            int c = preferences[v][r]; // preferences[eleitora 1 ][ alice]

            if(candidates[c].eliminated == false)
            {
                candidates[c].votes++;
                break; // interrupção para pular pro próximo eleitor.
            }
        }
    }
      return;
}

// Imprimir o vencedor da eleição, se houver um.
bool print_winner(void)
{
    for(int c = 0; c < candidate_count; c++)
    {
        if(candidates[c].votes > (voter_count / 2)) // vencedor tem que ter no mínimo a metade de todos os votos
        {
            printf("%s\n", candidates[c].name);
            return true;
        }
    }
    return false;
}

// Devolver o número mínimo de votos de qualquer candidato restante
int find_min(void)
{
    int minVotos = voter_count;

    for( int c =0; c < candidate_count; c++)
    {
        if( candidates[c].votes < minVotos && candidates[c].eliminated == false)
        {
            minVotos = candidates[c].votes; // atualizador de quem tem menos votos
        }
    }
    return minVotos;
}

// Retornar verdadeiro se a eleição estiver ligada entre todos os candidatos, falso caso contrário
bool is_tie(int min)
{
    int executavel = 0;
    int empate = 0;

    for( int c = 0; c < candidate_count; c++)
    {
        if(candidates[c].eliminated == false && candidates[c].votes == min)
        {
            executavel++;
            empate++;
        }
        else if( candidates[c].eliminated == false)
        {
            executavel++;
        }

    }
      if(empate == executavel)
        {
            return true;
        }
    return false;
}

// Eliminar o candidato (ou candidatos) em último lugar
void eliminate(int min)
{
    for( int c =0; c < candidate_count;c++)
    {
        if(candidates[c].eliminated == false && candidates[c].votes == min)
        {
            candidates[c].eliminated = true;
        }
    }
    return;
}
