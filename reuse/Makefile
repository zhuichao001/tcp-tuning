all:
	g++ -o run_reuse_addr_svr svr_reuse_addr.cpp -std=c++11 -lpthread -I..
	g++ -o run_reuse_port_svr svr_reuse_port.cpp -std=c++11 -lpthread -I.. 
	g++ -o run_client client.cpp
clean:
	rm -f run_reuse_addr_svr
	rm -f run_reuse_port_svr
	rm -f run_client
