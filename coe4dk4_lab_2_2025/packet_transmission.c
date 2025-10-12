
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
schedule_end_packet_transmission_event1(Simulation_Run_Ptr simulation_run,           //PT4 BECAUSE OF THIS WE NEED TO MAKE COPIES OF EVERY PEICE OF CODE, CAN'T HAVE 2 .ARGUMENTS 
				       double event_time,
				       Server_Ptr link1)
{
  Event event;

  event.description = "Packet Xmt End";
  event.function = end_packet_transmission_event1;
  event.attachment = (void *) link1;

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

long 
schedule_end_packet_transmission_event3(Simulation_Run_Ptr simulation_run,           
				       double event_time,
				       Server_Ptr link3)
{
  Event event;

  event.description = "Packet Xmt End";
  event.function = end_packet_transmission_event3;
  event.attachment = (void *) link3;

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
  fprintf(LAB2_Excel, "NEW_TRIAL, %02d:%02d:%02d \n p12, Mean Delay1, Mean Delay2, Mean Delay3 \n", t->tm_hour, t->tm_min, t->tm_sec);
  fflush(LAB2_Excel);
}

void CSVNewLine(const char* file) {
  /*FILE**/ LAB2_Excel = fopen(file, "a"); //the FILE* made it a local variable and functions were writing to NULL because global variable was destroyed
}

// just wanted to try implementation of writing every single line of data, since there is 10 million packets i am commenting that
//making it print just the final value, of all delay counts etc
void CSVWriter(double p12, double mean_delay1, double mean_delay2, double mean_delay_3) { 
  fprintf(LAB2_Excel, "%f, %f, %f, %f \n", p12, mean_delay1,mean_delay2, mean_delay_3);
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
end_packet_transmission_event1(Simulation_Run_Ptr simulation_run, void * link) //link = server
{
  Simulation_Run_Data_Ptr data;
  Packet_Ptr this_packet, next_packet;

  TRACE(printf("End Of Packet.\n"););

  data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run);

  /* 
   * Packet transmission is finished. Take the packet off the data link.
   */

  this_packet = (Packet_Ptr) server_get(link);    //sets srever to free


  data->number_of_packets_processed_1++;
  data->accumulated_delay_1 += simulation_run_get_time(simulation_run) -    //SIMILAR TO THIS BUT INDIVIDUALLY AND RESET EVERY PACKET
    this_packet->arrive_time;

  /* Output activity blip every so often. */
  output_progress_msg_to_screen_1(simulation_run);

  /* This packet is done ... give the memory back. */
  /* Do not free here: this_packet is forwarded to link2/link3.
     It will be freed when transmission completes on link2/link3. */

  /* 
   * See if there is are packets waiting in the buffer. If so, take the next one
   * out and transmit it immediately.
  */

  double r = uniform_generator();
  if (r < data->p12){
    data->n_s1_to_l2++;
    // this_packet->service_time = (double) PACKET_XMT_TIME_2; //transmission time for link 2
    fifoqueue_put(data->buffer2, (void*) this_packet);
    if (server_state(data->link2) == FREE) { //if link 2 is free, start transmission
      next_packet = (Packet_Ptr) fifoqueue_get(data->buffer2);
      start_transmission_on_link2(simulation_run, next_packet, data->link2);
    }
  } else {
    data->n_s1_to_l3++;
    // this_packet->service_time = (double) PACKET_XMT_TIME_3; //transmission time for link 3
    fifoqueue_put(data->buffer3, (void*) this_packet);
    if (server_state(data->link3) == FREE) { //if link 3 is free, start transmission
      next_packet = (Packet_Ptr) fifoqueue_get(data->buffer3);
      start_transmission_on_link3(simulation_run, next_packet, data->link3);
    }
  }



  if(fifoqueue_size(data->buffer1) > 0) {                                          //does everything in the buffer and then terminates, goes back to packet_arrival.c to repeat cycle
    next_packet = (Packet_Ptr) fifoqueue_get(data->buffer1);
    next_packet->service_time = (double) PACKET_XMT_TIME_1; //transmission time for link 1
    start_transmission_on_link1(simulation_run, next_packet, link);
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
  data->number_of_packets_processed_2++;
  data->accumulated_delay_2 += simulation_run_get_time(simulation_run) -    //SIMILAR TO THIS BUT INDIVIDUALLY AND RESET EVERY PACKET
  this_packet->arrive_time;
  
  output_progress_msg_to_screen_2(simulation_run);

  xfree((void *) this_packet);

  /* 
   * See if there is are packets waiting in the buffer. If so, take the next one
   * out and transmit it immediately.
  */

  if(fifoqueue_size(data->buffer2) > 0) {                                          //does everything in the buffer and then terminates, goes back to packet_arrival.c to repeat cycle
    next_packet = (Packet_Ptr) fifoqueue_get(data->buffer2);
    start_transmission_on_link2(simulation_run, next_packet, link2);
  }
}

void
end_packet_transmission_event3(Simulation_Run_Ptr simulation_run, void * link3)
{
  Simulation_Run_Data_Ptr data;
  Packet_Ptr this_packet, next_packet;

  TRACE(printf("End Of Packet.\n"););

  data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run);

  /* 
   * Packet transmission is finished. Take the packet off the data link.
   */
  this_packet = (Packet_Ptr) server_get(link3);

  
  /* Collect statistics. */
  // //PT 3, ADDING IMPLEMENTATION OF DELAY PER PACKET
  data->number_of_packets_processed_3++;
  data->accumulated_delay_3 += simulation_run_get_time(simulation_run) -    //SIMILAR TO THIS BUT INDIVIDUALLY AND RESET EVERY PACKET
  this_packet->arrive_time;
  
  // double dly = simulation_run_get_time(simulation_run) - this_packet->arrive_time;
  // if (this_packet->source_id == 1) { 
  //   data->delay_sum_S1 += dly; data->n_done_S1++; 
  // } else if (this_packet->source_id == 2) { 
  //   data->delay_sum_S2 += dly; data->n_done_S2++; 
  // } else if (this_packet->source_id == 3){ 
  //   data->delay_sum_S3 += dly; data->n_done_S3++; 
  // }
  output_progress_msg_to_screen_3(simulation_run);

  xfree((void *) this_packet);

  /* 
   * See if there is are packets waiting in the buffer. If so, take the next one
   * out and transmit it immediately.
  */

  if(fifoqueue_size(data->buffer3) > 0) {                                          //does everything in the buffer and then terminates, goes back to packet_arrival.c to repeat cycle
    next_packet = (Packet_Ptr) fifoqueue_get(data->buffer3);
    start_transmission_on_link3(simulation_run, next_packet, link3);
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
start_transmission_on_link1(Simulation_Run_Ptr simulation_run, 
			   Packet_Ptr this_packet,
			   Server_Ptr link1)
{
  TRACE(printf("Start Of Packet.\n");)

  server_put(link1, (void*) this_packet);      //takes this_packet out of queue, and puts in link(server)
  this_packet->status = XMTTING;

  this_packet->service_time = (double) PACKET_XMT_TIME_1;
  /* Schedule the end of packet transmission event. */
  schedule_end_packet_transmission_event1(simulation_run,
	 simulation_run_get_time(simulation_run) + this_packet->service_time,      //time it ends it based on avg service time
	 (void *) link1);
}



void
start_transmission_on_link2(Simulation_Run_Ptr simulation_run, 
			   Packet_Ptr this_packet,
			   Server_Ptr link2)
{
  TRACE(printf("Start Of Packet.\n");)

  server_put(link2, (void*) this_packet);
  this_packet->status = XMTTING;
  this_packet->service_time = (double)PACKET_LENGTH / LINK_BIT_RATE_2;
  /* Schedule the end of packet transmission event. */
  schedule_end_packet_transmission_event2(simulation_run,
	 simulation_run_get_time(simulation_run) + this_packet->service_time,      //time it ends it based on avg service time
	 (void *) link2);
}


void
start_transmission_on_link3(Simulation_Run_Ptr simulation_run, 
			   Packet_Ptr this_packet,
			   Server_Ptr link3)
{
  TRACE(printf("Start Of Packet.\n");)

  server_put(link3, (void*) this_packet);
  this_packet->status = XMTTING;
  this_packet->service_time = (double)PACKET_LENGTH / LINK_BIT_RATE_3;
  /* Schedule the end of packet transmission event. */
  schedule_end_packet_transmission_event3(simulation_run,
	 simulation_run_get_time(simulation_run) + this_packet->service_time,      //time it ends it based on avg service time
	 (void *) link3);
}

/*
 * Get a packet transmission time. For now it is a fixed value defined in
 * simparameters.h
 */

// double
// get_packet_transmission_time(void)
// {
//   return ((double) PACKET_XMT_TIME_1);
// }


