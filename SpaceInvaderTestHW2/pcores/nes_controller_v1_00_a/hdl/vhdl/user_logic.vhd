------------------------------------------------------------------------------
-- user_logic.vhd - entity/architecture pair
------------------------------------------------------------------------------
--
-- ***************************************************************************
-- ** Copyright (c) 1995-2011 Xilinx, Inc.  All rights reserved.            **
-- **                                                                       **
-- ** Xilinx, Inc.                                                          **
-- ** XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS"         **
-- ** AS A COURTESY TO YOU, SOLELY FOR USE IN DEVELOPING PROGRAMS AND       **
-- ** SOLUTIONS FOR XILINX DEVICES.  BY PROVIDING THIS DESIGN, CODE,        **
-- ** OR INFORMATION AS ONE POSSIBLE IMPLEMENTATION OF THIS FEATURE,        **
-- ** APPLICATION OR STANDARD, XILINX IS MAKING NO REPRESENTATION           **
-- ** THAT THIS IMPLEMENTATION IS FREE FROM ANY CLAIMS OF INFRINGEMENT,     **
-- ** AND YOU ARE RESPONSIBLE FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE      **
-- ** FOR YOUR IMPLEMENTATION.  XILINX EXPRESSLY DISCLAIMS ANY              **
-- ** WARRANTY WHATSOEVER WITH RESPECT TO THE ADEQUACY OF THE               **
-- ** IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OR        **
-- ** REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE FROM CLAIMS OF       **
-- ** INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS       **
-- ** FOR A PARTICULAR PURPOSE.                                             **
-- **                                                                       **
-- ***************************************************************************
--
------------------------------------------------------------------------------
-- Filename:          user_logic.vhd
-- Version:           1.00.a
-- Description:       User logic.
-- Date:              Wed Nov 09 14:59:47 2016 (by Create and Import Peripheral Wizard)
-- VHDL Standard:     VHDL'93
------------------------------------------------------------------------------
-- Naming Conventions:
--   active low signals:                    "*_n"
--   clock signals:                         "clk", "clk_div#", "clk_#x"
--   reset signals:                         "rst", "rst_n"
--   generics:                              "C_*"
--   user defined types:                    "*_TYPE"
--   state machine next state:              "*_ns"
--   state machine current state:           "*_cs"
--   combinatorial signals:                 "*_com"
--   pipelined or register delay signals:   "*_d#"
--   counter signals:                       "*cnt*"
--   clock enable signals:                  "*_ce"
--   internal version of output port:       "*_i"
--   device pins:                           "*_pin"
--   ports:                                 "- Names begin with Uppercase"
--   processes:                             "*_PROCESS"
--   component instantiations:              "<ENTITY_>I_<#|FUNC>"
------------------------------------------------------------------------------

-- DO NOT EDIT BELOW THIS LINE --------------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

library proc_common_v3_00_a;
use proc_common_v3_00_a.proc_common_pkg.all;

-- DO NOT EDIT ABOVE THIS LINE --------------------

--USER libraries added here

------------------------------------------------------------------------------
-- Entity section
------------------------------------------------------------------------------
-- Definition of Generics:
--   C_NUM_REG                    -- Number of software accessible registers
--   C_SLV_DWIDTH                 -- Slave interface data bus width
--
-- Definition of Ports:
--   Bus2IP_Clk                   -- Bus to IP clock
--   Bus2IP_Resetn                -- Bus to IP reset
--   Bus2IP_Data                  -- Bus to IP data bus
--   Bus2IP_BE                    -- Bus to IP byte enables
--   Bus2IP_RdCE                  -- Bus to IP read chip enable
--   Bus2IP_WrCE                  -- Bus to IP write chip enable
--   IP2Bus_Data                  -- IP to Bus data bus
--   IP2Bus_RdAck                 -- IP to Bus read transfer acknowledgement
--   IP2Bus_WrAck                 -- IP to Bus write transfer acknowledgement
--   IP2Bus_Error                 -- IP to Bus error response
------------------------------------------------------------------------------

entity user_logic is
  generic
  (
    -- ADD USER GENERICS BELOW THIS LINE ---------------
    --USER generics added here
    -- ADD USER GENERICS ABOVE THIS LINE ---------------

    -- DO NOT EDIT BELOW THIS LINE ---------------------
    -- Bus protocol parameters, do not add to or delete
    C_NUM_REG                      : integer              := 1;
    C_SLV_DWIDTH                   : integer              := 32
    -- DO NOT EDIT ABOVE THIS LINE ---------------------
  );
  port
  (
    -- ADD USER PORTS BELOW THIS LINE ------------------
		NES_interrupt									: out std_logic;
		NES_data                     	: in  std_logic;
<<<<<<< HEAD
		NES_latch                     : out  std_logic;
		NES_pulse                     : out  std_logic;
		--buttons                     : out  std_logic_vector(7 downto 0);
=======
		NES_latch                     : in  std_logic;
		NES_clk                     	: in  std_logic;
>>>>>>> 46133ae2d6a022a717e89ee90e8959352f12048c
    -- ADD USER PORTS ABOVE THIS LINE ------------------

    -- DO NOT EDIT BELOW THIS LINE ---------------------
    -- Bus protocol ports, do not add to or delete
    Bus2IP_Clk                     : in  std_logic;
    Bus2IP_Resetn                  : in  std_logic;
    Bus2IP_Data                    : in  std_logic_vector(C_SLV_DWIDTH-1 downto 0);
    Bus2IP_BE                      : in  std_logic_vector(C_SLV_DWIDTH/8-1 downto 0);
    Bus2IP_RdCE                    : in  std_logic_vector(C_NUM_REG-1 downto 0);
    Bus2IP_WrCE                    : in  std_logic_vector(C_NUM_REG-1 downto 0);
    IP2Bus_Data                    : out std_logic_vector(C_SLV_DWIDTH-1 downto 0);
    IP2Bus_RdAck                   : out std_logic;
    IP2Bus_WrAck                   : out std_logic;
    IP2Bus_Error                   : out std_logic
    -- DO NOT EDIT ABOVE THIS LINE ---------------------
  );

  attribute MAX_FANOUT : string;
  attribute SIGIS : string;

  attribute SIGIS of Bus2IP_Clk    : signal is "CLK";
  attribute SIGIS of Bus2IP_Resetn : signal is "RST";

end entity user_logic;

------------------------------------------------------------------------------
-- Architecture section
------------------------------------------------------------------------------

architecture IMP of user_logic is
<<<<<<< HEAD
  --USER signal declarations added here, as needed for user logic
	constant sample : integer range 0 to 17 := 0;
	constant latch0 : integer range 0 to 17 := 1;
	constant latch1 : integer range 0 to 17 := 2;
	constant button : integer range 0 to 17 := 3;
	
	signal current_state, next_state : integer range 0 to 17 := 0;
	signal sample_counter, sample_counter_next : integer range 0 to 1666666 := 0;
	signal pulse_counter, pulse_counter_next : integer range 0 to 600 := 0;
	signal button_cycle : integer range 0 to 16 := 0;
	signal latch_en, pulse_en : std_logic := '0';
=======

	-- w is for wait state, b is for button found state
	constant w : integer range 0 to 17 := 0;
	constant w0 : integer range 0 to 17 := 1;
	constant b0 : integer range 0 to 17 := 2;
	constant w1 : integer range 0 to 17 := 3;
	constant b1 : integer range 0 to 17 := 4;
	constant w2 : integer range 0 to 17 := 5;
	constant b2 : integer range 0 to 17 := 6;
	constant w3 : integer range 0 to 17 := 7;
	constant b3 : integer range 0 to 17 := 8;
	constant w4 : integer range 0 to 17 := 9;
	constant b4 : integer range 0 to 17 := 10;
	constant w5 : integer range 0 to 17 := 11;
	constant b5 : integer range 0 to 17 := 12;
	constant w6 : integer range 0 to 17 := 13;
	constant b6 : integer range 0 to 17 := 14;
	constant w7 : integer range 0 to 17 := 15;
	constant b7 : integer range 0 to 17 := 16;
	constant w8 : integer range 0 to 17 := 17;

	signal current_state, next_state : integer range 0 to 17 := 0;
  --USER signal declarations added here, as needed for user logic

>>>>>>> 46133ae2d6a022a717e89ee90e8959352f12048c
  ------------------------------------------
  -- Signals for user logic slave model s/w accessible register example
  ------------------------------------------
  signal slv_reg0                       : std_logic_vector(C_SLV_DWIDTH-1 downto 0) := (others=>'0');
<<<<<<< HEAD
=======
  signal prev_reg                       : std_logic_vector(C_SLV_DWIDTH-1 downto 0) := (others=>'0');
>>>>>>> 46133ae2d6a022a717e89ee90e8959352f12048c
  signal slv_reg_write_sel              : std_logic_vector(0 to 0);
  signal slv_reg_read_sel               : std_logic_vector(0 to 0);
  signal slv_ip2bus_data                : std_logic_vector(C_SLV_DWIDTH-1 downto 0);
  signal slv_read_ack                   : std_logic;
  signal slv_write_ack                  : std_logic;

begin

  --USER logic implementation added here

  ------------------------------------------
  -- Example code to read/write user logic slave model s/w accessible registers
  -- 
  -- Note:
  -- The example code presented here is to show you one way of reading/writing
  -- software accessible registers implemented in the user logic slave model.
  -- Each bit of the Bus2IP_WrCE/Bus2IP_RdCE signals is configured to correspond
  -- to one software accessible register by the top level template. For example,
  -- if you have four 32 bit software accessible registers in the user logic,
  -- you are basically operating on the following memory mapped registers:
  -- 
  --    Bus2IP_WrCE/Bus2IP_RdCE   Memory Mapped Register
  --                     "1000"   C_BASEADDR + 0x0
  --                     "0100"   C_BASEADDR + 0x4
  --                     "0010"   C_BASEADDR + 0x8
  --                     "0001"   C_BASEADDR + 0xC
  -- 
  ------------------------------------------
  slv_reg_write_sel <= Bus2IP_WrCE(0 downto 0);
  slv_reg_read_sel  <= Bus2IP_RdCE(0 downto 0);
  slv_write_ack     <= Bus2IP_WrCE(0);
  slv_read_ack      <= Bus2IP_RdCE(0);

  -- implement slave model software accessible register(s)
  SLAVE_REG_WRITE_PROC : process( Bus2IP_Clk ) is
  begin

    if Bus2IP_Clk'event and Bus2IP_Clk = '1' then
      if Bus2IP_Resetn = '0' then
<<<<<<< HEAD
				current_state <= sample; --reset NES state machine
				sample_counter <= 0;
				pulse_counter <= 0;
      else
				current_state <= next_state;
				sample_counter <= sample_counter_next;
				pulse_counter <= pulse_counter_next;
      end if;
    end if;
  end process SLAVE_REG_WRITE_PROC;
	
	
	NES_LOGIC : process(current_state, sample_counter, pulse_counter) is
	begin
		-----------NES Controller FSM
		if current_state = sample then
			NES_interrupt <= '0';
			if sample_counter = 1666666 then
				latch_en <= '1';
				NES_latch <= '1';
				sample_counter_next <= 0;
				next_state <= latch0;
			else
				sample_counter_next <= sample_counter_next+1;
			end if;
		elsif current_state = latch0 then
			if pulse_counter = 600 then
					next_state <= latch1;
					pulse_counter_next <= 0;
			else
					next_state <= latch0;
					pulse_counter_next <= pulse_counter_next+1;
			end if;
		elsif current_state = latch1 then
			if latch_en = '1' and pulse_counter_next = 600 then
					next_state <= button;
					pulse_counter_next <= 0;
					latch_en <= '0';
					NES_latch <= '0';
					slv_reg0(button_cycle) <= not NES_data;
					--buttons(button_cycle) <= not NES_data;
					button_cycle <= button_cycle+1;
			else
					next_state <= latch1;
					pulse_counter_next <= pulse_counter_next+1;
			end if;
		elsif current_state = button then
			if button_cycle = 9 then
				pulse_en <= '0';
				NES_pulse <= '0';
				button_cycle <= 0;
				pulse_counter_next <= 0;
				NES_interrupt <= '1';
				next_state <= sample;
			elsif pulse_en = '0' and pulse_counter = 600 then
				pulse_en <= '1';
				pulse_counter_next <= 0;
				NES_pulse <= '1';
				next_state <= button;
			elsif pulse_en = '1' and pulse_counter = 600 then
				pulse_en <= '0';
				pulse_counter_next <= 0;
				NES_pulse <= '0';
				next_state <= button;
				slv_reg0(button_cycle) <= not NES_data;
					--buttons(button_cycle) <= not NES_data;
				button_cycle <= button_cycle+1;
			else
				pulse_counter_next <= pulse_counter_next+1;
			end if;
		else
			NES_interrupt <= '0';
		end if;
	end process NES_LOGIC;
=======
				current_state <= w; --reset NES state machine
        --slv_reg0 <= (others => '0');
				prev_reg <= (others => '0');
				NES_interrupt <= '0';
      else
				current_state <= next_state;
				if prev_reg = slv_reg0 then
					NES_interrupt <= '0';
				else
					NES_interrupt <= '1';
				end if;
				
				prev_reg <= slv_reg0;
				
      end if;
    end if;

  end process SLAVE_REG_WRITE_PROC;
>>>>>>> 46133ae2d6a022a717e89ee90e8959352f12048c

  -- implement slave model software accessible register(s) read mux
  SLAVE_REG_READ_PROC : process( slv_reg_read_sel, slv_reg0 ) is
  begin

    case slv_reg_read_sel is
      when "1" => slv_ip2bus_data <= slv_reg0;
      when others => slv_ip2bus_data <= (others => '0');
    end case;
<<<<<<< HEAD
  end process SLAVE_REG_READ_PROC;
end IMP;
=======

  end process SLAVE_REG_READ_PROC;

	-----------NES Controller FSM
	NES_logic:process(current_state, NES_latch, NES_clk, NES_data)
    begin
		case current_state is
			when w =>
				if NES_latch = '1' then
						next_state <= w0;
				else
						next_state <= w;
				end if;
			when w0 =>
				if NES_latch = '0' then
						next_state <= b0;
						slv_reg0(0) <= not NES_data;
				else
						next_state <= w0;
				end if;
			when b0 =>
				if NES_clk = '1' then
						next_state <= w1;
				else
						next_state <= b0;
				end if;
			when w1 =>
				if NES_clk = '0' then
						next_state <= b1;
						slv_reg0(1) <= not NES_data;
				else
						next_state <= w1;
				end if;
			when b1 =>
				if NES_clk = '1' then
						next_state <= w2;
				else
						next_state <= b1;
				end if;
			when w2 =>
				if NES_clk = '0' then
						next_state <= b2;
						slv_reg0(2) <= not NES_data;
				else
						next_state <= w2;
				end if;
			when b2 =>
				if NES_clk = '1' then
						next_state <= w3;
				else
						next_state <= b2;
				end if;
			when w3 =>
				if NES_clk = '0' then
						next_state <= b3;
						slv_reg0(3) <= not NES_data;
				else
						next_state <= w3;
				end if;
			when b3 =>
				if NES_clk = '1' then
						next_state <= w4;
				else
						next_state <= b3;
				end if;
			when w4 =>
				if NES_clk = '0' then
						next_state <= b4;
						slv_reg0(4) <= not NES_data;
				else
						next_state <= w4;
				end if;
			when b4 =>
				if NES_clk = '1' then
						next_state <= w5;
				else
						next_state <= b4;
				end if;
			when w5 =>
				if NES_clk = '0' then
						next_state <= b5;
						slv_reg0(5) <= not NES_data;
				else
						next_state <= w5;
				end if;
			when b5 =>
				if NES_clk = '1' then
						next_state <= w6;
				else
						next_state <= b5;
				end if;
			when w6 =>
				if NES_clk = '0' then
						next_state <= b6;
						slv_reg0(6) <= not NES_data;
				else
						next_state <= w6;
				end if;
			when b6 =>
				if NES_clk = '1' then
						next_state <= w7;
				else
						next_state <= b6;
				end if;
			when w7 =>
				if NES_clk = '0' then
						next_state <= b7;
						slv_reg0(7) <= not NES_data;
				else
						next_state <= w7;
				end if;
			when b7 =>
				if NES_clk = '1' then
						next_state <= w8;
				else
						next_state <= b7;
				end if;
			when w8 =>
				if NES_clk = '0' then
						next_state <= w;
				else
						next_state <= w8;
				end if;
			when others =>
				if NES_latch = '1' then
						next_state <= w0;
				end if;
		end case;
	end process NES_logic;
end IMP;
>>>>>>> 46133ae2d6a022a717e89ee90e8959352f12048c
