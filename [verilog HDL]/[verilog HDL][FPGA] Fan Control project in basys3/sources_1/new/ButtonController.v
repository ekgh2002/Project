`timescale 1ns / 1ps

module ButtonController(
    input i_clk,
    input i_reset,
    input i_button,
    output o_button
    );

    parameter   PUSHED = 1'b1,
                RELEASED = 1'b0,
                TRUE = 1'b1,
                FALSE = 1'b0;
    parameter   DEBOUNCE = 1_000_000;

    reg r_prevState = RELEASED;
    reg [31:0] r_counter = 0;
    reg r_button;
    assign o_button = r_button;

    always @(posedge i_clk or posedge i_reset) begin
        if (i_reset) begin
            r_prevState <= RELEASED;
            r_counter <= 0;
            r_button <= FALSE;
        end
        else begin
            if (i_button == PUSHED && r_prevState == RELEASED && r_counter < DEBOUNCE) begin
                r_counter <= r_counter + 1;
                r_button <= FALSE;
            end
            else if (i_button == PUSHED && r_prevState == RELEASED && r_counter == DEBOUNCE) begin
                r_prevState <= PUSHED;
                r_counter <= 0;
                r_button <= FALSE;
            end
            else if (i_button == RELEASED && r_prevState == PUSHED && r_counter < DEBOUNCE) begin
                r_counter <= r_counter + 1;
                r_button <= FALSE;
            end
            else if (i_button == RELEASED && r_prevState == PUSHED && r_counter == DEBOUNCE) begin
                r_prevState <= RELEASED;
                r_counter <= 0;
                r_button <= TRUE;
            end
            else begin
                r_button <= FALSE;
            end
        end
        
    end

endmodule
