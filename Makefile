.PHONY: test_all_

test_all_:
	./test_all.sh

clean:
	rm out/*.out s/*.s

lexer_check:
	gcc -Wall lexer_check.c lexer.c -o out/lexer_check.out
	echo '123+456-789' | ./out/lexer_check.out 2> res.txt
	diff res.txt expected.txt

lexer_check2:
	gcc -Wall lexer_check.c lexer.c -o out/lexer_check.out
	echo '7*5 	,	(0xC,(41   )*(4-(011>8)))+7*(((1-~1)>=3)<<4)/(9,(4>>(10<=10))+(3<3))-10/(	  ( 	!0  <<3)	%3)' | ./out/lexer_check.out 2> res2.txt
	diff res2.txt expected2.txt

lexer_check3:
	gcc -Wall lexer_check.c lexer.c -o out/lexer_check.out
	echo '_1qw12_er345ty = 123, 51 + _1qw12_er345ty' | ./out/lexer_check.out 2> expected3.txt
	diff res3.txt expected3.txt

full_compile001:
	gcc -Wall compiler2.c intmap.c vector.c print_assembly.c lexer.c -o out/compiler.out
	./test_ret.sh '123' s/full_compile001.s out/task001.out 123 out/compiler.out

print_assembly_check:
	gcc -Wall print_assembly_check.c print_assembly.c -o out/pac.out
	./test_ret.sh '' s/print_assembly_check003.s out/pa.out 174 out/pac.out

print_assembly_check2:
	gcc -Wall print_assembly_check2.c print_assembly.c -o out/pac2.out
	./test_ret.sh '' s/print_assembly_check005.s out/pa2.out 174 out/pac2.out

print_assembly_check3:
	gcc -Wall print_assembly_check3.c print_assembly.c -o out/pac3.out
	./test_ret.sh '' s/print_assembly_check006.s out/pa3.out 174 out/pac3.out

print_assembly_check4:
	gcc -Wall print_assembly_check4.c print_assembly.c -o out/pac4.out
	./test_ret.sh '' s/print_assembly_check007.s out/pa4.out 174 out/pac4.out

print_assembly_check5:
	gcc -Wall print_assembly_check5.c print_assembly.c -o out/pac5.out
	./test_ret.sh '' s/print_assembly_check008.s out/pa5.out 174 out/pac5.out

print_assembly_check6:
	gcc -Wall print_assembly_check6.c print_assembly.c -o out/pac6.out
	./test_ret.sh '' s/print_assembly_check009.s out/pa6.out 174 out/pac6.out

intmap_check:
	gcc -Wall intmap.c intmap_check.c vector.c -o out/intmap_check.out
	./out/intmap_check.out

full_compile002:
	gcc -Wall compiler2.c intmap.c vector.c print_assembly.c lexer.c -o out/compiler.out
	./test_ret.sh '123+56-5' s/full_compile002.s out/task002.out 174 out/compiler.out

full_compile003:
	gcc -Wall compiler2.c intmap.c vector.c print_assembly.c lexer.c -o out/compiler.out
	./test_ret.sh '0x29*3+7*8-5*1' s/full_compile003.s out/task003.out 174 out/compiler.out
	diff s/full_compile003.s s/print_assembly_check003.s

full_compile004:
	gcc -Wall compiler2.c intmap.c vector.c print_assembly.c lexer.c -o out/compiler.out
	./test_ret.sh '6*(3+7)-5*1' s/full_compile004.s out/task004.out 55 out/compiler.out

full_compile005:
	gcc -Wall compiler2.c intmap.c vector.c print_assembly.c lexer.c -o out/compiler.out
	./test_ret.sh '043,41*3+07*010-0Xa/(010%(!!1+2))' s/full_compile005.s out/task005.out 174 out/compiler.out
	diff s/full_compile005.s s/print_assembly_check005.s

full_compile006:
	gcc -Wall compiler2.c intmap.c vector.c print_assembly.c lexer.c -o out/compiler.out
	./test_ret.sh '7*5,(12,41*3)+7*16/(9,2)-10/(8%3)' s/full_compile006.s out/task006.out 174 out/compiler.out
	diff s/full_compile006.s s/print_assembly_check006.s

full_compile007:
	gcc -Wall compiler2.c intmap.c vector.c print_assembly.c lexer.c -o out/compiler.out
	./test_ret.sh '7*5 	,	(0xC,(41   )*(4-(011>8)))+7*(((1-~1)>=3)<<4)/(9,(4>>(10<=10))+(3<3))-10/(	  ( 	!0  <<3)	%3)' s/full_compile007.s out/task007.out 174 out/compiler.out
	diff s/full_compile007.s s/print_assembly_check007.s

full_compile008:
	gcc -Wall compiler2.c intmap.c vector.c print_assembly.c lexer.c -o out/compiler.out
	./test_ret.sh '35,	((	41|	(8   !=     15))*  ((3==3)+2))+((5|2)*(9&10))   -   (10/(8%3))' s/full_compile008.s out/task008.out 174 out/compiler.out
	diff s/full_compile008.s s/print_assembly_check008.s

