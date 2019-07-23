`default_nettype none
`timescale 1ns / 1ps

module keypad_decode #(
    parameter CLK_FREQ = 125e6,
    parameter POLL_PERIOD = 1e-3
)(
    input wire sysclk,
    input wire [3:0] row,
    (* fsm_encoding = "none" *) output logic [3:0] col = 'b0111,
    output logic [3:0] led = 0,
    output logic led6_b
);
    localparam int CLK_DIV_COUNT = CLK_FREQ * POLL_PERIOD;
    logic [$clog2(CLK_DIV_COUNT)-1:0] counter = 0;
    
    // worked fine
    // assign led6_b = 'b1;
    
    // logic tmp = 'b0;
    // always_ff @(posedge sysclk)
    //     tmp <= 'b1;
    // assign led6_b = tmp;

    logic tmp = 'b0;
    logic tmp2 = 'b0;
    logic tmp3 = 'b0;
    logic tmp4 = 'b0;
    assign led6_b = tmp & tmp2 & tmp3 & tmp4;    

    always_ff @(posedge sysclk)
        if (counter == CLK_DIV_COUNT - 1)
            counter <= 0;
        else
            counter <= counter + 1;

    always_ff @(posedge sysclk)
        if (counter == CLK_DIV_COUNT - 1)
            unique case (col)
            'b0111: col <= 'b1011;
            'b1011: col <= 'b1101;
            'b1101: col <= 'b1110;
            'b1110: col <= 'b0111;
            endcase

    always_ff @(posedge sysclk)
        if (counter == CLK_DIV_COUNT/2 - 1)    
            unique case (col)
            'b0111:
                unique case(row)
                'b0111: begin
                    led <= 'h1;
                    tmp <= 'b0;
                end
                'b1011: begin
                    led <= 'h4;
                    tmp <= 'b0;
                end
                'b1101: begin
                    led <= 'h7;
                    tmp <= 'b0;
                end
                'b1110: begin
                    led <= 'h0;
                    tmp <= 'b0;
                end
                default: tmp <= 'b1;
                endcase
            'b1011:
                unique case(row)
                'b0111: begin
                    led <= 'h2;
                    tmp2 <= 'b0;
                end
                'b1011: begin
                    led <= 'h5;
                    tmp2 <= 'b0;
                end
                'b1101: begin
                    led <= 'h8;
                    tmp2 <= 'b0;
                end
                'b1110: begin
                    led <= 'hF;
                    tmp2 <= 'b0;
                end
                default: tmp2 <= 'b1;
                endcase
            'b1101:
                unique case(row)
                'b0111: begin 
                    led <= 'h3;
                    tmp3 <= 'b0;
                end
                'b1011: begin 
                    led <= 'h6;
                    tmp3 <= 'b0;
                 end
                'b1101: begin 
                    led <= 'h9; 
                    tmp3 <= 'b0;
                end
                'b1110: begin 
                    led <= 'hE;
                    tmp3 <= 'b0;
                end
                default: tmp3 <= 'b1 ;
                endcase
            'b1110:
                unique case(row)
                'b0111: begin 
                    led <= 'hA; 
                    tmp4 <= 'b0;
                end
                'b1011: begin 
                    led <= 'hB; 
                    tmp4 <= 'b0;
                end
                'b1101: begin 
                    led <= 'hC;
                    tmp4 <= 'b0;
                end
                'b1110: begin
                    led <= 'hD;
                    tmp4 <= 'b0;
                end
                default: tmp4 <= 'b1 ;
                endcase
            endcase
endmodule
`default_nettype wire