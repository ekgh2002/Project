`timescale 1ns / 1ps


module Clock_Comparator(
    input [6:0] i_msec,
    output [3:0] o_fndDP
    );

    assign o_fndDP = (i_msec < 50) ? 11 : 10;
endmodule
