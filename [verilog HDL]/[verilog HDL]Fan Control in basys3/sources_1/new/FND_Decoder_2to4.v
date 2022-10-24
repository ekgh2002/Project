`timescale 1ns / 1ps

module FND_Decoder_2to4(
    input [1:0] i_digitPosition,
    output [3:0] o_Digit
    );

    reg [3:0] r_Digit;
    assign o_Digit = r_Digit;

    always @(*) begin
        case (i_digitPosition)
            2'b00 : r_Digit = 4'b1110;
            2'b01 : r_Digit = 4'b1101;
            2'b10 : r_Digit = 4'b1011;
            2'b11 : r_Digit = 4'b0111;
        endcase
    end
endmodule