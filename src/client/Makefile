#通用Makefile模板
SRCS=$(wildcard *.c ../public/*.c) 	#wildcard把指定目录下的后缀为.c的文件全部展开
OBJS=$(patsubst %.c, ../../obj/%.o, $(SRCS)) 	#OBJS将$(SRCS)下的.c转化为.o
CC=gcc
INCLUDES= -I../include \
		  -I. \

LIBS:=-L../inc \

CFLAGS=-g -Wall -o0
OUTPUT=server

all:$(OUTPUT)

$(OUTPUT):$(OBJS)
	$(CC) $^ -o $@ $(INCLUDES) $(LIBS)

$(OBJS):../../obj/%.o:%.c
	$(CC) -c $< -o $@

.PHONY:clean

clean:
	rm -rf $(OUTPUT) ../../obj/*.o
