`default_nettype none
`timescale 1ns / 1ps

module keypad_decode #(
    parameter CLK_FREQ = 125e6,
    parameter POLL_PERIOD = 1e-3
)(
    input wire sysclk,
    input wire [3:0] row,
    (* fsm_encoding = "none" *) output logic [3:0] col = 'b0111,
    output logic [3:0] led = 0
);
    localparam int CLK_DIV_COUNT = CLK_FREQ * POLL_PERIOD;
    logic [$clog2(CLK_DIV_COUNT)-1:0] counter = 0;
    
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
                'b0111: led <= 'h1;
                'b1011: led <= 'h4;
                'b1101: led <= 'h7;
                'b1110: led <= 'h0;
                default:;
                endcase
            'b1011:
                unique case(row)
                'b0111: led <= 'h2;
                'b1011: led <= 'h5;
                'b1101: led <= 'h8;
                'b1110: led <= 'hF;
                default:;
                endcase
            'b1101:
                unique case(row)
                'b0111: led <= 'h3;
                'b1011: led <= 'h6;
                'b1101: led <= 'h9;
                'b1110: led <= 'hE;
                default:;
                endcase
            'b1110:
                unique case(row)
                'b0111: led <= 'hA;
                'b1011: led <= 'hB;
                'b1101: led <= 'hC;
                'b1110: led <= 'hD;
                default:;
                endcase
            endcase
endmodule
`default_nettype wire