#include<bits/stdc++.h>
using namespace std;

#define FCFS  1
#define RR    2
#define SJF   3
#define SRTF  4
#define PRIOc 5
#define PRIOp 6
#define PRIOd 7

#define Tt 0 //Tempo de execução médio
#define Tw 1 //Tempo de espera médio
#define TC 2 //Quantas trocas de contexto
#define TT 3 //Tempo total

#define RR_TQ 2

#define pb push_back
#define mp make_pair
#define INF 0x3f3f3f3f

int algoritmo_atual;
int tempo_execucao_total = 0;
vector<int> instante_chegada, tempo_execucao, prioridades;

// AA = Algotimo Atual
vector<int> AA_instante_chegada, AA_tempo_restante, AA_prioridades;
vector<int> AA_tempo_espera, AA_tempo_execucao;
int AA_tempo_total_restante;
int AA_instante_atual;
int AA_antigo_instante_atual;
int AA_tarefa_em_execucao;
int AA_troca_contexto;
int AA_ultima_tarefa_em_execucao;
int AA_instante_inicio_tarefa_em_execucao;
queue<int> AA_fila;
set<pair<int, int> > AA_heap; // estrutura de dados que possui os métodos que precisamos de uma heap

float tabela[7][4];

void read (void){
    int n;
    scanf("%d", &n);
    
    for(int j=0;j<3;j++){
        for(int i=0;i<n;i++){
            int x;
            scanf("%d", &x);
            if(j==0) instante_chegada.pb(x); // leitura dos instantes de chegada
            if(j==1) {
                tempo_execucao.pb(x);   // leitura dos tempos de execução
                tempo_execucao_total += x;     
            }
            if(j==2) prioridades.pb(x);      // leitura das prioridades
        }
    }
}

// retorna o tempo da próxima chegada
int get_instante_proxima_chegada(void){
    int minimo = INF;
    for(int i=0;i<AA_instante_chegada.size();i++){
        minimo = min(minimo, AA_instante_chegada[i]> AA_instante_atual ? AA_instante_chegada[i] : INF );
    }
    return minimo;
}

// retorna o instante para o fim da execucao
int get_instante_fim_execucao(void){
    if(AA_tarefa_em_execucao == -1) return INF;
    else{
        if(
            algoritmo_atual == FCFS || 
            algoritmo_atual == SJF  || 
            algoritmo_atual == SRTF ||
            algoritmo_atual == PRIOc
         ){
            return AA_instante_atual + AA_tempo_restante[AA_tarefa_em_execucao];
        }
        if(algoritmo_atual == RR ||
            algoritmo_atual == PRIOp
        ){
            int fim_tarefa_atual = AA_instante_atual + AA_tempo_restante[AA_tarefa_em_execucao];
            int preempcao_tempo =  AA_instante_inicio_tarefa_em_execucao + RR_TQ;
            return min(fim_tarefa_atual, preempcao_tempo); 
        }
    }
}

//int count_ = 0;



// função responsável por realizar a simulação de execução e avanço temporal
void executa(void){
    int proxima_chegada = get_instante_proxima_chegada();       
    int fim_execucao = get_instante_fim_execucao();
    AA_antigo_instante_atual = AA_instante_atual;
    AA_instante_atual  = min(proxima_chegada,fim_execucao);
    int variacao_tempo = AA_instante_atual - AA_antigo_instante_atual;

    printf("\n\n\nFim execucao: %d\n", fim_execucao);
    printf("Proxima Chegada: %d\n", proxima_chegada);

//    count_++;  (macete para debugar)
//    if(count_ == 10){
//        while(1){count_++;}
//    }

    if(algoritmo_atual == FCFS){
        printf("Instante Atual : %d\n", AA_instante_atual);

        for(int i=0;i<AA_instante_chegada.size();i++){
            if(AA_instante_chegada[i] == AA_instante_atual){
                AA_fila.push(i);
                printf("Adicionando %d na fila \n", i);
            }

            // atualizando o tempo de espera de cada tarefa
            if(AA_tempo_restante[i] > 0 && AA_instante_atual > AA_instante_chegada[i] && i!= AA_tarefa_em_execucao){
                AA_tempo_espera[i] += variacao_tempo;
            }
        }

        if(AA_tarefa_em_execucao != -1){
            AA_tempo_restante[AA_tarefa_em_execucao] -= variacao_tempo;  
            printf("Reduzido o tempo da tarefa %d para %d restantes\n", AA_tarefa_em_execucao, AA_tempo_restante[AA_tarefa_em_execucao]);
            if(AA_tempo_restante[AA_tarefa_em_execucao] == 0){
                printf("Finalizada a tarefa %d\n", AA_tarefa_em_execucao);
                AA_tempo_execucao[AA_tarefa_em_execucao] = AA_instante_atual - AA_instante_chegada[AA_tarefa_em_execucao];

                AA_ultima_tarefa_em_execucao = AA_tarefa_em_execucao;
                AA_tarefa_em_execucao = -1;
            }
        }
        // não pode ser else, pois AA_tarefa_execução pode ter ser valor atribuído em -1 no bloco acima
        if(AA_tarefa_em_execucao == -1 && !AA_fila.empty()){
            AA_tarefa_em_execucao = AA_fila.front();
            AA_fila.pop();
            printf("Escolhido a tarefa %d\n", AA_tarefa_em_execucao);
            // verificar se houve troca de contexto
            if(AA_ultima_tarefa_em_execucao != -1 && AA_tarefa_em_execucao != AA_ultima_tarefa_em_execucao){
                AA_troca_contexto ++;
            }
        }

    }

    if(algoritmo_atual == RR){
        printf("Instante Atual : %d\n", AA_instante_atual);

        for(int i=0;i<AA_instante_chegada.size();i++){
            if(AA_instante_chegada[i] == AA_instante_atual){
                AA_fila.push(i);
                printf("Adicionando %d na fila \n", i);
            }

            // atualizando o tempo de espera de cada tarefa
            if(AA_tempo_restante[i] > 0 && AA_instante_atual > AA_instante_chegada[i] && i!= AA_tarefa_em_execucao){
                AA_tempo_espera[i] += variacao_tempo;
            }
        }

        if(AA_tarefa_em_execucao != -1){
            AA_tempo_restante[AA_tarefa_em_execucao] -= variacao_tempo;  
            printf("Reduzido o tempo da tarefa %d para %d restantes\n", AA_tarefa_em_execucao, AA_tempo_restante[AA_tarefa_em_execucao]);
            if(AA_instante_atual == fim_execucao){
                printf("Finalizada a tarefa %d\n", AA_tarefa_em_execucao);
                
                if(AA_tempo_restante[AA_tarefa_em_execucao] == 0){ // se efetivamente terminou
                    AA_tempo_execucao[AA_tarefa_em_execucao] = AA_instante_atual - AA_instante_chegada[AA_tarefa_em_execucao];
                }
                else {
                    printf("Adicionada a tarefa %d\n", AA_tarefa_em_execucao);
                    AA_fila.push(AA_tarefa_em_execucao);
                }

                AA_ultima_tarefa_em_execucao = AA_tarefa_em_execucao;
                AA_tarefa_em_execucao = -1;
            }
        }
        // não pode ser else, pois AA_tarefa_execução pode ter ser valor atribuído em -1 no bloco acima
        if(AA_tarefa_em_execucao == -1 && !AA_fila.empty()){
            AA_tarefa_em_execucao = AA_fila.front();
            AA_fila.pop();
            printf("Escolhido a tarefa %d\n", AA_tarefa_em_execucao);
            AA_instante_inicio_tarefa_em_execucao = AA_instante_atual;
            // verificar se houve troca de contexto
            if(AA_ultima_tarefa_em_execucao != -1 && AA_tarefa_em_execucao != AA_ultima_tarefa_em_execucao){
                AA_troca_contexto ++;
            }
        }

    }

    if(algoritmo_atual == SJF){
        printf("Instante Atual : %d\n", AA_instante_atual);

        for(int i=0;i<AA_instante_chegada.size();i++){
            if(AA_instante_chegada[i] == AA_instante_atual){
                AA_heap.insert(mp(AA_tempo_restante[i],i));
                printf("Adicionando %d na fila \n", i);
            }

            // atualizando o tempo de espera de cada tarefa
            if(AA_tempo_restante[i] > 0 && AA_instante_atual > AA_instante_chegada[i] && i!= AA_tarefa_em_execucao){
                AA_tempo_espera[i] += variacao_tempo;
            }
        }

        if(AA_tarefa_em_execucao != -1){
            AA_tempo_restante[AA_tarefa_em_execucao] -= variacao_tempo;  
            printf("Reduzido o tempo da tarefa %d para %d restantes\n", AA_tarefa_em_execucao, AA_tempo_restante[AA_tarefa_em_execucao]);
            if(AA_tempo_restante[AA_tarefa_em_execucao] == 0){
                printf("Finalizada a tarefa %d\n", AA_tarefa_em_execucao);
                AA_tempo_execucao[AA_tarefa_em_execucao] = AA_instante_atual - AA_instante_chegada[AA_tarefa_em_execucao];

                AA_ultima_tarefa_em_execucao = AA_tarefa_em_execucao;
                AA_tarefa_em_execucao = -1;
            }
        }
        // não pode ser else, pois AA_tarefa_execução pode ter ser valor atribuído em -1 no bloco acima
        if(AA_tarefa_em_execucao == -1 && !AA_heap.empty()){
            AA_tarefa_em_execucao = (*AA_heap.begin()).second;
            AA_heap.erase(AA_heap.begin());
            printf("Escolhido a tarefa %d\n", AA_tarefa_em_execucao);
            // verificar se houve troca de contexto
            if(AA_ultima_tarefa_em_execucao != -1 && AA_tarefa_em_execucao != AA_ultima_tarefa_em_execucao){
                AA_troca_contexto ++;
            }
        }


    }

    if(algoritmo_atual == SRTF){
        printf("Instante Atual : %d\n", AA_instante_atual);

        for(int i=0;i<AA_instante_chegada.size();i++){
            if(AA_instante_chegada[i] == AA_instante_atual){
                AA_heap.insert(mp(AA_tempo_restante[i],i));
                printf("Adicionando %d na fila \n", i);
            }

            // atualizando o tempo de espera de cada tarefa
            if(AA_tempo_restante[i] > 0 && AA_instante_atual > AA_instante_chegada[i] && i!= AA_tarefa_em_execucao){
                AA_tempo_espera[i] += variacao_tempo;
            }
        }

        if(AA_tarefa_em_execucao != -1){
            AA_tempo_restante[AA_tarefa_em_execucao] -= variacao_tempo;  
            printf("Reduzido o tempo da tarefa %d para %d restantes\n", AA_tarefa_em_execucao, AA_tempo_restante[AA_tarefa_em_execucao]);

            // sempre finalizar a tarefa, para que seja feita a verificação se existe alguma tarefa com tempo restante menoE
            printf("Finalizada a tarefa %d\n", AA_tarefa_em_execucao);
            AA_tempo_execucao[AA_tarefa_em_execucao] = AA_instante_atual - AA_instante_chegada[AA_tarefa_em_execucao];

            AA_ultima_tarefa_em_execucao = AA_tarefa_em_execucao;
            if(AA_tempo_restante[AA_tarefa_em_execucao] != 0){ // se a tarefa ainda não terminou de fato
                AA_heap.insert( mp(AA_tempo_restante[AA_tarefa_em_execucao],AA_tarefa_em_execucao) );

            }

            AA_tarefa_em_execucao = -1;
        }
        // não pode ser else, pois AA_tarefa_execução pode ter ser valor atribuído em -1 no bloco acima
        if(AA_tarefa_em_execucao == -1 && !AA_heap.empty()){
            AA_tarefa_em_execucao = (*AA_heap.begin()).second;
            AA_heap.erase(AA_heap.begin());
            printf("Escolhido a tarefa %d\n", AA_tarefa_em_execucao);
            // verificar se houve troca de contexto
            if(AA_ultima_tarefa_em_execucao != -1 && AA_tarefa_em_execucao != AA_ultima_tarefa_em_execucao){
                AA_troca_contexto ++;
            }
        }


    }

    if(algoritmo_atual == PRIOc){
        printf("Instante Atual : %d\n", AA_instante_atual);

        for(int i=0;i<AA_instante_chegada.size();i++){
            if(AA_instante_chegada[i] == AA_instante_atual){
                AA_heap.insert(mp(-AA_prioridades[i],i));
                printf("Adicionando %d na fila \n", i);
            }

            // atualizando o tempo de espera de cada tarefa
            if(AA_tempo_restante[i] > 0 && AA_instante_atual > AA_instante_chegada[i] && i!= AA_tarefa_em_execucao){
                AA_tempo_espera[i] += variacao_tempo;
            }
        }

        if(AA_tarefa_em_execucao != -1){
            AA_tempo_restante[AA_tarefa_em_execucao] -= variacao_tempo;  
            printf("Reduzido o tempo da tarefa %d para %d restantes\n", AA_tarefa_em_execucao, AA_tempo_restante[AA_tarefa_em_execucao]);
            if(AA_tempo_restante[AA_tarefa_em_execucao] == 0){
                printf("Finalizada a tarefa %d\n", AA_tarefa_em_execucao);
                AA_tempo_execucao[AA_tarefa_em_execucao] = AA_instante_atual - AA_instante_chegada[AA_tarefa_em_execucao];

                AA_ultima_tarefa_em_execucao = AA_tarefa_em_execucao;
                AA_tarefa_em_execucao = -1;
            }
        }
        // não pode ser else, pois AA_tarefa_execução pode ter ser valor atribuído em -1 no bloco acima
        if(AA_tarefa_em_execucao == -1 && !AA_heap.empty()){
            AA_tarefa_em_execucao = (*AA_heap.begin()).second;
            AA_heap.erase(AA_heap.begin());
            printf("Escolhido a tarefa %d\n", AA_tarefa_em_execucao);
            // verificar se houve troca de contexto
            if(AA_ultima_tarefa_em_execucao != -1 && AA_tarefa_em_execucao != AA_ultima_tarefa_em_execucao){
                AA_troca_contexto ++;
            }
        }


    }

    if(algoritmo_atual == PRIOp){
        printf("Instante Atual : %d\n", AA_instante_atual);

        for(int i=0;i<AA_instante_chegada.size();i++){
            if(AA_instante_chegada[i] == AA_instante_atual){
                AA_heap.insert(mp(-AA_prioridades[i],i));
                printf("Adicionando %d na fila \n", i);
            }

            // atualizando o tempo de espera de cada tarefa
            if(AA_tempo_restante[i] > 0 && AA_instante_atual > AA_instante_chegada[i] && i!= AA_tarefa_em_execucao){
                AA_tempo_espera[i] += variacao_tempo;
            }
        }

        if(AA_tarefa_em_execucao != -1){
            AA_tempo_restante[AA_tarefa_em_execucao] -= variacao_tempo;  
            printf("Reduzido o tempo da tarefa %d para %d restantes\n", AA_tarefa_em_execucao, AA_tempo_restante[AA_tarefa_em_execucao]);
            printf("Finalizada a tarefa %d\n", AA_tarefa_em_execucao);
            
            if(AA_tempo_restante[AA_tarefa_em_execucao] == 0){ // se efetivamente terminou
                AA_tempo_execucao[AA_tarefa_em_execucao] = AA_instante_atual - AA_instante_chegada[AA_tarefa_em_execucao];
            }
            else {
                printf("Adicionada a tarefa %d\n", AA_tarefa_em_execucao);
                AA_heap.insert( mp(-AA_prioridades[AA_tarefa_em_execucao],AA_tarefa_em_execucao)  );
            }

            AA_ultima_tarefa_em_execucao = AA_tarefa_em_execucao;
            AA_tarefa_em_execucao = -1;
        }
        // não pode ser else, pois AA_tarefa_execução pode ter ser valor atribuído em -1 no bloco acima
        if(AA_tarefa_em_execucao == -1 && !AA_heap.empty()){
            AA_tarefa_em_execucao = (*AA_heap.begin()).second;
            AA_heap.erase(AA_heap.begin());
            printf("Escolhido a tarefa %d\n", AA_tarefa_em_execucao);
            AA_instante_inicio_tarefa_em_execucao = AA_instante_atual;
            // verificar se houve troca de contexto
            if(AA_ultima_tarefa_em_execucao != -1 && AA_tarefa_em_execucao != AA_ultima_tarefa_em_execucao){
                AA_troca_contexto ++;
            }
        }


    }
}

void set_data_on_table(int data_type, float value){//PEDRO
    //coloca VALUE em tabela[algoritmo_atual][data_type], onde tabela é global
    //Ex.: tabela[FCFS][Tw] = 5.6;
    tabela[algoritmo_atual-1][data_type] = value;
}
void inicia(void){
    // bloco condicional temporário para pular algoritmos ainda não implementados
    if(
        algoritmo_atual == PRIOd 
    ) {
        return;
    }

    // limpeza dos vectors
    AA_instante_chegada.clear();
    AA_tempo_restante.clear();
    AA_prioridades.clear();
    AA_tempo_espera.clear();
    AA_tempo_execucao.clear();
    AA_heap.clear();
    
    //limpeza da fila
    queue<int> empty;
    swap( AA_fila, empty );


    // Cada um dos 7 algoritmos de priorização receberá uma cópia dessa entrada para
    // trabalhar conforme sua descrição
    for(int i=0;i<instante_chegada.size();i++){
        AA_instante_chegada.pb(instante_chegada[i]);
        AA_tempo_restante.pb(tempo_execucao[i]);
        AA_prioridades.pb(prioridades[i]);
        AA_tempo_espera.pb(0);
        AA_tempo_execucao.pb(0);
    }
    AA_instante_atual = -1;
    AA_tarefa_em_execucao = -1;
    AA_troca_contexto = 0;
    AA_ultima_tarefa_em_execucao = -1;
    AA_instante_inicio_tarefa_em_execucao = INF;

    int count = 0;
    while(tempo_execucao_total != AA_instante_atual){
        executa();
        count++;
        if(count == 30) break;
    }
    printf("\n\nTempo de execução total: %d\n", tempo_execucao_total);
    printf("AA_instante_atual: %d\n", AA_instante_atual);
    
    float tw_mean = 0;
    for(int i=0;i<AA_tempo_espera.size();i++){
        tw_mean += AA_tempo_espera[i];
        printf("%d ", AA_tempo_espera[i]);
    }
    
    printf("\n");
    float tt_mean = 0;
    for(int i=0;i<AA_tempo_execucao.size();i++){
        tt_mean += AA_tempo_execucao[i];
        printf("%d ", AA_tempo_execucao[i]);
    }
    set_data_on_table(Tw, tw_mean/AA_tempo_espera.size());
    set_data_on_table(Tt, tt_mean/AA_tempo_execucao.size());
    set_data_on_table(TC, AA_troca_contexto);
    set_data_on_table(TT, tempo_execucao_total);
    
    printf("\n%d\n", AA_troca_contexto);
}
void draw_table(){
    int i = 0;
    int j = 0;
    printf("--------------------------------------------------------------------------------------------------\n");
    printf("| TABELA DE RESULTADOS     | FCFS    | RR      | SJF     | SRTF    | PRIOc   | PRIOp   | PRIOd   |\n");
    printf("--------------------------------------------------------------------------------------------------\n");
    for(i = 0; i < 4; i++){
        if(i == Tw) printf("| Tempo de Espera Médio    ");
        if(i == Tt) printf("| Tempo de Execução Médio  ");
        if(i == TC) printf("| Qnt Trocas de Contexto   ");
        if(i == TT) printf("| Tempo de Execução Total  ");
        for(j = 0; j < 7; j++){
            printf("| %4.1f    ", tabela[j][i]);
        }
        printf("|\n");
    }
    printf("--------------------------------------------------------------------------------------------------\n");
}

int main(void){
    int i = 0;
    int j = 0;
    read();
    // realizando a execução para cada algoritmo
    for(algoritmo_atual = 1;algoritmo_atual <= 7; algoritmo_atual++ ){
        printf("############ ALGORITMO %d ##############\n", algoritmo_atual);
        inicia();
    }
    draw_table();
}