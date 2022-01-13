#
# Makefile for klotski runner
#
klotski: klotski.cpp
	g++ -o klotski klotski.cpp -Wall -Wextra -Werror
	
test: side_testing.cpp
	g++ -o test side_testing.cpp -Wall -Wextra -Werror
