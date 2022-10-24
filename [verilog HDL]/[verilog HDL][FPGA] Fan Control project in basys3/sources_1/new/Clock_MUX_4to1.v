`timescale 1ns / 1ps

module Clock_MUX_4to1(
    input [3:0] i_a, i_b, i_c, i_d, i_a1, i_b1, i_c1, i_d1,
    input [2:0] i_sel,
    output [3:0] o_y
    );

    reg [3:0] r_y;
    assign o_y = r_y;

    always @(*) begin
        case (i_sel)
            3'b000 : r_y = i_a;
            3'b001 : r_y = i_b;
            3'b010 : r_y = i_c;
            3'b011 : r_y = i_d;
            3'b100 : r_y = i_a1;
            3'b101 : r_y = i_b1;
            3'b110 : r_y = i_c1;
            3'b111 : r_y = i_d1;
        endcase
    end
endmodule
