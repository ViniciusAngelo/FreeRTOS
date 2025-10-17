#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Simulação de um elevador de 3 andares */
#define NUM_ANDARES 3

/* Tipos de mensagens */
typedef struct {
    int andar_chamado;
} PedidoElevador;

/* Fila de pedidos */
QueueHandle_t filaPedidos;

/* Estado global do elevador */
int andar_atual = 0;
char *estado = "Parado";

/* --- Tarefas --- */

/* Tarefa 1: Gera chamadas aleatórias para o elevador */
void tarefaBotoes(void *pvParameters) {
    PedidoElevador pedido;
    for (;;) {
        pedido.andar_chamado = rand() % NUM_ANDARES;
        printf("[BOTAO] Andar %d chamou o elevador.\n", pedido.andar_chamado);
        xQueueSend(filaPedidos, &pedido, portMAX_DELAY);
        vTaskDelay(pdMS_TO_TICKS(5000)); // Novo pedido a cada 5s
    }
}

/* Tarefa 2: Controla o movimento do elevador */
void tarefaControle(void *pvParameters) {
    PedidoElevador pedido;
    for (;;) {
        if (xQueueReceive(filaPedidos, &pedido, portMAX_DELAY)) {
            printf("[CONTROLE] Indo para o andar %d...\n", pedido.andar_chamado);

            while (andar_atual != pedido.andar_chamado) {
                if (andar_atual < pedido.andar_chamado) {
                    andar_atual++;
                    estado = "Subindo";
                } else {
                    andar_atual--;
                    estado = "Descendo";
                }
                vTaskDelay(pdMS_TO_TICKS(2000)); // tempo para mudar de andar
            }

            estado = "Parado";
            printf("[CONTROLE] Chegou ao andar %d.\n", andar_atual);
        }
    }
}

/* Tarefa 3: Mostra o estado atual do elevador */
void tarefaDisplay(void *pvParameters) {
    for (;;) {
        printf("[DISPLAY] Andar atual: %d | Estado: %s\n", andar_atual, estado);
        vTaskDelay(pdMS_TO_TICKS(1000)); // Atualiza a cada 1s
    }
}

/* --- MAIN --- */
int main() {
    srand(time(NULL));
    filaPedidos = xQueueCreate(5, sizeof(PedidoElevador));

    if (filaPedidos != NULL) {
        xTaskCreate(tarefaBotoes, "Botoes", 1000, NULL, 1, NULL);
        xTaskCreate(tarefaControle, "Controle", 1000, NULL, 2, NULL);
        xTaskCreate(tarefaDisplay, "Display", 1000, NULL, 1, NULL);

        vTaskStartScheduler();
    } else {
        printf("Erro ao criar fila de pedidos!\n");
    }

    for (;;);
    return 0;
}

