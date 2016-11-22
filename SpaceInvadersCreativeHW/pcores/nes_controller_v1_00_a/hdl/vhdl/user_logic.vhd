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
-- Date:              Thu Nov 19 13:48:35 2015 (by Create and Import Peripheral Wizard)
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
    --USER ports added here
		LEDs													 : out std_logic_vector (7 downto 0);
		C1_PULSE											 : out std_logic;
		C1_LATCH											 : out std_logic;
		C1_DATA												 : in std_logic;
		C2_PULSE											 : out std_logic;
		C2_LATCH											 : out std_logic;
		C2_DATA												 : in std_logic;
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
  
  --Enum type
  type rx_state_type is
    (IDLE, WA, BA, WB, BB, WSE, BSE, WST, BST, WU, BU, WD, BD, WL, BL, WR, BR, DONE);
  
  --Constants
  constant READ_TIMER_LIMIT    	        : natural   := 1_000_000;
  constant PULSE_TIMER_LIMIT            : natural   := 1000;
  
  --USER signal declarations added here, as needed for user logic
  
  signal clrTimer, pulse_on, 
		data_done, latch, readbtn						: STD_LOGIC;
  signal c1_buttons, c1_buttons_next,
		c2_buttons, c2_buttons_next	        : STD_LOGIC_VECTOR(7 downto 0);
  signal rx_state_reg, rx_state_next		: rx_state_type;
  
  signal read_timer                     : std_logic_vector(19 downto 0); -- Timer to read buttons every 100 Hz
  signal pulse_timer                    : std_logic_vector(9 downto 0); -- Timer to space out pulses 
  ------------------------------------------
  -- Signals for user logic slave model s/w accessible register example
  ------------------------------------------
  signal slv_reg0                       : std_logic_vector(C_SLV_DWIDTH-1 downto 0);
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
        slv_reg0 <= (others => '0');
      else
        case slv_reg_write_sel is
          when "1" =>
            for byte_index in 0 to (C_SLV_DWIDTH/8)-1 loop
              if ( Bus2IP_BE(byte_index) = '1' ) then
                slv_reg0(byte_index*8+7 downto byte_index*8) <= Bus2IP_Data(byte_index*8+7 downto byte_index*8);
              end if;
            end loop;
          when others => 
						slv_reg0(31 downto 28) <= read_timer(19 downto 16); -- set highest bits of timer to top 4 bits of reg
						if(data_done = '1') then
							slv_reg0(15 downto 8) <= c2_buttons;
							slv_reg0(7 downto 0) <= c1_buttons;
						end if;
        end case;
      end if;
    end if;

  end process SLAVE_REG_WRITE_PROC;

  -- implement slave model software accessible register(s) read mux
  SLAVE_REG_READ_PROC : process( slv_reg_read_sel, slv_reg0 ) is
  begin

    case slv_reg_read_sel is
      when "1" => slv_ip2bus_data <= slv_reg0;
      when others => slv_ip2bus_data <= (others => '0');
    end case;

  end process SLAVE_REG_READ_PROC;

  ------------------------------------------
  -- Example code to drive IP to Bus signals
  ------------------------------------------
  IP2Bus_Data  <= slv_ip2bus_data when slv_read_ack = '1' else
                  (others => '0');

  IP2Bus_WrAck <= slv_write_ack;
  IP2Bus_RdAck <= slv_read_ack;
  IP2Bus_Error <= '0';
	
	-- Create counter for read timer and pulse timer
	counter: process (Bus2IP_Clk)
	begin
		if (Bus2IP_Resetn = '0') then
			read_timer <= (others => '0');
			pulse_timer <= (others => '0');
      rx_state_reg <= IDLE;
			c1_buttons <= (others => '0');
			c2_buttons <= (others => '0');
		elsif (Bus2IP_Clk 'event and Bus2IP_Clk = '1') then
			if read_timer = READ_TIMER_LIMIT then
        read_timer <= (others => '0'); -- Rollover read controller timer
				pulse_timer <= (others => '0'); -- Reset pulse timer
      else
				read_timer <= read_timer + 1;				
      end if;
			
			if(data_done = '1') then
				LEDs <= c1_buttons; -- Attach button values to LEDs
			end if;
			c1_buttons <= c1_buttons_next;
			c2_buttons <= c2_buttons_next;
			
			rx_state_reg <= rx_state_next;
			
			if(clrTimer='1' or pulse_timer = PULSE_TIMER_LIMIT) then
				pulse_timer <= (others => '0');
			else
				pulse_timer <= pulse_timer + 1;
			end if;
		end if;
	end process counter;

  --Control logic
  pulse_on <= '1' when pulse_timer >= PULSE_TIMER_LIMIT/2 else '0';
	
	latch <= '1' when read_timer > 0 and read_timer < PULSE_TIMER_LIMIT else '0';
	
	clrTimer <= '1' when rx_state_reg = IDLE or rx_state_reg = WA else '0';
	
	c1_buttons_next <= (others => '0') when latch = '1' else 
											c1_buttons(6 downto 0) & not C1_DATA when readBtn = '1' else
											c1_buttons;
	
	c2_buttons_next <= (others => '0') when latch = '1' else 
											c2_buttons(6 downto 0) & not C2_DATA when readBtn = '1' else
											c2_buttons;
      
  --Next state logic
  process(rx_state_reg, pulse_on, latch)
  begin
    --Defaults
    rx_state_next <= rx_state_reg;
    data_done <= '0';
		readBtn <= '0';
    case rx_state_reg is
      when IDLE => 
        if(latch = '1') then
          rx_state_next <= WA;
        end if;
			when WA => 
				if(latch = '0') then
					rx_state_next <= BA; -- set next state to BA
					readBtn <= '1';
				end if;
      when BA => 
				if(pulse_on = '1') then -- wait for pulse to go high to change state
          rx_state_next <= WB; -- set next state to WB
        end if;
      when WB => 
				if(pulse_on = '0') then -- wait for pulse to go low to change state
					rx_state_next <= BB;
					readBtn <= '1';
        end if;
			when BB => 
				if(pulse_on = '1') then -- wait for pulse to go high to change state
					rx_state_next <= WSE;
        end if;
      when WSE => 
        if(pulse_on = '0') then -- wait for pulse to go low to change state
          rx_state_next <= BSE;
					readBtn <= '1';
        end if;
			when BSE => 
        if(pulse_on = '1') then -- wait for pulse to go high to change state
          rx_state_next <= WST;
        end if;
			when WST => 
        if(pulse_on = '0') then -- wait for pulse to go low to change state
          rx_state_next <= BST;
					readBtn <= '1';
        end if;
			when BST => 
        if(pulse_on = '1') then -- wait for pulse to go high to change state
          rx_state_next <= WU;
        end if;
			when WU => 
        if(pulse_on = '0') then -- wait for pulse to go low to change state
          rx_state_next <= BU;
					readBtn <= '1';
        end if;
			when BU => 
        if(pulse_on = '1') then -- wait for pulse to go high to change state
          rx_state_next <= WD;
        end if;
			when WD => 
        if(pulse_on = '0') then -- wait for pulse to go low to change state
          rx_state_next <= BD;
					readBtn <= '1';
        end if;
			when BD => 
        if(pulse_on = '1') then -- wait for pulse to go high to change state
          rx_state_next <= WL;
        end if;
			when WL => 
        if(pulse_on = '0') then -- wait for pulse to go low to change state
          rx_state_next <= BL;
					readBtn <= '1';
        end if;
			when BL => 
        if(pulse_on = '1') then -- wait for pulse to go high to change state
          rx_state_next <= WR;
        end if;
			when WR => 
        if(pulse_on = '0') then -- wait for pulse to go low to change state
          rx_state_next <= BR;
					readBtn <= '1';
        end if;
			when BR => 
        if(pulse_on = '1') then -- wait for pulse to go high to change state
          rx_state_next <= DONE;
        end if;
      when DONE =>
        if(pulse_on = '0') then
					rx_state_next <= IDLE;
          data_done <= '1';
        end if;
			when others =>
				if(latch = '1') then
          rx_state_next <= WA;
				end if;
    end case;
  end process;
    
  --Output logic
	-- write buttons to slv register
	C1_PULSE <= pulse_on; -- assert pulse
	C1_LATCH <= latch; -- assert latch
	C2_PULSE <= pulse_on; -- assert pulse
	C2_LATCH <= latch; -- assert latch

end IMP;
