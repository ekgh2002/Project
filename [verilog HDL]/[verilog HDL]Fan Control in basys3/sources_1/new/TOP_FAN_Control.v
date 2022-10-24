`timescale 1ns / 1ps

module TOP_FAN_Control(
    input i_clk,
    input i_reset,
    input [4:0] i_button,
    input [1:0] i_switch,
    input i_modeSW,
    output o_motor,
    output [3:0] o_light,
    output [3:0] o_FND_Digit,
    output [7:0] o_FND_Font
    );


    // Motor
    wire w_1M_clk;
    FAN_ClockDivider clk_1M(
        .i_clk(i_clk),
        .i_reset(i_reset),
        .o_clk(w_1M_clk)
    );

    wire [9:0] w_counter;
    FAN_Counter fan_count(
        .i_clk(w_1M_clk),
        .i_reset(i_reset),
        .o_counter(w_counter)
    );

    wire [3:0] w_fan;
    Comparator comp(
        .i_counter(w_counter),
        .o_fan_0(w_fan[0]), 
        .o_fan_1(w_fan[1]), 
        .o_fan_2(w_fan[2]), 
        .o_fan_3(w_fan[3])
    );

    wire [3:0] w_button; 
    ButtonController upbtn(
        .i_clk(i_clk),
        .i_reset(i_reset),
        .i_button(i_button[0]),
        .o_button(w_button[0])
    );

    ButtonController downbtn(
        .i_clk(i_clk),
        .i_reset(i_reset),
        .i_button(i_button[1]),
        .o_button(w_button[1])
    );

    ButtonController reset(
        .i_clk(i_clk),
        .i_reset(i_reset),
        .i_button(i_button[2]),
        .o_button(w_button[2])
    );

    wire [5:0] w_fanState;
    Fan_Mode fan_mod(
        .i_clk(i_clk),
        .i_switch(i_switch),
        .i_reset(i_reset),
        .i_button(w_button),
        .i_sec(w_sec),
        .o_fanState(w_fanState)
    ); 

    MUX_4to1 stateselect(
        .i_x(w_fan),
        .sel(w_fanState),
        .i_button(i_button),
        .i_sec(w_sec),
        .o_y(o_motor)
    );

    FAN_Light light(
        .i_fanState(w_fanState),
        .o_light(o_light)
    );

    // FND
    wire w_1K_clk;
    FND_ClockDivdier clk_1K(
        .i_clk(i_clk),
        .i_reset(i_reset),
        .o_clk(w_1K_clk)
    );

    wire [1:0] w_digitPosition;
    FND_Counter fnd_count(
        .i_clk(w_1K_clk),
        .i_reset(i_reset),
        .o_counter(w_digitPosition)
    );

    FND_Decoder_2to4 digitselect(
        .i_digitPosition(w_digitPosition),
        .o_Digit(o_FND_Digit)
    );

    wire [3:0] w_1000_value, w_100_value, w_10_value, w_1_value;
    FND_Value fndvalue(
        .i_fanState(w_fanState),
        .o_1000_value(w_1000_value), 
        .o_100_value(w_100_value), 
        .o_10_value(w_10_value), 
        .o_1_value(w_1_value)
    );


    wire [3:0] w_value;
    FND_Value_MUX fndmux(
        .i_1000_value(w_1000_value), 
        .i_100_value(w_100_value), 
        .i_10_value(w_10_value), 
        .i_1_value(w_1_value),
        .i_digitPosition(w_digitPosition),
        .o_value(w_value)
    );

    BCDToFND_Decoder font(
        .i_value(w_value),
        .o_font(o_FND_Font),
        .i_clock_value(w_fndValueMux),
        .i_switch(i_switch)
    );


    //clock

    wire [6:0] w_hour, w_min, w_sec, w_msec;
    Time_ClockCounter time_clockcnt(
    .i_clk(w_1K_clk),
    .i_reset(i_reset),
    .o_hour(w_hour),
    .o_min(w_min),
    .o_sec(w_sec),
    .o_msec(w_msec)
    );

    wire [3:0] w_min10, w_min1, w_hour10, w_hour1;
    FND_DigitDivider digitDiv_hourmin(
    .i_a(w_min),
    .i_b(w_hour),
    .o_a10(w_min10),
    .o_a1(w_min1),
    .o_b10(w_hour10),
    .o_b1(w_hour1)
    );

    wire [3:0] w_sec10, w_sec1, w_msec10, w_msec1;
    FND_DigitDivider digitDiv_secmsec(
    .i_a(w_msec),
    .i_b(w_sec),
    .o_a10(w_msec10),
    .o_a1(w_msec1),
    .o_b10(w_sec10),
    .o_b1(w_sec1)
    );

    wire [3:0] w_hourMinMux;
    Clock_MUX_4to1 Mux_hourmin(
    .i_a(w_min1),
    .i_b(w_min10),
    .i_c(w_hour1),
    .i_d(w_hour10),
    .i_a1(11),
    .i_b1(11),
    .i_c1(w_fndDP),
    .i_d1(11),
    .i_sel(w_digitPosition),
    .o_y(w_hourMinMux)
    );

    wire [3:0] w_secMsecMux;
    Clock_MUX_4to1 Mux_secmsec(
    .i_a(w_msec1),
    .i_b(w_msec10),
    .i_c(w_sec1),
    .i_d(w_sec10),
    .i_a1(11),
    .i_b1(11),
    .i_c1(w_fndDP),
    .i_d1(11),
    .i_sel(w_digitPosition),
    .o_y(w_secMsecMux)
    );

    wire [3:0] w_fndDP;
    Clock_Comparator c_comp(
    .i_msec(w_msec),
    .o_fndDP(w_fndDP)
    );

    wire [3:0] w_fndValueMux;
    Clock_MUX_2to1 mux_fndValue(
    .i_a(w_secMsecMux),
    .i_b(w_hourMinMux),
    .i_sel(i_modeSW),
    .o_y(w_fndValueMux)
    );

endmodule
