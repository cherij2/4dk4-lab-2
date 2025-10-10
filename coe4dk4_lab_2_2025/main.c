
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

int
main(void)
{
  Simulation_Run_Ptr simulation_run;
  Simulation_Run_Data data;

  /*
   * Declare and initialize our random number generator seeds defined in
   * simparameters.h
   */

  unsigned RANDOM_SEEDS[] = {RANDOM_SEED_LIST, 0};
  unsigned ARRIVAL_RATES[] = {PACKET_ARRIVAL_RATE, 0};
  //LAB 2 
  unsigned arr_rates;
  unsigned random_seed;
  
  //LAB 3
  double serv_rate = PACKET_LENGTH / LINK_BIT_RATE; 
  double serv_time = 1/serv_rate; // we want this to be less than 20ms for 98% of the time
  
  int j=0;
  int i=0;

  /* 
   * Loop for each random number generator seed, doing a separate
   * simulation_run run for each.
   */
  
  //for loop  for PACKET_ARRIVAL_RATE array in simparameters.h
  CSVInit(CSV_FILENAME);
  while((arr_rates = ARRIVAL_RATES[i++]) != 0) {
    j = 0;
    
  while ((random_seed = RANDOM_SEEDS[j++]) != 0) {

    simulation_run = simulation_run_new(); /* Create a new simulation run. */

    /*
     * Set the simulation_run data pointer to our data object.
     */

    simulation_run_attach_data(simulation_run, (void *) & data);

    /* 
     * Initialize the simulation_run data variables, declared in main.h.
     */
    
    data.blip_counter = 0;
    data.arrival_count = 0;
    data.number_of_packets_processed = 0;
    data.accumulated_delay = 0.0;
    data.random_seed = random_seed;
    //LAB2
    data.arrival_rate = arr_rates;
    data.delay_above_20ms = 0;


    /* 
     * Create the packet buffer and transmission link, declared in main.h.
     */

    data.buffer = fifoqueue_new();
    data.link   = server_new();
    data.link2   = server_new();

    /* 
     * Set the random number generator seed for this run.
     */

    random_generator_initialize(random_seed);

    /* 
     * Schedule the initial packet arrival for the current clock time (= 0).
     */

    schedule_packet_arrival_event(simulation_run,           //set the .function member of simulation run to packet_arrival_event, this function runs every packet for arrivals + sets up and runs transmission function
				  simulation_run_get_time(simulation_run));         //set the start time of this event


      //PART 3, CSV WRITER INIT, FUCNTION IN PACKET_TRANSMIT.c
    
    
    
    
    /* 
     * Execute events until we are finished. 
     */
    
    while(data.number_of_packets_processed < RUNLENGTH) {
      // printf("did another packet! \n");
      simulation_run_execute_event(simulation_run);        //runs the function in the .function member of simulation_run, also includes the .attatchement member of simulation_run in the running of .function  
    }

    
    

    /*
     * Output results and clean up after ourselves.
     */

    output_results(simulation_run);
    cleanup_memory(simulation_run);
  }

  // getchar();   /* Pause before finishing. */  
}
  CSVClose();
  return 0;
}











