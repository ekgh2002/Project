`timescale 1ns / 1ps

module FND_Value(
    input [5:0] i_fanState,
    output [3:0] o_1000_value, o_100_value, o_10_value, o_1_value
    );

    reg [3:0] r_1000_value, r_100_value, r_10_value, r_1_value;

    assign o_1000_value = r_1000_value;
    assign o_100_value = r_100_value;
    assign o_10_value = r_10_value;
    assign o_1_value = r_1_value;

    always @(*) begin
        r_1000_value = 4'd0;
        r_100_value = 4'd0;
        r_10_value = 4'd0;
        r_1_value = 4'd0;
        case (i_fanState)
            5'd0 : r_1_value = 5'd0;
            5'd1 : r_1_value = 5'd1;
            5'd2 : r_1_value = 5'd2;
            5'd3 : r_1_value = 5'd3;
            5'd4 : r_1_value = 5'd4;
            5'd5 : r_1_value = 5'd5;
        endcase
    end
endmodule
