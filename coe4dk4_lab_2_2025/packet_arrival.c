
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

#include <math.h>
#include <stdio.h>
#include "main.h"
#include "packet_transmission.h"
#include "packet_arrival.h"

/******************************************************************************/

/*
 * This function will schedule a packet arrival at a time given by
 * event_time. 
 * 
 * At that time the function "packet_arrival" (located in
 * packet_arrival.c) is executed. 
 * 
 * An object can be attached to the event and
 * can be recovered in packet_arrival.c.
 */

long int
schedule_packet_arrival_event1(Simulation_Run_Ptr simulation_run,
			      double event_time)
{
  Event event;

  event.description = "Packet Arrival1";
  event.function = packet_arrival_event1;
  event.attachment = (void *) NULL;
  return simulation_run_schedule_event(simulation_run, event, event_time);
}

long int
schedule_packet_arrival_event2(Simulation_Run_Ptr simulation_run,
			      double event_time)
{
  Event event;

  event.description = "Packet Arrival2";
  event.function = packet_arrival_event2;
  event.attachment = (void *) NULL;
  return simulation_run_schedule_event(simulation_run, event, event_time);
}

long int
schedule_packet_arrival_event3(Simulation_Run_Ptr simulation_run,
			      double event_time)
{
  Event event;

  event.description = "Packet Arrival3";
  event.function = packet_arrival_event3;
  event.attachment = (void *) NULL;
  return simulation_run_schedule_event(simulation_run, event, event_time);
}


void
packet_arrival_event1(Simulation_Run_Ptr simulation_run, void * ptr)
{
  Simulation_Run_Data_Ptr data;
  Packet_Ptr new_packet;

  data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run);

  new_packet = (Packet_Ptr) xmalloc(sizeof(Packet));                  //make new packet datatype
  new_packet->arrive_time = simulation_run_get_time(simulation_run);  //get the arrival time/start time of the packet, and set it to arrive time member
  //new_packet->service_time = get_packet_transmission_time();          // PACKET_LENGTH/LINK_BIT_RATE, (bits * sec / bit) how many seconds for each packet to service
  new_packet->status = WAITING;
  new_packet->source_id = 1;
  /* 
   * Start transmission if the data link is free. Otherwise put the packet into
   * the buffer.
   */

  if(server_state(data->link1) == FREE){
    start_transmission_on_link1(simulation_run, new_packet, data->link1); //if link is free, start transmission
  } else {
    fifoqueue_put(data->buffer1, (void*) new_packet);                    //else put it in the buffer
  }

  schedule_packet_arrival_event1(simulation_run,
            simulation_run_get_time(simulation_run) +
            exponential_generator(1.0 / PACKET_ARRIVAL_RATE_1)); // schedule next arrival for source 1
  }

  void
packet_arrival_event2(Simulation_Run_Ptr simulation_run, void * ptr)
{
  Simulation_Run_Data_Ptr data;
  Packet_Ptr new_packet;

  data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run);

  new_packet = (Packet_Ptr) xmalloc(sizeof(Packet));                  //make new packet datatype
  new_packet->arrive_time = simulation_run_get_time(simulation_run);  //get the arrival time/start time of the packet, and set it to arrive time member
  //new_packet->service_time = get_packet_transmission_time();          // PACKET_LENGTH/LINK_BIT_RATE, (bits * sec / bit) how many seconds for each packet to service
  new_packet->status = WAITING;
  new_packet->source_id = 2;
  /* 
   * Start transmission if the data link is free. Otherwise put the packet into
   * the buffer.
   */

  if(server_state(data->link2) == FREE){
    start_transmission_on_link2(simulation_run, new_packet, data->link2); //if link is free, start transmission
  } else {
    fifoqueue_put(data->buffer2, (void*) new_packet);                    //else put it in the buffer
  }

  schedule_packet_arrival_event2(simulation_run,
            simulation_run_get_time(simulation_run) +
            exponential_generator(1.0 / PACKET_ARRIVAL_RATE_2)); // schedule next arrival for source 2
  }


  void
packet_arrival_event3(Simulation_Run_Ptr simulation_run, void * ptr)
{
  Simulation_Run_Data_Ptr data;
  Packet_Ptr new_packet;

  data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run);

  new_packet = (Packet_Ptr) xmalloc(sizeof(Packet));                  //make new packet datatype
  new_packet->arrive_time = simulation_run_get_time(simulation_run);  //get the arrival time/start time of the packet, and set it to arrive time member
  //new_packet->service_time = get_packet_transmission_time();          // PACKET_LENGTH/LINK_BIT_RATE, (bits * sec / bit) how many seconds for each packet to service
  new_packet->status = WAITING;
  new_packet->source_id = 3;
  /* 
   * Start transmission if the data link is free. Otherwise put the packet into
   * the buffer.
   */

  if(server_state(data->link3) == FREE){
    start_transmission_on_link3(simulation_run, new_packet, data->link3); //if link is free, start transmission
  } else {
    fifoqueue_put(data->buffer3, (void*) new_packet);                    //else put it in the buffer
  }

  schedule_packet_arrival_event3(simulation_run,
            simulation_run_get_time(simulation_run) +
            exponential_generator(1.0 / PACKET_ARRIVAL_RATE_3)); // schedule next arrival for source 3
  }







