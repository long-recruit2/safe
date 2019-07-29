`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2019/07/25 20:36:33
// Design Name: 
// Module Name: clkgen
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module clkgen #(
    parameter THIS_CLK_FREQ = 125e6
)(
    input wire sysclk,
    output logic clk
);

    // logic [$clog2(THIS_CLK_FREQ)-1:0] counter = 0;
    logic [$clog2(THIS_CLK_FREQ)-1:0] counter = 0;
    always_ff @(posedge sysclk)
        if(counter == THIS_CLK_FREQ -1)
            counter <= 0;
        else
            counter <= counter + 1;

    //logic clk = 0;
    always_ff @(posedge sysclk) 
        if(counter == THIS_CLK_FREQ - 1)
            clk <= 'b1;
        else if(counter == THIS_CLK_FREQ/2 - 1)
            clk <= 'b0;

endmodule
