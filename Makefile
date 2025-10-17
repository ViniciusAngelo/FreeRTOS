CC = gcc
CFLAGS = -I. \
         -I../FreeRTOS-Kernel/include \
         -I../FreeRTOS-Kernel/portable/ThirdParty/GCC/Posix \
         -I../FreeRTOS-Kernel/portable/ThirdParty/GCC/Posix/utils

SRC = main.c \
      ../FreeRTOS-Kernel/*.c \
      ../FreeRTOS-Kernel/portable/MemMang/heap_3.c \
      ../FreeRTOS-Kernel/portable/ThirdParty/GCC/Posix/utils/wait_for_event.c \
      ../FreeRTOS-Kernel/portable/ThirdParty/GCC/Posix/utils/wait_for_event.h \
      ../FreeRTOS-Kernel/portable/ThirdParty/GCC/Posix/port.c

OUT = rtos_demo

all:
	$(CC) $(SRC) $(CFLAGS) -o $(OUT) -lpthread -lrt

run: all
	./$(OUT)

