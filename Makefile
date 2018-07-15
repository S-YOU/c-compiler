.PHONY: test_all_

test_all_:
	./test_all.sh

format:
	clang-format -i *.c -style=file

clean:
	rm out/*.out s/*.s


assembly_sandbox:
	gcc -Wall assembly_sandbox.c vector.c print_x86_64.c -o out/assembly_sandbox.out
	./test_ret3.sh '' s/assembly_sandbox.s out/assembly_sandbox.out 174 out/assembly_sandbox.out

intmap_check:
	gcc -Wall intmap.c intmap_check.c vector.c -o out/intmap_check.out
	./out/intmap_check.out


full_compile:
	gcc -Wall compiler2.c intmap.c vector.c print_x86_64.c lexer.c -o out/compiler.out
	./test_ret4.sh 001 'main(){return 123;}' 123 out/compiler.out
	./test_ret4.sh 010 'main(){return (123);}' 123 out/compiler.out
	./test_ret4.sh 011 'main(){return ((((123))));}' 123 out/compiler.out
	./test_ret4.sh 012 'main(){return 123+51;}' 174 out/compiler.out
	./test_ret4.sh 002 'main(){return 123+56-5;}' 174 out/compiler.out
	./test_ret4.sh 013 'main(){return 175-(4-3);}' 174 out/compiler.out
	./test_ret4.sh 014 'main(){return 181-4-3;}' 174 out/compiler.out
	./test_ret4.sh 003 'main(){return 0x29*3+7*8-5*1;}' 174 out/compiler.out
	./test_ret4.sh 004 'main(){return 6*(3+7)-5*1;}' 55 out/compiler.out
	./test_ret4.sh 015 'main(){return 43,6*(3+7)-5*1;}' 55 out/compiler.out
	./test_ret4.sh 016 'main(){return 43,6*(3+(4|3))-(5|1)*1;}' 55 out/compiler.out
	./test_ret4.sh 017 'main(){return 043,41*3+07*010-0Xa/(010%(1+2));}' 174 out/compiler.out
	./test_ret4.sh 006 'main(){return 7*5,(12,41*3)+7*16/(9,2)-10/(8%3);}' 174 out/compiler.out
	./test_ret4.sh 018 'main(){return 7*5 	,	(0xC,(41   )*(4-(011>8)))+7*(((1+2)>=3)<<4)/(9,(4>>(10<=10))+(3<3))-10/(	  ( 	1  <<3)	%3);}'  174 out/compiler.out
	./test_ret4.sh 008 'main(){return 35,	((	41|	(8   !=     15))*  ((3==3)+2))+((5|2)*(9&10))   -   (10/(8%3));}'  174 out/compiler.out
	./test_ret4.sh 005 'main(){return 043,41*3+07*010-0Xa/(010%(!!1+2));}' 174 out/compiler.out
	./test_ret4.sh 007 'main(){return 7*5 	,	(0xC,(41   )*(4-(011>8)))+7*(((1-~1)>=3)<<4)/(9,(4>>(10<=10))+(3<3))-10/(	  ( 	!0  <<3)	%3);}' 174 out/compiler.out
	./test_ret4.sh 019 'main(){return +174;}' 174 out/compiler.out
	./test_ret4.sh 019 'main(){return -(1-175);}' 174 out/compiler.out
	./test_ret4.sh 020 'main(){23; 45+37; ((12-1)*75); return -(1-175);}' 174 out/compiler.out
	./test_ret4.sh 021 'main(){23; 45+37; return -(1-175); ((12-1)*75);}' 174 out/compiler.out
	./test_ret4.sh 022 'main(){return (a = b = 9, a = 41*3, 55 - (b = 4) + a);}' 174 out/compiler.out
	./test_ret4.sh 023 'main(){a = b = c = 9; d = 5; a = 41*3; return (c, _q432 = 8, d = 11*5) - (b = 4) + a;}'  174 out/compiler.out
	./test_ret4.sh 024 'main(){return 175^1;}' 174 out/compiler.out
	./test_ret4.sh 025 'main(){return 2 + (1? 100 + 72 : 17);}' 174 out/compiler.out
	./test_ret4.sh 026 'main(){return (0? 234 : 2) + (1? 100 + 72 : 17);}' 174 out/compiler.out
	./test_ret4.sh 027 'main(){return (3, always87() + always87());}' 174 out/compiler.out
	./test_ret4.sh 027 'main(){return always87() + ((always8()* 11) -1);}' 174 out/compiler.out
	./test_ret4.sh 029 'main(){return add(170,4);}' 174 out/compiler.out
	./test_ret4.sh 028 'main(){return always87() + ((always8()* add(4,7)) -1);}' 174 out/compiler.out
	./test_ret4.sh 029 'main(){return always87() + ((always8()* subtract(12,1)) -1);}' 174 out/compiler.out
	./test_ret4.sh 030 'main(){3; {5; 7; 11; } return 175^1;}' 174 out/compiler.out
	./test_ret4.sh 031 'always87_(){return 87;} main(){return (3, always87() + always87_());}' 174 out/compiler.out
	./test_ret4.sh 032 'add_(x,y){4; return x+y;} main(){3; return add_(87,87);}' 174 out/compiler.out
	./test_ret4.sh 033 'fib(n){ return n < 2? n : fib(n - 1) + fib(n - 2); } main(){3; return fib(10);}' 55 out/compiler.out
	./test_ret4.sh 034 'tarai(x,y,z){ return x <= y? y : tarai(tarai(x-1, y, z), tarai(y-1, z, x), tarai(z-1, x, y)); } main(){return tarai(12,6,0);}' 12 out/compiler.out
	./test_ret4.sh 035 'main() { return (3 && 2 && 5) + 173; }' 174 out/compiler.out
	./test_ret4.sh 036 'main() { return (3 && 2) + !(3 && 0) + !(0 && 3)+ !(0 && 0) + 170; }' 174 out/compiler.out
	./test_ret4.sh 037 'main() { return (3 || 2 || 5) + 173; }' 174 out/compiler.out
	./test_ret4.sh 038 'main() { return (3 || 2) + (3 || 0) + (0 || 3)+ !(0 || 0) + 170; }' 174 out/compiler.out
	./test_ret4.sh 039 'main() { a = 3; a += 5;  return a + 166; }' 174 out/compiler.out
	./test_ret4.sh 040 'main() { a = 3; b = (a += 5);  return a + b + 158; }' 174 out/compiler.out

