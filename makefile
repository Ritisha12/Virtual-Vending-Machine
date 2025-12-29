vendingmachine: vendingmachine.c inventory.c payment.c purchase.c
	gcc -o vendingmachine vendingmachine.c inventory.c payment.c purchase.c -lm

bctest: unit_testing.c inventory.c payment.c purchase.c
	gcc --coverage -o bctest unit_testing.c inventory.c payment.c purchase.c -lm
	
vendingmachine_cov: vendingmachine.c inventory.c payment.c purchase.c
	gcc --coverage -o vendingmachine_cov vendingmachine.c inventory.c payment.c purchase.c -lm

gui:
	gcc -o guiMachine receiptGUI.c `pkg-config --cflags --libs gtk+-3.0`

clean:
	rm -f vendingmachine bctest vendingmachine_cov *.o *.gcno *.gcda *.gcov test_results.txt
