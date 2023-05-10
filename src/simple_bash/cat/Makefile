NAME = s21_cat

FLAGS = -std=c11 -Wall -Wextra -Werror

all : $(NAME)

$(NAME) : 
	gcc $(FLAGS) s21_cat.c -o s21_cat

test :
	bash cat_test.sh

ck-linter:
	clang-format --Werror -n $(NAME).c $(NAME).h

mk-linter:
	clang-format --Werror -i $(NAME).c $(NAME).h

clean : 
	@rm -rf $(NAME)

re : clean all
