
#include "console.h"

static flow_data *c_flow;
static radio *c_radio;
static gain *c_gain;

void init_cli(flow_data *f, gain *g, radio *r){
	c_flow = f;
	c_gain = g;
	c_radio = r;
}

void top_menu(){
	uart0_printf("---------------------------\r\n");
	uart0_printf("-- Test Console ver1.4 ---\r\n");
	uart0_printf("---------------------------\r\n");
	uart0_printf("r: read radio\r\n");
	uart0_printf("f: read flow\r\n");
	uart0_printf("w: read radio and flow\r\n");
	uart0_printf("g: set gain\r\n");
	uart0_printf("---------------------------\r\n");
}	

void gain_menu(){
	uart0_printf("-----Gain Setting Menu-----\r\n");
	uart0_printf("a: display all gain\r\n");
	uart0_printf("p: set p_gain\r\n");
	uart0_printf("i: set i_gain\r\n");
	uart0_printf("d: set d_gain\r\n");
	uart0_printf("q: set valid OF quality threshold\r\n");
	uart0_printf("s: save all gain setting\r\n");
	uart0_printf("r: restore parameters\r\n");
	uart0_printf("e: return top menu\r\n");
	uart0_printf("---------------------------\r\n");
}

void top_menu_branch(uint8_t com_type){
	if(p_flg == 1){
		switch(com_type){
			case RADIO:
				uart0_printf("IN: %d, %d, %d OUT: %d, %d\r\n", c_radio->input[0], c_radio->input[1], c_radio->input[2],  c_radio->output[0], c_radio->output[1]);
				break;
			
			case FLOW:
				
				uart0_printf("%5d, %5d, %5d, %3d, ", c_flow->frame_count, c_flow->flow_comp_m_x, c_flow->flow_comp_m_y, c_flow->qual);
				//uart0_printf("%5d\r\n", c_flow->ground_distance);
				uart0_printf("%5d, %5d, %5d, %5d\r\n", c_flow->gyro_x_rate, c_flow->gyro_y_rate, c_flow->gyro_z_rate, c_flow->ground_distance);
			  break;
			
			case RADIO_FLOW:
				uart0_printf("IN: %d, %d OUT: %d, %d ", c_radio->input[0], c_radio->input[1], c_radio->output[0], c_radio->output[1]);
				
				uart0_printf("Flow: %+.4f, %+.4f, %3d, %4d\r\n", c_flow->x, c_flow->y, c_flow->qual, c_flow->ground_distance);
			  break;
			
			case SET_GAIN:
				input_detect = 0;
				menu_flg = 1;
				gain_menu();
			  break;
			
			default:
				uart0_printf("invalid command\r\n");
				input_detect = 0;
				break;
		}
		p_flg = 0;
	}
}

void gain_menu_branch(uint8_t com_type){
	
	switch(com_type){
		case DISPLAY:
			uart0_printf("P: %f, I: %f, D: %f\r\n", c_gain->p_gain, c_gain->i_gain, c_gain->d_gain);
			break;
		
		case SET_P:
			uart0_printf("P Gain is %f, input new value.\r\n", c_gain->p_gain);
			c_gain->p_gain = uart0_get_float_input();
			break;
		
		case SET_I:
			uart0_printf("I Gain is %f, input new value.\r\n", c_gain->i_gain);
			c_gain->i_gain = uart0_get_float_input();
			break;
		
		case SET_D:
			uart0_printf("D Gain is %f, input new value.\r\n", c_gain->d_gain);
			c_gain->d_gain = uart0_get_float_input();
			break;
		
		case SET_Q_TH:
			uart0_printf("OF quality threshold is %d, input new value. (range: 0~255)\r\n", c_flow->qual_th);
			c_flow->qual_th = (uint16_t)uart0_get_float_input();
			if(c_flow->qual_th > 255) c_flow->qual_th = 255;
			break;
		
		case SAVE:
			write_float(P_ADD, c_gain->p_gain);
			write_float(I_ADD, c_gain->i_gain);
			write_float(D_ADD, c_gain->d_gain);
			write_uint16(Q_TH_ADD, c_flow->qual_th);
			uart0_printf("Saved!\r\n");
			break;
		
		case RESTORE:
			c_gain->p_gain = read_float(P_ADD);
			c_gain->i_gain = read_float(I_ADD);
			c_gain->d_gain = read_float(D_ADD);
			uart0_printf("Restore Parameters.\r\n");
			break;
		
		case EXIT:
			menu_flg = 0;
			input_detect = 0;
			top_menu();
			break;
		
		default:
			uart0_printf("invalid command\r\n");
			input_detect = 0;
			break;
	}
}

