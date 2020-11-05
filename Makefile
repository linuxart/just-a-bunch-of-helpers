#************************************************
#
#  Author: Artem Grischenko <artlinux@gmail.com>
#
#************************************************
CC=gcc
CPP=g++
CFLAGS=-g -Wall -Werror -c -I/usr/include -I/usr/include/json-c -I/usr/include/openssl
LFLAGS=-g -Wall -Werror -L/usr/lib
LIBS= -lpthread -ljson-c -lpq
OBJS=bin/main.o bin/request_handlers.o bin/threads.o bin/logger.o bin/signals.o bin/tpool.o bin/routes.o bin/helper_strings.o bin/helper_response.o bin/helper_http_headers.o bin/helper_http_request.o bin/helper_http_router.o bin/helper_postgres.o

all: tests runtests

bin/logger.o : src/logger.c includes/logger.h
	${CC} ${CFLAGS} src/logger.c -o bin/logger.o

bin/signals.o : src/signals.c includes/signals.h
	${CC} ${CFLAGS} src/signals.c -o bin/signals.o

bin/tpool.o : src/tpool.c includes/tpool.h
	${CC} ${CFLAGS} src/tpool.c -o bin/tpool.o

bin/routes.o : src/routes.c includes/routes.h
	${CC} ${CFLAGS} ${LIBS} src/routes.c -o bin/routes.o

bin/helper_strings.o : src/helpers/helper_strings.c includes/helpers/helper_strings.h
	${CC} ${CFLAGS} src/helpers/helper_strings.c -o bin/helper_strings.o

bin/helper_response.o : src/helpers/helper_response.c includes/helpers/helper_response.h
	${CC} ${CFLAGS} src/helpers/helper_response.c -o bin/helper_response.o

bin/helper_http_headers.o : src/helpers/helper_http_headers.c includes/helpers/helper_http_headers.h
	${CC} ${CFLAGS} src/helpers/helper_http_headers.c -o bin/helper_http_headers.o

bin/helper_http_request.o : src/helpers/helper_http_request.c includes/helpers/helper_http_request.h
	${CC} ${CFLAGS} src/helpers/helper_http_request.c -o bin/helper_http_request.o

bin/helper_http_router.o : src/helpers/helper_http_router.c includes/helpers/helper_http_router.h
	${CC} ${CFLAGS} src/helpers/helper_http_router.c -o bin/helper_http_router.o

bin/helper_postgres.o : src/helpers/helper_postgres.c includes/helpers/helper_postgres.h
	${CC} ${CFLAGS} src/helpers/helper_postgres.c -o bin/helper_postgres.o

strip : entrance-api
	strip bin/entrance-api

bin:
	if [ ! -d ./bin ]; then
	    mkdir bin
	fi

clean:
	rm -rf bin/*
	rm -f includes/*~
	rm -f src/*~
	rm -f *~


tests : tests_bin_dir bin/tests/test_helpers_helper_http_headers bin/tests/test_helpers_helper_strings bin/tests/test_helpers_helper_http_request bin/tests/test_helpers_helper_http_router bin/tests/test_helpers_helper_postgres

runtests : tests_bin_dir bin/tests/test_helpers_helper_http_headers bin/tests/test_helpers_helper_strings bin/tests/test_helpers_helper_http_request bin/tests/test_helpers_helper_http_router bin/tests/test_helpers_helper_postgres
	mkdir -p bin/tests
	bin/tests/test_helpers_helper_strings
	bin/tests/test_helpers_helper_http_headers
	bin/tests/test_helpers_helper_http_request
	bin/tests/test_helpers_helper_http_router
	bin/tests/test_helpers_helper_postgres

tests_bin_dir: 
	mkdir -p bin/tests

bin/tests/test_helpers_helper_http_request: bin/tests/test_helpers_helper_http_request.o bin/helper_http_headers.o bin/helper_strings.o bin/helper_http_request.o
	${CPP} ${LFLAGS} -o bin/tests/test_helpers_helper_http_request bin/tests/test_helpers_helper_http_request.o bin/helper_http_headers.o bin/helper_strings.o bin/helper_http_request.o

bin/tests/test_helpers_helper_http_request.o : tests/test_helpers_helper_http_request.c
	${CC} ${CFLAGS} tests/test_helpers_helper_http_request.c -o bin/tests/test_helpers_helper_http_request.o

bin/tests/test_helpers_helper_http_headers : bin/tests/test_helpers_helper_http_headers.o bin/helper_http_headers.o bin/helper_strings.o
	${CPP} ${LFLAGS} -o bin/tests/test_helpers_helper_http_headers bin/tests/test_helpers_helper_http_headers.o bin/helper_http_headers.o bin/helper_strings.o

bin/tests/test_helpers_helper_http_headers.o : tests/test_helpers_helper_http_headers.c
	${CC} ${CFLAGS} tests/test_helpers_helper_http_headers.c -o bin/tests/test_helpers_helper_http_headers.o
	
bin/tests/test_helpers_helper_strings : bin/tests/test_helpers_helper_strings.o bin/helper_strings.o
	${CPP} ${LFLAGS} -o bin/tests/test_helpers_helper_strings bin/tests/test_helpers_helper_strings.o bin/helper_strings.o	

bin/tests/test_helpers_helper_strings.o : tests/test_helpers_helper_strings.c
	${CC} ${CFLAGS} tests/test_helpers_helper_strings.c -o bin/tests/test_helpers_helper_strings.o

bin/tests/test_helpers_helper_http_router : bin/tests/test_helpers_helper_http_router.o bin/helper_http_router.o bin/helper_strings.o bin/helper_http_request.o bin/helper_http_headers.o bin/logger.o
	${CPP} ${LFLAGS} -o bin/tests/test_helpers_helper_http_router bin/tests/test_helpers_helper_http_router.o bin/helper_http_router.o bin/helper_strings.o bin/helper_http_request.o bin/helper_http_headers.o bin/logger.o

bin/tests/test_helpers_helper_http_router.o : tests/test_helpers_helper_http_router.c
	${CC} ${CFLAGS} tests/test_helpers_helper_http_router.c -o bin/tests/test_helpers_helper_http_router.o

bin/tests/test_helpers_helper_postgres : bin/tests/test_helpers_helper_postgres.o bin/helper_postgres.o bin/helper_strings.o
	${CPP} ${LFLAGS} -o bin/tests/test_helpers_helper_postgres bin/tests/test_helpers_helper_postgres.o bin/helper_postgres.o bin/helper_strings.o -lpq

bin/tests/test_helpers_helper_postgres.o : tests/test_helpers_helper_postgres.c
	${CC} ${CFLAGS} tests/test_helpers_helper_postgres.c -o bin/tests/test_helpers_helper_postgres.o
