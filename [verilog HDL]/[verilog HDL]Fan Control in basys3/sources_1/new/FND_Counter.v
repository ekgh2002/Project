`timescale 1ns / 1ps

module FND_Counter(
    input i_clk,
    input i_reset,
    output [1:0] o_counter
    );

    reg [1:0] r_counter;
    assign o_counter = r_counter;

    always @(posedge i_clk or posedge i_reset) begin
        if (i_reset) begin
            r_counter <= 0;
        end
        else begin
            r_counter <= r_counter + 1;
        end
    end

endmodule