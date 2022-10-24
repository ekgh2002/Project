`timescale 1ns / 1ps

module Fan_Mode(
    input i_clk,
    input i_reset,
    input [4:0] i_button,
    input [1:0] i_switch,
    input [6:0] i_sec,
    output [5:0] o_fanState
    ); 

    parameter   S_FAN_0 = 6'd0,
                S_FAN_1 = 6'd1,
                S_FAN_2 = 6'd2,
                S_FAN_3 = 6'd3,
                S_FAN_4 = 6'd4,
                S_FAN_5 = 6'd5,
                S_FAN_6 = 6'd6;
    
    reg [5:0] r_curState = S_FAN_0;
    reg [5:0] r_nextState;
    reg [5:0] r_fanState;
    assign o_fanState = r_fanState;

    /* 상태 변경 */
    always @(posedge i_clk or posedge i_reset) begin
        if (i_reset)    r_curState <= S_FAN_0;
        else            r_curState <= r_nextState;
    end

    /* 이벤트 처리 */
    always @(i_button or r_curState or i_switch or i_sec) begin
        r_nextState <= S_FAN_0;

        case (r_curState)
            S_FAN_0 : begin
                if      (i_button[0])     r_nextState <= S_FAN_1;
                else if (i_button[1])     r_nextState <= S_FAN_3;
                else                      r_nextState <= S_FAN_0;
            end
            S_FAN_1 : begin
                if     (i_button[0])      r_nextState <= S_FAN_2;
                else if(i_button[1])      r_nextState <= S_FAN_0;
                // else if(i_button[2])      r_nextState <= S_FAN_0;
                else if(i_button[2])      r_nextState <= S_FAN_4;
                else if(i_button[3])      r_nextState <= S_FAN_5;
                else                      r_nextState <= S_FAN_1;
            end
            S_FAN_2 : begin
                if     (i_button[0])      r_nextState <= S_FAN_3;
                else if(i_button[1])      r_nextState <= S_FAN_1;
                // else if(i_button[2])      r_nextState <= S_FAN_0;
                else if(i_button[2])      r_nextState <= S_FAN_4;
                else if(i_button[3])      r_nextState <= S_FAN_5;
                else                      r_nextState <= S_FAN_2;
            end
            S_FAN_3 : begin
                if     (i_button[0])      r_nextState <= S_FAN_0;
                else if(i_button[1])      r_nextState <= S_FAN_2;
                // else if(i_button[2])      r_nextState <= S_FAN_0;
                else if(i_button[2])      r_nextState <= S_FAN_4;
                else if(i_button[3])      r_nextState <= S_FAN_5;
                else                      r_nextState <= S_FAN_3;
            end
            S_FAN_4 : begin
                if      (i_button[2])         r_nextState <= S_FAN_0;
                else if (i_sec == 50)         r_nextState <= S_FAN_0;
                else                          r_nextState <= S_FAN_4;
            end
            S_FAN_5 : begin
                if      (i_button[3])         r_nextState <= S_FAN_0;
                else if (i_sec == 40)         r_nextState <= S_FAN_0;
                else                          r_nextState <= S_FAN_5;
            end
        endcase
    end

    /* 상태에 따른 동작 */
    always @(r_curState) begin
        case (r_curState)
            S_FAN_0 : r_fanState <= 6'd0;
            S_FAN_1 : r_fanState <= 6'd1;
            S_FAN_2 : r_fanState <= 6'd2;
            S_FAN_3 : r_fanState <= 6'd3;
            S_FAN_4 : r_fanState <= 6'd4;
            S_FAN_5 : r_fanState <= 6'd5;
        endcase
    end

endmodule
