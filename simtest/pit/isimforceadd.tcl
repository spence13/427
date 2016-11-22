wave add / -radix hex
restart

isim force add Bus2IP_Clk 1 -value 0 -time 10 ns -repeat 20 ns


run 100 ns

isim force add NES_latch 0 -value 1 -time 10 us -value 0 -time 20 us 
isim force add NES_data 0 -value 1 -time 20 ns -repeat 20 ns
isim force add NES_clk 1 -value 0 -time 10 us -repeat 20 us


run 1000 us