`timescale 1ns / 1ps

module FND_Value_MUX(
    input [3:0] i_1000_value, i_100_value, i_10_value, i_1_value,
    input [1:0] i_digitPosition,
    output [3:0] o_value
    );

    reg [3:0] r_value;
    assign o_value = r_value;

    always @(*) begin
        case (i_digitPosition)
            2'b00 : r_value = i_1_value;
            2'b01 : r_value = i_10_value;
            2'b10 : r_value = i_100_value;
            2'b11 : r_value = i_1000_value;
        endcase
    end
endmodule