wave add / -radix hex
restart

isim force add Bus2IP_Resetn 0
isim force add Bus2IP_Resetn 1 -time 10 ns
isim force add nes_data 0
isim force add Bus2IP_Clk 1 -value 0 -time 5 ns -repeat 10 ns

run 50 ms

isim force add nes_data 1

run 50 ms