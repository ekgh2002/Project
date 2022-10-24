`timescale 1ns / 1ps

module FND_DigitDivider(  //  형태는 똑같으므로 하나만 만들어도 됨.
    input [6:0] i_a, i_b,
    output [3:0] o_a10, o_a1, o_b10, o_b1
    );

    assign o_a1 = i_a % 10;
    assign o_a10 = i_a / 10 % 10;
    assign o_b1 = i_b % 10;
    assign o_b10 = i_b / 10 % 10;


endmodule
