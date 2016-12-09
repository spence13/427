wave add / -radix hex
restart

isim force add Bus2IP_Resetn 0
isim force add Bus2IP_Resetn 1 -time 10 ns
isim force add Bus2IP_Clk 1 -value 0 -time 5 ns -repeat 10 ns

run 20 ns

isim force add slv_reg1 307200
isim force add slv_reg2 0
isim force add slv_reg3 307200

run 20 ns 

put mst_go 1

run 20 ms

put mst_go 1

run 20 ms

put mst_go 1

run 20 ms