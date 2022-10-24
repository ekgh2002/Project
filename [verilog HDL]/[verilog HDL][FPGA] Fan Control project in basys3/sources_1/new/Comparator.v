`timescale 1ns / 1ps

module Comparator(
    input [9:0] i_counter,
    output o_fan_0, o_fan_1, o_fan_2, o_fan_3
    );

    assign o_fan_0 = (i_counter < 0)   ? 1'b1 : 1'b0;
    assign o_fan_1 = (i_counter < 300) ? 1'b1 : 1'b0;
    assign o_fan_2 = (i_counter < 400) ? 1'b1 : 1'b0;
    assign o_fan_3 = (i_counter < 600) ? 1'b1 : 1'b0;
endmodule
