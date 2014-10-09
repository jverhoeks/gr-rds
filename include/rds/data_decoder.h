/* -*- c++ -*- */
/*
 * Copyright 2004 Free Software Foundation, Inc.
 * 
 * GNU Radio is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 * 
 * GNU Radio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with GNU Radio; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef INCLUDED_GR_RDS_DATA_DECODER_H
#define INCLUDED_GR_RDS_DATA_DECODER_H

#include <rds/api.h>
#include <gnuradio/sync_block.h>
#include <gnuradio/thread/thread.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <stdio.h>

namespace gr {
namespace rds {


class RDS_API data_decoder : public gr::sync_block
{
private:
	unsigned long  bit_counter;
	unsigned long  lastseen_offset_counter, reg;
	unsigned int   block_bit_counter;
	unsigned int   wrong_blocks_counter;
	unsigned int   blocks_counter;
	unsigned int   group_good_blocks_counter;
	unsigned int   group[4];
	unsigned int   program_identification;
	bool           presync;
	bool           good_block;
	bool           group_assembly_started;
	unsigned char  program_type;
	unsigned char  lastseen_offset;
	unsigned char  block_number;
	unsigned char  pi_country_identification;
	unsigned char  pi_area_coverage;
	unsigned char  pi_program_reference_number;
	char           radiotext[65];
	char           clocktime_string[33];
	char           af1_string[10];
	char           af2_string[10];
	char           af_string[21];
	char           program_service_name[9];
	bool           radiotext_AB_flag;
	bool           traffic_program;
	bool           traffic_announcement;
	bool           music_speech;
	bool           mono_stereo;
	bool           artificial_head;
	bool           compressed;
	bool           static_pty;

	gr::thread::mutex d_mutex;

	enum state_t { ST_NO_SYNC, ST_SYNC };
	state_t d_state;


	data_decoder();
	void enter_no_sync();
	void enter_sync(unsigned int);
	void reset_rds_data();
	unsigned int calc_syndrome(unsigned long, unsigned char);
	unsigned long bin2dec(char*);
	void send_message(long, std::string);
	void decode_group(unsigned int*);
	double decode_af(unsigned int);
	void decode_optional_content(int, unsigned long int *);
	void decode_type0(unsigned int*, bool);
	void decode_type1(unsigned int*, bool);
	void decode_type2(unsigned int*, bool);
	void decode_type3a(unsigned int*);
	void decode_type4a(unsigned int*);
	void decode_type8a(unsigned int*);
	void decode_type14(unsigned int*, bool);
	void decode_type15b(unsigned int*);


public:
	typedef boost::shared_ptr<data_decoder> sptr;
	static sptr make();
	~data_decoder();		// public destructor
	void reset(void);
	int work (int noutput_items,
			gr_vector_const_void_star &input_items,
			gr_vector_void_star &output_items);
};

}
}

#endif /* INCLUDED_GR_RDS_DATA_DECODER_H */
