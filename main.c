#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#define MAX_FLOOR 5

static QueueHandle_t xQueueCalls = NULL;
static int currentFloor = 0;

void vTaskCallGenerator(void *pvParameters) {
    srand((unsigned int) time(NULL));
    for(;;) {
        int floor = rand() % (MAX_FLOOR + 1);
        xQueueSend(xQueueCalls, &floor, portMAX_DELAY);
        printf("[CallGen] Chamando elevador para andar %d\n", floor);
        fflush(stdout);
        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}

void vTaskElevatorControl(void *pvParameters) {
    int targetFloor;
    for(;;) {
        if (xQueueReceive(xQueueCalls, &targetFloor, portMAX_DELAY) == pdPASS) {
            printf("[Elevator] Indo do andar %d para %d\n", currentFloor, targetFloor);
            fflush(stdout);

            while (currentFloor < targetFloor) {
                currentFloor++;
                vTaskDelay(pdMS_TO_TICKS(1000));
                printf("[Elevator] Subindo... agora no andar %d\n", currentFloor);
                fflush(stdout);
            }
            while (currentFloor > targetFloor) {
                currentFloor--;
                vTaskDelay(pdMS_TO_TICKS(1000));
                printf("[Elevator] Descendo... agora no andar %d\n", currentFloor);
                fflush(stdout);
            }

            printf("[Elevator] Chegou ao andar %d\n", currentFloor);
            fflush(stdout);
        }
    }
}

int main(void) {
    xQueueCalls = xQueueCreate(10, sizeof(int));

    xTaskCreate(vTaskCallGenerator, "CallGen", 1000, NULL, 1, NULL);
    xTaskCreate(vTaskElevatorControl, "Elevator", 1000, NULL, 2, NULL);

    vTaskStartScheduler();

    for(;;);
}
