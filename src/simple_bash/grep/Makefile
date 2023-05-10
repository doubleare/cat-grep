NAME = s21_grep

FLAGS = -std=c11 -Wall -Wextra -Werror

all : $(NAME)

$(NAME) : 
	gcc $(FLAGS) s21_grep.c -o s21_grep

test :
	bash grep_test.sh

ck-linter:
	clang-format --Werror -n $(NAME).c $(NAME).h

mk-linter:
	clang-format --Werror -i $(NAME).c $(NAME).h

clean : 
	@rm -rf $(NAME)

re : clean all