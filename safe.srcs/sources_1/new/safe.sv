`default_nettype none
`timescale 1ns / 1ps

module safe #(
    parameter CLK_FREQ = 125e6,
    parameter POLL_PERIOD = 1e-3
)(
    input wire sysclk,
    inout wire [3:0] row,
    (* fsm_encoding = "none" *) output logic [3:0] col,
    output logic [3:0] led,
    output logic led6_b,
    output logic led5_b,
    output logic led5_r,
    output logic [7:0] jb
    );

    logic [3:0] key;
    // logic [3:0] key = 0; // this doesnt work, apparantly its wire
    
    logic all_not_pressed;
    // logic all_not_pressed = 0; // this doesnt work

    keypad_decode #(CLK_FREQ, POLL_PERIOD) decode(
    .sysclk(sysclk),
    .row(row),
    .col(col),
    .led(key),
    .all_not_pressed(all_not_pressed)
    );
    
    always_comb
        led6_b = all_not_pressed;
    
    logic prev_all_not_pressed = 0;
    always_ff @(posedge sysclk) begin
        prev_all_not_pressed <= all_not_pressed;
    end
    
    logic [3:0] keys[2:0];
    always_ff @(posedge sysclk) begin
        if (key == 'hA) begin
            if(keys[2] == 'h1 && keys[1] == 'h2 && keys[0] == 'h3) begin
                led5_b <= 'b1;
                led5_r <= 'b0;
            end
            else begin
                led5_b <= 'b0;
                led5_r <= 'b1;
            end
        end
        else if (prev_all_not_pressed == 'b0 && all_not_pressed == 'b1) begin
            keys <= {keys[1:0], key};
            led5_b <= 'b0;
            led5_r <= 'b0;            
        end
    end
    
    always_comb    
        led = key == 'hA ? keys[2] : key;
    
    /*
	output CS;
    output SDIN;
    output SCLK;
    output DC;
    output RES;
    output VBAT;
    output VDD;    
    */
    
    localparam int M10_CLK = CLK_FREQ * 10 / 125;
    logic [$clog2(M10_CLK)-1:0] m10counter = 0;
    always_ff @(posedge sysclk)
        if (m10counter == M10_CLK -1)
            m10counter <= 0;
        else
            m10counter <= m10counter + 1;

    logic m10clk = 0;
    always_ff @(posedge sysclk) begin
        if(m10counter == M10_CLK -1)
            m10clk <= 'b1;
        else if(m10counter == M10_CLK/2 -1)
            m10clk <= 'b0;
    end    
    
    PmodOLEDCtrl ctrl(
        // .CLK(sysclk),
        .CLK(m10clk),
        .RST(),
        .CS(jb[0]),
        .SDIN(jb[1]),
        .SCLK(jb[3]),
        .DC(jb[4]),
        .RES(jb[5]),
        .VBAT(jb[6]),
        .VDD(jb[7])
    );
    /*
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
    */    
endmodule
`default_nettype wire
