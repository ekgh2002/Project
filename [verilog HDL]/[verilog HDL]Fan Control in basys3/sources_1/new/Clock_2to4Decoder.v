`timescale 1ns / 1ps

module Clock_2to4Decoder(
    input i_pwswitch,
    input [2:0] i_select,
    output [3:0] o_digitposition
    );

    reg [3:0] r_digitposition;
    assign o_digitposition = r_digitposition;

    always @(i_select, i_pwswitch) begin
        if(i_pwswitch) begin
            r_digitposition = 4'b1111;
        end

        else begin
            case (i_select)
                
                3'b000 : r_digitposition = 4'b1110;
                3'b001 : r_digitposition = 4'b1101;
                3'b010 : r_digitposition = 4'b1011;
                3'b011 : r_digitposition = 4'b0111;
                3'b100 : r_digitposition = 4'b1110;
                3'b101 : r_digitposition = 4'b1101;
                3'b110 : r_digitposition = 4'b1011;
                3'b111 : r_digitposition = 4'b0111;
                default : r_digitposition = 4'b1111;
            endcase
        end
    end
endmodule

