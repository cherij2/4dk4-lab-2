
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

/******************************************************************************/


#include <stdio.h>

#include "trace.h"
#include "main.h"
#include "output.h"
#include "packet_transmission.h"

/******************************************************************************/

/*
 * This function will schedule the end of a packet transmission at a time given
 * by event_time. 
 * 
 * At that time the function "end_packet_transmission" (defined
 * in packet_transmission.c) is executed.
 * 
 * A packet object is attached to the
 * event and is recovered in end_packet_transmission.c.
 */

static FILE* LAB2_Excel = NULL;

long
schedule_end_packet_transmission_event(Simulation_Run_Ptr simulation_run,
				       double event_time,
				       Server_Ptr link)
{
  Event event;

  event.description = "Packet Xmt End";
  event.function = end_packet_transmission_event;
  event.attachment = (void *) link;

  return simulation_run_schedule_event(simulation_run, event, event_time);        //starts another .function call, this time it is set to end_packet_transmission_event, how does it go back to arrival?
}

/******************************************************************************/

/*
 * This is the event function which is executed when the end of a packet
 * transmission event occurs. 
 * 
 * It updates its collected data then checks to see
 * if there are other packets waiting in the fifo queue. 
 * 
 * If that is the case it
 * starts the transmission of the next packet.
 */

void CSVInit(const char* file) {
  FILE* LAB2_Excel = fopen(file, "w");
  fprintf(LAB2_Excel, "Delay above 20ms \n");
}

void CSVWriter(double delay, int count_of_delays) {
  fprintf(LAB2_Excel, "%f, %d \n", delay, count_of_delays);
  fflush(LAB2_Excel);
}

void CSVClose() {
  fclose(LAB2_Excel);
  LAB2_Excel == NULL;
}

void
end_packet_transmission_event(Simulation_Run_Ptr simulation_run, void * link)
{
  Simulation_Run_Data_Ptr data;
  Packet_Ptr this_packet, next_packet;

  TRACE(printf("End Of Packet.\n"););

  data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run);

  /* 
   * Packet transmission is finished. Take the packet off the data link.
   */

  this_packet = (Packet_Ptr) server_get(link);

  /* Collect statistics. */
  //PT 3, ADDING IMPLEMENTATION OF DELAY PER PACKET
  data->number_of_packets_processed++;
  data->accumulated_delay += simulation_run_get_time(simulation_run) -    //SIMILAR TO THIS BUT INDIVIDUALLY AND RESET EVERY PACKET
    this_packet->arrive_time;
  
  //LAB3
  /*
  Get delay value, 
  if statement to 
  add a list to data struct, maybe delayValues[]
  add delay values to list in this funciton 
  extract values in main.c and excel it
  
  */
  data->delay_per_packet = simulation_run_get_time(simulation_run) -    //INDIVIDUAL DELAY PER PACKET, IS RESET EVERY TRANSMISSION
  this_packet->arrive_time;

  if(data->delay_per_packet > 20) {                                     //if delay is higher than constraint, add it to a counter, we can use this later to find percent which are unaccepatble
    (data->delay_above_20ms)++;
  }
  
  /*ADD CSV FUNCTION IMPELEMENTATION HERE*/
  CSVWriter(data->delay_per_packet, data->delay_above_20ms);



  /* Output activity blip every so often. */
  output_progress_msg_to_screen(simulation_run);

  /* This packet is done ... give the memory back. */
  xfree((void *) this_packet);

  /* 
   * See if there is are packets waiting in the buffer. If so, take the next one
   * out and transmit it immediately.
  */

  if(fifoqueue_size(data->buffer) > 0) {                                          //does everything in the buffer and then terminates, goes back to packet_arrival.c to repeat cycle
    next_packet = (Packet_Ptr) fifoqueue_get(data->buffer);
    start_transmission_on_link(simulation_run, next_packet, link);
  }
}

/*
 * This function ititiates the transmission of the packet passed to the
 * function. 
 * 
 * This is done by placing the packet in the server. 
 * 
 * The packet
 * transmission end event for this packet is then scheduled.
 */

void
start_transmission_on_link(Simulation_Run_Ptr simulation_run, 
			   Packet_Ptr this_packet,
			   Server_Ptr link)
{
  TRACE(printf("Start Of Packet.\n");)

  server_put(link, (void*) this_packet);
  this_packet->status = XMTTING;

  /* Schedule the end of packet transmission event. */
  schedule_end_packet_transmission_event(simulation_run,
	 simulation_run_get_time(simulation_run) + this_packet->service_time,      //time it ends it based on avg service time
	 (void *) link);
}

/*
 * Get a packet transmission time. For now it is a fixed value defined in
 * simparameters.h
 */

double
get_packet_transmission_time(void)
{
  return ((double) PACKET_XMT_TIME);
}


