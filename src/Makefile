CC = gcc
CFLAGS = -Wall -Werror -Wextra -g -std=c11
GCOVFLAGS = --coverage
CHECKFLAGS = -lcheck -lrt -lpthread -lsubunit -lm
QMAKE5 = qmake

SHELL = /bin/sh

GRAPHICS = ./graphics/graphics.a
GRAPHICS_FLAGS = -L. -l:$(GRAPHICS)
GRAPHICS_SOURCES = $(wildcard ./graphics/*.c)
GRAPHICS_OBJECTS = $(GRAPHICS_SOURCES:.c=.o)

TESTS = ./tests/tests.c


CHECK = ./tests/include.check $(wildcard ./tests/t_*)

VALGRIND_FLAGS = -s --leak-check=full

all: install

open: 
	./../3DViewer/3DViewer

install: build
	-mkdir ../3DViewer
	cp -rf ../build/* ../3DViewer
	./../3DViewer/3DViewer

build:  clean
	-mkdir ../build
	cd ../build; $(QMAKE5) ./../src/qt/3DViewer/3DViewer.pro
	cd ../build; pwd; make -C . 2> /dev/null 1> /dev/null

uninstall:
	rm -rf ../3DViewer/

clean:
	rm -f ./graphics/*.gcda ./graphics/*.gcno ./graphics/*.a ./graphics/*.o
	rm -f ./tests/tests.c ./tests/tests ./tests/*.gcda ./tests/*.gcno 
	rm -rf report
	rm -rf qt/build*
	rm -f graphics.info
	rm -rf ../build
	rm -rf ../3DViewer
	rm -rf qt/dist
	rm -f valgrind-out.txt

dvi:
	open ./dvi.html

dist: clean
	mkdir ./qt/dist
	cd ./qt/dist; $(QMAKE5) ../3DViewer/3DViewer.pro
	cd ./qt/dist; make dist
	mkdir ../build
	mv -f ./qt/dist/3DViewer1.0.0.tar.gz ../build/



gcov_report: test
	lcov -t "graphics" -o graphics.info -c -d .
	genhtml -o report graphics.info
	open ./report/index.html

test: graphics
	rm -f ./tests/tests
	checkmk clean_mode=1 $(CHECK) > $(TESTS)
	$(CC) -g $(GCOVFLAGS) -o ./tests/tests $(TESTS) $(CLIB_FLAGS) $(GRAPHICS_FLAGS) $(CHECKFLAGS)
	cd ./tests; ./tests

nofork : graphics
	rm -f ./tests/tests
	checkmk clean_mode=1 $(CHECK) > $(TESTS)
	perl -pi -e 's/SRunner \*sr = srunner_create\(s1\);/SRunner \*sr = srunner_create\(s1\); srunner_set_fork_status\(sr, CK_NOFORK\);/g' $(TESTS)
	$(CC) $(CFLAGS) $(GCOVFLAGS) -o ./tests/tests $(TESTS) $(CLIB_FLAGS) $(GRAPHICS_FLAGS) $(CHECKFLAGS)
	cd ./tests; ./tests

valgrind : nofork
	cd ./tests; valgrind --leak-check=full --log-file=../valgrind-out.txt \
         ./tests

graphics: $(GRAPHICS_OBJECTS)
	ar r $(GRAPHICS) $(GRAPHICS_OBJECTS)

style :
	clang-format -style=Google -n ./graphics/*.c

style_fix :
	clang-format -style=Google -i ./graphics/*.c

%.o : %.c
	$(CC) $(CFLAGS) $(GCOVFLAGS) -c $< -o $@


