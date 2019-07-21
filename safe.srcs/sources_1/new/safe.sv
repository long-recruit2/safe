`default_nettype none
`timescale 1ns / 1ps

module safe #(
    parameter CLK_FREQ = 125e6,
    parameter POLL_PERIOD = 1e-3
)(
    input wire sysclk,
    inout wire [3:0] row,
    (* fsm_encoding = "none" *) output logic [3:0] col,
    output logic [3:0] led
    // this does not work, why?
    // output logic [3:0] led = 0
    );

    logic [3:0] key;

    keypad_decode #(CLK_FREQ, POLL_PERIOD) decode(
    .sysclk(sysclk),
    .row(row),
    .col(col),
    .led(key)
    );
    
    
    
    // logic [$clog2(CLK_FREQ * 4)-1:0] four_sec = 0;
    localparam int TIMER = CLK_FREQ;
    logic [$clog2(TIMER)-1:0] four_sec = 0;

    always_ff @(posedge sysclk)
        if(four_sec == TIMER - 1)
            four_sec <= 0;
        else
            four_sec <= four_sec + 1;

    logic [3:0] key2 = 'h0;
    logic switch = 'b1;
    always_ff @(posedge sysclk)    
        if (four_sec == 'h0)
            switch <= ~switch;

    always_ff @(posedge sysclk)
        if (four_sec == 'h0 && switch == 'b0)
            key2 <= 'h0;
        else if  (four_sec == 'h0 && switch == 'b1)
            key2 <= key;
    
    assign led = key2;
    
endmodule
`default_nettype wire
