
/*
 * 
 * Simulation of A Single Server Queueing System
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

#ifndef _SIMPARAMETERS_H_
#define _SIMPARAMETERS_H_

/******************************************************************************/
// CHANGED !!!!
#define PACKET_ARRIVAL_RATE 250, 500, 1000, 1500, 1800, 1900, 1950, 1975, 1987  /* packets per second */ 
#define PACKET_LENGTH 500 /* bits */
#define LINK_BIT_RATE 5e5 /* bits per second */ //service rate
#define RUNLENGTH 10e6 /* packets */ //reduced it for part 3, to try out more arrival rates quicker

#define CSV_FILENAME "PART4_DELAY.csv"

/* Comma separated list of random seeds to run. */
#define RANDOM_SEED_LIST 400381481, 400343389, 730345678, 170987654, 300987654, 200987654, 500987654, 500894209, 489567100, 999999777

#define PACKET_XMT_TIME ((double) PACKET_LENGTH/LINK_BIT_RATE)  //service time, transmission time per packet, useful for Throughput equation
#define BLIPRATE (RUNLENGTH/1000)

/******************************************************************************/

#endif /* simparameters.h */



// PART 5 PSUEDO CODE

// #ifndef _SIMPARAMETERS_H_
// #define _SIMPARAMETERS_H_

// /******************************************************************************/
// 
/*
 - we set arrival time manually for L1, L2, L3 in main.c, 
 - we need to iterate through P12 values.
 - we need to have 3 links, we have a 2 link system alread, just gotta rename them from link->link2 and link2->link3
     - create link1, its transmisison sends the packet to L2/L3
         - we need to make a conditional that checks for if it goes to L2, or L3
             - if((rand() % 100) <= PROBABILITY_12) {SEND TO LINK2}
               else {SEND TO LINK 3}
             - should probably do this in transmit_packet for L1, in end_transmission_packetL1() r smthn
 - we need to create buffers(queues) for each of the sections, we only have 1 right now
     - actually we only need 2 queues, the arrival process shoudl determine wherether packet goes L2, or L3. Think about this later

 - think if you need to add any data members


*/
// #define PROB_12 0.1, 0.25, 0.5, 0.75, 0.9  /* packets per second */ //this is what we will iterate through
// #define PACKET_LENGTH 500 /* bits */
// #define LINK_BIT_RATE 5e5 /* bits per second */ //service rate
// #define RUNLENGTH 10e6 /* packets */ //reduced it for part 3, to try out more arrival rates quicker

// #define CSV_FILENAME "PART4_DELAY.csv"

// /* Comma separated list of random seeds to run. */
// #define RANDOM_SEED_LIST 400381481, 400343389, 730345678, 170987654, 300987654, 200987654, 500987654, 500894209, 489567100, 999999777

// #define PACKET_XMT_TIME ((double) PACKET_LENGTH/LINK_BIT_RATE)  //service time, transmission time per packet, useful for Throughput equation
// #define BLIPRATE (RUNLENGTH/1000)

// /******************************************************************************/

// #endif /* simparameters.h */


