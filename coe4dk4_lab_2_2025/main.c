
/*
 * 
 * Simulation_Run of A Single Server Queueing System
 * 
 * Copyright (C) 2014 Terence D. Todd Hamilton, Ontario, CANADA,
 * todd@mcmaster.ca
 * 
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 3 of the License, or (at your option)
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/*******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "output.h"
#include "simparameters.h"
#include "packet_arrival.h"
#include "cleanup_memory.h"
#include "trace.h"
#include "main.h"

/******************************************************************************/

/*
 * main.c declares and creates a new simulation_run with parameters defined in
 * simparameters.h. The code creates a fifo queue and server for the single
 * server queueuing system. It then loops through the list of random number
 * generator seeds defined in simparameters.h, doing a separate simulation_run
 * run for each. To start a run, it schedules the first packet arrival
 * event. When each run is finished, output is printed on the terminal.
 */

int main(void)
{
  Simulation_Run_Ptr simulation_run;
  Simulation_Run_Data data;

  /*
   * Declare and initialize our random number generator seeds defined in
   * simparameters.h
   */

  unsigned RANDOM_SEEDS[] = {RANDOM_SEED_LIST, 0};
  unsigned random_seed;
  double PROBABILITY_12_LIST[] = {PROBABILITY_12, 0};
  double p12;
  int i=0;
  
  

  /* 
   * Loop for each random number generator seed, doing a separate
   * simulation_run run for each.
   */

 CSVInit(CSV_FILENAME); 
 while((p12 = PROBABILITY_12_LIST[i++]) != 0) {
   
  int j=0;  
  while ((random_seed = RANDOM_SEEDS[j++]) != 0) {

    simulation_run = simulation_run_new(); /* Create a new simulation run. */

    /*
     * Set the simulation_run data pointer to our data object.
     */

    simulation_run_attach_data(simulation_run, (void *) & data);

    /* 
     * Initialize the simulation_run data variables, declared in main.h.
     */
    data.p12 = p12;
    data.n_done_S1 = data.n_done_S2 = data.n_done_S3 = 0;
    data.delay_sum_S1 = data.delay_sum_S2 = data.delay_sum_S3 = 0.0;
    data.n_s1_to_l2 = data.n_s1_to_l3 = 0;
    data.blip_counter = 0;
    data.arrival_count_1 = 0;
    data.arrival_count_2 = 0;
    data.arrival_count_3 = 0;
    data.number_of_packets_processed_1 = 0;
    data.number_of_packets_processed_2 = 0;
    data.number_of_packets_processed_3 = 0;
    data.accumulated_delay_1 = 0.0;
    data.accumulated_delay_2 = 0.0;
    data.accumulated_delay_3 = 0.0;
    data.random_seed = random_seed;
    

    /* 
     * Create the packet buffer and transmission link, declared in main.h.
     */

    data.buffer1 = fifoqueue_new(); //queue1
    data.buffer2 = fifoqueue_new(); //queue2
    data.buffer3 = fifoqueue_new(); //queue3
    data.link1   = server_new();    //output (server)
    data.link2   = server_new();  //output2 (server2)
    data.link3   = server_new();  //output3 (server3)


    data.sim_start_time = simulation_run_get_time(simulation_run);  //PART 4, to get total sim time for throughput
    /* 
     * Set the random number generator seed for this run.
     */

    random_generator_initialize(random_seed);

    /* 
     * Schedule the initial packet arrival for the current clock time (= 0).
     */

    schedule_packet_arrival_event1(simulation_run,           
				  simulation_run_get_time(simulation_run));         
    schedule_packet_arrival_event2(simulation_run,           
				  simulation_run_get_time(simulation_run)); 
    schedule_packet_arrival_event3(simulation_run,           
				  simulation_run_get_time(simulation_run)); 

    /* 
     * Execute events until we are finished. 
     */
    
    while((data.number_of_packets_processed_2 + data.number_of_packets_processed_3) < RUNLENGTH) {
      // printf("did another packet! \n");
      simulation_run_execute_event(simulation_run);        //runs the function in the .function member of simulation_run, also includes the .attatchement member of simulation_run in the running of .function  
    }
    /*
     * Output results and clean up after ourselves.
     */

    data.sim_end_time = simulation_run_get_time(simulation_run); //PART 4, to get total sim time for throughput
    output_results(simulation_run);
    cleanup_memory(simulation_run);
  }
} 
CSVClose();
return 0;   
}











