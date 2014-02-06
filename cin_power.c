#include <stdio.h>
#include <unistd.h> /* for sleep() */

#include "cin.h"
// Set HARDWARE to 1 on real system
//#define HARDWARE 1
#undef HARDWARE
#define LOCAL static

LOCAL char fccd_config_dir[]="/home/jaimef/Desktop/FCCD Software Development/FCCD Qt/CINController_BNL_v0.1/config/Startup/";

LOCAL char fpga_configfile[]="top_frame_fpga_r1004.bit";
LOCAL char cin_configfile_waveform[]="waveform_10ms_readout_timing_125MHz_frameStore.txt";
LOCAL char cin_configfile_fcric[]="ARRA_fcrics_config_x8_11112011.txt";
LOCAL char cin_configfile_bias[]="bias_setting_lbl_gold2.txt";

LOCAL char cin_fpga_config[1024];
LOCAL char cin_waveform_config[1024];
LOCAL char cin_fcric_config[1024];
LOCAL char cin_bias_config[1024];
LOCAL struct cin_port cp[2];
int cin_power_up (){

   int ret1, ret2, ret3;
   // debug
   printf("***cin_power_up\n");
   
#ifdef HARDWARE
   cin_init_ctl_port(&cp[0], 0, 0);
   cin_init_ctl_port(&cp[1], 0,CIN_DATA_CTL_PORT);


   sprintf(cin_fpga_config,"%s%s", fccd_config_dir,fpga_configfile);
   sprintf(cin_waveform_config,"%s%s", fccd_config_dir,cin_configfile_waveform);
   sprintf(cin_fcric_config,"%s%s", fccd_config_dir,cin_configfile_fcric);
   sprintf(cin_bias_config,"%s%s", fccd_config_dir,cin_configfile_bias);

   cin_off(&cp[0]);                                                                                        //Power OFF CIN
   sleep(1);

   cin_on(&cp[0]);                                                                                          //Power ON CIN
   sleep(4);

   cin_load_firmware(&cp[1],cin_fpga_config);        //Load CIN Firmware Configuration
   sleep(3);

   ret1 = cin_get_cfg_fpga_status(&cp[0]);                  //Get CIN FPGA status 
   sleep(1);

   ret2 = cin_set_fclk_125mhz(&cp[0]);                      //Set CIN clocks to 125MHz
   sleep(1);

   ret3 = cin_get_fclk_status(&cp[0]);                      //Get CIN clock status 
   sleep(1);

   cin_fp_on(&cp[0]);                                                                                      //Power ON CIN front Panel
   sleep(2);                                                                                                                                                //Wait to allow visual check
                                                                                                                                                                                                                                                                           
   /************************* FCCD Configuration **************************/        
   cin_load_config(&cp[0],cin_waveform_config);        //Load FCCD clock configuration
   sleep(3);

   cin_load_config(&cp[0],cin_fcric_config);           //Load CIN fcric Configuration
   sleep(3);

   cin_load_config(&cp[0],cin_bias_config);            //Load FCCD bias Configuration
   sleep(3);
   /**********************************************************************/
   printf("CIN startup complete!!\nRet Status: %d, %d, %d\n", ret1, ret2, ret3);  

#endif
   return 0;
}


int cin_power_down(){

   struct cin_port cp[2];

   // debug
   printf("***cin_power_down\n");
   
#ifdef HARDWARE
    
   // YF  Don't need to init again
   //cin_init_ctl_port(&cp[0], 0, 0);/* use default CIN control-port IP addr and IP port */
   //cin_init_ctl_port(&cp[1], 0,CIN_DATA_CTL_PORT);
   // But should add a check that cp is valid...

   cin_set_bias(&cp[0],0);                   //Turn OFF camera CCD bias
   sleep(1);                                                

   cin_set_clocks(&cp[0],0);                //Turn OFF camera CCD bias
   sleep(1);

   cin_fp_off(&cp[0]);                      //Power OFF CIN front Panel
   sleep(2);        

   cin_off(&cp[0]);                    //Power OFF CIN
   sleep(4);

   printf("Closing ports.......\n");
   cin_close_ctl_port(&cp[0]);       //Close Control port
   cin_close_ctl_port(&cp[1]);       //Close Stream-in port
   sleep(1);

   printf("CIN shutdown complete!!\n");
#endif   
   return(0);
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
