
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
#include <time.h>

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



long
schedule_end_packet_transmission_event(Simulation_Run_Ptr simulation_run,           //PT4 BECAUSE OF THIS WE NEED TO MAKE COPIES OF EVERY PEICE OF CODE, CAN'T HAVE 2 .ARGUMENTS 
				       double event_time,
				       Server_Ptr link)
{
  Event event;

  event.description = "Packet Xmt End";
  event.function = end_packet_transmission_event;
  event.attachment = (void *) link;

  return simulation_run_schedule_event(simulation_run, event, event_time);        //starts another .function call, this time it is set to end_packet_transmission_event, how does it go back to arrival?
}
long
schedule_end_packet_transmission_event2(Simulation_Run_Ptr simulation_run,           //PT4 BECAUSE OF THIS WE NEED TO MAKE COPIES OF EVERY PEICE OF CODE, CAN'T HAVE 2 .ARGUMENTS 
				       double event_time,
				       Server_Ptr link2)
{
  Event event;

  event.description = "Packet Xmt End";
  event.function = end_packet_transmission_event2;
  event.attachment = (void *) link2;

  return simulation_run_schedule_event(simulation_run, event, event_time);        //starts another .function call, this time it is set to end_packet_transmission_event, how does it go back to arrival?
}

/******************************************************************************/

/*
PART 3 ADDITION

csv initializer, writer, and closer functions so that it can be used/referencesd in packet_transmission.c as well as main.c (function is setup in header file)
*/
static FILE* LAB2_Excel = NULL;

void CSVInit(const char* file) {
  time_t now = time(NULL);
  struct tm *t = localtime(&now);        //getting the time now
  /*FILE**/ LAB2_Excel = fopen(file, "a"); //the FILE* made it a local variable and functions were writing to NULL because global variable was destroyed
  fprintf(LAB2_Excel, "NEW_TRIAL, %02d:%02d:%02d \n Arrival Rate, Total Packets Processed, mean_delay, throughput \n", t->tm_hour, t->tm_min, t->tm_sec);
  fflush(LAB2_Excel);
}

void CSVNewLine(const char* file) {
  /*FILE**/ LAB2_Excel = fopen(file, "a"); //the FILE* made it a local variable and functions were writing to NULL because global variable was destroyed
}

// just wanted to try implementation of writing every single line of data, since there is 10 million packets i am commenting that
//making it print just the final value, of all delay counts etc
void CSVWriter(double arr_rate, long int total_number_processed, double mean_delay, double throughput) { 
  fprintf(LAB2_Excel, "%f, %ld, %f, %f \n", arr_rate, total_number_processed, mean_delay, throughput);
  fflush(LAB2_Excel);     
}

void CSVClose() {
  fclose(LAB2_Excel);   //does fflush and fclose in the same function 
}



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
void
end_packet_transmission_event(Simulation_Run_Ptr simulation_run, void * link) //link = server
{
  Simulation_Run_Data_Ptr data;
  Packet_Ptr this_packet, next_packet;

  TRACE(printf("End Of Packet.\n"););

  data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run);

  /* 
   * Packet transmission is finished. Take the packet off the data link.
   */

  this_packet = (Packet_Ptr) server_get(link);    //sets srever to free

  /* Collect statistics. */
  //PT 3, ADDING IMPLEMENTATION OF DELAY PER PACKET
  data->number_of_packets_processed++;
  data->accumulated_delay += simulation_run_get_time(simulation_run) -    //SIMILAR TO THIS BUT INDIVIDUALLY AND RESET EVERY PACKET
    this_packet->arrive_time;
  
  //PART3
  /*
  Get delay value, 
  if statement to add to count of delayed>20ms values
  extract values in main.c and excel it
  
  */
 data->delay_per_packet = 0;
  data->delay_per_packet = simulation_run_get_time(simulation_run) -    //INDIVIDUAL DELAY PER PACKET, IS RESET EVERY TRANSMISSION
  this_packet->arrive_time;

  /*
  REMOVED FOR PART 4
  if(data->delay_per_packet > 0.020) {                                     //if delay is higher than constraint, add it to a counter, we can use this later to find percent which are unaccepatble
    printf("delay! LINK1 \t%f\t%ld / %ld\n", data->delay_per_packet, data->delay_above_20ms, data->number_of_packets_processed);
    // printf("%ld \n", data->number_of_packets_processed);              //to see see progress in terminal 
    data->delay_above_20ms++;                                            //to keep count of delay packets
  }
  
  */
  /*ADD CSV FUNCTION IMPELEMENTATION HERE*/
  // CSVWriter(data->delay_per_packet, data->delay_above_20ms);



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


//PART 4, COPY OF END_TRANSMISSION_EVENT FOR LINK2
void
end_packet_transmission_event2(Simulation_Run_Ptr simulation_run, void * link2)
{
  Simulation_Run_Data_Ptr data;
  Packet_Ptr this_packet, next_packet;

  TRACE(printf("End Of Packet.\n"););

  data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run);

  /* 
   * Packet transmission is finished. Take the packet off the data link.
   */

  this_packet = (Packet_Ptr) server_get(link2);

  /* Collect statistics. */
  //PT 3, ADDING IMPLEMENTATION OF DELAY PER PACKET
  data->number_of_packets_processed++;
  data->accumulated_delay += simulation_run_get_time(simulation_run) -    //SIMILAR TO THIS BUT INDIVIDUALLY AND RESET EVERY PACKET
  this_packet->arrive_time;
  

  //PART3
  /*
  Get delay value, 
  if statement to add to count of delayed>20ms values
  extract values in main.c and excel it
  
  */
 data->delay_per_packet = 0;
  data->delay_per_packet = simulation_run_get_time(simulation_run) -    //INDIVIDUAL DELAY PER PACKET, IS RESET EVERY TRANSMISSION
  this_packet->arrive_time;

  
  /*
  REMOVED FOR PART 4 
  if(data->delay_per_packet > 0.020) {                                     //if delay is higher than constraint, add it to a counter, we can use this later to find percent which are unaccepatble
    printf("delay! LINK2 \t%f\t%ld / %ld\n", data->delay_per_packet, data->delay_above_20ms, data->number_of_packets_processed);
    // printf("%ld \n", data->number_of_packets_processed);              //to see see progress in terminal 
    data->delay_above_20ms++;                                            //to keep count of delay packets
  }
  
  */

  /*ADD CSV FUNCTION IMPELEMENTATION HERE*/
  // CSVWriter(data->delay_per_packet, data->delay_above_20ms);



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
    start_transmission_on_link2(simulation_run, next_packet, link2);
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

  server_put(link, (void*) this_packet);      //takes this_packet out of queue, and puts in link(server)
  this_packet->status = XMTTING;

  /* Schedule the end of packet transmission event. */
  schedule_end_packet_transmission_event(simulation_run,
	 simulation_run_get_time(simulation_run) + this_packet->service_time,      //time it ends it based on avg service time
	 (void *) link);
}



void
start_transmission_on_link2(Simulation_Run_Ptr simulation_run, 
			   Packet_Ptr this_packet,
			   Server_Ptr link2)
{
  TRACE(printf("Start Of Packet.\n");)

  server_put(link2, (void*) this_packet);
  this_packet->status = XMTTING;

  /* Schedule the end of packet transmission event. */
  schedule_end_packet_transmission_event2(simulation_run,
	 simulation_run_get_time(simulation_run) + this_packet->service_time,      //time it ends it based on avg service time
	 (void *) link2);
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


