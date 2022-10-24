`timescale 1ns / 1ps

module FAN_Counter(
    input i_clk,
    input i_reset,
    output [9:0] o_counter
    );

    reg [9:0] r_counter = 0;
    assign o_counter = r_counter;

    always @(posedge i_clk or posedge i_reset) begin
        if(i_reset) begin
            r_counter <= 0;
        end
        else begin
            if(r_counter == 999) begin
                r_counter <= 0;
            end
            else begin
                r_counter <= r_counter + 1;
            end
        end
    end
endmodule
