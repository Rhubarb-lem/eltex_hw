shop :  libcustomer.a
	gcc shop.c -o shop -L.  -lcustomer -lpthread
customer.o : customer.c
		gcc customer.c -c 
libcustomer.a : customer.o
	ar rc libcustomer.a customer.o  

run: shop shop
	./shop
clean: 
	rm *.a
	rm *.o
	rm shop