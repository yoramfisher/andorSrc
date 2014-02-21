#include <stdio.h>
#include <unistd.h> /* for sleep() */

#include "cin.h"

// These function prototype should be moved to cin.h!!!!
int cin_set_bias(struct cin_port* cp,int val);
int cin_set_clocks(struct cin_port* cp,int val);
int cin_set_trigger(struct cin_port* cp,int val);
int cin_set_exposure_time(struct cin_port* cp,float ftime);
uint16_t cin_get_trigger_status (struct cin_port* cp);
int cin_set_trigger_delay(struct cin_port* cp,float ftime);

// Set HARDWARE to 1 on real system
//#define HARDWARE 1
#undef HARDWARE
#define LOCAL static

LOCAL char fccd_config_dir[]="../cin_config/";
LOCAL char fpga_configfile[]="top_frame_fpga-v1019j.bit";
LOCAL char cin_configfile_waveform[]="2013_Nov_30-200MHz_CCD_timing.txt";
LOCAL char cin_configfile_fcric[]="ARRA_fcrics_config_x8_11112011.txt";
LOCAL char cin_configfile_bias[]="bias_setting_lbl_gold2.txt";

LOCAL char cin_fpga_config[1024];
LOCAL char cin_waveform_config[1024];
LOCAL char cin_fcric_config[1024];
LOCAL char cin_bias_config[1024];
LOCAL struct cin_port cp[2];
void cin_power_up (){

   int ret_fclk,ret_fpga;
   // debug
   printf("***cin_power_up\n");
   
#ifdef HARDWARE
   cin_init_ctl_port(&cp[0], 0, 0);
   cin_init_ctl_port(&cp[1], 0,CIN_DATA_CTL_PORT);


   sprintf(cin_fpga_config,"%s%s", fccd_config_dir,fpga_configfile);
   sprintf(cin_waveform_config,"%s%s", fccd_config_dir,cin_configfile_waveform);
   sprintf(cin_fcric_config,"%s%s", fccd_config_dir,cin_configfile_fcric);
   sprintf(cin_bias_config,"%s%s", fccd_config_dir,cin_configfile_bias);

   
   cin_off(&cp[0]);
	sleep(5);

	cin_on(&cp[0]);
	sleep(5);

	cin_fp_on(&cp[0]);
	sleep(5);
	
	cin_get_cfg_fpga_status(&cp[0]);
	sleep(1);

	cin_load_firmware(&cp[0],&cp[1],cin_fpga_config);	
	sleep(5);

	ret_fpga=cin_get_cfg_fpga_status(&cp[0]);
	sleep(1);

	ret_fclk=cin_get_fclk_status(&cp[0]);			
	sleep(1);
/************************* FCCD Configuration **************************/	

	cin_load_config(&cp[0],cin_waveform_config);		//Load FCCD clock configuration
	sleep(3);
/*	
	cin_load_config(&cp[0],cin_fcric_config);		//Load CIN fcric Configuration
	sleep(3);
	
	cin_load_config(&cp[0],cin_bias_config);		//Load FCCD bias Configuration
	sleep(3);
*/
/**********************************************************************/		
	fprintf(stdout,"\nCIN startup complete!!\n");

	if (ret_fpga==0)
   {
      printf(stdout,"  *FPGA Status: OK\n");
   }
	else
   {
      printf(stdout,"  *FPGA Status: ERROR\n");
   }
	
	if (ret_fclk==0)
   {
      printf(stdout,"  *FCLK Status: OK\n");
   }
	else
   {
      printf(stdout,"  *FCLK Status: ERROR\n");
   }	
   

#endif
 
}


void cin_power_down(){

   struct cin_port cp[2];

   // debug
   printf("***cin_power_down\n");
   
#ifdef HARDWARE
    
   // YF  Don't need to init again
   //cin_init_ctl_port(&cp[0], 0, 0);/* use default CIN control-port IP addr and IP port */
   //cin_init_ctl_port(&cp[1], 0,CIN_DATA_CTL_PORT);
   // But should add a check that cp is valid...

   printf(stdout,"Turning off clock and bias.......\n");
	cin_set_bias(&cp[0],0);   	//Turn OFF camera CCD bias
	sleep(1);						

	cin_set_clocks(&cp[0],0);		//Turn OFF camera CCD bias
	sleep(1);

	cin_fp_off(&cp[0]);      		//Power OFF CIN front Panel
	sleep(2);	

	cin_off(&cp[0]);          	//Power OFF CIN
	sleep(4);

	printf(stdout,"Closing ports.......\n");
	cin_close_ctl_port(&cp[0]);       //Close Control port
	cin_close_ctl_port(&cp[1]); 			//Close Stream-in port
	sleep(1);

	printf(stdout,"CIN shutdown complete!!\n");
	
#endif   
  
}


// Public wrappers to cin_api functions
int CIN_set_bias(int val)
{
   printf("CIN_set_bias:%d\n", val);
   return cin_set_bias(cp,val);
}

int CIN_set_clocks(int val)
{
   printf("CIN_set_clocks:%d\n", val);
   return cin_set_clocks(cp, val);
}

//val:{0-Internal, 1-External1, 2-External2, 3-External 1 or 2}
int CIN_set_trigger(int val)
{
   printf("CIN_set_trigger:%d\n", val);
   return cin_set_trigger(cp,val); 
}

//Return:{0-Internal, 1-External1, 2-External2, 3-External 1 or 2}
int CIN_get_trigger_status()
{
   return cin_get_trigger_status(cp);
}

// Set the Camera exposure time
// Input:e_time (ms)
int CIN_set_exposure_time(float e_time)
{
   return cin_set_exposure_time(cp,e_time); 
}

//Set the trigger delay time
//Input:t_time (ms)  
int CIN_set_trigger_delay(float t_time)
{  
   return cin_set_trigger_delay(cp,t_time);
}  

//Set the Camera cyle time time
//Input:c_time (ms)             
int CIN_set_cycle_time(float c_time)
{
   return cin_set_cycle_time(cp,c_time);  
}

