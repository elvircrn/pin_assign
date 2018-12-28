library ieee ;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;

entity display is
  port( 
    number: in std_logic_vector(12 downto 0); -- @ 87 86 85 84 88 25 23
    segments: out std_logic_vector(10 downto 0);
    enable: out std_logic 
      );
end display;

architecture behav of display is
begin
  process(clk)
  begin
    segments <= (others => '0');
  end process;
end behav;
