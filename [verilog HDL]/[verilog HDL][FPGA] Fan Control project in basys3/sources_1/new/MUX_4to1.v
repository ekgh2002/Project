`timescale 1ns / 1ps

module MUX_4to1(
    input [3:0] i_x,
    input [5:0] sel,
    input [4:0] i_button,
    input [6:0] i_sec,
    output o_y
    );

    reg r_y;
    assign o_y = r_y;

    always @(*) begin
        r_y = 1'b0;
        if(i_button) begin
            if(i_sec == 20)
            r_y <= i_x[0];
        end
        case (sel)
            6'd0 : begin
                r_y <= i_x[0];
            end
            6'd1 : begin
                r_y <= i_x[1];
            end
            6'd2 : begin
                r_y <= i_x[2];
            end
            6'd3 : begin
                r_y <= i_x[3];
            end
            6'd4 : begin
                r_y <= i_x[2];
            end
            6'd5 : begin
                r_y <= i_x[1];
            end
        endcase
    end
endmodule
